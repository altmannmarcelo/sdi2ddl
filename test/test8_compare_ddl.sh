#!/bin/bash -ex

. ${TEST_FOLDER}/common.sh

BASEDIR=$1
DATADIR=$(${MYSQL} ${MYSQL_ARGS} -NB -e "SELECT @@datadir")

TABLE_NAME="test8_fields"


echo "
CREATE TABLE ${TABLE_NAME}_parent_1 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;
CREATE TABLE ${TABLE_NAME}_parent_2 (     id INT NOT NULL,     second_id INT NOT NULL,     PRIMARY KEY (id, second_id) ) ENGINE=INNODB;
CREATE TABLE ${TABLE_NAME}_parent_3 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;
CREATE TABLE ${TABLE_NAME}_parent_4 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;
CREATE DATABASE IF NOT EXISTS test2;
CREATE TABLE test2.${TABLE_NAME}_parent_5 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;

CREATE table ${TABLE_NAME} (
  ID int NOT NULL,
  col_vc_n varchar(10) DEFAULT NULL,
  col_vc_nn varchar(10) NOT NULL DEFAULT 'abc',
  col_int_n INT DEFAULT NULL,
  col_int_nn INT NOT NULL DEFAULT 20,
  col_dt_n DATETIME DEFAULT NULL,
  col_dt_nn DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  col_dt_nn2 DATETIME NOT NULL DEFAULT '2023-09-18 11:38:10',
  col_gen_nn INT AS (SQRT(col_int_n * col_int_n + col_int_nn * col_int_nn)) NOT NULL,
  col_gen_s_nn INT AS (SQRT(col_int_n * col_int_n + col_int_nn * col_int_nn)) STORED NOT NULL UNIQUE KEY,
  col_js_n json DEFAULT NULL,
  col_js_nn json NOT NULL,
  parent1_id int DEFAULT NULL,
  parent2_id int DEFAULT NULL,
  parent2_second_id int DEFAULT NULL,
  parent3_id int DEFAULT NULL,
  parent4_id int DEFAULT NULL,
  parent5_id int DEFAULT NULL,
  PRIMARY KEY (ID),
  KEY prefix_idx (col_vc_n(5)),
  KEY func_1 ((SUBSTRING(col_vc_nn, 1, 5))),
  KEY func_2 ((cast(col_dt_nn as date))),
  CONSTRAINT ${TABLE_NAME}_child_ibfk_1 FOREIGN KEY (parent1_id) REFERENCES ${TABLE_NAME}_parent_1 (id) ON UPDATE RESTRICT ON DELETE RESTRICT,
  CONSTRAINT ${TABLE_NAME}_child_ibfk_2 FOREIGN KEY (parent2_id, parent2_second_id) REFERENCES ${TABLE_NAME}_parent_2 (id, second_id) ON UPDATE CASCADE ON DELETE CASCADE ,
  CONSTRAINT ${TABLE_NAME}_child_ibfk_3 FOREIGN KEY (parent3_id) REFERENCES ${TABLE_NAME}_parent_3 (id) ON UPDATE SET NULL ON DELETE SET NULL,
  CONSTRAINT ${TABLE_NAME}_child_ibfk_4 FOREIGN KEY (parent4_id) REFERENCES ${TABLE_NAME}_parent_4 (id) ON UPDATE NO ACTION ON DELETE NO ACTION,
  CONSTRAINT ${TABLE_NAME}_child_ibfk_5 FOREIGN KEY (parent5_id) REFERENCES test2.${TABLE_NAME}_parent_5 (id) ON UPDATE SET DEFAULT ON DELETE SET DEFAULT
  );" | ${MYSQL} ${MYSQL_ARGS} test
innodb_wait_for_flush_all
${SUDO} ibd2sdi ${DATADIR}/test/${TABLE_NAME}.ibd | ${BASEDIR}/sdi2ddl > ${BASEDIR}/test/${TABLE_NAME}_ddl.sql
mysqldump ${MYSQL_ARGS} -d --skip-add-drop-table test ${TABLE_NAME}  | grep -v '\/\*' | grep -v '\-\-' | egrep -v '^$' > ${BASEDIR}/test/${TABLE_NAME}_dump.sql

if ! diff -y -b --suppress-common-lines ${BASEDIR}/test/${TABLE_NAME}_ddl.sql ${BASEDIR}/test/${TABLE_NAME}_dump.sql;
then
  exit 1
fi
exit 0
