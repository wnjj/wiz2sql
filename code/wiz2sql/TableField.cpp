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

#include "TableField.h"

CTableField::CTableField(void)
{
  field = "";
  type = TYPE_NA;
  first_ver = 0;
  last_ver = INT_MAX;
  subTable = NULL;
  numSub = 1;
  cond = 0;
  elseTable = NULL;
}

CTableField::CTableField(std::string in_field, TABLE_DATA_T in_type, CTableField *in_subTable, CTableField *in_elseTable, int in_cond, int in_numSub)
{
  field = in_field;
  type = in_type;
  first_ver = 0;
  last_ver = INT_MAX;
  subTable = in_subTable;
  elseTable = in_elseTable;
  cond = in_cond;
  numSub = in_numSub;
}

CTableField::CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver, CTableField *in_subTable, CTableField *in_elseTable, int in_cond, int in_numSub)
{
  field = in_field;
  type = in_type;
  first_ver = in_first_ver;
  last_ver = INT_MAX;
  subTable = in_subTable;
  elseTable = in_elseTable;
  cond = in_cond;
  numSub = in_numSub;
}

CTableField::CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver, int in_last_ver, CTableField *in_subTable, CTableField *in_elseTable, int in_cond, int in_numSub)
{
  field = in_field;
  type = in_type;
  first_ver = in_first_ver;
  last_ver = in_last_ver;
  subTable = in_subTable;
  elseTable = in_elseTable;
  cond = in_cond;
  numSub = in_numSub;
}

CTableField::CTableField(std::string in_field, TABLE_DATA_T in_type, CTableField *in_subTable, int in_numSub)
{
  field = in_field;
  type = in_type;
  first_ver = 0;
  last_ver = INT_MAX;
  subTable = in_subTable;
  numSub = in_numSub;
  cond = 0;
  elseTable = NULL;
}

CTableField::CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver, CTableField *in_subTable, int in_numSub)
{
  field = in_field;
  type = in_type;
  first_ver = in_first_ver;
  last_ver = INT_MAX;
  subTable = in_subTable;
  numSub = in_numSub;
  cond = 0;
  elseTable = NULL;
}

CTableField::CTableField(std::string in_field, TABLE_DATA_T in_type, int in_first_ver, int in_last_ver, CTableField *in_subTable, int in_numSub)
{
  field = in_field;
  type = in_type;
  first_ver = in_first_ver;
  last_ver = in_last_ver;
  subTable = in_subTable;
  numSub = in_numSub;
  cond = 0;
  elseTable = NULL;
}

CTableField::~CTableField(void)
{
}
