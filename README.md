# wiz2sql
SageTV wiz.bin to SQL extractor

A standalone C++ program that reads in a wiz.bin file and generates database table schema (CREATE TABLE) and data import (INSERT) SQL commands. Currently SQLite and Postgres formats are supported with the only differences being a few data types and syntax. I recommend SQLite since it's very lightweight and requires no setup.


https://forums.sagetv.com/forums/showthread.php?p=577768
Here is the content of the original SageTV forum post:

I wrote a standalone C++ program that reads in a wiz.bin file and generates database table schema (CREATE TABLE) and data import (INSERT) SQL commands. Currently SQLite and Postgres formats are supported with the only differences being a few data types and syntax. I recommend SQLite since it's very lightweight and requires no setup.

It's not necessarily complete but seemed to run fine on my version7 (wiz.bin ver 75) and version9 (wiz.bin ver 85 & ver 86). I have not tested any other versions so there could be errors in the format definitions for those which can be fixed easily.

The code is 4 .cpp and 3 .h files and compiles as a Windows console app or Linux.

I'm not sure what explicit purpose this program serves yet but it gave me an excuse to learn the insides of wiz.bin as well as confidently and portably save my TV show history. It could potentially be modified to write wiz.bin files which would allow for SQL data edits (repairs) before exporting back to wiz.bin.

I've included the Windows .exe inside the .zip. Compile on Linux with:
```g++ -o wiz2sql Wiz2SQL.cpp Wizard.cpp Tables.cpp TableField.cpp
```
Run the program without any arguments for a usage description. An example run with arguments looks like this:
```wiz2sql.exe wiz.bin -schema schema.sql -data data.sql
```
To import into SQLite:
```sqlite3 wiz.db < schema.sql
sqlite3 wiz.db < data.sql
```
