#!/bin/bash

cd job
export DEBUGGING=YES
. ./setupOnline.sh $*

exec -a ${UTGID} ${gaudi_exe} -tasktype=LHCb::Class2Task -main=${HLTOPTS}/Main.opts -opts=${HLTOPTS}/DiskWR${IS_TAE_RUN}.opts
