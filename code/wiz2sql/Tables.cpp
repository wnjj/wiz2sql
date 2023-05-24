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

#define AIRING_SEGMENT 1
#define PLAYLIST_SEGMENT 2
#define ALBUM_SEGMENT 3
#define TEMPMEDIAFILE_SEGMENT 4


int MAX_SUPPORTED_VERSION=0x57;

/**********************************************************************************

 The wiz.bin database format is described in this file.  CTableField has several
 overloads to minimize the colmuns needed.  All CTableField arrays are terminated
 with an empty CTableField object.

CTableField formats:

1. field_name, type
2. field_name, type,  minver
3. field_name, type,  minver, maxver
4. field_name, type,  minver, maxver, arrayref
5. field_name, type,  minver, maxver, arrayref, num_subtables
6. field_name, type,  minver, maxver, arrayref_pass, arrayref_fail, cond


1. Simplest form.  The field is used for all versions.
      ex.  <value>

2. Includes minimum version.  Versions at the minimum or greater will include this field.
      ex.  <value>

3. Includes minimum & maximum versions.  Versions at the minimum or greater up to the
   maximum will include this field.
      ex.  <value>

4. Includes an array reference.  The array is another array of type CTableField which
   contains a definition of a subtable.  wiz.bin includes subtable data inline.  The type
   column will include an array count type (short,int,long) that defines how many elements
   are in the array.
      ex.  <count>
           for (i=0; i<count; i++)
           {
             <value1>
             [value2]
           }

5. Overrides number 4 above with a count greater than one for how many separate subtables
   are contained in the array.  This is meant to support the special cases like in the
   person table where the awards are 2 complete arrays stored back to back but sharing a
   common count variable.
      ex.  <count>
           for (i=0; i<count; i++)
             <value1>
           for (i=0; i<count; i++)
             <value2>

6. Includes 2 array references and a conditional value.  The tested value is compared
   against the conditional value and then used to select a pass or fail subtable for the
   data following.
      ex.  <tested value>
           <value1>
      OR
           <tested value>
           <value2>
           <value3>

/**********************************************************************************/


CTableField stringerTable[] = {
  CTableField("id"       , TYPE_I),
  CTableField("mediamask", TYPE_B,  0x37, 0x51),
  CTableField("mediamask", TYPE_I,  0x52),
  CTableField("str"      , TYPE_U),
  CTableField()};


CTableField logoImagesArr[] = { CTableField("logoImages", TYPE_I), CTableField() };

