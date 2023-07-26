#!/bin/bash -ex

. ${TEST_FOLDER}/common.sh

BASEDIR=$1
DATADIR=$(${MYSQL} ${MYSQL_ARGS} -NB -e "SELECT @@datadir")

TABLE_NAME="test2_generated_columns"
echo "CREATE table test.${TABLE_NAME} (
  sidea DOUBLE,
  sideb DOUBLE,
  sidec DOUBLE AS (SQRT(sidea * sidea + sideb * sideb)) NOT NULL,
  sided DOUBLE AS (SQRT(sidea * sidea + sideb * sideb)) STORED NOT NULL UNIQUE KEY
);" | ${MYSQL} ${MYSQL_ARGS}
echo "INSERT INTO test.${TABLE_NAME} (sidea, sideb) VALUES(1,1),(3,4),(6,8);" | ${MYSQL} ${MYSQL_ARGS}
innodb_wait_for_flush_all

${SUDO} ibd2sdi ${DATADIR}/test/${TABLE_NAME}.ibd | ${BASEDIR}/sdi2ddl > ${BASEDIR}/test/${TABLE_NAME}.sql

# Open connection and keep it open
exec 3> >($MYSQL $MYSQL_ARGS)
echo "FLUSH TABLES test.${TABLE_NAME} FOR EXPORT;" >&3
${SUDO} cp ${DATADIR}/test/${TABLE_NAME}.ibd ${BASEDIR}/test/${TABLE_NAME}.ibd
echo "UNLOCK TABLES;" >&3
exec 3>&-

echo "INSERT INTO test.${TABLE_NAME} (sidea, sideb) VALUES(2,2);" | ${MYSQL} ${MYSQL_ARGS}
echo "DROP TABLE test.${TABLE_NAME};" | ${MYSQL} ${MYSQL_ARGS}
cat ${BASEDIR}/test/${TABLE_NAME}.sql | ${MYSQL} ${MYSQL_ARGS} test
echo "ALTER TABLE test.${TABLE_NAME} DISCARD TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
${SUDO} cp ${BASEDIR}/test/${TABLE_NAME}.ibd ${DATADIR}/test/${TABLE_NAME}.ibd
if [ "${LOCAL_MODE}" == "0" ]; then
  ${SUDO} chown mysql.mysql ${DATADIR}/test/${TABLE_NAME}.ibd
fi
echo "ALTER TABLE test.${TABLE_NAME} IMPORT TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
ROW=$(echo "SELECT CONCAT(sidec, '_', sided) FROM test.${TABLE_NAME} WHERE sidea=6;" | ${MYSQL} ${MYSQL_ARGS} -BN)
if [ "${ROW}" != "10_10" ]; then
  exit 1;
fi
ROW_COUNT=$(echo "SELECT COUNT(*) FROM test.${TABLE_NAME};" | ${MYSQL} ${MYSQL_ARGS} -BN)
if [ "${ROW_COUNT}" != "3" ]; then
  exit 1;
fi
exit 0;
