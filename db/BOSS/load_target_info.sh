#!/usr/bin/env bash

TARGETDIR=/scratch/padmanabhan/BOSS/target

STILTLOC=/home/np274/myWork/nputils/stilts
STILTARGS1='omode=tosql protocol=postgresql host=devel db=sdss'
STILTUSER='user=np274 password=postgres'
JDBC='-Djdbc.drivers=org.postgresql.Driver'

# First time need to dropcreate, after that we append

FILELIST=`find $TARGETDIR -type f -iname *.fits`
FIRST=TRUE

for ii in $FILELIST; do
  if $FIRST; then 
    echo $STILTLOC/stilts $JDBC -verbose tpipe in=$SPALLDIR/spAll-v$SPALLVER.fits dbtable=targetmeta write=dropcreate $STILTARGS1 $STILTUSER 
  else 
    echo $STILTLOC/stilts $JDBC -verbose tpipe in=$SPALLDIR/spAll-v$SPALLVER.fits dbtable=targetmeta write=append $STILTARGS1 $STILTUSER 
  fi
done
