#!/bin/bash
#export node=$1
test -n "$1" ; export TOPLEVEL=$1
test -n "$2" ; export UTGID=$2
test -n "$3" ; export PARTNAME=$3
export OPTIONS=/home/frankm/options/${PARTNAME}/${PARTNAME}_Info.opts
export PARTOPTIONS=/home/online/Online_v4r0/Online/OnlineTasks/v1r5/options/Adder${TOPLEVEL}.opts
shift
cd /home/online/Online_v4r0/Online/OnlineTasks/v1r5/job
. ./setupAdderOnline.sh
#${gaudi_exe3} -options=../options/Adder$node.opts -loop &
if test -n "$PARTNAME" 
  then ${gaudi_exe3} -options=../options/Adder.opts -loop &
fi
if test -z "$PARTNAME" 
  then ${gaudi_exe3} -options=../options/Adder$TOPLEVEL.opts -loop &
fi
