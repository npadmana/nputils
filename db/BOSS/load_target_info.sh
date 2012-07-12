#!/usr/bin/env bash

TARGETDIR=/scratch/padmanabhan/BOSS/target

STILTLOC=/home/np274/myWork/nputils/stilts
STILTARGS1='omode=tosql protocol=postgresql host=localhost db=sdss'
STILTUSER='user=np274 password=postgres'
JDBC='-Djdbc.drivers=org.postgresql.Driver'

# First time need to dropcreate, after that we append

FILELIST=`find $TARGETDIR -type f -iname *lrg*.fits`

psql -f galtargetmeta.sql sdss
for ii in $FILELIST; do
    $STILTLOC/stilts $JDBC -verbose tpipe in=$ii dbtable=galtargetmeta write=append $STILTARGS1 $STILTUSER cmd="keepcols 'TARGET* *_V PHOTO_*'"
done
