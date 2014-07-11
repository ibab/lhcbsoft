"""
Module for construction of Radiative Decays Stripping Selections and StrippingLines.
Provides functions to build Bd, Bs, K*, Phi selections.
Provides class StrippingB2XGammaConf, which constructs the Selections and
StrippingLines given a configuration dictionary.
Exported selection makers: 'makePhoton', 'makePhi2KK', 'makeKstar', 'makeBs2PhiGamma',
'makeBd2KstGamma'
"""

__author__ = ['Fatima Soomro', 'Albert Puig', 'Pablo Ruiz Valls']
__date__ = '11/07/2014'
__version__ = '$Revision: 1.9 $'

__all__ = ('StrippingB2XGammaConf', 'makePhoton', 'makePhi2KK', 'makeKstar', 'makeBs2PhiGamma', 'makeBd2KstGamma', 'default_config')

from Gaudi.Configuration import *
from GaudiConfUtils.ConfigurableGenerators import FilterDesktop, CombineParticles
from PhysSelPython.Wrappers import Selection, DataOnDemand
from StrippingConf.StrippingLine import StrippingLine
from StrippingUtils.Utils import LineBuilder
from StandardParticles import StdLooseAllPhotons

default_name = 'B2XGamma'
default_config = {
    'NAME'        : 'Beauty2XGammaExclusive',
    'WGs'         : ['RD'],
    'BUILDERTYPE' : 'Beauty2XGammaExclusiveConf',
    'CONFIG'      : {
          'TrIPchi2'             : 10.       # Dimensionless
          ,'TrChi2'              : 5.        # dimensionless
          
          ,'PhiMassWin'          : 15.       # MeV
          ,'KstMassWin'          : 100.      # MeV
          ,'PhiVCHI2'            : 15.       # dimensionless
          ,'KstVCHI2'            : 15.       # dimensionless

          ,'photonPT'            : 2600.     # MeV
          
          ,'BsMassWin'           : 1000.     # MeV
          ,'B0MassWin'           : 1000.     # MeV
          ,'BsDirAngle'          : 0.02      # radians
          ,'B0DirAngle'          : 0.02      # radians
          ,'BsPVIPchi2'          : 15.       # Dimensionless
          ,'B0PVIPchi2'          : 15.       # Dimensionless

          # Pre- and postscales
          ,'Bs2PhiGammaPreScale'               : 1.0
          ,'Bs2PhiGammaPostScale'              : 1.0
          ,'Bd2KstGammaPreScale'               : 1.0
          ,'Bd2KstGammaPostScale'              : 1.0
          }
    }

