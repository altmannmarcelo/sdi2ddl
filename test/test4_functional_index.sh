#!/bin/bash -ex

. ${TEST_FOLDER}/common.sh

BASEDIR=$1
DATADIR=$(${MYSQL} ${MYSQL_ARGS} -NB -e "SELECT @@datadir")

TABLE_NAME="test4_functional_index"


echo "CREATE table test.${TABLE_NAME} (col1 LONGTEXT, INDEX idx1 ((SUBSTRING(col1, 1, 10))));" | ${MYSQL} ${MYSQL_ARGS}
echo "INSERT INTO test.${TABLE_NAME} VALUES('foo');" | ${MYSQL} ${MYSQL_ARGS}
innodb_wait_for_flush_all

${SUDO} ibd2sdi ${DATADIR}/test/${TABLE_NAME}.ibd | ${BASEDIR}/sdi2ddl > ${BASEDIR}/test/${TABLE_NAME}.sql

# Open connection and keep it open
exec 3> >($MYSQL $MYSQL_ARGS)
echo "FLUSH TABLES test.${TABLE_NAME} FOR EXPORT;" >&3
${SUDO} cp ${DATADIR}/test/${TABLE_NAME}.ibd ${BASEDIR}/test/${TABLE_NAME}.ibd
echo "UNLOCK TABLES;" >&3
exec 3>&-

echo "INSERT INTO test.${TABLE_NAME} VALUES('bar');" | ${MYSQL} ${MYSQL_ARGS}
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
