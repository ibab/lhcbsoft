# echo "Setting VeloWebGUI v1r0 in /afs/cern.ch/work/j/jrharris/Lovell/LovellPackagesDraft"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/cern.ch/sw/contrib/CMT/v1r20p20090520
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=VeloWebGUI -version=v1r0 -path=/afs/cern.ch/work/j/jrharris/Lovell/LovellPackagesDraft  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

