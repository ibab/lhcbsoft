set TAN_PORT=YES
set TAN_NODE=%COMPUTERNAME%.cern.ch
set DIM_DNS_NODE=%COMPUTERNAME%.cern.ch
rem
set test_exe=%ONLINEKERNELROOT%\win32_vc71_dbg\test.exe
set gaudi_exe=%GAUDIONLINEROOT%\win32_vc71_dbg\Gaudi.exe GaudiOnline.dll OnlineTask -auto
set msg_svc=MessageSvc
set msg_svc=LHCb::DimMessageSvc
set UTGID=MBMMon
start "%UTGID%" %test_exe% mbm_mon
set UTGID=MBMInit
start "%UTGID%" %gaudi_exe% -main=../options/MEPinit.opts -opt=../options/Daemon.opts -msgsvc=%msg_svc%
set UTGID=ErrServ
start "%UTGID%" %gaudi_exe% -opt=../options/ErrorSrv.opts     -msgsvc=%msg_svc%
set UTGID=ErrLog
start "%UTGID%" %gaudi_exe% -opt=../options/ErrorLogger.opts  -msgsvc=MessageSvc

sleep 5
set UTGID=EvtProd
start "%UTGID%" %gaudi_exe% -opt=../options/MEPConverter.opts -msgsvc=%msg_svc%
set UTGID=EvtHolder
start "%UTGID%" %gaudi_exe% -opt=../options/MEPHolder.opts    -msgsvc=%msg_svc%
set UTGID=Moore_0
start "%UTGID%" %gaudi_exe% -opt=../options/ReadMBM.opts      -msgsvc=%msg_svc%
set UTGID=Moore_1
start "%UTGID%" %gaudi_exe% -opt=../options/ReadMBM.opts      -msgsvc=%msg_svc%
set UTGID=Moore_2
start "%UTGID%" %gaudi_exe% -opt=../options/ReadMBM.opts      -msgsvc=%msg_svc%
set UTGID=Spy_0
start "%UTGID%" %gaudi_exe% -opt=../options/SpyMBMSlow.opts   -msgsvc=%msg_svc%
set UTGID=Spy_1
start "%UTGID%" %gaudi_exe% -opt=../options/SpyMBM.opts       -msgsvc=%msg_svc%
set UTGID=Spy_2
start "%UTGID%" %gaudi_exe% -opt=../options/SpyMBMFast.opts   -msgsvc=%msg_svc%
rem
set UTGID=OutputBuffer
rem start "%UTGID%" %test_exe% mbm_install -s=8096 -e=64 -u=64 -i=OUTPUT
start "%UTGID%" %gaudi_exe% -main=../options/MBMinit.opts -opt=../options/Daemon.opts -msgsvc=%msg_svc%
set UTGID=Sender
start "%UTGID%" %gaudi_exe% -opt=../options/MDFSender.opts    -msgsvc=%msg_svc%
set UTGID=Receiver
start "%UTGID%" %gaudi_exe% -opt=../options/MDFReceiver.opts  -msgsvc=%msg_svc%
set UTGID=DiskWR
start "%UTGID%" %gaudi_exe% -opt=../options/DiskWR.opts       -msgsvc=%msg_svc%
set UTGID=MDFWriter
rem start "%UTGID%" %gaudi_exe% -opt=../options/MDFWriter.opts    -msgsvc=%msg_svc%

rem start /b xterm -132 -geometry 132x45  -title ErrorLogger  -e /bin/bash -l -c "export UTGID=ErrorLogger; cd J:/Gaudi/Online/UPI/cmt; ./upi.sh ../../GaudiOnline/win32_vc71_dbg/gaudi.exe ../../GaudiOnline/win32_vc71_dbg/GaudiOnline OnlineTask -opts=../../GaudiUPI/options/Errlog.opts -msgsvc=%msg_svc%"

rem start "prod_0" ..\win32_vc71_dbg\Gaudi.exe GaudiOnline mep_producer -n=prod_0 -p=333 -s=500 -r=2
