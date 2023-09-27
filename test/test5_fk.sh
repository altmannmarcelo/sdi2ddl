#!/bin/bash -ex

. ${TEST_FOLDER}/common.sh

BASEDIR=$1
DATADIR=$(${MYSQL} ${MYSQL_ARGS} -NB -e "SELECT @@datadir")

TABLE_NAME="test5_fk"
echo "
CREATE TABLE ${TABLE_NAME}_parent_1 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;
CREATE TABLE ${TABLE_NAME}_parent_2 (     id INT NOT NULL,     second_id INT NOT NULL,     PRIMARY KEY (id, second_id) ) ENGINE=INNODB;
CREATE TABLE ${TABLE_NAME}_parent_3 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;
CREATE TABLE ${TABLE_NAME}_parent_4 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;
CREATE DATABASE IF NOT EXISTS test2;
CREATE TABLE test2.${TABLE_NAME}_parent_5 (     id INT NOT NULL,     PRIMARY KEY (id) ) ENGINE=INNODB;
INSERT INTO ${TABLE_NAME}_parent_1 VALUES(1);
INSERT INTO ${TABLE_NAME}_parent_2 VALUES(1, 1);
INSERT INTO ${TABLE_NAME}_parent_3 VALUES(1);
INSERT INTO ${TABLE_NAME}_parent_4 VALUES(1);
INSERT INTO test2.${TABLE_NAME}_parent_5 VALUES(1);" | ${MYSQL} ${MYSQL_ARGS} test

echo "CREATE table test.${TABLE_NAME} (
 id int PRIMARY KEY,
 parent1_id int DEFAULT NULL,
 parent2_id int DEFAULT NULL,
 parent2_second_id int DEFAULT NULL,
 parent3_id int DEFAULT NULL,
 parent4_id int DEFAULT NULL,
 parent5_id int DEFAULT NULL,
 CONSTRAINT ${TABLE_NAME}_child_ibfk_1 FOREIGN KEY (parent1_id) REFERENCES ${TABLE_NAME}_parent_1 (id) ON UPDATE RESTRICT ON DELETE RESTRICT,
 CONSTRAINT ${TABLE_NAME}_child_ibfk_2 FOREIGN KEY (parent2_id, parent2_second_id) REFERENCES ${TABLE_NAME}_parent_2 (id, second_id) ON UPDATE CASCADE ON DELETE CASCADE ,
 CONSTRAINT ${TABLE_NAME}_child_ibfk_3 FOREIGN KEY (parent3_id) REFERENCES ${TABLE_NAME}_parent_3 (id) ON UPDATE SET NULL ON DELETE SET NULL,
 CONSTRAINT ${TABLE_NAME}_child_ibfk_4 FOREIGN KEY (parent4_id) REFERENCES ${TABLE_NAME}_parent_4 (id) ON UPDATE NO ACTION ON DELETE NO ACTION,
 CONSTRAINT ${TABLE_NAME}_child_ibfk_5 FOREIGN KEY (parent5_id) REFERENCES test2.${TABLE_NAME}_parent_5 (id) ON UPDATE SET DEFAULT ON DELETE SET DEFAULT
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;" | ${MYSQL} ${MYSQL_ARGS} test

echo "INSERT INTO test.${TABLE_NAME} VALUES(1,1,1,1,1,1,1);" | ${MYSQL} ${MYSQL_ARGS}
innodb_wait_for_flush_all

${SUDO} ibd2sdi ${DATADIR}/test/${TABLE_NAME}.ibd | ${BASEDIR}/sdi2ddl > ${BASEDIR}/test/${TABLE_NAME}.sql

# Open connection and keep it open
exec 3> >($MYSQL $MYSQL_ARGS)
echo "FLUSH TABLES test.${TABLE_NAME} FOR EXPORT;" >&3
${SUDO} cp ${DATADIR}/test/${TABLE_NAME}.ibd ${BASEDIR}/test/${TABLE_NAME}.ibd
echo "UNLOCK TABLES;" >&3
exec 3>&-

echo "INSERT INTO test.${TABLE_NAME} VALUES(2,1,1,1,1,1,1);" | ${MYSQL} ${MYSQL_ARGS}
echo "DROP TABLE test.${TABLE_NAME};" | ${MYSQL} ${MYSQL_ARGS}
cat ${BASEDIR}/test/${TABLE_NAME}.sql | ${MYSQL} ${MYSQL_ARGS} test
echo "ALTER TABLE test.${TABLE_NAME} DISCARD TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
${SUDO} cp ${BASEDIR}/test/${TABLE_NAME}.ibd ${DATADIR}/test/${TABLE_NAME}.ibd
if [ "${LOCAL_MODE}" == "0" ]; then
  ${SUDO} chown mysql.mysql ${DATADIR}/test/${TABLE_NAME}.ibd
fi
echo "ALTER TABLE test.${TABLE_NAME} IMPORT TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
ROWS=$(echo "SELECT COUNT(*) FROM test.${TABLE_NAME};" | ${MYSQL} ${MYSQL_ARGS} -BN)

if [ "${ROWS}" == "1" ]; then
  exit 0;
fi
exit 1;
