# sdi2ddl - Convert SDI JSON to SHOW CREATE TABLE statement
[![Build](https://github.com/altmannmarcelo/sdi2ddl/actions/workflows/build.yml/badge.svg?branch=main)](https://github.com/altmannmarcelo/sdi2ddl/actions/workflows/build.yml)

Tool to parse MySQL [SDI](https://dev.mysql.com/doc/refman/8.0/en/serialized-dictionary-information.html) and convert into SHOW CREATE TABLE.

MySQL 8 got rid of .frm files in favor of a transactional data dictionary that resides inside `mysql.ibd` file.

Sometimes is useful to be able to read the DDL of a .ibd as we can restore the table using DISCARD/IMPORT TABLESPACE.


# How to use
`sdi2ddl` reads from STDIN, you can either pass a file with the SDI JSON of the table or utilize [`ibd2sdi`](https://dev.mysql.com/doc/refman/8.0/en/ibd2sdi.html).

```
ibd2sdi /var/lib/mysql/test/example.ibd | ./sdi2ddl
CREATE TABLE `example` (
 `ID` int unsigned NOT NULL AUTO_INCREMENT,
 `name` varchar(30) DEFAULT NULL,
 `address` text DEFAULT NULL,
 `date` datetime DEFAULT NULL,
 PRIMARY KEY (`ID`),
 UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

# How to compile
This tool requires [RapidJSON](https://github.com/Tencent/rapidjson), which is available on most OS via package managers.

```
cd sdi2ddl
mkdir bld && cd bld
cmake .. && make
```

# Limitations

Note: as of 8.0.34, MySQL does not expose auto-increment data as part of SDI. Importing the original table with AI > 0 on a schema without explicit AI counter works:

```
mysql> CREATE TABLE `table2` (
    ->  `ID` int NOT NULL AUTO_INCREMENT,
    ->  `count` int DEFAULT '0',
    -> PRIMARY KEY (`ID`)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
Query OK, 0 rows affected (0,05 sec)

mysql> SELECT * FROM table2;
Empty set (0,00 sec)

mysql> ALTER TABLE table2 DISCARD TABLESPACE;
Query OK, 0 rows affected (0,02 sec)

mysql> ALTER TABLE table2 IMPORT TABLESPACE;
Query OK, 0 rows affected, 1 warning (0,03 sec)

mysql> SELECT * FROM table2;
+----+-------+
| ID | count |
+----+-------+
|  1 |     0 |
|  2 |     0 |
|  3 |     0 |
|  4 |     0 |
|  5 |     0 |
|  6 |     0 |
|  7 |     0 |
|  8 |     0 |
+----+-------+
8 rows in set (0,01 sec)

mysql> SHOW CREATE TABLE table2;
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Table  | Create Table                                                                                                                                                                                    |
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| table2 | CREATE TABLE `table2` (
  `ID` int NOT NULL AUTO_INCREMENT,
  `count` int DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci |
+--------+-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
1 row in set (0,01 sec)

```

:white_check_mark: - Pull Requests are welcome.

:star: - If you like it, considered giving it a star.
