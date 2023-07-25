#!/bin/bash -ex

. ${TEST_FOLDER}/common.sh

BASEDIR=$1
DATADIR=$(${MYSQL} ${MYSQL_ARGS} -NB -e "SELECT @@datadir")

echo "CREATE table test.test1 (id int);" | ${MYSQL} ${MYSQL_ARGS}
echo "INSERT INTO test.test1 VALUES(1);" | ${MYSQL} ${MYSQL_ARGS}
innodb_wait_for_flush_all

sudo ibd2sdi ${DATADIR}/test/test1.ibd | ${BASEDIR}/sdi2ddl > ${BASEDIR}/test/test1.sql

# Open connection and keep it open
exec 3> >($MYSQL $MYSQL_ARGS)
echo "FLUSH TABLES test.test1 FOR EXPORT;" >&3
sudo cp ${DATADIR}/test/test1.ibd ${BASEDIR}/test/test1.ibd
echo "UNLOCK TABLES;" >&3
exec 3>&-

echo "INSERT INTO test.test1 VALUES(2);" | ${MYSQL} ${MYSQL_ARGS}
echo "DROP TABLE test.test1;" | ${MYSQL} ${MYSQL_ARGS}
cat ${BASEDIR}/test/test1.sql | ${MYSQL} ${MYSQL_ARGS} test
echo "ALTER TABLE test.test1 DISCARD TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
sudo cp ${BASEDIR}/test/test1.ibd ${DATADIR}/test/test1.ibd
sudo chown mysql.mysql ${DATADIR}/test/test1.ibd
echo "ALTER TABLE test.test1 IMPORT TABLESPACE;" | ${MYSQL} ${MYSQL_ARGS}
echo "SELECT * FROM test.test1;" | ${MYSQL} ${MYSQL_ARGS}
exit 0;
