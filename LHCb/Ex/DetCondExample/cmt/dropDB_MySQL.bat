rem $Id: dropDB_MySQL.bat,v 1.2 2004-12-08 17:22:37 marcocle Exp $
rem Remove the MySQL ConditionsDB
call %CONDDBROOT%\cmt\condDBMySQL_dropCondDB.bat %CONDDBMYSQL_DBUSER% %CONDDBMYSQL_DBPSWD% %CONDDBMYSQL_DBHOST% %CONDDBMYSQL_DBNAME%
