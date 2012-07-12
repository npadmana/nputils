#!/usr/bin/env bash

SPALLDIR=/scratch/padmanabhan/BOSS/boss_spectro_redux
SPALLVER=5_4_45

STILTLOC=/home/np274/myWork/nputils/stilts
STILTARGS1='omode=tosql protocol=postgresql host=localhost db=sdss write=dropcreate'
STILTUSER='user=np274 password=postgres'
JDBC='-Djdbc.drivers=org.postgresql.Driver'


for ii in core photo flux resolve; do
$STILTLOC/stilts $JDBC -Xmx8G  -verbose tpipe in=$SPALLDIR/spAll-v$SPALLVER.fits dbtable=spall_v$SPALLVER.$ii cmd=@spall_$ii.txt $STILTARGS1 $STILTUSER 
done
