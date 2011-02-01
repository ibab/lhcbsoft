"""
 =============================================================
 Class to configure MuonID tools and algos. To import do
 'from MuonID import ConfiguredMuonIDs'
 author -> Xabier Cid Vidal (xabier.cid.vidal@cern.ch)
 =============================================================
"""

import os
import sys

from Gaudi.Configuration import *
import GaudiKernel.ProcessJobOptions

from Configurables import MuonIDAlg,Chi2MuIDTool,DistMuIDTool
from Configurables import TrackMasterFitter,IsMuonCandidateC,MakeMuonMeasurements, CLTool, GetArrival
from Configurables import GaudiSequencer
from TrackFitter.ConfiguredFitters import *

class ConfiguredMuonIDs():

  def __init__(self, data="DC06",version="def",specialData=[],debug=False):
    """
    initialization for the class. Use as input data type (DC06,MC08,etc) and version of it if necessary.
    """
    self.debug=debug
    if self.debug: print "# CONFIGUREDMUONIDs v7r8"
    if self.debug: print "# INITIALIZING"

    self.specialData=specialData
    if self.debug: print "# SPECIAL DATA=",self.specialData
    
    ## from datatype and version look for module with data. Store extra modules in case desired does not exist
    mod=[data+"_"+version,data+"_def","2009_def"]
    mod = map(lambda x: "Muon_"+x,mod)

    if self.debug: print "# \tmods -> ",mod
    
    ## check if modules exist and load them
    try: exec("from MuonID import "+mod[0]+" as info")
    except:
      log.warning("ConfiguredMuonIDs: Not available info for DATA=%s,VERSION=%s. Loading default" %(data,version))
      try: exec("from MuonID import "+mod[1]+" as info")
      except: 
        exec("from MuonID import "+mod[2]+" as info")

    GaudiKernel.ProcessJobOptions.PrintOn()
    log.info("ConfiguredMuonIDs: Loaded configuration info %s"%info.FILENAME)
    GaudiKernel.ProcessJobOptions.PrintOff()
        
    ## set final module with info to be loaded
    self.info=info
    self.info.DEBUG = debug
    self.initializeAll = True
    
  def configureFitter(self,fitter):
    """
    configure fitter to be used inside MuonID chi2 calculation
    """

    if self.debug: print "# CONFIGURING FITTER"
    ## check if input is already an instance or this must be created
    if isinstance(fitter,TrackMasterFitter): myfitter=fitter
    else: myfitter=ConfiguredMasterFitter( Name = str(fitter) )
    
    myfitter.UseSeedStateErrors = self.info.UseSeedStateErrors
    myfitter.FitUpstream = self.info.FitUpstream
    myfitter.MaxNumberOutliers = self.info.MaxNumberOutliers
    myfitter.NumberFitIterations = self.info.NumberFitIterations
    myfitter.StateAtBeamLine = self.info.StateAtBeamLine
    myfitter.AddDefaultReferenceNodes = self.info.AddDefaultReferenceNodes
    ## Change TrackKalmanFilter default to avoid warning about negative nDOF
    from Configurables import TrackKalmanFilter
    myfitter.addTool(TrackKalmanFilter, name="NodeFitter")
    myfitter.NodeFitter.DoF = 0

    return myfitter


  def configureIsMuonCandidateC(self, ismc):
    """
    configure ismuoncandidatec tool. select ismuon type
    """
    if self.debug: print "# CONFIGURING ISMUONCANDIDATEC"

    ## check if input is already an instance or this must be created
    if isinstance(ismc,IsMuonCandidateC): myismc=ismc
    else: myismc=CLTool(str(ismc))

    myismc.MomRangeIsMuon = self.info.MomRangeIsMuon
    myismc.IsMuonOpt = self.info.IsMuonOpt
    myismc.MinHits = self.info.MinHits

    return myismc


  def configureGetArrival(self,geta):
    """
    configure getarrival tool. set histograms for arrival prob and clarr
    """
    if self.debug: print "# CONFIGURING GETARRIVAL"
    ## check if input is already an instance or this must be created
    if isinstance(geta,GetArrival): mygeta=geta
    else: mygeta=GetArrival(str(geta))

    mygeta.moms = self.info.gamoms
    mygeta.probs = self.info.gaprobs
    mygeta.useFunct = self.info.useFunct
    mygeta.alpha = self.info.alpha
    mygeta.beta = self.info.beta
    mygeta.eff = self.info.eff
    mygeta.MinHits = self.info.MinHits

    return mygeta


  def configureCLTool(self, cltool, use_dist=False):
    """
    configure cltool used to get dll from chi2 or distance.
    Use of chi2 or dist histos as input.
    """

    if self.debug: print "# CONFIGURING CLTOOL"
    ## check if input is already an instance or this must be created
    if isinstance(cltool,CLTool): mycltool=cltool
    else: mycltool=CLTool(str(cltool))

    mycltool.Range = self.info.Range
    mycltool.RangeNmuons = self.info.RangeNmuons

    mycltool.LeftRight = self.info.LeftRight
    mycltool.Overlap = self.info.Overlap
    mycltool.lbinCenter = self.info.lbinCenter
    mycltool.lbinCenterNmuons = self.info.lbinCenterNmuons
    mycltool.applyOvlapLast = self.info.applyOvlapLast

    ## depending on use_dist or no load histograms from dist or chi2
    if use_dist:
      if self.debug: print "# \tCONFIGURING CLTOOL: use_dist=True"
      mycltool.Signal = self.info.distSignal
      mycltool.Bkg = self.info.distBkg
    else:
      if self.debug: print "# \tCONFIGURING CLTOOL: use_dist=False"
      mycltool.Signal = self.info.chi2Signal
      mycltool.Bkg = self.info.chi2Bkg

    return mycltool
    

  def configureMuIDTool(self,muidtool,use_dist=False):
    """
    configure either chiMuIDtool or DistMuIDTool with all subdependent tools.
    If one of the two instances is input, ignore use_dist.
    If not, use_dist will either set chi2 or dist tools.
    """

    if self.debug: print "# CONFIGURING MUIDTOOL"

    ## check if input is already an instance or this must be created.
    ## also set use_dist1, to be used when configuring CLQuality tool
    use_dist1 = use_dist
    mymuidtool = muidtool
    
    if isinstance(muidtool,Chi2MuIDTool): use_dist1 = False
    elif isinstance(muidtool,DistMuIDTool): use_dist1 = True
    else:
      if use_dist: mymuidtool=DistMuIDTool(str(muidtool))
      else: mymuidtool=Chi2MuIDTool(str(muidtool))

    if self.debug: print "# \tCONFIGURING MUIDTOOL: use_dist1 = ",use_dist1

    ## general muid tool options
    mymuidtool.NSigmas = self.info.NSigmas
    mymuidtool.DiscrValue = self.info.DiscrValue
    mymuidtool.UseBkg = self.info.UseBkg
    mymuidtool.MinMomSt3 = self.info.MinMomSt3
    mymuidtool.MinMomSt4 = self.info.MinMomSt4

    ## add and configure fitter to be used for finding chi2
    if self.debug: print "# \tCONFIGURING MUIDTOOL: adding tools"
    mymuidtool.addTool(TrackMasterFitter, name='fitter') 
    ConfiguredMasterFitter( Name = mymuidtool.fitter )
    self.configureFitter(mymuidtool.fitter)

    ## for MuonIDAlg, can stop here (no use or extra subtools)
    if self.initializeAll:
      if self.debug: print "# \tCONFIGURING MUIDTOOL: initializeAll=True"
      ## add and configure ismuoncandidatec tool
      mymuidtool.addTool(IsMuonCandidateC(), name='IsMuonCandidateC')
      out=self.configureIsMuonCandidateC(mymuidtool.IsMuonCandidateC)

      ## add and configure getarrival tool
      mymuidtool.addTool(GetArrival(), name='GetArrival')
      out=self.configureGetArrival(mymuidtool.GetArrival)

      ## add and configure CLQuality. use_dist1 will set either chi2 or dist histos
      mymuidtool.addTool(CLTool(), name='CLQuality')
      out=self.configureCLTool(mymuidtool.CLQuality,use_dist1)

      return mymuidtool
    

  def configureMuonIDAlg (self,muonid,use_dist=False):
    """
    general configuration of MuonIDAlg. Equivalent to old MuonID.py
    Also configures muonIDtool used as extra_info. This can either be chi2 or dist.
    Default chi2.
    """

    if self.debug: print "# CONFIGURING MUONIDALG"

    ## check if input is already an instance or this must be created
    if isinstance(muonid,MuonIDAlg): mymuid=muonid
    else: mymuid=MuonIDAlg(str(muonid))

    ## general MuonIDAlg properties
    mymuid.OverrideDB = self.info.OverrideDB
    
    mymuid.MomentumCuts  = self.info.MomentumCuts
    mymuid.AllMuonTracks = self.info.AllMuonTracks
    mymuid.FindQuality = self.info.FindQuality
    if "cosmics" in self.specialData:
      print "# MuonID WARNING: MuonTrack Fit disabled for SpecialData = cosmics"
      mymuid.FindQuality=False
    
    mymuid.XFOIParameter1 = self.info.XFOIParameter1
    mymuid.XFOIParameter2 = self.info.XFOIParameter2
    mymuid.XFOIParameter3 = self.info.XFOIParameter3
    mymuid.YFOIParameter1 = self.info.YFOIParameter1
    mymuid.YFOIParameter2 = self.info.YFOIParameter2
    mymuid.YFOIParameter3 = self.info.YFOIParameter3
      
    mymuid.FOIfactor = self.info.FOIfactor
      
    mymuid.PreSelMomentum = self.info.PreSelMomentum

    mymuid.distMuon= self.info.distMuon
    mymuid.distPion= self.info.distPion

    ## GL & SF:
    if "DLL_flag" in dir(self.info): mymuid.DLL_flag= self.info.DLL_flag
    if "Weight_flag" in dir(self.info): mymuid.Weight_flag= self.info.Weight_flag
    if "step" in dir(self.info): mymuid.step=self.info.step
    if "nMax_bin" in dir(self.info): mymuid.nMax_bin=self.info.nMax_bin
    
    #Landau's paramateres:
    #Muons - Region1:
    if "MuLandauParameterR1_1" in dir(self.info): mymuid.MuLandauParameterR1_1= self.info.MuLandauParameterR1_1
    if "MuLandauParameterR1_2" in dir(self.info): mymuid.MuLandauParameterR1_2= self.info.MuLandauParameterR1_2
    if "MuLandauParameterR1_3" in dir(self.info): mymuid.MuLandauParameterR1_3= self.info.MuLandauParameterR1_3
    if "MuLandauParameterR1_4" in dir(self.info): mymuid.MuLandauParameterR1_4= self.info.MuLandauParameterR1_4
    if "MuLandauParameterR1_5" in dir(self.info): mymuid.MuLandauParameterR1_5= self.info.MuLandauParameterR1_5
    if "MuLandauParameterR1_6" in dir(self.info): mymuid.MuLandauParameterR1_6= self.info.MuLandauParameterR1_6
    if "MuLandauParameterR1_7" in dir(self.info): mymuid.MuLandauParameterR1_7= self.info.MuLandauParameterR1_7
    #Muons - Region2:
    if "MuLandauParameterR2_1" in dir(self.info): mymuid.MuLandauParameterR2_1= self.info.MuLandauParameterR2_1
    if "MuLandauParameterR2_2" in dir(self.info): mymuid.MuLandauParameterR2_2= self.info.MuLandauParameterR2_2
    if "MuLandauParameterR2_3" in dir(self.info): mymuid.MuLandauParameterR2_3= self.info.MuLandauParameterR2_3
    if "MuLandauParameterR2_4" in dir(self.info): mymuid.MuLandauParameterR2_4= self.info.MuLandauParameterR2_4
    if "MuLandauParameterR2_5" in dir(self.info): mymuid.MuLandauParameterR2_5= self.info.MuLandauParameterR2_5
    #Muons - Region3:
    if "MuLandauParameterR3_1" in dir(self.info): mymuid.MuLandauParameterR3_1= self.info.MuLandauParameterR3_1
    if "MuLandauParameterR3_2" in dir(self.info): mymuid.MuLandauParameterR3_2= self.info.MuLandauParameterR3_2
    if "MuLandauParameterR3_3" in dir(self.info): mymuid.MuLandauParameterR3_3= self.info.MuLandauParameterR3_3
    if "MuLandauParameterR3_4" in dir(self.info): mymuid.MuLandauParameterR3_4= self.info.MuLandauParameterR3_4
    if "MuLandauParameterR3_5" in dir(self.info): mymuid.MuLandauParameterR3_5= self.info.MuLandauParameterR3_5
    #Muons - Region4:
    if "MuLandauParameterR4_1" in dir(self.info): mymuid.MuLandauParameterR4_1= self.info.MuLandauParameterR4_1
    if "MuLandauParameterR4_2" in dir(self.info): mymuid.MuLandauParameterR4_2= self.info.MuLandauParameterR4_2
    if "MuLandauParameterR4_3" in dir(self.info): mymuid.MuLandauParameterR4_3= self.info.MuLandauParameterR4_3
    if "MuLandauParameterR4_4" in dir(self.info): mymuid.MuLandauParameterR4_4= self.info.MuLandauParameterR4_4
    if "MuLandauParameterR4_5" in dir(self.info): mymuid.MuLandauParameterR4_5= self.info.MuLandauParameterR4_5

    #Non-Muons - Region1-2-3-4:
    if "NonMuLandauParameterR1" in dir(self.info): mymuid.NonMuLandauParameterR1= self.info.NonMuLandauParameterR1
    if "NonMuLandauParameterR2" in dir(self.info): mymuid.NonMuLandauParameterR2= self.info.NonMuLandauParameterR2
    if "NonMuLandauParameterR3" in dir(self.info): mymuid.NonMuLandauParameterR3= self.info.NonMuLandauParameterR3    
    if "NonMuLandauParameterR4" in dir(self.info): mymuid.NonMuLandauParameterR4= self.info.NonMuLandauParameterR4

    # Binning for the Distance for muons:
    if "nMupBinsR1" in dir(self.info): mymuid.nMupBinsR1= self.info.nMupBinsR1
    if "nMupBinsR2" in dir(self.info): mymuid.nMupBinsR2= self.info.nMupBinsR2
    if "nMupBinsR3" in dir(self.info): mymuid.nMupBinsR3= self.info.nMupBinsR3
    if "nMupBinsR4" in dir(self.info): mymuid.nMupBinsR4= self.info.nMupBinsR4

    if "MupBinsR1" in dir(self.info): mymuid.MupBinsR1= self.info.MupBinsR1
    if "MupBinsR2" in dir(self.info): mymuid.MupBinsR2= self.info.MupBinsR2
    if "MupBinsR3" in dir(self.info): mymuid.MupBinsR3= self.info.MupBinsR3
    if "MupBinsR4" in dir(self.info): mymuid.MupBinsR4= self.info.MupBinsR4
    
    ## Configure hyperbolic tangent tanh(dist2) parameters
    if "tanhScaleFactorsR1" in dir(self.info): mymuid.tanhScaleFactorsR1= self.info.tanhScaleFactorsR1
    if "tanhScaleFactorsR2" in dir(self.info): mymuid.tanhScaleFactorsR2= self.info.tanhScaleFactorsR2
    if "tanhScaleFactorsR3" in dir(self.info): mymuid.tanhScaleFactorsR3= self.info.tanhScaleFactorsR3
    if "tanhScaleFactorsR4" in dir(self.info): mymuid.tanhScaleFactorsR4= self.info.tanhScaleFactorsR4

    ## Signal muons
    if "tanhCumulHistoMuonR1_1" in dir(self.info): mymuid.tanhCumulHistoMuonR1_1= self.info.tanhCumulHistoMuonR1_1
    if "tanhCumulHistoMuonR1_2" in dir(self.info): mymuid.tanhCumulHistoMuonR1_2= self.info.tanhCumulHistoMuonR1_2
    if "tanhCumulHistoMuonR1_3" in dir(self.info): mymuid.tanhCumulHistoMuonR1_3= self.info.tanhCumulHistoMuonR1_3
    if "tanhCumulHistoMuonR1_4" in dir(self.info): mymuid.tanhCumulHistoMuonR1_4= self.info.tanhCumulHistoMuonR1_4
    if "tanhCumulHistoMuonR1_5" in dir(self.info): mymuid.tanhCumulHistoMuonR1_5= self.info.tanhCumulHistoMuonR1_5
    if "tanhCumulHistoMuonR1_6" in dir(self.info): mymuid.tanhCumulHistoMuonR1_6= self.info.tanhCumulHistoMuonR1_6
    if "tanhCumulHistoMuonR1_7" in dir(self.info): mymuid.tanhCumulHistoMuonR1_7= self.info.tanhCumulHistoMuonR1_7

    if "tanhCumulHistoMuonR2_1" in dir(self.info): mymuid.tanhCumulHistoMuonR2_1= self.info.tanhCumulHistoMuonR2_1
    if "tanhCumulHistoMuonR2_2" in dir(self.info): mymuid.tanhCumulHistoMuonR2_2= self.info.tanhCumulHistoMuonR2_2
    if "tanhCumulHistoMuonR2_3" in dir(self.info): mymuid.tanhCumulHistoMuonR2_3= self.info.tanhCumulHistoMuonR2_3
    if "tanhCumulHistoMuonR2_4" in dir(self.info): mymuid.tanhCumulHistoMuonR2_4= self.info.tanhCumulHistoMuonR2_4
    if "tanhCumulHistoMuonR2_5" in dir(self.info): mymuid.tanhCumulHistoMuonR2_5= self.info.tanhCumulHistoMuonR2_5

    if "tanhCumulHistoMuonR3_1" in dir(self.info): mymuid.tanhCumulHistoMuonR3_1= self.info.tanhCumulHistoMuonR3_1
    if "tanhCumulHistoMuonR3_2" in dir(self.info): mymuid.tanhCumulHistoMuonR3_2= self.info.tanhCumulHistoMuonR3_2
    if "tanhCumulHistoMuonR3_3" in dir(self.info): mymuid.tanhCumulHistoMuonR3_3= self.info.tanhCumulHistoMuonR3_3
    if "tanhCumulHistoMuonR3_4" in dir(self.info): mymuid.tanhCumulHistoMuonR3_4= self.info.tanhCumulHistoMuonR3_4
    if "tanhCumulHistoMuonR3_5" in dir(self.info): mymuid.tanhCumulHistoMuonR3_5= self.info.tanhCumulHistoMuonR3_5

    if "tanhCumulHistoMuonR4_1" in dir(self.info): mymuid.tanhCumulHistoMuonR4_1= self.info.tanhCumulHistoMuonR4_1
    if "tanhCumulHistoMuonR4_2" in dir(self.info): mymuid.tanhCumulHistoMuonR4_2= self.info.tanhCumulHistoMuonR4_2
    if "tanhCumulHistoMuonR4_3" in dir(self.info): mymuid.tanhCumulHistoMuonR4_3= self.info.tanhCumulHistoMuonR4_3
    if "tanhCumulHistoMuonR4_4" in dir(self.info): mymuid.tanhCumulHistoMuonR4_4= self.info.tanhCumulHistoMuonR4_4
    if "tanhCumulHistoMuonR4_5" in dir(self.info): mymuid.tanhCumulHistoMuonR4_5= self.info.tanhCumulHistoMuonR4_5

    ## Bakground Comb muons: Also per regions AND momentum bins. Not suitable for low statistics
    if "tanhCumulHistoNonMuonR1_1" in dir(self.info): mymuid.tanhCumulHistoNonMuonR1_1= self.info.tanhCumulHistoNonMuonR1_1
    if "tanhCumulHistoNonMuonR1_2" in dir(self.info): mymuid.tanhCumulHistoNonMuonR1_2= self.info.tanhCumulHistoNonMuonR1_2
    if "tanhCumulHistoNonMuonR1_3" in dir(self.info): mymuid.tanhCumulHistoNonMuonR1_3= self.info.tanhCumulHistoNonMuonR1_3
    if "tanhCumulHistoNonMuonR1_4" in dir(self.info): mymuid.tanhCumulHistoNonMuonR1_4= self.info.tanhCumulHistoNonMuonR1_4
    if "tanhCumulHistoNonMuonR1_5" in dir(self.info): mymuid.tanhCumulHistoNonMuonR1_5= self.info.tanhCumulHistoNonMuonR1_5
    if "tanhCumulHistoNonMuonR1_6" in dir(self.info): mymuid.tanhCumulHistoNonMuonR1_6= self.info.tanhCumulHistoNonMuonR1_6
    if "tanhCumulHistoNonMuonR1_7" in dir(self.info): mymuid.tanhCumulHistoNonMuonR1_7= self.info.tanhCumulHistoNonMuonR1_7

    if "tanhCumulHistoNonMuonR2_1" in dir(self.info): mymuid.tanhCumulHistoNonMuonR2_1= self.info.tanhCumulHistoNonMuonR2_1
    if "tanhCumulHistoNonMuonR2_2" in dir(self.info): mymuid.tanhCumulHistoNonMuonR2_2= self.info.tanhCumulHistoNonMuonR2_2
    if "tanhCumulHistoNonMuonR2_3" in dir(self.info): mymuid.tanhCumulHistoNonMuonR2_3= self.info.tanhCumulHistoNonMuonR2_3
    if "tanhCumulHistoNonMuonR2_4" in dir(self.info): mymuid.tanhCumulHistoNonMuonR2_4= self.info.tanhCumulHistoNonMuonR2_4
    if "tanhCumulHistoNonMuonR2_5" in dir(self.info): mymuid.tanhCumulHistoNonMuonR2_5= self.info.tanhCumulHistoNonMuonR2_5

    if "tanhCumulHistoNonMuonR3_1" in dir(self.info): mymuid.tanhCumulHistoNonMuonR3_1= self.info.tanhCumulHistoNonMuonR3_1
    if "tanhCumulHistoNonMuonR3_2" in dir(self.info): mymuid.tanhCumulHistoNonMuonR3_2= self.info.tanhCumulHistoNonMuonR3_2
    if "tanhCumulHistoNonMuonR3_3" in dir(self.info): mymuid.tanhCumulHistoNonMuonR3_3= self.info.tanhCumulHistoNonMuonR3_3
    if "tanhCumulHistoNonMuonR3_4" in dir(self.info): mymuid.tanhCumulHistoNonMuonR3_4= self.info.tanhCumulHistoNonMuonR3_4
    if "tanhCumulHistoNonMuonR3_5" in dir(self.info): mymuid.tanhCumulHistoNonMuonR3_5= self.info.tanhCumulHistoNonMuonR3_5

    if "tanhCumulHistoNonMuonR4_1" in dir(self.info): mymuid.tanhCumulHistoNonMuonR4_1= self.info.tanhCumulHistoNonMuonR4_1
    if "tanhCumulHistoNonMuonR4_2" in dir(self.info): mymuid.tanhCumulHistoNonMuonR4_2= self.info.tanhCumulHistoNonMuonR4_2
    if "tanhCumulHistoNonMuonR4_3" in dir(self.info): mymuid.tanhCumulHistoNonMuonR4_3= self.info.tanhCumulHistoNonMuonR4_3
    if "tanhCumulHistoNonMuonR4_4" in dir(self.info): mymuid.tanhCumulHistoNonMuonR4_4= self.info.tanhCumulHistoNonMuonR4_4
    if "tanhCumulHistoNonMuonR4_5" in dir(self.info): mymuid.tanhCumulHistoNonMuonR4_5= self.info.tanhCumulHistoNonMuonR4_5

    ## add and configure either DistMuIDTool or Chi2MuIDTool.
    ## no need to initialize all sub tools there
    prev = self.initializeAll
    self.initializeAll = False
    if use_dist: mymuid.addTool(DistMuIDTool(), name='myMuIDTool')
    else: mymuid.addTool(Chi2MuIDTool(), name='myMuIDTool')
  
    out = self.configureMuIDTool(mymuid.myMuIDTool,use_dist)
    self.initializeAll = prev

    return mymuid



  def getMuonIDSeq(self):
    """
    general method for MuonIDAlg configuration.
    Creates MuonIDAlg instance and configures it.
    Finally puts it in gaudi sequencer.
    """

    if self.debug: print "# APPLYING GENERAL MUONID CONFIGURATION"
    ## create output gaudi sequencer
    myg = GaudiSequencer("MuonIDSeq")
    ## create and configure MuonIDAlg instance
    muid = MuonIDAlg()
    self.configureMuonIDAlg(muid)
    ## add to gaudi sequencer and return
    myg.Members.append(muid)
    return myg
