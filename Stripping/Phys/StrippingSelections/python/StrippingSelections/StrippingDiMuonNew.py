'''
Inclusive DiMuon lines based on the lines by Gaia Lanfranchi, Alessio Sarti,
with inputs from Joel Bressieux, Giulia Manca, Matthew Needham and Patrick Robbe.

Including the following lines:
1. DiMuonLine
   FullDST (keep 10%) and MicroDST 
2. DiMuonSameSignLine
   FullDST (keep  1%) and MicroDST (keep 10%) 
3. DiMuonHighMassLine                     
4. DiMuonHighMassSameSignLine             
5. DiMuonLowMassLine
   Keep Hlt2UnbiasedDiMuonLowMassDecision triggered events 
6. Jpsi2MuMuLine
7. Psi2MuMuLine
8. DiMuonExclusiveLine
   Selection is the same as DiMuonLine at present,
   a cut of "1mm upstream of any PV" applied further  
9. DiMuonNoPVLine
   Using the same selection as DiMuonExclusiveLine at present (except the PV cut)
   Requring no reconstructed PV.

More details can be found here:
http://indico.cern.ch/contributionDisplay.py?contribId=2&confId=100755

--------------------------
To include lines for DiMuon stream
--------------------------
from StrippingSelections.StrippingDiMuonNew import DiMuonConf
DiMuonStreamDiMuonConf = DiMuonConf( config = DiMuonConf.config_default )
stream.appendLines( DiMuonStreamDiMuonConf.DiMuonStreamLines )

--------------------------
To include all lines in Full DST Leptonic stream 
--------------------------
from StrippingSelections.StrippingDiMuonNew import DiMuonConf
FullDSTDiMuonConf = DiMuonConf( name= 'FullDST', config = DiMuonConf.config_default )
stream.appendLines( FullDSTDiMuonConf.MicroDSTLines  )

--------------------------
For MicroDST
--------------------------
from StrippingSelections.StrippingDiMuonNew import DiMuonConf
MicroDSTDiMuonConf = DiMuonConf( name = 'MicroDST', config = DiMuonConf.config_microDST )
stream.appendLines( MicroDSTDiMuonConf.MicroDSTLines )
'''

__author__=['Jibo He']
__date__ = '30/09/2010'
__version__= '$Revision: 1.0 $'

__all__ = (
    'DiMuonConf'
    )

from Gaudi.Configuration import *
from Configurables import FilterDesktop, CombineParticles, LoKi__VoidFilter
from PhysSelPython.Wrappers import Selection, DataOnDemand, EventSelection
from StrippingConf.StrippingLine import StrippingLine
from StrippingSelections.Utils import checkConfig

