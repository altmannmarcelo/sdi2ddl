#!/bin/bash

MYSQL=mysql
MYSQL_ARGS="-u root -P 3306 -proot"

# Wait for InnoDB to flush all dirty pages
########################################################################
function innodb_wait_for_flush_all()
{
    while ! innodb_n_dirty_pages ; do
        sleep 1
    done

    flushed_lsn=`innodb_flushed_lsn`
    while [ `innodb_checkpoint_lsn` -lt "$flushed_lsn" ] ; do
      sleep 1
    done
}

# Return current LSN
########################################################################
function innodb_lsn()
{
    ${MYSQL} ${MYSQL_ARGS} -e "SHOW ENGINE InnoDB STATUS\G" | \
        grep "Log sequence number" | awk '{ print $4 }'
}

# Return flushed LSN
########################################################################
function innodb_flushed_lsn()
{
    ${MYSQL} ${MYSQL_ARGS} -e "SHOW ENGINE InnoDB STATUS\G" | \
        grep "Log flushed up to" | awk '{ print $5 }'
}

# Return checkpoint LSN
########################################################################
function innodb_checkpoint_lsn()
{
    ${MYSQL} ${MYSQL_ARGS} -e "SHOW ENGINE InnoDB STATUS\G" | \
        grep "Last checkpoint at" | awk '{ print $4 }'
}

# Return number of dirty pages
########################################################################
function innodb_n_dirty_pages()
{
    result=$( $MYSQL $MYSQL_ARGS -se \
        "SHOW STATUS LIKE 'innodb_buffer_pool_pages_dirty'" | \
        awk '{ print $2 }' )
    echo "Dirty pages left $result"
    return $result
}

