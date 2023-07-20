# sdi2ddl - Convert SDI JSON to SHOW CREATE TABLE statement
Tool to parse MySQL [SDI](https://dev.mysql.com/doc/refman/8.0/en/serialized-dictionary-information.html) and convert into SHOW CREATE TABLE.

MySQL 8 got rid of .frm files in favor of transactional data dictionary that resides inside `mysql.ibd` file.

Sometimes is useful to be able to read the DDL of an .ibd as we can restore the table using DISCARD/IMPORT TABLESPACE.


# How to compile
This tool requires [RapidJSON](https://github.com/Tencent/rapidjson) which is available on majority of OS via package managers.

```
cd sdi2ddl
mkdir bld && cd bld
cmake .. && make
```

# How to use
`sdi2ddl` reads from STDIN you can either pass a file with the SDI JSON of the table, or utilize [`ibd2sdi`](https://dev.mysql.com/doc/refman/8.0/en/ibd2sdi.html).

```
ibd2sdi /var/lib/mysql/test/example.ibd | ./sdi2ddl
CREATE TABLE `example` (
 `ID` int unsigned NOT NULL AUTO_INCREMENT,
 `name` varchar(30),
 `address` text,
 `date` datetime,
PRIMARY KEY (`ID`),
UNIQUE KEY (`name`)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
```

# Limitations

This project is still in its early stages, partitions, FK and most of data types have not yet been implemented.

:white_check_mark: - Pull Requests are welcome.

:star: - If you like it, considered giving it a star.