class DiMuonConf(object):
    
    __configuration_keys__ = (
        # DiMuon line
        'DiMuon_Prescale',
        'DiMuon_Postscale',
        'DiMuon_checkPV',
        
        'DiMuon_MuonPT',
        'DiMuon_MuonP',
        'DiMuon_MuonTRCHI2DOF', 
        'DiMuon_MinMass',
        'DiMuon_VCHI2PDOF',
        'DiMuon_PT',

        # DiMuon Same Sign line
        'DiMuonSameSign_Prescale',
        'DiMuonSameSign_Postscale',
        'DiMuonSameSign_checkPV',
        
        # DiMuonExclusive line
        'DiMuonExclusive_Prescale',
        'DiMuonExclusive_Postscale',
        'DiMuonExclusive_checkPV',
        
        'DiMuonExclusive_MuonPT',
        'DiMuonExclusive_MuonP',
        'DiMuonExclusive_MuonTRCHI2DOF', 
        'DiMuonExclusive_MinMass',
        'DiMuonExclusive_VCHI2PDOF',
        'DiMuonExclusive_PT',
        'DiMuonExclusive_DZ',

        # DiMuonNoPV line
        'DiMuonNoPV_Prescale',
        'DiMuonNoPV_Postscale',
        
        # DiMuon High Mass line
        'DiMuonHighMass_Prescale',
        'DiMuonHighMass_Postscale',
        'DiMuonHighMass_checkPV',
        
        'DiMuonHighMass_MuonPT',
        'DiMuonHighMass_MuonP',
        'DiMuonHighMass_MuonTRCHI2DOF', 
        'DiMuonHighMass_MinMass',
        'DiMuonHighMass_VCHI2PDOF',
        'DiMuonHighMass_PT',

        # DiMuonHighMassSameSign line
        'DiMuonHighMassSameSign_Prescale',
        'DiMuonHighMassSameSign_Postscale',
        'DiMuonHighMassSameSign_checkPV',

        # DiMuon Low Mass line
        'DiMuonLowMass_Prescale',
        'DiMuonLowMass_Postscale',
        'DiMuonLowMass_checkPV',
        
        'DiMuonLowMass_MuonPT',
        'DiMuonLowMass_MuonP',
        'DiMuonLowMass_MuonTRCHI2DOF', 
        'DiMuonLowMass_MinMass',
        'DiMuonLowMass_VCHI2PDOF',       
        'DiMuonLowMass_PT',

        # Tight Jpsi line
        'Jpsi2MuMu_Prescale',
        'Jpsi2MuMu_Postscale',
        'Jpsi2MuMu_checkPV',
        
        'Jpsi2MuMu_MuonPT',
        'Jpsi2MuMu_MuonP',
        'Jpsi2MuMu_MuonTRCHI2DOF', 
        'Jpsi2MuMu_MinMass',
        'Jpsi2MuMu_MaxMass',
        'Jpsi2MuMu_VCHI2PDOF',
        'Jpsi2MuMu_PT',
        
        # Tight Psi(2S) line
        'Psi2MuMu_Prescale',
        'Psi2MuMu_Postscale',
        'Psi2MuMu_checkPV',

        'Psi2MuMu_ParticleName', 
        'Psi2MuMu_MuonPT',
        'Psi2MuMu_MuonP',
        'Psi2MuMu_PIDmu',
        'Psi2MuMu_MuonTRCHI2DOF', 
        'Psi2MuMu_MassWindow',
        'Psi2MuMu_VCHI2PDOF',
        'Psi2MuMu_PT'        
        )
    
    config_default= {
        # DiMuon line
        'DiMuon_Prescale'                          :     1.   ,
        'DiMuon_Postscale'                         :     1.   ,
        'DiMuon_checkPV'                           : False    ,  
        
        'DiMuon_MuonPT'                            :   650.   ,  # MeV
        'DiMuon_MuonP'                             : -8000.   ,  # MeV, no cut now 
        'DiMuon_MuonTRCHI2DOF'                     :     5.   , 
        'DiMuon_MinMass'                           :  2900.   ,  # MeV
        'DiMuon_VCHI2PDOF'                         :    20.   , 
        'DiMuon_PT'                                : -1000.   ,  # MeV, no cut now 

        # DiMuon Same Sign line
        'DiMuonSameSign_Prescale'                  :     0.1  ,
        'DiMuonSameSign_Postscale'                 :     1.   ,
        'DiMuonSameSign_checkPV'                   : False    ,

        # DiMuonExclusive line
        'DiMuonExclusive_Prescale'                 :     1.   ,
        'DiMuonExclusive_Postscale'                :     1.   ,
        'DiMuonExclusive_checkPV'                  :  True    ,
        
        'DiMuonExclusive_MuonPT'                   :   650.   ,  # MeV
        'DiMuonExclusive_MuonP'                    : -8000.   ,  # MeV, no cut now 
        'DiMuonExclusive_MuonTRCHI2DOF'            :     5.   , 
        'DiMuonExclusive_MinMass'                  :  2900.   ,
        'DiMuonExclusive_VCHI2PDOF'                :    20.   ,
        'DiMuonExclusive_PT'                       : -1000.   ,  # MeV, no cut now 
        'DiMuonExclusive_DZ'                       :    -1.   ,  # mm, upstream of any PV

        # DiMuonNoPV line
        'DiMuonNoPV_Prescale'                      :     1.   ,
        'DiMuonNoPV_Postscale'                     :     1.   ,
        
        # DiMuon High Mass line
        'DiMuonHighMass_Prescale'                  :     1.   ,
        'DiMuonHighMass_Postscale'                 :     1.   ,
        'DiMuonHighMass_checkPV'                   : False    ,
        
        'DiMuonHighMass_MuonPT'                    :   650.   , # MeV
        'DiMuonHighMass_MuonP'                     : -8000.   , 
        'DiMuonHighMass_MuonTRCHI2DOF'             :     5.   , 
        'DiMuonHighMass_MinMass'                   :  8000.   , # MeV
        'DiMuonHighMass_VCHI2PDOF'                 :    20.   ,
        'DiMuonHighMass_PT'                        : -1000.   , # MeV, no cut now 

        # DiMuon High Mass Same Sign line
        'DiMuonHighMassSameSign_Prescale'          :     1.   ,
        'DiMuonHighMassSameSign_Postscale'         :     1.   ,
        'DiMuonHighMassSameSign_checkPV'           : False    ,

        # DiMuon Low Mass line
        'DiMuonLowMass_Prescale'                   :     1.   ,
        'DiMuonLowMass_Postscale'                  :     1.   ,
        'DiMuonLowMass_checkPV'                    : False    , 
        
        'DiMuonLowMass_MuonPT'                     :   650.   ,  # MeV
        'DiMuonLowMass_MuonP'                      : -8000.   ,  # MeV, no cut now
        'DiMuonLowMass_MuonTRCHI2DOF'              :     5.   , 
        'DiMuonLowMass_MinMass'                    :   500.   ,  # MeV
        'DiMuonLowMass_VCHI2PDOF'                  :    20.   ,       
        'DiMuonLowMass_PT'                         : -1000.   ,  # MeV, no cut now

        # Jpsi2MuMu line
        'Jpsi2MuMu_Prescale'                       :     1.   ,
        'Jpsi2MuMu_Postscale'                      :     1.   ,
        'Jpsi2MuMu_checkPV'                        : False    ,
        
        'Jpsi2MuMu_MuonPT'                         :   650.   ,  # MeV
        'Jpsi2MuMu_MuonP'                          : -8000.   ,  # MeV, no cut now
        'Jpsi2MuMu_MuonTRCHI2DOF'                  :     5.   , 
        'Jpsi2MuMu_MinMass'                        :  3010.   ,  # MeV
        'Jpsi2MuMu_MaxMass'                        :  3170.   ,  # MeV
        'Jpsi2MuMu_VCHI2PDOF'                      :    20.   ,
        'Jpsi2MuMu_PT'                             :  1500.   ,  # MeV

        # Psi2MuMu line
        'Psi2MuMu_Prescale'                        :     1.   ,
        'Psi2MuMu_Postscale'                       :     1.   ,
        'Psi2MuMu_checkPV'                         : False    ,

        'Psi2MuMu_ParticleName'                    : "'psi(2S)'", # Particle Name, like "'psi(2S)'"   
        'Psi2MuMu_MuonPT'                          :  1000.   ,  # MeV
        'Psi2MuMu_MuonP'                           :  8000.   ,  # MeV
        'Psi2MuMu_PIDmu'                           :     0.   , 
        'Psi2MuMu_MuonTRCHI2DOF'                   :     5.   , 
        'Psi2MuMu_MassWindow'                      :   120.   ,  # MeV
        'Psi2MuMu_VCHI2PDOF'                       :    20.   ,
        'Psi2MuMu_PT'                              :  2000.      # MeV  
        }


    config_checkPV= {
        # DiMuon line
        'DiMuon_Prescale'                          :     1.   ,
        'DiMuon_Postscale'                         :     1.   ,
        'DiMuon_checkPV'                           :  True    ,  
        
        'DiMuon_MuonPT'                            :   650.   ,  # MeV
        'DiMuon_MuonP'                             : -8000.   ,  # MeV, no cut now 
        'DiMuon_MuonTRCHI2DOF'                     :     5.   , 
        'DiMuon_MinMass'                           :  2900.   ,  # MeV
        'DiMuon_VCHI2PDOF'                         :    20.   , 
        'DiMuon_PT'                                : -1000.   ,  # MeV, no cut now 

        # DiMuon Same Sign line
        'DiMuonSameSign_Prescale'                  :     0.1  ,
        'DiMuonSameSign_Postscale'                 :     1.   ,
        'DiMuonSameSign_checkPV'                   :  True    ,

        # DiMuonExclusive line
        'DiMuonExclusive_Prescale'                 :     1.   ,
        'DiMuonExclusive_Postscale'                :     1.   ,
        'DiMuonExclusive_checkPV'                  :  True    ,
        
        'DiMuonExclusive_MuonPT'                   :   650.   ,  # MeV
        'DiMuonExclusive_MuonP'                    : -8000.   ,  # MeV, no cut now 
        'DiMuonExclusive_MuonTRCHI2DOF'            :     5.   , 
        'DiMuonExclusive_MinMass'                  :  2900.   ,
        'DiMuonExclusive_VCHI2PDOF'                :    20.   ,
        'DiMuonExclusive_PT'                       : -1000.   ,  # MeV, no cut now 
        'DiMuonExclusive_DZ'                       :    -1.   ,  # mm, upstream of any PV

        # DiMuonNoPV line
        'DiMuonNoPV_Prescale'                      :     1.   ,
        'DiMuonNoPV_Postscale'                     :     1.   ,
                
        # DiMuon High Mass line
        'DiMuonHighMass_Prescale'                  :     1.   ,
        'DiMuonHighMass_Postscale'                 :     1.   ,
        'DiMuonHighMass_checkPV'                   :  True    ,
        
        'DiMuonHighMass_MuonPT'                    :   650.   , # MeV
        'DiMuonHighMass_MuonP'                     : -8000.   , 
        'DiMuonHighMass_MuonTRCHI2DOF'             :     5.   , 
        'DiMuonHighMass_MinMass'                   :  8000.   , # MeV
        'DiMuonHighMass_VCHI2PDOF'                 :    20.   ,
        'DiMuonHighMass_PT'                        : -1000.   , # MeV, no cut now 

        # DiMuon High Mass Same Sign line
        'DiMuonHighMassSameSign_Prescale'          :     1.   ,
        'DiMuonHighMassSameSign_Postscale'         :     1.   ,
        'DiMuonHighMassSameSign_checkPV'           :  True    ,

        # DiMuon Low Mass line
        'DiMuonLowMass_Prescale'                   :     1.   ,
        'DiMuonLowMass_Postscale'                  :     1.   ,
        'DiMuonLowMass_checkPV'                    :  True    , 
        
        'DiMuonLowMass_MuonPT'                     :   650.   ,  # MeV
        'DiMuonLowMass_MuonP'                      : -8000.   ,  # MeV, no cut now
        'DiMuonLowMass_MuonTRCHI2DOF'              :     5.   , 
        'DiMuonLowMass_MinMass'                    :   500.   ,  # MeV
        'DiMuonLowMass_VCHI2PDOF'                  :    20.   ,       
        'DiMuonLowMass_PT'                         : -1000.   ,  # MeV, no cut now

        # Jpsi2MuMu line
        'Jpsi2MuMu_Prescale'                       :     1.   ,
        'Jpsi2MuMu_Postscale'                      :     1.   ,
        'Jpsi2MuMu_checkPV'                        :  True    ,
        
        'Jpsi2MuMu_MuonPT'                         :   650.   ,  # MeV
        'Jpsi2MuMu_MuonP'                          : -8000.   ,  # MeV, no cut now
        'Jpsi2MuMu_MuonTRCHI2DOF'                  :     5.   , 
        'Jpsi2MuMu_MinMass'                        :  3010.   ,  # MeV
        'Jpsi2MuMu_MaxMass'                        :  3170.   ,  # MeV
        'Jpsi2MuMu_VCHI2PDOF'                      :    20.   ,
        'Jpsi2MuMu_PT'                             :  1500.   ,  # MeV

        # Psi2MuMu line
        'Psi2MuMu_Prescale'                        :     1.   ,
        'Psi2MuMu_Postscale'                       :     1.   ,
        'Psi2MuMu_checkPV'                         :  True    ,

        'Psi2MuMu_ParticleName'                    : "'psi(2S)'", # Particle Name, like "'psi(2S)'"   
        'Psi2MuMu_MuonPT'                          :  1000.   ,  # MeV
        'Psi2MuMu_MuonP'                           :  8000.   ,  # MeV
        'Psi2MuMu_PIDmu'                           :     0.   , 
        'Psi2MuMu_MuonTRCHI2DOF'                   :     5.   , 
        'Psi2MuMu_MassWindow'                      :   120.   ,  # MeV
        'Psi2MuMu_VCHI2PDOF'                       :    20.   ,
        'Psi2MuMu_PT'                              :  2000.      # MeV  
        }

    
    config_microDST= {
        # DiMuon line
        'DiMuon_Prescale'                          :     1.   ,
        'DiMuon_Postscale'                         :     1.   ,
        'DiMuon_checkPV'                           :  True    ,  
        
        'DiMuon_MuonPT'                            :   650.   ,  # MeV
        'DiMuon_MuonP'                             : -8000.   ,  # MeV, no cut now 
        'DiMuon_MuonTRCHI2DOF'                     :     5.   , 
        'DiMuon_MinMass'                           :  2900.   ,  # MeV
        'DiMuon_VCHI2PDOF'                         :    20.   , 
        'DiMuon_PT'                                : -1000.   ,  # MeV, no cut now 

        # DiMuon Same Sign line
        'DiMuonSameSign_Prescale'                  :     0.1  ,
        'DiMuonSameSign_Postscale'                 :     1.   ,
        'DiMuonSameSign_checkPV'                   :  True    ,

        # DiMuonExclusive line
        'DiMuonExclusive_Prescale'                 :     1.   ,
        'DiMuonExclusive_Postscale'                :     1.   ,
        'DiMuonExclusive_checkPV'                  :  True    ,
        
        'DiMuonExclusive_MuonPT'                   :   650.   ,  # MeV
        'DiMuonExclusive_MuonP'                    : -8000.   ,  # MeV, no cut now 
        'DiMuonExclusive_MuonTRCHI2DOF'            :     5.   , 
        'DiMuonExclusive_MinMass'                  :  2900.   ,
        'DiMuonExclusive_VCHI2PDOF'                :    20.   ,
        'DiMuonExclusive_PT'                       : -1000.   ,  # MeV, no cut now 
        'DiMuonExclusive_DZ'                       :    -1.   ,  # mm, upstream of any PV

        # DiMuonNoPV line
        'DiMuonNoPV_Prescale'                      :     1.   ,
        'DiMuonNoPV_Postscale'                     :     1.   ,
               
        # DiMuon High Mass line
        'DiMuonHighMass_Prescale'                  :     1.   ,
        'DiMuonHighMass_Postscale'                 :     1.   ,
        'DiMuonHighMass_checkPV'                   :  True    ,
        
        'DiMuonHighMass_MuonPT'                    :   650.   , # MeV
        'DiMuonHighMass_MuonP'                     : -8000.   , 
        'DiMuonHighMass_MuonTRCHI2DOF'             :     5.   , 
        'DiMuonHighMass_MinMass'                   :  8000.   , # MeV
        'DiMuonHighMass_VCHI2PDOF'                 :    20.   ,
        'DiMuonHighMass_PT'                        : -1000.   , # MeV, no cut now 

        # DiMuon High Mass Same Sign line
        'DiMuonHighMassSameSign_Prescale'          :     1.   ,
        'DiMuonHighMassSameSign_Postscale'         :     1.   ,
        'DiMuonHighMassSameSign_checkPV'           :  True    ,

        # DiMuon Low Mass line
        'DiMuonLowMass_Prescale'                   :     1.   ,
        'DiMuonLowMass_Postscale'                  :     1.   ,
        'DiMuonLowMass_checkPV'                    :  True    , 
        
        'DiMuonLowMass_MuonPT'                     :   650.   ,  # MeV
        'DiMuonLowMass_MuonP'                      : -8000.   ,  # MeV, no cut now
        'DiMuonLowMass_MuonTRCHI2DOF'              :     5.   , 
        'DiMuonLowMass_MinMass'                    :   500.   ,  # MeV
        'DiMuonLowMass_VCHI2PDOF'                  :    20.   ,       
        'DiMuonLowMass_PT'                         : -1000.   ,  # MeV, no cut now

        # Jpsi2MuMu line
        'Jpsi2MuMu_Prescale'                       :     1.   ,
        'Jpsi2MuMu_Postscale'                      :     1.   ,
        'Jpsi2MuMu_checkPV'                        :  True    ,
        
        'Jpsi2MuMu_MuonPT'                         :   650.   ,  # MeV
        'Jpsi2MuMu_MuonP'                          : -8000.   ,  # MeV, no cut now
        'Jpsi2MuMu_MuonTRCHI2DOF'                  :     5.   , 
        'Jpsi2MuMu_MinMass'                        :  3010.   ,  # MeV
        'Jpsi2MuMu_MaxMass'                        :  3170.   ,  # MeV
        'Jpsi2MuMu_VCHI2PDOF'                      :    20.   ,
        'Jpsi2MuMu_PT'                             :  1500.0  ,  # MeV

        # Psi2MuMu line
        'Psi2MuMu_Prescale'                        :     1.   ,
        'Psi2MuMu_Postscale'                       :     1.   ,
        'Psi2MuMu_checkPV'                         :  True    ,

        'Psi2MuMu_ParticleName'                    : "'psi(2S)'", # Particle Name, like "'psi(2S)'"   
        'Psi2MuMu_MuonPT'                          :  1000.   ,  # MeV
        'Psi2MuMu_MuonP'                           :  8000.   ,  # MeV
        'Psi2MuMu_PIDmu'                           :     0.   , 
        'Psi2MuMu_MuonTRCHI2DOF'                   :     5.   , 
        'Psi2MuMu_MassWindow'                      :   120.   ,  # MeV
        'Psi2MuMu_VCHI2PDOF'                       :    20.   ,
        'Psi2MuMu_PT'                              :  2000.      # MeV  
        }


    
    def __init__(self,
                name = None,
                config = None
                ):
        
        checkConfig(DiMuonConf.__configuration_keys__, config)

        # if name not set outside, set it to empty 
        if name == None:
            name = ""

        """
        DiMuon line
        """
        self.SelDiMuon = filterDiMuon( name + 'DiMuon',
                                       MuonPT        = config['DiMuon_MuonPT'],
                                       MuonP         = config['DiMuon_MuonP'],
                                       MuonTRCHI2DOF = config['DiMuon_MuonTRCHI2DOF'],
                                       MuMuMinMass   = config['DiMuon_MinMass'],
                                       MuMuVCHI2PDOF = config['DiMuon_VCHI2PDOF'],
                                       MuMuPT        = config['DiMuon_PT']
                                       )

        self.DiMuonLine = StrippingLine( name + 'DiMuonInc' + 'Line',
                                         prescale  = config['DiMuon_Prescale'],
                                         postscale = config['DiMuon_Postscale'],
                                         checkPV   = config['DiMuon_checkPV'],
                                         algos = [self.SelDiMuon]
                                         )

        """
        DiMuon same sign line
        """
        self.SelDiMuonSameSign = filterDiMuonSameSign( name + 'DiMuonSameSign',
                                                       MuonPT        = config['DiMuon_MuonPT'],
                                                       MuonP         = config['DiMuon_MuonP'],
                                                       MuonTRCHI2DOF = config['DiMuon_MuonTRCHI2DOF'],
                                                       MuMuMinMass   = config['DiMuon_MinMass'],
                                                       MuMuVCHI2PDOF = config['DiMuon_VCHI2PDOF'],
                                                       MuMuPT        = config['DiMuon_PT']
                                                       )
        self.DiMuonSameSignLine = StrippingLine( name + 'DiMuonSameSign' + 'Line',
                                                 prescale  = config['DiMuonSameSign_Prescale'],
                                                 postscale = config['DiMuonSameSign_Postscale'],
                                                 checkPV   = config['DiMuonSameSign_checkPV'],
                                                 algos = [self.SelDiMuonSameSign]
                                                 )
        
        """
        DiMuonExclusiveline
        """
        self.SelDiMuonExclusive = filterDiMuonAndDZ( name + 'DiMuonExclusive',
                                                     MuonPT        = config['DiMuonExclusive_MuonPT'],
                                                     MuonP         = config['DiMuonExclusive_MuonP'],
                                                     MuonTRCHI2DOF = config['DiMuonExclusive_MuonTRCHI2DOF'],
                                                     MuMuMinMass   = config['DiMuonExclusive_MinMass'],
                                                     MuMuVCHI2PDOF = config['DiMuonExclusive_VCHI2PDOF'],
                                                     MuMuPT        = config['DiMuonExclusive_PT'],
                                                     MuMuDZ        = config['DiMuonExclusive_DZ']
                                                     )
                
        self.DiMuonExclusiveLine = StrippingLine( name + 'DiMuonExclusive' + 'Line',
                                                  prescale  = config['DiMuonExclusive_Prescale'],
                                                  postscale = config['DiMuonExclusive_Postscale'],
                                                  checkPV   = config['DiMuonExclusive_checkPV'],
                                                  algos = [self.SelDiMuonExclusive]
                                                  )

        """
        DiMuonNoPVline
        """
        self.SelDiMuonNoPV = filterDiMuon( name + 'DiMuonNoPV',
                                           MuonPT        = config['DiMuonExclusive_MuonPT'],
                                           MuonP         = config['DiMuonExclusive_MuonP'],
                                           MuonTRCHI2DOF = config['DiMuonExclusive_MuonTRCHI2DOF'],
                                           MuMuMinMass   = config['DiMuonExclusive_MinMass'],
                                           MuMuVCHI2PDOF = config['DiMuonExclusive_VCHI2PDOF'],
                                           MuMuPT        = config['DiMuonExclusive_PT']
                                           )

        self.DiMuonNoPVLine = StrippingLine( name + 'DiMuonNoPV' + 'Line',
                                             prescale  = config['DiMuonNoPV_Prescale'],
                                             postscale = config['DiMuonNoPV_Postscale'],
                                             checkPV   = False,
                                             FILTER = { 'Code'       : "CONTAINS('Rec/Vertex/Primary')<0.5" , 
                                                        'Preambulo'  : [ 'from LoKiTracks.decorators import *' ,
                                                                         'from LoKiCore.functions    import *' ]
                                                        },
                                             algos = [self.SelDiMuonExclusive]
                                             )       
        
        """
        DiMuon High Mass line
        """
        self.SelDiMuonHighMass = filterDiMuon( name + 'DiMuonHighMass',
                                               MuonPT        = config['DiMuonHighMass_MuonPT'],
                                               MuonP         = config['DiMuonHighMass_MuonP'],
                                               MuonTRCHI2DOF = config['DiMuonHighMass_MuonTRCHI2DOF'],
                                               MuMuMinMass   = config['DiMuonHighMass_MinMass'],
                                               MuMuVCHI2PDOF = config['DiMuonHighMass_VCHI2PDOF'],
                                               MuMuPT        = config['DiMuonHighMass_PT']
                                               )
        self.DiMuonHighMassLine = StrippingLine( name + 'DiMuonHighMass' + 'Line',
                                                 prescale  = config['DiMuonHighMass_Prescale'],
                                                 postscale = config['DiMuonHighMass_Postscale'],
                                                 checkPV   = config['DiMuonHighMass_checkPV'],
                                                 algos = [self.SelDiMuonHighMass]
                                                 )


        
        """
        DiMuon High Mass Same Sign line
        """
        self.SelDiMuonHighMassSameSign = filterDiMuonSameSign( name + 'DiMuonHighMassSameSign',
                                                               MuonPT        = config['DiMuonHighMass_MuonPT'],
                                                               MuonP         = config['DiMuonHighMass_MuonP'],
                                                               MuonTRCHI2DOF = config['DiMuonHighMass_MuonTRCHI2DOF'],
                                                               MuMuMinMass   = config['DiMuonHighMass_MinMass'],
                                                               MuMuVCHI2PDOF = config['DiMuonHighMass_VCHI2PDOF'],
                                                               MuMuPT        = config['DiMuonHighMass_PT']
                                                               )
        
        self.DiMuonHighMassSameSignLine = StrippingLine( name + 'DiMuonHighMassSameSign' + 'Line',
                                                         prescale  = config['DiMuonHighMassSameSign_Prescale'],
                                                         postscale = config['DiMuonHighMassSameSign_Postscale'],
                                                         checkPV   = config['DiMuonHighMassSameSign_checkPV'],
                                                         algos = [self.SelDiMuonHighMassSameSign]
                                                         )


        
        """
        DiMuon Low Mass line
        """
        self.SelDiMuonLowMass = filterDiMuon( name + 'DiMuonLowMass',
                                              MuonPT        = config['DiMuonLowMass_MuonPT'],
                                              MuonP         = config['DiMuonLowMass_MuonP'],
                                              MuonTRCHI2DOF = config['DiMuonLowMass_MuonTRCHI2DOF'],
                                              MuMuMinMass   = config['DiMuonLowMass_MinMass'],
                                              MuMuVCHI2PDOF = config['DiMuonLowMass_VCHI2PDOF'],
                                              MuMuPT        = config['DiMuonLowMass_PT']
                                              )
        self.DiMuonLowMassLine = StrippingLine( name + 'DiMuonLowMass' + 'Line',
                                                HLT = "HLT_PASS('Hlt2UnbiasedDiMuonLowMassDecision')",
                                                prescale  = config['DiMuonLowMass_Prescale'],
                                                postscale = config['DiMuonLowMass_Postscale'],
                                                checkPV   = config['DiMuonLowMass_checkPV'],
                                                algos = [ self.SelDiMuonLowMass ]
                                                )
        
        """
        Jpsi-> mumu tight line
        """
        self.SelJpsi2MuMu = filterJpsi2MuMu( name + 'Jpsi2MuMu',
                                             MuonPT        = config['Jpsi2MuMu_MuonPT'],
                                             MuonP         = config['Jpsi2MuMu_MuonP'],
                                             MuonTRCHI2DOF = config['Jpsi2MuMu_MuonTRCHI2DOF'],
                                             MuMuMinMass   = config['Jpsi2MuMu_MinMass'],
                                             MuMuMaxMass   = config['Jpsi2MuMu_MaxMass'],
                                             MuMuVCHI2PDOF = config['Jpsi2MuMu_VCHI2PDOF'],
                                             MuMuPT        = config['Jpsi2MuMu_PT']
                                             )
        self.Jpsi2MuMuLine = StrippingLine( name + 'Jpsi2MuMu' + 'Line',
                                            prescale  = config['Jpsi2MuMu_Prescale'],
                                            postscale = config['Jpsi2MuMu_Postscale'],
                                            checkPV   = config['Jpsi2MuMu_checkPV'],
                                            algos = [self.SelJpsi2MuMu]
                                            )

        """
        Psi(2S)->mumu tight line
        """
        self.SelPsi2MuMu = filterSignal( name + 'Psi2MuMu',
                                         ParticleName  = config['Psi2MuMu_ParticleName'],
                                         MuonPT        = config['Psi2MuMu_MuonPT'],
                                         MuonP         = config['Psi2MuMu_MuonP'],
                                         MuonPIDmu     = config['Psi2MuMu_PIDmu'],  
                                         MuonTRCHI2DOF = config['Psi2MuMu_MuonTRCHI2DOF'],
                                         MuMuMassWindow= config['Psi2MuMu_MassWindow'],                                          
                                         MuMuVCHI2PDOF = config['Psi2MuMu_VCHI2PDOF'],
                                         MuMuPT        = config['Psi2MuMu_PT']
                                         )

        self.Psi2MuMuLine = StrippingLine( name + 'Psi2MuMu' + 'Line',
                                           prescale  = config['Psi2MuMu_Prescale'],
                                           postscale = config['Psi2MuMu_Postscale'],
                                           checkPV   = config['Psi2MuMu_checkPV'],
                                           algos = [self.SelPsi2MuMu]
                                           )
            
        
        self.Lines = [
            self.DiMuonLine,
            self.DiMuonSameSignLine,
            self.DiMuonExclusiveLine,
            self.DiMuonNoPVLine, 
            self.DiMuonHighMassLine,
            self.DiMuonHighMassSameSignLine,
            self.DiMuonLowMassLine,
            self.Jpsi2MuMuLine,
            self.Psi2MuMuLine
            ]

        self.DiMuonStreamLines = [
            self.DiMuonExclusiveLine,
            self.DiMuonNoPVLine, 
            self.DiMuonHighMassLine,
            self.DiMuonHighMassSameSignLine,
            self.DiMuonLowMassLine,
            self.Jpsi2MuMuLine,
            self.Psi2MuMuLine           
            ]

        self.FullDSTLines = [
            self.DiMuonLine,
            self.DiMuonSameSignLine,
            self.DiMuonExclusiveLine,
            self.DiMuonNoPVLine, 
            self.DiMuonLowMassLine
            ]
        
        self.MicroDSTLines = [
            self.DiMuonLine,
            self.DiMuonSameSignLine            
            ]


