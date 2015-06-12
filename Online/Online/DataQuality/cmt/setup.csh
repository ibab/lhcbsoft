# echo "Setting DataQualitySys v5r9 in /group/online/dataflow/cmtuser/DataQuality_v5r18"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/lhcb.cern.ch/lib/contrib/CMT/v1r20p20090520
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=DataQualitySys -version=v5r9 -path=/group/online/dataflow/cmtuser/DataQuality_v5r18  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