class Beauty2XGammaExclusiveConf(LineBuilder):
    """
    Definition of B -> X Gamma stripping
    
    Constructs B0 -> K* Gamma and Bs -> Phi Gamma Selections and StrippingLines from
    a configuration dictionary.
    Usage:
    >>> config = { .... }
    >>> gammaConf = Beauty2XGammaExclusiveConf('Beauty2XGammaExclusiveTest',config)
    >>> gammaLines = gammaConf.lines
    >>> for line in line :
    >>>  print line.name(), line.outputLocation()
    The lines can be used directly to build a StrippingStream object.

    Exports as instance data members:
    selPhoton                    : Photon Selection object
    selPhi2KK                    : nominal Phi -> K+K- Selection object
    selKst                       : nominal K* -> K pi Selection object
    selKstWide                   : K* -> K pi Selection object with wide K* mass window
    selBs2PhiGammaWideBMass      : Bs -> Phi Gamma Selection object with wide Bs mass window
    selBs2PhiGammaLooseDira      : Bs -> Phi Gamma Selection object with loose direction angle cut
    selBs2PhiGamma               : nominal Bs -> Phi Gamma Selection object with wide Bs mass window
    selBd2KstGammaWideBMass      : B0 -> K* Gamma Selection object with wide Bs mass window
    selBd2KstGammaWideKstMass    : B0 -> K* Gamma Selection object with wide K* mass window
    selBd2KstGammaLooseDira      : B0 -> K* Gamma Selection object with loose direction angle cut
    selBd2KstGammaWide           : B0 -> K* Gamma Selection object with wide mass windows for K* and B
    selBd2KstGamma               : nominal B0 -> K* Gamma object Object 
    Bs2PhiGammaWideBMassLine     : Stripping line out of selBs2PhiGammaWideBMass
    Bs2PhiGammaWideLooseDiraLine : Stripping line out of selBs2PhiGammaLooseDira
    Bs2PhiGammaLine              : Stripping line out of selBs2PhiGamma
    Bd2KstGammaWideBMassLine     : Stripping line out of selBd2KstGammaWideBMass
    Bd2KstGammaWideKstMassLine   : Stripping line out of selBd2KstGammaWideKstMass
    Bd2KstGammaLooseDiraLine     : Stripping line out of selBd2KstGammaLooseDira
    Bd2KstGammaWide              : Stripping line out of selBd2KstGammaWide
    Bd2KstGamma                  : Stripping line out of selBd2KstGamma
    lines                  : List of lines

    Exports as class data member:
    Beauty2XGammaExclusiveConf.__configuration_keys__ : List of required configuration parameters.    
    """
    __configuration_keys__ = ('TrIPchi2'
                              ,'TrChi2'
                              ,'PhiMassWin'
                              ,'KstMassWin'
                              ,'PhiVCHI2'
                              ,'KstVCHI2'
                              ,'photonPT'
                              ,'BsMassWin'
                              ,'B0MassWin'
                              ,'BsDirAngle'
                              ,'B0DirAngle'
                              ,'BsPVIPchi2'
                              ,'B0PVIPchi2'
                              # Pre- and postscales
                              ,'Bs2PhiGammaPreScale'
                              ,'Bs2PhiGammaPostScale'
                              ,'Bd2KstGammaPreScale'
                              ,'Bd2KstGammaPostScale'
                              )
    
    def __init__(self, name, config):
        LineBuilder.__init__(self, name, config)
        # Selection of B daughters: photon, phi and kstar
        self.selPhoton = makePhoton('PhotonSel' + self.default_name,
                                    config['photonPT'])
        self.selPhi2KK = makePhi2KK('PhiSel' + self.default_name,
                                    config['TrIPchi2'],
                                    config['TrChi2'],
                                    config['PhiMassWin'],
                                    config['PhiVCHI2'])
        self.selKst = makeKstar('KStarSel' + self.default_name,
                                config['TrIPchi2'],
                                config['TrChi2'],
                                config['KstMassWin'],
                                config['KstVCHI2'])
        # Bs->Phi Gamma selections
        self.selBs2PhiGamma = makeBs2PhiGamma(self.default_name + '_Bs2PhiGamma',
                                              self.selPhi2KK,
                                              self.selPhoton,
                                              config['BsDirAngle'],
                                              config['BsPVIPchi2'],
                                              config['BsMassWin'])
        # Bd->Kst Gamma selections
        self.selBd2KstGamma = makeBd2KstGamma(self.default_name + '_Bd2KstGamma',
                                              self.selKst,
                                              self.selPhoton,
                                              config['B0DirAngle'],
                                              config['B0PVIPchi2'],
                                              config['B0MassWin'])
        # Stripping lines
        self.Bs2PhiGammaLine = StrippingLine(self.default_name + '_Bs2PhiGamma_Line',
                                             prescale=config['Bs2PhiGammaPreScale'],
                                             postscale=config['Bs2PhiGammaPostScale'],
                                             RequiredRawEvents = [ "Other","Calo", "Muon","Rich" ],
                                             selection=self.selBs2PhiGamma)
        self.registerLine(self.Bs2PhiGammaLine)

        self.Bd2KstGammaLine = StrippingLine(self.default_name + '_Bd2KstGamma_Line',
                                             prescale=config['Bd2KstGammaPreScale'],
                                             postscale=config['Bd2KstGammaPostScale'],
                                             RequiredRawEvents = [ "Other","Calo", "Muon","Rich" ],
                                             selection=self.selBd2KstGamma)
        self.registerLine(self.Bd2KstGammaLine)
        
def makePhoton(name, photonPT):
    """Create photon Selection object starting from DataOnDemand 'Phys/StdLooseAllPhotons'.

    @arg name: name of the Selection.
    @arg photonPT: PT of the photon
    
    @return: Selection object
    
    """
    # Configure clusterization
    #from Configurables import CaloClusterizationTool, CellularAutomatonAlg
    #clust = CellularAutomatonAlg("EcalClust")
    #clust.addTool(CaloClusterizationTool,'CaloClusterizationTool')
    #clust.CaloClusterizationTool.ETcut = 300
    #clust.CaloClusterizationTool.withET = True
    # Prepare selection
    _code = "(PT> %(photonPT)s*MeV)" % locals()
    _gammaFilter = FilterDesktop(Code=_code)
    _stdGamma = StdLooseAllPhotons
    return Selection(name, Algorithm=_gammaFilter, RequiredSelections=[_stdGamma])

def makePhi2KK(name, TrIPchi2Phi, TrChi2, PhiMassWin, PhiVCHI2) :
    """
    Create and return a Phi->KK Selection object, starting from DataOnDemand 'Phys/StdLoosePhi2KK'.
    
    @arg name: name of the Selection.
    @arg TrIPchi2Phi: minimum IP chi2 of the K+ tracks
    @arg TrChi2: minimum chi2 of the K+ tracks
    @arg PhiMassWin: selected Phi mass window
    @arg PhiVCHI2: vertex chi2 of the Phi
    
    @return: Selection object
    
    """
    _preambulo = ["goodKaon = ((MIPCHI2DV(PRIMARY) > %(TrIPchi2Phi)s) & (TRCHI2DOF < %(TrChi2)s))" % locals(),
                  "goodPhi = (((VFASPF(VCHI2/VDOF) < %(PhiVCHI2)s)) & (ADMASS('phi(1020)') < %(PhiMassWin)s*MeV))" % locals()]
    _code = 'goodPhi & CHILDCUT( goodKaon, 1 ) & CHILDCUT( goodKaon, 2 )'
    _phiFilter = FilterDesktop(Preambulo=_preambulo, Code=_code)
    _stdPhi2KK = DataOnDemand(Location="Phys/StdLoosePhi2KK/Particles")
    return Selection(name, Algorithm=_phiFilter, RequiredSelections=[_stdPhi2KK])

