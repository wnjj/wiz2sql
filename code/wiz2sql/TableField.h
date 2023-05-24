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

#ifndef _DEFINED_TABLEFIELD_H
#define _DEFINED_TABLEFIELD_H

#include <string>
#include <stdint.h>

typedef enum table_data_t
{
  TYPE_NA  = 0,
  TYPE_B   = 1,  // byte/boolean
  TYPE_S   = 2,  // short
  TYPE_I   = 3,  // int
  TYPE_L   = 4,  // long
  TYPE_U   = 5,  // UTF
  TYPE_C   = 6,  // byte array with short count var (char *)
  TYPE_BA  = 7,  // array with byte count var
  TYPE_SA  = 8,  // array with short count var
  TYPE_IA  = 9,  // array with int count var
  TYPE_BE  = 10, // conditional == with byte var
  TYPE_SE  = 11, // conditional == with short var
  TYPE_IE  = 12, // conditional == with int var
  TYPE_LE  = 13, // conditional == with long var
  TYPE_BN  = 14, // conditional != with byte var
  TYPE_SN  = 15, // conditional != with short var
  TYPE_IN  = 16, // conditional != with int var
  TYPE_LN  = 17, // conditional != with long var
} TABLE_DATA_T;


class CTableField
{

public:
  CTableField();
  CTableField(std::string in_field, TABLE_DATA_T in_type, CTableField *in_subTable, CTableField *in_elseTable, int in_cond=0, int in_numSub=1);
  CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver, CTableField *in_subTable, CTableField *in_elseTable, int in_cond=0, int in_numSub=1);
  CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver, int in_last_ver, CTableField *in_subTable, CTableField *in_elseTable, int in_cond=0, int in_numSub=1);
  CTableField(std::string in_field, TABLE_DATA_T in_type, CTableField *in_subTable, int in_numSub=1);
  CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver, CTableField *in_subTable, int in_numSub=1);
  CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver=0, int in_last_ver=INT_MAX, CTableField *in_subTable=NULL, int in_numSub=1);
  ~CTableField();

public:
  std::string field;
  TABLE_DATA_T type;
  int first_ver;
  int last_ver;
  CTableField *subTable;
  int numSub;
  CTableField *elseTable;
  int64_t cond;
};

#endif