def filterDiMuon( name,
                  MuonPT,
                  MuonP,
                  MuonTRCHI2DOF,
                  MuMuMinMass,
                  MuMuVCHI2PDOF,
                  MuMuPT 
                  ):
    
    _StdLooseDiMuon = DataOnDemand( Location = 'Phys/StdLooseDiMuon/Particles' )
    
    MuonCut = "(MINTREE('mu+'==ABSID,PT) > %(MuonPT)s *MeV) & (MINTREE('mu+'==ABSID,P) > %(MuonP)s *MeV) & (MAXTREE('mu+'==ABSID,TRCHI2DOF) < %(MuonTRCHI2DOF)s)" % locals()
    
    MuMuCut = "(MM > %(MuMuMinMass)s) & (VFASPF(VCHI2PDOF)< %(MuMuVCHI2PDOF)s) & (PT > %(MuMuPT)s)" % locals()
    
    _MuMu = FilterDesktop( "_Filter" + name,
                           Code = MuonCut + " & " + MuMuCut )

    return Selection( name + "_SelMuMu",
                      Algorithm = _MuMu,
                      RequiredSelections = [ _StdLooseDiMuon ]
                      )


def filterDiMuonSameSign( name,
                          MuonPT,
                          MuonP,
                          MuonTRCHI2DOF,
                          MuMuMinMass,
                          MuMuVCHI2PDOF,
                          MuMuPT
                          ):
    
    _StdLooseDiMuonSameSign = DataOnDemand( Location = 'Phys/StdLooseDiMuonSameSign/Particles' )
    
    MuonCut = "(MINTREE('mu+'==ABSID,PT) > %(MuonPT)s *MeV) & (MINTREE('mu+'==ABSID,P) > %(MuonP)s *MeV) & (MAXTREE('mu+'==ABSID,TRCHI2DOF) < %(MuonTRCHI2DOF)s)" % locals()
    
    MuMuCut = "(MM > %(MuMuMinMass)s) & (VFASPF(VCHI2PDOF)< %(MuMuVCHI2PDOF)s) & (PT > %(MuMuPT)s *MeV)" % locals()
    
    _MuMu = FilterDesktop( "_Filter" + name,
                           Code = MuonCut + " & " + MuMuCut )
    
    return Selection( name + "_SelMuMuSS",
                      Algorithm = _MuMu,
                      RequiredSelections = [ _StdLooseDiMuonSameSign ]
                      )