def makeKstar(name, TrIPchi2Kst, TrChi2, KstMassWin, KstVCHI2) :
    """
    Create and return a K*->Kpi Selection object, starting from DataOnDemand 'Phys/StdVeryLooseDetachedKst2Kpi'.
    
    @arg name: name of the Selection.
    @arg TrIPchi2Kst: tracks IP chi2
    @arg TrChi2: tracks chi2
    @arg KstMassWin: K* mass window
    @arg KstVCHI2: vertex chi2 of the K*
    
    @return: Selection object
    
    """
    _preambulo = ["goodTrack = ((MIPCHI2DV(PRIMARY) > %(TrIPchi2Kst)s) & (TRCHI2DOF < %(TrChi2)s))" % locals(),
                  "goodKstar = (((VFASPF(VCHI2/VDOF) < %(KstVCHI2)s)) & (ADMASS('K*(892)0') < %(KstMassWin)s*MeV))" % locals()]
    _code = "goodKstar & CHILDCUT( goodTrack , 1 ) & CHILDCUT( goodTrack , 2 )"
    _kstFilter = FilterDesktop(Preambulo=_preambulo, Code=_code)
    _stdKst2Kpi = DataOnDemand(Location="Phys/StdVeryLooseDetachedKst2Kpi/Particles")
    return Selection(name, Algorithm=_kstFilter, RequiredSelections=[_stdKst2Kpi])

def makeBs2PhiGamma(name, phiSel, gammaSel, BsDirAngle, BsPVIPchi2, BsMassWin):
    """
    Create and return a Bs -> Phi Gamma Selection object, starting with the daughters' selections.
  
    @arg name: name of the Selection.
    @arg phiSel: Phi -> K+ K+ selection
    @arg gammaSel: photon selection
    @arg BsDirAngle: direction angle of the Bs wrt the PV
    @arg BsPVIPchi2: IP chi2 of the Bs wrt the PV
    @arg BsMassWin: Bs mass window
    
    @return: Selection object
    
    """  
    _motherCut = "(acos(BPVDIRA) < %(BsDirAngle)s) & (BPVIPCHI2() < %(BsPVIPchi2)s) & (ADMASS('B_s0')<%(BsMassWin)s*MeV)" % locals()
    _combinationCut = "(ADAMASS('B_s0')<1.5*%(BsMassWin)s*MeV)"  % locals()
    _Bs = CombineParticles(DecayDescriptor="B_s0 -> phi(1020) gamma",
                           CombinationCut=_combinationCut,
                           MotherCut=_motherCut,
                           ReFitPVs=False)#True)
    #_Bs.addTool(OfflineVertexFitter)
    return Selection(name, Algorithm=_Bs, RequiredSelections=[gammaSel, phiSel])

def makeBd2KstGamma(name, kstSel, gammaSel, B0DirAngle, B0PVIPchi2, B0MassWin):
    """
    Create and return a Bd -> K* Gamma Selection object, starting with the daughters' selections.

    @arg name: name of the Selection.
    @arg kstSel: K* -> K pi selection
    @arg gammaSel: photon selection
    @arg B0DirAngle: direction angle of the B0 wrt the PV
    @arg B0PVIPchi2: IP chi2 of the B0 wrt the PV
    @arg B0MassWin: B0 mass window
    
    @return: Selection object

    """  
    _combinationCut = "ADAMASS('B0') < 1.5*%(B0MassWin)s*MeV" % locals()
    _motherCut = "(acos(BPVDIRA) <%(B0DirAngle)s) & (BPVIPCHI2() < %(B0PVIPchi2)s) & (ADMASS('B0')<%(B0MassWin)s*MeV)" % locals()
    _Bd = CombineParticles(DecayDescriptor="[B0 -> K*(892)0 gamma]cc",
                           CombinationCut=_combinationCut,
                           MotherCut=_motherCut,
                           ReFitPVs=False)#True)
    #_Bd.addTool(OfflineVertexFitter)
    #_Bd.VertexFitters.update({"": "OfflineVertexFitter"})
    #return Selection(name, Algorithm=_Bd, RequiredSelections=[kstSel, gammaSel])
    return Selection(name, Algorithm=_Bd, RequiredSelections=[gammaSel, kstSel])
        
# EOF
