#!/usr/local/bin/tcsh
# $Id: setupDB.csh,v 1.5 2002-03-28 13:29:30 andreav Exp $
if ( ${?CONDDB_implementation} != 1 ) then
  echo "The environment variable CONDDB_implementation is not set"
  echo "This should never happen: check your requirements file(s)!"
  exit 1
else if ( "${CONDDB_implementation}" == "CondDBOracle" ) then
  source setupDB_Oracle.csh
  exit 0
else if ( "${CONDDB_implementation}" == "CondDBObjy" ) then
  source setupDB_Objy.csh
  exit 0
else
  echo "CONDDB_implementation is set to ${CONDDB_implementation}"
  echo "ERROR: the only valid values are CondDBOracle and CondDBObjy!!!"
  exit 1
endif

