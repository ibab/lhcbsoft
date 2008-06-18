#uses "ProcessorFarm.cpp"
#uses "StreamTaskMgr.cpp"

createPartition(string name,int i) {
  string nn, nam;
  sprintf(nn,"%s_Farm%02X",name,i);
  sprintf(nam,"Part%d",i);
  if ( !dpExists(nn) ) dpCreate(nn,"FarmRunInfo");
  dpSet(nn+".general.partName",nam,
	nn+".general.partId",1<<i,
	nn+".general.activePartId",1<<i,
	nn+".general.runType","RECFARM:FarmActivity_REPRO_1",
	nn+".HLTFarm.nSubFarms",0,
	nn+".HLTFarm.subFarms",makeDynString());
}

createSlice(string name,int i) {
  string nn;
  sprintf(nn,"%s_Slice%02X",name,i);
  if ( !dpExists(nn) ) dpCreate(nn,"FarmSlice");
  dpSet(nn+".InUse",0,
	nn+".FSMSlice","",
	nn+".RunInfo","");
}

createActivity(string name) {
  string nn = "FarmActivity_"+name;
  if ( !dpExists(nn) ) dpCreate(nn,"FarmActivity");
  dpSet(nn+".Name",name,
	nn+".Farm.Infrastructure",makeDynString("MBM/Class0","Receiver/Class1","Sender/Class2"),
	nn+".Farm.Worker","Brunel/Class1",
        nn+".Control.Infrastructure","Adder/Class1");
}

main() {
  string sys, nam, typ="Reco";
  string ctrl_node = strtoupper(getHostname());
  dyn_string farms = dpNames("*:Reco_*.Name","FarmSubInfo");
  dyn_string names, used;

  fwFsmTree_removeNode("FSM","Data"+typ,1);
  fwFsmTree_removeNode("FSM",typ+"_Slices",1);
  for(int i=0; i<16;++i) {
    sprintf(nam,"%s_Farm%02X",typ,i);
    fwFsmTree_removeNode("FSM",nam,1);
    //sprintf(nam,"%s_Farm%02X","MONA08",i);
    //fwFsmTree_removeNode("FSM",nam,1);
  }
  fwFsmTree_refreshTree();
  ProcessorFarm_uninstallDataTypes();
  ProcessorFarm_uninstallStreamTypes();

  StreamTaskMgr_install();
  DebugN("Stream Control  type installation finished.");
  ProcessorFarm_installDataTypes();  
  DebugN("Procesor Farm   type installation finished.");

  for(int i=1; i<=dynlen(farms); ++i) {
    dpGet(farms[i],nam);
    string sys = strsplit(farms[i],":")[1];
    nam = strsplit(nam,"_")[2];
    dynAppend(names,nam+"/"+sys);
    dynAppend(used,"");
  }
  if ( !dpExists(typ) ) dpCreate(typ,"FarmInfo");
  dpSet(typ+".SubFarms",names,
        typ+".InUse",used,
	typ+".State","NOT_READY",
	typ+".Name",typ,
	typ+".Command","");
  createActivity("REPRO_1");
  createActivity("REPRO_2");
  createActivity("REPRO_3");
  for(int i=0; i<16;++i) {
    createSlice(typ,i);
    createPartition(typ,i);
  }
  DebugN("Known subfarms:"+names);
  ctrlUtils_stopAllTree();

  StreamTaskMgr_installStream(typ);
  ProcessorFarm_installAllocator(typ,0);
  for(int i=0; i<16;++i) {
    ProcessorFarmInstallation_createFarm(typ,i,0);
  }
  // Finally generate all FSMs
  ctrlUtils_refreshDEN();
  ctrlUtils_genAllFSM();
  ctrlUtils_stopAllTree();
  ctrlUtils_startAllTree();

  fwInstallation_addManager("PVSS00dim","always", 30, 3, 3, "-num 40 -dim_dp_config DimStorage -dim_dns_node "+ctrl_node);
  ctrlUtils_installPythonManager(58,"PVSS00Farm","../python/PVSS00Farm.py");
  exit(0);
}
