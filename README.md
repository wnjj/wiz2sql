# wiz2sql
SageTV wiz.bin to SQL extractor

A standalone C++ program that reads in a wiz.bin file and generates database table schema (CREATE TABLE) and data import (INSERT) SQL commands. Currently SQLite and Postgres formats are supported with the only differences being a few data types and syntax. I recommend SQLite since it's very lightweight and requires no setup.

https://forums.sagetv.com/forums/showthread.php?p=577768