def filterJpsi2MuMu( name,
                     MuonPT,
                     MuonP,
                     MuonTRCHI2DOF,
                     MuMuMinMass,
                     MuMuMaxMass, 
                     MuMuVCHI2PDOF,
                     MuMuPT
                     ):
    
    _StdLooseJpsi2MuMu = DataOnDemand( Location = 'Phys/StdLooseJpsi2MuMu/Particles' )
    
    MuonCut = "(MINTREE('mu+'==ABSID,PT) > %(MuonPT)s *MeV) & (MINTREE('mu+'==ABSID,P) > %(MuonP)s *MeV) & (MAXTREE('mu+'==ABSID,TRCHI2DOF) < %(MuonTRCHI2DOF)s)" % locals()
    
    MuMuCut = "(MM > %(MuMuMinMass)s) & (MM < %(MuMuMaxMass)s) & (VFASPF(VCHI2PDOF)< %(MuMuVCHI2PDOF)s) & (PT > %(MuMuPT)s)" % locals()
    
    _MuMu = FilterDesktop( "_Filter" + name,
                           Code = MuonCut + " & " + MuMuCut )
    
    return Selection( name + "_SelJpsi2MuMu",
                      Algorithm = _MuMu,
                      RequiredSelections = [ _StdLooseJpsi2MuMu ]
                      )


