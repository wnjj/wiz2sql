/*
 * Copyright 2015 The SageTV Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Wizard.h"
#include "Tables.h"
#include <cctype>


CWizard::CWizard(void)
{
  wiz_name = "";
  CWizard::Init();
}

CWizard::~CWizard(void)
{
}

CWizard::CWizard(std::string name)
{
  wiz_name = name;
  CWizard::Init();
}


void CWizard::DefineDBString(DB_STRING_T type, std::string str)
{
  DBStrings.insert(std::make_pair(type, str));
}


void CWizard::Init()
{
  if (wiz_name != "")
  {
    wiz_in.open(wiz_name.c_str(), std::ios_base::binary);
    if (!wiz_in.is_open())
    {
      std::cout << "Cannot open " << wiz_name << " for read." << std::endl;
      exit (-1);
    }
  }
  fp = 0;
  crypto = false;
  version = 0;
  PRINT = false;
  DBStrings.clear();

  writeOrder.push_back(YEAR_CODE);
  writeOrder.push_back(NETWORK_CODE);
  writeOrder.push_back(TITLE_CODE);
  writeOrder.push_back(CHANNEL_CODE);
  writeOrder.push_back(BONUS_CODE);
  writeOrder.push_back(PEOPLE_CODE);
  writeOrder.push_back(SUBCATEGORY_CODE);
  writeOrder.push_back(RATED_CODE);
  writeOrder.push_back(PR_CODE);
  writeOrder.push_back(ER_CODE);
  writeOrder.push_back(CATEGORY_CODE);
  writeOrder.push_back(PRIME_TITLE_CODE);
  writeOrder.push_back(SHOW_CODE);
  writeOrder.push_back(AIRING_CODE);
  writeOrder.push_back(MANUAL_CODE);
  writeOrder.push_back(MEDIAFILE_CODE);
  writeOrder.push_back(WATCH_CODE);
  writeOrder.push_back(AGENT_CODE);
  writeOrder.push_back(WASTED_CODE);
  writeOrder.push_back(PLAYLIST_CODE);
  writeOrder.push_back(TVEDITORIAL_CODE);
  writeOrder.push_back(SERIESINFO_CODE);
  writeOrder.push_back(USERRECORD_CODE);

  indexOrder.push_back(TITLES_BY_NAME_CODE);
  indexOrder.push_back(PEOPLE_BY_NAME_CODE);
  indexOrder.push_back(SHOWS_BY_EXTID_CODE);
  indexOrder.push_back(AIRINGS_BY_CT_CODE);
  indexOrder.push_back(AIRINGS_BY_SHOW_CODE);
  indexOrder.push_back(WATCHED_BY_TIME_CODE);
  indexOrder.push_back(PRIME_TITLES_BY_NAME_CODE);
  indexOrder.push_back(SHOWS_BY_TITLEID_CODE);
  indexOrder.push_back(MEDIAFILE_BY_AIRING_ID_CODE);
  indexOrder.push_back(AGENTS_BY_CARNY_CODE);
  indexOrder.push_back(MEDIAFILE_BY_FILE_PATH);
  indexOrder.push_back(SERIESINFO_BY_LEGACYSERIESID_CODE);
  indexOrder.push_back(USERRECORD_BY_STOREKEY_CODE);
  indexOrder.push_back(SERIESINFO_BY_SHOWCARDID_CODE);
  indexOrder.push_back(PEOPLE_BY_EXTID_CODE);
}


char magicKey[] = {
          -94, -34, -9, -31, -120, 35, 59, -114, -124, -101, 77, 1, 22, -89, 121, -118,
          126, 95, 106, -110, -55, 74, -60, -127,
          107, -105, 104, -24, 113, 19, -89, -36, -105, 27, -105,
          -117, -11, 80, -106, 126, 42, -4, 81, 52, -115, 20, 57, -53, -83, -65, 20, 69,
          15, -104, -48, -72, -6, 83, -123, 98, 46, 62, 127, -62, 86, 21, 25, 81, 57, 34,
          25, 57, -121, 8, 56, 8, -121, -67, -106, -60, -56, 37, -63, -102, 109, -117, 49, 113, -104,
          -107, 17, -16, -1, -73, 109, 8, -64, 37, -19, -17, 2, 46, -113, -114, 37, 105,
          -11, -43, -56, -66, 60, 10, 39, 64, 116, 19, -27, 5, 76, 23, -76, -70, -65, 45, -27, 87, 6, -57 };


void CWizard::SetVersion(int ver, bool postgres)
{
  version = ver;
  crypto = (ver >= 0x2F) && (ver < 0x54);

  if (postgres)
  { // POSTGRES
    DefineDBString(DB_DEFAULT_STR, " DEFAULT NULL");
    DefineDBString(DB_DEFAULT_NUM, " DEFAULT NULL");
    DefineDBString(DB_SHORT,       "smallint");
    DefineDBString(DB_INT,         "integer");
    DefineDBString(DB_LONG,        "bigint");
    DefineDBString(DB_TEXT,        "text");
    DefineDBString(DB_BIN,         "bytea");
  }
  else
  { // SQLITE
    DefineDBString(DB_DEFAULT_STR, " DEFAULT \"\"");
    DefineDBString(DB_DEFAULT_NUM, " DEFAULT 0");
    DefineDBString(DB_SHORT,       "integer");
    DefineDBString(DB_INT,         "integer");
    DefineDBString(DB_LONG,        "integer");
    DefineDBString(DB_TEXT,        "text");
    DefineDBString(DB_BIN,         "blob");
  }
}


int  CWizard::GetVersion() { return version; }
bool CWizard::readBoolean() { return (CWizard::readByte() != 0); }

uint8_t CWizard::readByte()
{
  uint8_t ret=0;

  ret = wiz_in.get();
  if (crypto)
  {
    ret ^= magicKey[((int) fp) % 128];
    ret = (((ret & 0x0F) << 4) | ((ret & 0xF0) >> 4));
  }
  fp++;

  if (PRINT) printf("%2.2x ", ret&0xFF);

  return ret;
}


int CWizard::readShort()
{
  int ret=0;

  ret |= (CWizard::readByte() & 0xFF) << 8;
  ret |= (CWizard::readByte() & 0xFF);

  return ret;
}


int CWizard::readInt()
{
  int ret=0;

  ret |= (CWizard::readByte() & 0xFF) << 24;
  ret |= (CWizard::readByte() & 0xFF) << 16;
  ret |= (CWizard::readByte() & 0xFF) << 8;
  ret |= (CWizard::readByte() & 0xFF);

  return ret;
}


int64_t CWizard::readLong()
{
  int64_t ret=0;

  ret |= ((int64_t) readInt() & 0xFFFFFFFF) << 32;
  ret |= ((int64_t) readInt() & 0xFFFFFFFF);

  return ret;
}

std::string CWizard::readChars()
{
  int i;
  int len;
  std::string str;

  len = readShort();

  if (!len) return "";

  str="";
  for (i=0; i<len; i++)
    str += readByte();

  return str;
}


// TODO: This doesn't actually support UTF yet, only single-byte char strings
std::string CWizard::readUTF()
{
  int i;
  int len;
  std::string str;

  len = readShort();

  if (!len) return "";

  if (len == 0xFFFF) len = readInt();

  str="";
  for (i=0; i<len; i++)
    str += readByte();

  return str;
}


bool CWizard::Eof()
{
  return (wiz_in.eof());
}


// 1. Turn all ' into '' so entire string can be quoted.
// 2. Remove binary characters (non-printable and non-space).
//    Binary metadata in some photos cannot be imported into 'text' DB fields.
std::string CWizard::DBText(std::string instr, bool stripBinary)
{
  std::string outstr;
  int i;
  unsigned char c;


  outstr = '\'';
  for (i=0; i<(int) instr.length(); i++)
  {
    c = instr[i];
    if (c == '\'') outstr += '\'';
    if (!stripBinary || isprint(c) || isspace(c)) // filter out binary bytes here
      outstr += c;
  }
  outstr += '\'';

  return outstr;
}


CTableField *CWizard::getTableForCode(TABLE_T code)
{
  switch (code)
  {
    case YEAR_CODE:        return stringerTable;
    case NETWORK_CODE:     return stringerTable;
    case TITLE_CODE:       return stringerTable;
    case CHANNEL_CODE:     return channelTable;
    case BONUS_CODE:       return stringerTable;
    case PEOPLE_CODE:      return personTable;
    case SUBCATEGORY_CODE: return stringerTable;
    case RATED_CODE:       return stringerTable;
    case PR_CODE:          return stringerTable;
    case ER_CODE:          return stringerTable;
    case CATEGORY_CODE:    return stringerTable;
    case PRIME_TITLE_CODE: return stringerTable;
    case SHOW_CODE:        return showTable;
    case AIRING_CODE:      return airingTable;
    case MANUAL_CODE:      return manualRecordTable;
    case MEDIAFILE_CODE:   return mediaFileTable;
    case WATCH_CODE:       return watchedTable;
    case AGENT_CODE:       return agentTable;
    case WASTED_CODE:      return wastedTable;
    case PLAYLIST_CODE:    return playlistTable;
    case TVEDITORIAL_CODE: return TVEditorialTable;
    case SERIESINFO_CODE:  return seriesInfoTable;
    case USERRECORD_CODE:  return userRecordTable;
    case WIDGET_CODE:      return NULL;
    default:               return NULL;
  }
}


std::string CWizard::getNameForCode(TABLE_T code)
{
  switch (code)
  {
    case YEAR_CODE:        return "year";
    case NETWORK_CODE:     return "network";
    case TITLE_CODE:       return "title";
    case CHANNEL_CODE:     return "channel";
    case BONUS_CODE:       return "bonus";
    case PEOPLE_CODE:      return "person";
    case SUBCATEGORY_CODE: return "subCategory";
    case RATED_CODE:       return "rated";
    case PR_CODE:          return "parentalRating";
    case ER_CODE:          return "extendedRating";
    case CATEGORY_CODE:    return "category";
    case PRIME_TITLE_CODE: return "primeTitle";
    case SHOW_CODE:        return "show";
    case AIRING_CODE:      return "airing";
    case MANUAL_CODE:      return "manualRecord";
    case MEDIAFILE_CODE:   return "mediaFile";
    case WATCH_CODE:       return "watched";
    case AGENT_CODE:       return "agent";
    case WASTED_CODE:      return "wasted";
    case PLAYLIST_CODE:    return "playlist";
    case TVEDITORIAL_CODE: return "tVEditorial";
    case SERIESINFO_CODE:  return "seriesInfo";
    case USERRECORD_CODE:  return "userRecord";
    case WIDGET_CODE:      return "widget";
    default:               return "UNKNOWN CODE";
  }
}


std::string CWizard::getNameForIndexCode(INDEX_T code)
{
  switch (code)
  {
    case DEFAULT_INDEX_CODE:                return "default_index";
    case TITLES_BY_NAME_CODE:               return "titles_by_name";
    case PEOPLE_BY_NAME_CODE:               return "person_by_name";
    case SHOWS_BY_EXTID_CODE:               return "shows_by_extid";
    case AIRINGS_BY_CT_CODE:                return "airings_by_ct";
    case AIRINGS_BY_SHOW_CODE:              return "airings_by_show";
    case WATCHED_BY_TIME_CODE:              return "watched_by_time";
    case PRIME_TITLES_BY_NAME_CODE:         return "prime_titles_by_name";
    case SHOWS_BY_TITLEID_CODE:             return "shows_by_titleid";
    case MEDIAFILE_BY_AIRING_ID_CODE:       return "mediafile_by_airing_id";
    case AGENTS_BY_CARNY_CODE:              return "agents_by_carny";
    case MEDIAFILE_BY_FILE_PATH:            return "mediafile_by_file_path";
    case SERIESINFO_BY_LEGACYSERIESID_CODE: return "seriesinfo_by_legacyseriesid";
    case USERRECORD_BY_STOREKEY_CODE:       return "userrecord_by_storekey";
    case SERIESINFO_BY_SHOWCARDID_CODE:     return "seriesinfo_by_showcardid";
    case PEOPLE_BY_EXTID_CODE:              return "person_by_extid";
    default:                                return "UNKNOWN CODE";
  }
}


std::string CWizard::getTableForIndexCode(INDEX_T code)
{
  switch (code)
  {
    case DEFAULT_INDEX_CODE:                return "default";
    case TITLES_BY_NAME_CODE:               return "title";
    case PEOPLE_BY_NAME_CODE:               return "people";
    case SHOWS_BY_EXTID_CODE:               return "show";
    case AIRINGS_BY_CT_CODE:                return "airing";
    case AIRINGS_BY_SHOW_CODE:              return "airing";
    case WATCHED_BY_TIME_CODE:              return "watched";
    case PRIME_TITLES_BY_NAME_CODE:         return "primeTitle";
    case SHOWS_BY_TITLEID_CODE:             return "show";
    case MEDIAFILE_BY_AIRING_ID_CODE:       return "mediafile";
    case AGENTS_BY_CARNY_CODE:              return "agent";
    case MEDIAFILE_BY_FILE_PATH:            return "mediafile";
    case SERIESINFO_BY_LEGACYSERIESID_CODE: return "seriesinfo";
    case USERRECORD_BY_STOREKEY_CODE:       return "userrecord";
    case SERIESINFO_BY_SHOWCARDID_CODE:     return "seriesinfo";
    case PEOPLE_BY_EXTID_CODE:              return "person";
    default:                                return "UNKNOWN CODE";
  }
}


void CWizard::DumpRawBytes(int num)
{
  char c;
  char buf[20];
  int i;
  int pos=0;

  for (i=0; i<num; i++)
  {
    if (i && ((i%16) == 0))
    {
      buf[16] = '\0';
      std::cout << " " << buf << std::endl;
      pos=0;
    }
    if (i && ((i%480) == 0))
    {
      std::cout << "===paused===" << std::endl;
      getchar();
    }
    c = readByte();
    printf("%2.2x ", c & 0xFF);
    c &= 0x7F;
    buf[pos++] = isprint(c) ? c : '.';
  }
}


void CWizard::ReadRow(TABLE_T code)
{
  ReadRow(getTableForCode(code));
}


void CWizard::ReadRow(CTableField *table)
{
  int i, j, n, num, start;
  int64_t l;
  std::string str;
  CTableField tf;


  if (!table) return;

  int rownum = 0;
  tf = table[rownum];

  while(tf.type != TYPE_NA)
  {
    if ((version>=tf.first_ver) && (version<=tf.last_ver))
    {
      switch(tf.type)
      {
        case TYPE_B:  n =   readByte();  if (PRINT) std::cout << tf.field << "=" << n   << std::endl; break;
        case TYPE_S:  n =   readShort(); if (PRINT) std::cout << tf.field << "=" << n   << std::endl; break;
        case TYPE_I:  n =   readInt();   if (PRINT) std::cout << tf.field << "=" << n   << std::endl; break;
        case TYPE_L:  l =   readLong();  if (PRINT) std::cout << tf.field << "=" << l   << std::endl; break;
        case TYPE_C:  str = readChars(); if (PRINT) std::cout << tf.field << "=" << str << std::endl; break;
        case TYPE_U:  str = readUTF();   if (PRINT) std::cout << tf.field << "=" << str << std::endl; break;
        case TYPE_BA:
        case TYPE_SA:
        case TYPE_IA: 
                      num = (tf.type==TYPE_BA) ? readByte() : ((tf.type==TYPE_SA) ? readShort() : readInt());
                      if (PRINT) std::cout << "array: " << tf.numSub << " columns, " << num << " rows" << std::endl;
                      for (i=0; i<tf.numSub; i++)
                      {
                        start = 0;
                        n = i;
                        while (n>0) // skip past 'n' subtables in the array (type 5 in tables.cpp)
                        {
                          if (tf.subTable[start].type==TYPE_NA) n--;
                          start++;
                        }
                        for(j=0; j<num; j++)
                          ReadRow(&tf.subTable[start]);
                      }
                      break;
        case TYPE_BE: if (readByte()  == (tf.cond & 0xFF      )) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        case TYPE_SE: if (readShort() == (tf.cond & 0xFFFF    )) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        case TYPE_IE: if (readInt()   == (tf.cond & 0xFFFFFFFF)) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        case TYPE_LE: if (readLong()  == (tf.cond & 0xFFFF    )) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        case TYPE_BN: if (readByte()  != (tf.cond & 0xFF      )) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        case TYPE_SN: if (readShort() != (tf.cond & 0xFFFF    )) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        case TYPE_IN: if (readInt()   != (tf.cond & 0xFFFFFFFF)) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        case TYPE_LN: if (readLong()  != (tf.cond             )) ReadRow(tf.subTable); else ReadRow(tf.elseTable); break;
        default : break;
      }
    }

    tf = table[++rownum];
  }
}


std::string CWizard::DumpSchema(INDEX_T code)
{
  std::string name = getNameForIndexCode(code);
  std::string tablename = getTableForIndexCode(code);

  std::ostringstream oStreamMain;
  oStreamMain.str("");
  oStreamMain << "DROP TABLE IF EXISTS " << name << ";\n";
  oStreamMain << "CREATE TABLE " << name << " (\n";
  oStreamMain << "  pos " << DBStrings[DB_LONG] << DBStrings[DB_DEFAULT_NUM] << ",\n";
  oStreamMain << "  " << tablename << "ID " << DBStrings[DB_LONG] << DBStrings[DB_DEFAULT_NUM] << "\n";
  oStreamMain << ");\n";

  return (oStreamMain.str());
}


std::string CWizard::DumpSchema(TABLE_T code)
{
  return DumpSchema(getTableForCode(code), getNameForCode(code));
}


std::string CWizard::DumpSchema(CTableField *table, std::string name, bool addID, std::string parentName)
{
  int i, n, start;
  std::string str;
  CTableField tf;
  std::ostringstream oStreamMain;
  std::ostringstream oStreamSub;
  std::ostringstream oStreamTemp;
  bool incMain = false;


  if (!table) return "";

  oStreamMain.str("");
  oStreamMain << "DROP TABLE IF EXISTS " << name << ";\n";
  oStreamMain << "CREATE TABLE " << name << " (\n";

  oStreamSub.str("");

  if (addID) oStreamMain << "  " << parentName << "ID " << DBStrings[DB_LONG] << DBStrings[DB_DEFAULT_NUM] << ",\n";

  int rownum = 0;
  tf = table[rownum];

  while(tf.type != TYPE_NA)
  {
    if ((version>=tf.first_ver) && (version<=tf.last_ver))
    {
      switch(tf.type)
      {
        case TYPE_B:  if (tf.field == "-")  // toss pad bool fields
                      break;
        case TYPE_S:  incMain = true;
                      if (tf.field == "id") tf.field = name+"ID";
                      oStreamMain << "  " << tf.field << " " << DBStrings[DB_SHORT] << DBStrings[DB_DEFAULT_NUM] << ",\n";
                      break;
        case TYPE_I:  incMain = true;
                      if (tf.field == "id") tf.field = name+"ID";
                      oStreamMain << "  " << tf.field << " " << DBStrings[DB_INT] << DBStrings[DB_DEFAULT_NUM] << ",\n";
                      break;
        case TYPE_L:  incMain = true;
                      if (tf.field == "id") tf.field = name+"ID";
                      oStreamMain << "  " << tf.field << " " << DBStrings[DB_LONG] << DBStrings[DB_DEFAULT_NUM] << ",\n";
                      break;
        case TYPE_C:
        case TYPE_U:
                      incMain = true;
                      oStreamMain << "  " << tf.field << " " << DBStrings[DB_TEXT] << DBStrings[DB_DEFAULT_STR] << ",\n";
                      break;

        case TYPE_BA:
        case TYPE_SA:
        case TYPE_IA:
                      for (i=0; i<tf.numSub; i++)
                      {
                        start = 0;
                        n = i;
                        while (n>0)
                        {
                          if (tf.subTable[start].type==TYPE_NA) n--;
                          start++;
                        }
                        oStreamTemp.str("");
                        oStreamTemp << name << "_" << tf.field;
                        if (tf.numSub > 1) oStreamTemp << (i+1);
                        str = oStreamTemp.str();
                        oStreamSub << DumpSchema(&tf.subTable[start], str, true, name);
                      }
                      break;

        case TYPE_BE:
        case TYPE_SE:
        case TYPE_IE:
        case TYPE_LE:
        case TYPE_BN:
        case TYPE_SN:
        case TYPE_IN:
        case TYPE_LN:
                      oStreamSub << DumpSchema(tf.subTable,  name+tf.field+"1", true, name);
                      oStreamSub << DumpSchema(tf.elseTable, name+tf.field+"2", true, name);
                      break;

        default : break;
      }
    }

    tf = table[++rownum];
  }

  str = oStreamMain.str();
  str = str.substr(0, str.size()-2);  // chop last ",\n"
  str += "\n);\n\n";

  if (incMain)
    str += oStreamSub.str();
  else
    str = oStreamSub.str();

  return str;
}


std::string CWizard::PopulateRow(OPERATION_T oper, TABLE_T code)
{
  std::string name = getNameForCode(code);
  return PopulateRow(oper, getTableForCode(code), name, name);
}

std::string CWizard::PopulateRow(OPERATION_T oper, CTableField *table, std::string name, std::string parentName, int parentID)
{
  int i, j, n, num, start;
  int64_t l;
  std::string str;
  std::string tableID;
  CTableField tf;
  CTableField *subtf;
  std::ostringstream oStreamMain;
  std::ostringstream oStreamSub;
  std::ostringstream oStreamTemp;
  int ID;
  bool idField;
  bool incMain = false;
  char subchar='1';


  if (!table) return "";

  ID = parentID;

  oStreamTemp.str("");
  oStreamTemp << parentName << "ID";
  tableID = oStreamTemp.str();

  oStreamMain.str("");
  switch (oper)
  {
    case FULL_DATA:
    case ADD:         oStreamMain << "INSERT INTO " << name << " VALUES (";
                      if (parentID>0)
                        oStreamMain << parentID << ","; // add the parent table ID column first
                      break;
    case UPDATE:      oStreamMain << "UPDATE " << name << " SET ";
                      break;
    default:          break;
  }

  oStreamSub.str("");

  int rownum = 0;
  tf = table[rownum];

  while(tf.type != TYPE_NA)
  {
    if ((version>=tf.first_ver) && (version<=tf.last_ver))
    {
      subtf = NULL;
      idField = (tf.field == "id");

      if ((oper == UPDATE) && !idField)
      {
        switch(tf.type)
        {
          case TYPE_B:
          case TYPE_S:
          case TYPE_I:
          case TYPE_L:
          case TYPE_C:
          case TYPE_U:  if (tf.field != "-") oStreamMain << tf.field << "="; break;
          default:      break;
        }
      }

      switch(tf.type)
      {
        case TYPE_B:  incMain = true; n =   readByte();  if (tf.field != "-") oStreamMain << (n & 0xFF) << ","; break;
        case TYPE_S:  incMain = true; n =   readShort(); oStreamMain << (n & 0xFFFF)      << ","; break;
        case TYPE_I:  incMain = true; n =   readInt();
                      if (idField) ID = n;
                      if ((oper != UPDATE) || !idField) oStreamMain << (n & 0xFFFFFFFF)   << ",";
                      break;
        case TYPE_L:  incMain = true; l =   readLong();  oStreamMain <<  l                << ","; break;
        case TYPE_C:  incMain = true; str = readChars(); oStreamMain << DBText(str, true) << ","; break;
        case TYPE_U:  incMain = true; str = readUTF();   oStreamMain << DBText(str)       << ","; break;
        case TYPE_BA:
        case TYPE_SA:
        case TYPE_IA: incMain = true;
                      num = (tf.type==TYPE_BA) ? readByte() : ((tf.type==TYPE_SA) ? readShort() : readInt());
                      for (i=0; i<tf.numSub; i++)
                      {
                        start = 0;
                        n = i;
                        while (n>0) // skip past 'n' subtables in the array (type 5 in tables.cpp)
                        {
                          if (tf.subTable[start].type==TYPE_NA) n--;
                          start++;
                        }
                        oStreamTemp.str("");
                        oStreamTemp << name << "_" << tf.field;
                        if (tf.numSub > 1) oStreamTemp << (i+1);
                        str = oStreamTemp.str();
                        for(j=0; j<num; j++)
                          oStreamSub << PopulateRow(oper, &tf.subTable[start], str, name, ID);
                      }
                      break;
        case TYPE_BE: if (readByte()  == (tf.cond & 0xFF      )) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        case TYPE_SE: if (readShort() == (tf.cond & 0xFFFF    )) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        case TYPE_IE: if (readInt()   == (tf.cond & 0xFFFFFFFF)) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        case TYPE_LE: if (readLong()  == (tf.cond & 0xFFFF    )) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        case TYPE_BN: if (readByte()  != (tf.cond & 0xFF      )) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        case TYPE_SN: if (readShort() != (tf.cond & 0xFFFF    )) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        case TYPE_IN: if (readInt()   != (tf.cond & 0xFFFFFFFF)) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        case TYPE_LN: if (readLong()  != (tf.cond             )) subtf = tf.subTable; else { subtf = tf.elseTable; subchar='2'; } break;
        default : break;
      }
      if (subtf) oStreamSub << PopulateRow(oper, subtf, name+tf.field+subchar, name, ID);
    }

    tf = table[++rownum];
  }

  str = oStreamMain.str();
  str = str.substr(0, str.size()-1);  // chop last ","
  switch (oper)
  {
    case FULL_DATA:
    case ADD:         str += ");\n";
                      break;

    case REMOVE:      oStreamMain.str("");
                      oStreamMain << "DELETE FROM " << name;
                      str = oStreamMain.str();
    case UPDATE:      oStreamTemp.str("");
                      oStreamTemp << " WHERE " << tableID << "=" << ID << ";\n";
                      str += oStreamTemp.str();
                      break;

    default:          break;
  }

  if (incMain)
    str += oStreamSub.str();
  else
    str = oStreamSub.str();

  return str;
}
