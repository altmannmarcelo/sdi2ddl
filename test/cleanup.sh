#!/bin/bash
BASEDIR=$1
DATADIR=${BASEDIR}/datadir

kill -9 $(cat ${DATADIR}/mysqld.pid)
wait $(cat ${DATADIR}/mysqld.pid)
rm -rf ${DATADIR}
