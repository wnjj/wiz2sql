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

#ifndef _DEFINED_WIZARD_H
#define _DEFINED_WIZARD_H

#include "TableField.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>


// operation codes
typedef enum operation_t
{
 ADD = 0x01,
 UPDATE = 0x02,
 REMOVE = 0x03,
 SIZE = 0x04,
 FULL_DATA = 0x05,
 INDEX_DATA = 0x06,
 XCTS_DONE = 0x10,
} OPERATION_T;


// table codes
typedef enum table_t
{
 UNKNOWN_CODE = 0x00,
 NETWORK_CODE = 0x01,
 CHANNEL_CODE = 0x02,
 TITLE_CODE = 0x03,
 PEOPLE_CODE = 0x04,
 CATEGORY_CODE = 0x05,
 SUBCATEGORY_CODE = 0x06,
 RATED_CODE = 0x07,
 PR_CODE = 0x08,
 ER_CODE = 0x09,
 YEAR_CODE = 0x0A,
 SHOW_CODE = 0x0B,
 AIRING_CODE = 0x0C,
 WATCH_CODE = 0x0D,
 BONUS_CODE = 0x0E,
 PRIME_TITLE_CODE = 0x0F,
 AGENT_CODE = 0x10,
 IDMAP_CODE = 0x11,
 MEDIAFILE_CODE = 0x12,
 MANUAL_CODE = 0x13,
 WASTED_CODE = 0x14,
 WIDGET_CODE = 0x15,
 PLAYLIST_CODE = 0x16,
 TVEDITORIAL_CODE = 0x17,
 SERIESINFO_CODE = 0x18,
 USERRECORD_CODE = 0x19,
} TABLE_T;


// index codes
typedef enum index_t
{
 DEFAULT_INDEX_CODE = 0x00,
 TITLES_BY_NAME_CODE = 0x01,
 PEOPLE_BY_NAME_CODE = 0x02,
 SHOWS_BY_EXTID_CODE = 0x03,
 AIRINGS_BY_CT_CODE = 0x05,
 AIRINGS_BY_SHOW_CODE = 0x06,
 WATCHED_BY_TIME_CODE = 0x07,
 PRIME_TITLES_BY_NAME_CODE = 0x08,
 SHOWS_BY_TITLEID_CODE = 0x09,
 MEDIAFILE_BY_AIRING_ID_CODE = 0x0A,
 AGENTS_BY_CARNY_CODE = 0x0B,
 MEDIAFILE_BY_FILE_PATH = 0x0C,
 SERIESINFO_BY_LEGACYSERIESID_CODE = 0x0D,
 USERRECORD_BY_STOREKEY_CODE = 0x0E,
 SERIESINFO_BY_SHOWCARDID_CODE = 0x0F,
 PEOPLE_BY_EXTID_CODE = 0x10,
} INDEX_T;


// Extended DBObject classes used in SageTV.
//
// CHANNEL_CODE -      Channel.java
// PEOPLE_CODE -       Person.java
// SHOW_CODE -         Show.java
// AIRING_CODE -       Airing.java
// MANUAL_CODE -       ManualRecord.java
// MEDIAFILE_CODE -    MediaFile.java
// WATCH_CODE -        Watched.java
// AGENT_CODE -        Agent.java
// WASTED_CODE -       Wasted.java
// PLAYLIST_CODE -     PlayList.java
// TVEDITORIAL_CODE -  TVEditorial.java
// SERIESINFO_CODE -   SeriesInfo.java
// USERRECORD_CODE -   UserRecord.java
// <all other CODEs> - Stringer.java


// DB String fields
typedef enum DB_string_t
{
 DB_DEFAULT_STR = 0,
 DB_DEFAULT_NUM = 1,
 DB_SHORT = 2,
 DB_INT = 3,
 DB_LONG = 4,
 DB_TEXT = 5,
 DB_BIN = 6,
} DB_STRING_T;



class CWizard
{

public:
  CWizard();
  CWizard(std::string name);
  ~CWizard();

public:
  void Init();
  void SetVersion(int ver, bool postgres=false);
  int  GetVersion();
  void DefineDBString(DB_STRING_T type, std::string str);
  bool readBoolean();
  uint8_t readByte();
  int  readShort();
  int  readInt();
  int64_t readLong();
  std::string readChars();
  std::string readUTF();
  bool Eof();
  std::string DBText(std::string instr, bool stripBinary=false);
  CTableField *getTableForCode(TABLE_T code);
  std::string getNameForCode(TABLE_T code);
  std::string getNameForIndexCode(INDEX_T code);
  std::string getTableForIndexCode(INDEX_T code);
  void DumpRawBytes(int num);
  void ReadRow(TABLE_T code);
  void ReadRow(CTableField *table);
  std::string DumpSchema(INDEX_T code);
  std::string DumpSchema(TABLE_T code);
  std::string DumpSchema(CTableField *table, std::string name, bool addID=false, std::string parentName="");
  std::string PopulateRow(OPERATION_T oper, TABLE_T code);
  std::string PopulateRow(OPERATION_T oper, CTableField *table, std::string name, std::string parentName="", int parentID=0);
  void DumpTableColumns(CTableField *table);

public:
  bool PRINT; // for debugging
  std::vector<TABLE_T> writeOrder;
  std::vector<INDEX_T> indexOrder;
  std::map<DB_STRING_T, std::string> DBStrings;

private:
  std::string wiz_name;
  std::ifstream wiz_in;
  long fp;
  bool crypto;
  int version;
};

#endif