def filterSignal( name,
                  ParticleName, 
                  MuonPT,
                  MuonP,
                  MuonPIDmu,
                  MuonTRCHI2DOF,
                  MuMuMassWindow, 
                  MuMuVCHI2PDOF,
                  MuMuPT
                  ):
        
    _StdLooseDiMuon = DataOnDemand( Location = 'Phys/StdLooseDiMuon/Particles' )
    
    MuonCut = "(MINTREE('mu+'==ABSID,PT) > %(MuonPT)s *MeV) & (MINTREE('mu+'==ABSID,P) > %(MuonP)s *MeV) & (MINTREE('mu+'==ABSID,PIDmu) > %(MuonPIDmu)s) & (MAXTREE('mu+'==ABSID,TRCHI2DOF) < %(MuonTRCHI2DOF)s)" % locals()
    
    MuMuCut = "(ADMASS(%(ParticleName)s) < %(MuMuMassWindow)s *MeV) & (VFASPF(VCHI2PDOF)< %(MuMuVCHI2PDOF)s) & (PT > %(MuMuPT)s *MeV)" % locals()
    
    _MuMu = FilterDesktop( "_Filter" + name,
                           Code = MuonCut + " & " + MuMuCut )
    
    return Selection( name + "_SelP2MuMu",
                      Algorithm = _MuMu,
                      RequiredSelections = [ _StdLooseDiMuon ]
                      )


