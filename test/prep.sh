#!/bin/bash
BASEDIR=$1
DATADIR=${BASEDIR}/datadir

if [ "${LOCAL_MODE}" == "0" ]; then
  echo "Running in remote mode"
  sudo systemctl start mysql.service
  mysql -u root -proot -P 3306 -e "CREATE DATABASE test;"
  exit 0;
fi

echo "Running in local mode"
mkdir -p ${DATADIR}
mysqld --no-defaults --initialize-insecure --log-error=${BASEDIR}/mysqld.log --datadir=${DATADIR}
mysqld --no-defaults  --datadir=${DATADIR} --log-error=${DATADIR}/mysqld.log --pid-file=${DATADIR}/mysqld.pid --daemonize

for i in $(seq 1 50);
do
  mysqladmin -u root -P 3306 ping > /dev/null 2>&1
  if [ $? -eq 0 ]; then
    mysql -u root -P 3306 -e "CREATE DATABASE test;"
    exit 0;
  else
    sleep 1
  fi
  done
exit 1;
