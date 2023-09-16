#!/bin/bash -ex

. ${TEST_FOLDER}/common.sh

BASEDIR=$1
DATADIR=$(${MYSQL} ${MYSQL_ARGS} -NB -e "SELECT @@datadir")

TABLE_NAME="test6_prefix_index"


echo "CREATE table test.${TABLE_NAME} (ID int NOT NULL, name blob DEFAULT NULL, PRIMARY KEY (ID), KEY `idx_name` (`name`(10)));" | ${MYSQL} ${MYSQL_ARGS}
echo "INSERT INTO test.${TABLE_NAME} VALUES(1, 'test1');" | ${MYSQL} ${MYSQL_ARGS}
innodb_wait_for_flush_all

${SUDO} ibd2sdi ${DATADIR}/test/${TABLE_NAME}.ibd | ${BASEDIR}/sdi2ddl > ${BASEDIR}/test/${TABLE_NAME}.sql

# Open connection and keep it open
exec 3> >($MYSQL $MYSQL_ARGS)
echo "FLUSH TABLES test.${TABLE_NAME} FOR EXPORT;" >&3
${SUDO} cp ${DATADIR}/test/${TABLE_NAME}.ibd ${BASEDIR}/test/${TABLE_NAME}.ibd
echo "UNLOCK TABLES;" >&3
exec 3>&-

echo "INSERT INTO test.${TABLE_NAME} VALUES(2, 'test2');" | ${MYSQL} ${MYSQL_ARGS}
echo "DROP TABLE test.${TABLE_NAME};" | ${MYSQL} ${MYSQL_ARGS}
cat ${BASEDIR}/test/${TABLE_NAME}.sql | ${MYSQL} ${MYSQL_ARGS} test
echo "ALTER TABLE test.${TABLE_NAME} DISCARD TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
${SUDO} cp ${BASEDIR}/test/${TABLE_NAME}.ibd ${DATADIR}/test/${TABLE_NAME}.ibd
if [ "${LOCAL_MODE}" == "0" ]; then
  ${SUDO} chown mysql.mysql ${DATADIR}/test/${TABLE_NAME}.ibd
fi
echo "ALTER TABLE test.${TABLE_NAME} IMPORT TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
ROWS=$(echo "SELECT ID FROM test.${TABLE_NAME};" | ${MYSQL} ${MYSQL_ARGS} -BN)

if [ "${ROWS}" == "1" ]; then
  exit 0;
fi
exit 1;