def filterDZ( name,
              DZAnyPV, 
              MySelection ):
    
    return EventSelection (
        #
        LoKi__VoidFilter( name + 'filterDZ', 
                          Code =  " ( minMyZ - minPVZ ) < %(DZAnyPV)s*mm " % locals() ,
                          Preambulo = [ "from LoKiPhys.decorators import *",
                                        "minMyZ = SOURCE('%s') >> min_value( VFASPF(VZ) )"  %(MySelection.outputLocation()) ,
                                        "minPVZ = VSOURCE('Rec/Vertex/Primary') >> min_value(VZ) "
                                        ]
                          )
        )
        


def filterDiMuonAndDZ( name,
                       MuonPT,
                       MuonP,
                       MuonTRCHI2DOF,
                       MuMuMinMass,
                       MuMuVCHI2PDOF,
                       MuMuPT,
                       MuMuDZ
                       ):
    
    _StdLooseDiMuon = DataOnDemand( Location = 'Phys/StdLooseDiMuon/Particles' )
    
    MuonCut = "(MINTREE('mu+'==ABSID,PT) > %(MuonPT)s *MeV) & (MINTREE('mu+'==ABSID,P) > %(MuonP)s *MeV) & (MAXTREE('mu+'==ABSID,TRCHI2DOF) < %(MuonTRCHI2DOF)s)" % locals()
    
    MuMuCut = "(MM > %(MuMuMinMass)s) & (VFASPF(VCHI2PDOF)< %(MuMuVCHI2PDOF)s) & (PT > %(MuMuPT)s) & (BPVVDZ < %(MuMuDZ)s*mm)" % locals()
    
    _MuMu = FilterDesktop( "_Filter" + name,
                           Code = MuonCut + " & " + MuMuCut )

    return Selection( name + "_SelMuMu",
                      Algorithm = _MuMu,
                      RequiredSelections = [ _StdLooseDiMuon ]
                      )