CTableField channelTable[] = {
  CTableField("id"         , TYPE_I),
  CTableField("mediamask"  , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"  , TYPE_I,  0x52),
  CTableField("name"       , TYPE_U),
  CTableField("stationid"  , TYPE_I),
  CTableField("network"    , TYPE_I),
  CTableField("-"          , TYPE_B,  0x2F),
  CTableField("longName"   , TYPE_U),
  CTableField(""           , TYPE_S,  0x00, 0x49),
  CTableField("logoMask"   , TYPE_I,  0x4A),
  CTableField("aid"        , TYPE_L,  0x51, 0x53),
  CTableField("aid"        , TYPE_I,  0x4C, 0x50),
  CTableField("mask"       , TYPE_I,  0x4E, 0x53),
  CTableField("logoImage"  , TYPE_SA, 0x50,        logoImagesArr),
  CTableField("logoURL"    , TYPE_C,  0x56),
  CTableField()};


CTableField awardArr[] = { CTableField("awardYear", TYPE_S), CTableField(), CTableField("awardName", TYPE_I), CTableField() };
CTableField headURLArr[] = { CTableField("headURL", TYPE_C), CTableField() };

CTableField personTable[] = {
  CTableField("id"             , TYPE_I),
  CTableField("mediamask"      , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"      , TYPE_I,  0x52),
  CTableField("name"           , TYPE_U),
  CTableField("extID"          , TYPE_I,  0x4F),
  CTableField("dateOfBirth"    , TYPE_I,  0x4F),
  CTableField("dateOfDeath"    , TYPE_I,  0x4F),
  CTableField("birthPlace"     , TYPE_I,  0x4F),
  CTableField("award"          , TYPE_SA, 0x4F, awardArr, 2), // 2 arrays in one loop
  CTableField("headshotImageId", TYPE_S,  0x4F),
  CTableField("headshotURL",     TYPE_BA, 0x57, headURLArr),
  CTableField() };


CTableField personIDArr1[] = { CTableField("personID", TYPE_I), CTableField() };
CTableField personIDArr2[] = { CTableField("personID", TYPE_I),
                               CTableField("role",     TYPE_B),
                               CTableField() };
CTableField ersArr[]       = { CTableField("er",       TYPE_I), CTableField() };
CTableField bonusArr[]     = { CTableField("bonus",    TYPE_I), CTableField() };
CTableField extraCatArr[]  = { CTableField("extraCat", TYPE_I), CTableField() };
CTableField imageIDArr[]   = { CTableField("imageID",  TYPE_S), CTableField() };
CTableField imageURLArr[]  = { CTableField("imageURL", TYPE_C), CTableField() };


CTableField showTable[] = {
  CTableField("id"                  , TYPE_I),
  CTableField("mediamask"           , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"           , TYPE_I,  0x52),
  CTableField("duration"            , TYPE_L),
  CTableField("titleID"             , TYPE_I),
  CTableField("episodeNameStr"      , TYPE_C),
  CTableField("descrStr"            , TYPE_C),
  CTableField("categories0"         , TYPE_I),
  CTableField("categories1"         , TYPE_I),
  CTableField("person"              , TYPE_IA, 0x00, 0x1B, personIDArr1),
  CTableField("person"              , TYPE_IA, 0x1C,       personIDArr2),
  CTableField("rated"               , TYPE_I),
  CTableField("ers"                 , TYPE_IA,             ersArr),
  CTableField("year"                , TYPE_I),
  CTableField("pr"                  , TYPE_I),
  CTableField("bonus"               , TYPE_IA,             bonusArr),
  CTableField("lastwatched"         , TYPE_L),
  CTableField("primeTitle"          , TYPE_I,  0x00, 0x29),
  CTableField("forcedFirstRun"      , TYPE_B,  0x2A),
  CTableField("extID"               , TYPE_C),
  CTableField("updateCount"         , TYPE_I),
  CTableField("language"            , TYPE_I),
  CTableField("originalAirDate"     , TYPE_L),
  CTableField("seasonNum"           , TYPE_S,  0x4A),
  CTableField("episodeNum"          , TYPE_S,  0x4A),
  CTableField("extraCat"            , TYPE_SA, 0x4A,       extraCatArr),
  CTableField("cachedUnique"        , TYPE_B,  0x4A),
  CTableField("photoCountTall"      , TYPE_B,  0x4A, 0x4E),
  CTableField("photoCountWide"      , TYPE_B,  0x4A, 0x4E),
  CTableField("photoThumbCountTall" , TYPE_B,  0x4A, 0x4E),
  CTableField("photoThumbCountWide" , TYPE_B,  0x4A, 0x4E),
  CTableField("posterCountTall"     , TYPE_B,  0x4A, 0x4E),
  CTableField("posterCountWide"     , TYPE_B,  0x4A, 0x4E),
  CTableField("posterThumbCountTall", TYPE_B,  0x4A, 0x4E),
  CTableField("posterThumbCountWide", TYPE_B,  0x4A, 0x4E),
  CTableField("altEpisodeNum"       , TYPE_S,  0x4F),
  CTableField("showcardID"          , TYPE_I,  0x4F),
  CTableField("seriesID"            , TYPE_I,  0x4F),
  CTableField("image"               , TYPE_SA, 0x4F,       imageIDArr),
  CTableField("imageurl"            , TYPE_SA, 0x56,       imageURLArr),
  CTableField()};


CTableField fooArr[] = {
  CTableField("price",    TYPE_S),
  CTableField("flags",    TYPE_I),
  CTableField("winstart", TYPE_I),
  CTableField("provider", TYPE_C, 0x4E),
  CTableField() };

CTableField airingTable[] = {
  CTableField("id"       , TYPE_I),
  CTableField("mediamask", TYPE_B,  0x37, 0x51),
  CTableField("mediamask", TYPE_I,  0x52),
  CTableField("showID"   , TYPE_I),
  CTableField("stationID", TYPE_I),
  CTableField("airTime"  , TYPE_L),
  CTableField("duration" , TYPE_L),
  CTableField("partsB"   , TYPE_B),
  CTableField("miscB"    , TYPE_B,  0x00, 0x49),
  CTableField("miscB"    , TYPE_I,  0x4A),
  CTableField("prB"      , TYPE_B),
  CTableField("persist"  , TYPE_B,  0x41),
  CTableField("url"      , TYPE_C,  0x4C, 0x53),
  CTableField(""         , TYPE_IN, 0x4C, 0x50, fooArr, NULL, 0),
  CTableField(""         , TYPE_LN, 0x51, 0x53, fooArr, NULL, 0),
  CTableField()};


CTableField extraInfoArr[]   = { CTableField("extraInfo",  TYPE_U), CTableField() };
CTableField weakAiringArr1[] = { CTableField("weakAiring", TYPE_I), CTableField() };

CTableField manualRecordTable[] = {
  CTableField("id"          , TYPE_I),
  CTableField("mediamask"   , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"   , TYPE_I,  0x52),
  CTableField("oldID"       , TYPE_I,  0x00, 0x22),
  CTableField("startTime"   , TYPE_L),
  CTableField("duration"    , TYPE_L),
  CTableField("providerID"  , TYPE_L),
  CTableField("stationID"   , TYPE_I),
  CTableField("extraInfo"   , TYPE_IA, extraInfoArr),
  CTableField("recur"       , TYPE_I),
  CTableField("weakAiring"  , TYPE_IA, weakAiringArr1),
  CTableField("infoAiringID", TYPE_I),
  CTableField("mrProperties", TYPE_U,  0x45),
  CTableField()};


CTableField fileArr[]    = { CTableField("currFile", TYPE_U),
                             CTableField("times1",   TYPE_L),
                             CTableField("times2",   TYPE_L),
                             CTableField() };
CTableField deadAirArr[] = { CTableField("start",    TYPE_L),
                             CTableField("end",      TYPE_L),
                             CTableField() };

CTableField mediaFileTable[] = {
  CTableField("id"              , TYPE_I),
  CTableField("mediamask"       , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"       , TYPE_I,  0x52),
  CTableField("oldID"           , TYPE_I,  0x00, 0x22),
  CTableField("createWatchCount", TYPE_I),
  CTableField("file"            , TYPE_IA,             fileArr),
  CTableField("deadAir"         , TYPE_IA, 0x00, 0x4C, deadAirArr),
  CTableField("videoDirectory"  , TYPE_U),
  CTableField("name"            , TYPE_U),
  CTableField("archive"         , TYPE_B),
  CTableField("-"               , TYPE_B,  0x2F),
  CTableField("infoAiringID"    , TYPE_I),
  CTableField("host"            , TYPE_U,  0x20),
  CTableField("providerID"      , TYPE_L,  0x21, 0x4C),
  CTableField("encodedBy"       , TYPE_U,  0x21),
  CTableField("mediaType"       , TYPE_B,  0x30),
  CTableField("mediaSubtype"    , TYPE_B,  0x30),
  CTableField("generalType"     , TYPE_B,  0x30),
  CTableField("acquisitionTech" , TYPE_B,  0x30),
  CTableField("forcedComplete"  , TYPE_B,  0x30),
  CTableField("mediaType"       , TYPE_B,  0x24, 0x2F),
  CTableField("forcedComplete"  , TYPE_B,  0x24, 0x2F),
  CTableField("thumbnailFile"   , TYPE_U,  0x27),
  CTableField("thumbnailOffset" , TYPE_L,  0x27),
  CTableField("thumbnailSize"   , TYPE_I,  0x27),
  CTableField("streamBufSize"   , TYPE_L,  0x33, 0x4C),
  CTableField("fileFormat"      , TYPE_U,  0x36),
  CTableField()};


CTableField watchedTable[] = {
  CTableField("id"        , TYPE_I),
  CTableField("mediamask" , TYPE_B,  0x37, 0x51),
  CTableField("mediamask" , TYPE_I,  0x52),
  CTableField("airingID"  , TYPE_I),
  CTableField("watchStart", TYPE_L),
  CTableField("watchEnd"  , TYPE_L),
  CTableField("realStart" , TYPE_L),
  CTableField("realEnd"   , TYPE_L),
  CTableField("showID"    , TYPE_I),
  CTableField("watchTime" , TYPE_L),
  CTableField("titleNum"  , TYPE_I,  0x40),
  CTableField()};


CTableField timeSlotArr[]   = { CTableField("timeslot",   TYPE_I), CTableField() };
CTableField weakAgentArr[]  = { CTableField("weakAgent",  TYPE_I), CTableField() };
CTableField weakAiringArr[] = { CTableField("weakAiring", TYPE_I), CTableField() };

CTableField agentTable[] = {
  CTableField("id"               , TYPE_I),
  CTableField("mediamask"        , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"        , TYPE_I,  0x52),
  CTableField("agentMask"        , TYPE_I),
  CTableField("title"            , TYPE_I),
  CTableField("primetitle"       , TYPE_I,  0x00, 0x29),
  CTableField("category"         , TYPE_I),
  CTableField("subCategory"      , TYPE_I),
  CTableField("person"           , TYPE_I),
  CTableField("-"                , TYPE_B,  0x2F),
  CTableField("rated"            , TYPE_I),
  CTableField("year"             , TYPE_I),
  CTableField("pr"               , TYPE_I),
  CTableField("chanName"         , TYPE_U),
  CTableField("network"          , TYPE_I),
  CTableField("createTime"       , TYPE_L),
  CTableField("timeslots"        , TYPE_I,  0x00, 0x4A),
  CTableField("timeSlot"         , TYPE_IA, 0x4B,       timeSlotArr),
  CTableField("slotType"         , TYPE_I),
  CTableField("weakAgent"        , TYPE_IA,             weakAgentArr),
  CTableField("weakAiring"       , TYPE_IA, 0x00, 0x2C, weakAiringArr),
  CTableField("quality"          , TYPE_U),
  CTableField("startPad"         , TYPE_L,  0x25),
  CTableField("stopPad"          , TYPE_L,  0x25),
  CTableField("agentFlags"       , TYPE_I,  0x28),
  CTableField("role"             , TYPE_B,  0x2B),
  CTableField("keyword"          , TYPE_U,  0x34),
  CTableField("autoConvertFormat", TYPE_U,  0x42),
  CTableField("autoConvertDest"  , TYPE_U,  0x43),
  CTableField("favProps"         , TYPE_U,  0x44),
  CTableField()};


CTableField wastedTable[] = {
  CTableField("id"       , TYPE_I),
  CTableField("mediamask", TYPE_B,  0x37, 0x51),
  CTableField("mediamask", TYPE_I,  0x52),
  CTableField("airingID" , TYPE_I),
  CTableField("manual"   , TYPE_B,  0x1E),
  CTableField()};


CTableField albumArr[]    = { CTableField("title"   , TYPE_I),
                              CTableField("artist"  , TYPE_I),
                              CTableField("genre"   , TYPE_I,  0x29),
                              CTableField("year"    , TYPE_I,  0x29),
                              CTableField() };

CTableField currTimeArr[] = { CTableField("currTime", TYPE_L), CTableField() };
CTableField nonAlbumArr[] = { CTableField("segElem" , TYPE_I),
                              CTableField("currTime", TYPE_IA, currTimeArr),
                              CTableField() };

CTableField segmentArr[]  = { CTableField(""        , TYPE_BE, albumArr, nonAlbumArr, ALBUM_SEGMENT),
                              CTableField() };

CTableField playlistTable[] = {
  CTableField("id"           , TYPE_I),
  CTableField("mediamask"    , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"    , TYPE_I,  0x52),
  CTableField("name"         , TYPE_U),
  CTableField("segment"      , TYPE_IA, segmentArr),
  CTableField("playlistProps", TYPE_U,  0x46),
  CTableField()};


CTableField TVEditorialTable[] = {
  CTableField("id"         , TYPE_I),
  CTableField("mediamask"  , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"  , TYPE_I,  0x52),
  CTableField("showID"     , TYPE_I),
  CTableField("title"      , TYPE_I),
  CTableField("network"    , TYPE_I),
  CTableField("airdate"    , TYPE_U),
  CTableField("description", TYPE_U),
  CTableField("imageURL"   , TYPE_U),
  CTableField()};


CTableField personArr[]      = { CTableField("person",      TYPE_I),
                                 CTableField("character",   TYPE_U),
                                 CTableField() };
CTableField seriesImageArr[] = { CTableField("seriesImage", TYPE_I), CTableField() };
CTableField castImageArr[]   = { CTableField("castImage",   TYPE_L), CTableField() };

CTableField seriesInfoTable[] = {
  CTableField("id"                  , TYPE_I),
  CTableField("mediamask"           , TYPE_B,  0x37, 0x51),
  CTableField("mediamask"           , TYPE_I,  0x52),
  CTableField("legacySeriesID"      , TYPE_I),
  CTableField("title"               , TYPE_I),
  CTableField("network"             , TYPE_I),
  CTableField("description"         , TYPE_U),
  CTableField("history"             , TYPE_U),
  CTableField("premiereDate"        , TYPE_U),
  CTableField("finaleDate"          , TYPE_U),
  CTableField("airDow"              , TYPE_U),
  CTableField("airHrMin"            , TYPE_U),
  CTableField("person"              , TYPE_IA, personArr),
  CTableField("imageUrl"            , TYPE_U,  0x00, 0x4E),
  CTableField("imageUrl"            , TYPE_U,  0x54),
  CTableField("props"               , TYPE_U,  0x47),
  CTableField("showcardID"          , TYPE_I,  0x4F),
  CTableField("seriesImage"         , TYPE_SA, 0x4F, seriesImageArr),
  CTableField("castImage"           , TYPE_SA, 0x4F, castImageArr),
  CTableField("imageurl"            , TYPE_SA, 0x56, imageURLArr),
  CTableField()};


CTableField userRecordTable[] = {
  CTableField("id"       , TYPE_I),
  CTableField("mediamask", TYPE_B,  0x37, 0x51),
  CTableField("mediamask", TYPE_I,  0x52),
  CTableField("store"    , TYPE_U),
  CTableField("usaerKey" , TYPE_U),
  CTableField("props"    , TYPE_U),
  CTableField()};
