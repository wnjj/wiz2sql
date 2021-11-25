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


void Usage(std::string progname)
{
  std::cout << std::endl << "Usage: " << progname << " <wiz.bin filename> [-schema [schemafile]] [-data [datafile]] [-postgres|-pg]" << std::endl << std::endl;
  std::cout << "  -schema     Dumps the table schema (to stdout if no schemafile specified)." << std::endl;
  std::cout << "  -data       Dumps the table data (to stdout if no datafile specified)." << std::endl;
  std::cout << "  -postgres   Uses postgres syntax instead of sqlite." << std::endl << std::endl;
}


int main(int argc, char *argv[])
{
  bool infilearg = false;
  std::string wizname;
  std::ofstream schema_ofs;
  std::ofstream data_ofs;
  std::string idxname;
  int ver = 0;
  int cmdlen, num, i;
  TABLE_T tc = (TABLE_T) 0;
  OPERATION_T oper = (OPERATION_T) 0;
  INDEX_T idx;
  int id;
  int num_tables=0;
  bool postgres=false;
  bool schema=false;
  bool data=false;
  bool read;



  if (argc < 2)
  {
    Usage(argv[0]);
    exit(0);
  }
  else
  {
    for (i=1; i<argc; i++)
    {
      if      (!strcmp(argv[i], "-postgres") || !strcmp(argv[i], "-pg"))
      {
        postgres = true;
      }
      else if (!strcmp(argv[i], "-schema"))
      {
        schema = true;
        i++;
        if (i < argc)
        {
          if (argv[i][0] == '-')
            i--;
          else
          {
            schema_ofs.open(argv[i]);
            if (!schema_ofs.is_open())
            {
              std::cout << "Cannot open " << argv[i] << " for write." << std::endl;
              exit (-1);
            }
          }
        }
      }
      else if (!strcmp(argv[i], "-data"))
      {
        data = true;
        i++;
        if (i < argc)
        {
          if (argv[i][0] == '-')
            i--;
          else
          {
            data_ofs.open(argv[i]);
            if (!data_ofs.is_open())
            {
              std::cout << "Cannot open " << argv[i] << " for write." << std::endl;
              exit (-1);
            }
          }
        }
      }
      else if (argv[i][0] == '-')
      {
        std::cout << "Unrecognized option \"" << argv[i] << "\"." << std::endl << std::endl;
        exit(-1);
      }
      else if (!infilearg)
      {
        wizname = argv[i];
        infilearg = true;
      }
    }
  }

  if (!infilearg)
  {
    std::cout << std::endl << "No DB file specified!" << std::endl;
    Usage(argv[0]);
    exit(-1);
  }

  read = !(schema || data);

  std::ostream &schema_out = schema_ofs.is_open() ? schema_ofs : std::cout;
  std::ostream &data_out = data_ofs.is_open() ? data_ofs : std::cout;

  CWizard wiz(wizname);
  wiz.SetVersion(0);

  if ((wiz.readByte() == 'W') &&
      (wiz.readByte() == 'I') &&
      (wiz.readByte() == 'Z'))
  {
    ver = wiz.readByte();
  }

  // Useful for studying wiz.bin raw format (after decrypt)
  //wiz.DumpRawBytes(2000);
  //exit(0);

  if (ver == 0)
  {
    std::cout << "Unsupported DB file format!" << std::endl;
    exit(-1);
  }

  if (ver > MAX_SUPPORTED_VERSION)
  {
    std::cout << "DB file version " << ver << " greater than supported version " << MAX_SUPPORTED_VERSION << "." << std::endl;
    exit(-1);
  }

  wiz.SetVersion(ver, postgres);

  if (schema)
  {
    for (std::vector<TABLE_T>::iterator it = wiz.writeOrder.begin(); it != wiz.writeOrder.end(); ++it)
      schema_out << wiz.DumpSchema(*it) << std::endl;

    for (std::vector<INDEX_T>::iterator it = wiz.indexOrder.begin(); it != wiz.indexOrder.end(); ++it)
      schema_out << wiz.DumpSchema(*it) << std::endl;

    if (!data) exit(0);
  }

  if (read) std::cout << "Reading " << wizname << "..." << std::endl;

  while (!wiz.Eof())
  {
    cmdlen =             wiz.readInt();
    oper = (OPERATION_T) wiz.readByte();
    if (wiz.Eof()) break;

    switch (oper)
    {
      case SIZE:
        tc =  (TABLE_T) wiz.readByte();
        num =           wiz.readInt();
        if (read) std::cout << wiz.getNameForCode(tc) << ": " << num << " records" << std::endl;
        break;

      case FULL_DATA:
        if (num > 0)
        {
          idx = (INDEX_T) wiz.readByte();

          if (data) data_out << "BEGIN;\n";

          for (i=0; i<num; i++)
          {
            if (read)
              wiz.ReadRow(tc);
            else if (data)
              data_out << wiz.PopulateRow(oper, tc);
          }

          if (data) data_out << "COMMIT;\n";
        }
        break;

      case INDEX_DATA:
        if (num > 0)
        {
          idx = (INDEX_T) wiz.readByte();
          idxname = wiz.getNameForIndexCode(idx);

          if (read) std::cout << "  idx:" << idxname << std::endl;
          if (data) data_out << "INSERT INTO " << idxname << " VALUES " << std::endl;
          for (i=1; i<=num; i++)
          {
            id = wiz.readInt();
            if (data) data_out << "(" << i << "," << id << ")" << ((i==num) ? "" : ",") << std::endl;
          }
          if (data) data_out << ";" << std::endl;
        }
        break;

      case XCTS_DONE:
        if (read) std::cout << "DONE" << std::endl;

      case ADD:
      case UPDATE:
      case REMOVE:
        tc = (TABLE_T) wiz.readByte();
        if (read)
        {
          std::cout << wiz.getNameForCode(tc) << " (" << ((oper == ADD) ? "ADD" : (oper == UPDATE) ? "UPDATE" : "REMOVE") << ")" << std::endl;
          wiz.ReadRow(tc);
        }
        else if (data)
          data_out << wiz.PopulateRow(oper, tc);
        break;

      default:
        if (read) std::cout << "Unknown code " << ((int) oper) << " (cmdlen=" << cmdlen << ")" << std::endl;
        for (i=0; i<cmdlen-6; i++)
          wiz.readByte();
        break;
    }
  }

  if (read)
  {
    std::cout << std::endl << "To dump schema or data, re-run with one or more of the following options:" << std::endl;
    Usage(argv[0]);
  }

  exit(0);
}
