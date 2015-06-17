from GaudiKernel.SystemOfUnits import GeV, MeV, picosecond, mm
from Hlt2Lines.Utilities.Hlt2LinesConfigurableUser import Hlt2LinesConfigurableUser

class CharmHadD2HHHLines() :
    def localcuts(self) : 
        return { # Now the combiner for the CPV lines 
                 'Dpm2HHH' : {
                                 'Trk_ALL_MIPCHI2DV_MIN'    :  4.0,
                                 'Trk_2OF3_MIPCHI2DV_MIN'   :  10.0,
                                 'Trk_1OF3_MIPCHI2DV_MIN'   :  50.0,
                                 'Trk_ALL_PT_MIN'           :  200.0 * MeV,
                                 'Trk_2OF3_PT_MIN'          :  400.0 * MeV,
                                 'Trk_1OF3_PT_MIN'          :  1000.0 * MeV,
                                 'BPVVDCHI2_MIN'            :  20.0,
                                 'BPVLTIME_MIN'             :  0.3 * picosecond,
                                 'BPVDIRA_MIN'              :  0.99995,
                                 'ASUMPT_MIN'               :  3000 * MeV,
                                 'AM_MIN'                   :  1779 * MeV,
                                 'AM_MAX'                   :  1959 * MeV,
                                 'Mass_M_MIN'               :  1789.0 * MeV,
                                 'Mass_M_MAX'               :  1949.0 * MeV,
                                },
                 'Ds2HHH' : {
                                 'Trk_ALL_MIPCHI2DV_MIN'    :  4.0,
                                 'Trk_2OF3_MIPCHI2DV_MIN'   :  10.0,
                                 'Trk_1OF3_MIPCHI2DV_MIN'   :  50.0,
                                 'Trk_ALL_PT_MIN'           :  200.0 * MeV,
                                 'Trk_2OF3_PT_MIN'          :  400.0 * MeV,
                                 'Trk_1OF3_PT_MIN'          :  1000.0 * MeV,
                                 'BPVVDCHI2_MIN'            :  20.0,
                                 'BPVLTIME_MIN'             :  0.3 * picosecond,
                                 'BPVDIRA_MIN'              :  0.99995,
                                 'ASUMPT_MIN'               :  3000 * MeV,
                                 'AM_MIN'                   :  1879 * MeV,
                                 'AM_MAX'                   :  2059 * MeV,
                                 'Mass_M_MIN'               :  1889.0 * MeV,
                                 'Mass_M_MAX'               :  2049.0 * MeV,
                                }, 
                 ## The masses of Lambda_c+ and Xi_c+ are separated by 181 MeV,
                 ##   so the mass windows are adjacent unless they are very
                 ##   narrow.  It makes sense to do joint combinatorics.
                 'LcXic2HHH' : {
                                 'Trk_ALL_MIPCHI2DV_MIN'    :  6.0,
                                 'Trk_2OF3_MIPCHI2DV_MIN'   :  9.0,
                                 'Trk_1OF3_MIPCHI2DV_MIN'   :  16.0,
                                 'Trk_ALL_PT_MIN'           :  200.0 * MeV,
                                 'Trk_2OF3_PT_MIN'          :  400.0 * MeV,
                                 'Trk_1OF3_PT_MIN'          :  1000.0 * MeV,
                                 'BPVVDCHI2_MIN'            :  25.0,
                                 'BPVLTIME_MIN'             :  0.15 * picosecond,
                                 'BPVDIRA_MIN'              :  0.99995,
                                 'ASUMPT_MIN'               :  3000 * MeV,
                                 'AM_MIN'                   :  2201 * MeV,
                                 'AM_MAX'                   :  2553. * MeV,
                                 'Mass_M_MIN'               :  2211.0 * MeV,
                                 'Mass_M_MAX'               :  2543.0 * MeV,
                                },
                 'Lc2HHH'    : {
                                 'Mass_M_MIN'               :  2211.0 * MeV,
                                 'Mass_M_MAX'               :  2362.0 * MeV,
                                },
                 'Xic2HHH'   : {
                                 'Mass_M_MIN'               :  2392.0 * MeV,
                                 'Mass_M_MAX'               :  2543.0 * MeV,
                                },

                 # The combiner for the lifetime unbiased lines 
                 'Dpm2HHH_LTUNB' : {
                                 'TisTosSpec'               : "Hlt1Track.*Decision%TIS",
                                 'BPVLTIME_MIN'             :  0.5 * picosecond,
                                 'BPVDIRA_MIN'              :  0.9,
                                 'ASUMPT_MIN'               :  3000 * MeV,
                                 'AM_MIN'                   :  1779 * MeV,
                                 'AM_MAX'                   :  1959 * MeV,
                                 'Mass_M_MIN'               :  1789.0 * MeV,
                                 'Mass_M_MAX'               :  1949.0 * MeV,
                                },
                 'Ds2HHH_LTUNB' : { 
                                 'TisTosSpec'               : "Hlt1Track.*Decision%TIS",
                                 'BPVLTIME_MIN'             :  0.25 * picosecond,
                                 'BPVDIRA_MIN'              :  0.9,
                                 'ASUMPT_MIN'               :  3000 * MeV,
                                 'AM_MIN'                   :  1879 * MeV,
                                 'AM_MAX'                   :  2059 * MeV,
                                 'Mass_M_MIN'               :  1889.0 * MeV,
                                 'Mass_M_MAX'               :  2049.0 * MeV,
                                },
                 # Wide mass window accomodates the Xi_c+ 
                 'Lc2HHH_LTUNB'    : {
                                 'TisTosSpec'               : "Hlt1Track.*Decision%TIS",
                                 'BPVLTIME_MIN'             :  0.15 * picosecond,
                                 'BPVDIRA_MIN'              :  0.95,
                                 'ASUMPT_MIN'               :  5000. * MeV,
                                 'AM_MIN'                   :  2201. * MeV,
                                 'AM_MAX'                   :  2553. * MeV,
                                 'Mass_M_MIN'               :  2211.0 * MeV,
                                 'Mass_M_MAX'               :  2543.0 * MeV,
                                },
                 # The combiner for a KPi asymmetry line
                 'Dpm2KPiPi_ForKPiAsym' : { 
                                 'Trk_ALL_MIPCHI2DV_MIN'    :  25.0,
                                 'Trk_2OF3_MIPCHI2DV_MIN'   :  49.0,
                                 'Trk_1OF3_MIPCHI2DV_MIN'   :  100.0,
                                 'Trk_ALL_PT_MIN'           :  200.0 * MeV,
                                 'Trk_2OF3_PT_MIN'          :  200.0 * MeV,
                                 'Trk_1OF3_PT_MIN'          :  200.0 * MeV,
                                 'BPVVDCHI2_MIN'            :  0.0,
                                 'BPVLTIME_MIN'             :  0.5 * picosecond,
                                 'BPVDIRA_MIN'              :  0.9999,
                                 'ASUMPT_MIN'               :  1000 * MeV,
                                 'AM_MIN'                   :  1809 * MeV,
                                 'AM_MAX'                   :  1929 * MeV,
                                 'Mass_M_MIN'               :  1819.0 * MeV,
                                 'Mass_M_MAX'               :  1919.0 * MeV,
                                }, 
                }
    
    def locallines(self):
        from Stages import MassFilter,TagDecay
        from Stages import D2KPiPi_SS_LTUNB,D2KKPi_OS_LTUNB,Lc2KPPi_LTUNB
        from Stages import DetAsym_DpToKmPipPip
        from Stages import SharedSoftTagChild_pi
        #
        from Stages import D2HHH_DpToKmPipPip, D2HHH_DpToKpPimPip
        from Stages import D2HHH_DpToKmKpPip
        from Stages import D2HHH_DpToPimPipPip, D2HHH_DpToKmKpKp
        #
        from Stages import D2HHH_DspToKpPimPip
        from Stages import D2HHH_DspToKpKpPim,   D2HHH_DspToKmKpPip
        from Stages import D2HHH_DspToPimPipPip, D2HHH_DspToKmKpKp
        #
        from Stages import LcXic2HHH_LcpToKmPpPip, LcXic2HHH_LcpToKmPpKp
        from Stages import LcXic2HHH_LcpToPimPpPip, LcXic2HHH_LcpToPimPpKp
        
        stages = {# First the CPV D+ -> HHH lines, does not include KpKpPim as D+ does not
                  # decay to same-charge kaons
                  'DpToKmPipPipTurbo'        : [MassFilter('DpToKmPipPip', nickname = 'Dpm2HHH',
                                                           inputs=[D2HHH_DpToKmPipPip], shared = True, reFitPVs = True)],
                  'DpToKpPimPipTurbo'        : [MassFilter('Dpm2HHH', inputs=[D2HHH_DpToKpPimPip], reFitPVs = True)],
                  'DpToKmKpPipTurbo'         : [MassFilter('Dpm2HHH', inputs=[D2HHH_DpToKmKpPip], reFitPVs = True)],
                  'DpToPimPipPipTurbo'       : [MassFilter('Dpm2HHH', inputs=[D2HHH_DpToPimPipPip], reFitPVs = True)],
                  'DpToKmKpKpTurbo'          : [MassFilter('Dpm2HHH', inputs=[D2HHH_DpToKmKpKp], reFitPVs = True)],
                  # Second the CPV D_s+ -> HHH lines, does not include KmPipPip as D_s+ does not
                  # decay to same-charge pions
                  'DspToKpKpPimTurbo'        : [MassFilter('Ds2HHH', inputs=[D2HHH_DspToKpKpPim], reFitPVs = True)],
                  'DspToKpPimPipTurbo'       : [MassFilter('Ds2HHH', inputs=[D2HHH_DspToKpPimPip], reFitPVs = True)],
                  'DspToKmKpPipTurbo'        : [MassFilter('DspToKmKpPip', nickname = 'Ds2HHH', 
                                                      inputs=[D2HHH_DspToKmKpPip], shared = True, reFitPVs = True)],
                  'DspToPimPipPipTurbo'      : [MassFilter('Ds2HHH', inputs=[D2HHH_DspToPimPipPip], reFitPVs = True)],
                  'DspToKmKpKpTurbo'         : [MassFilter('Ds2HHH', inputs=[D2HHH_DspToKmKpKp], reFitPVs = True)],
                  # Now the CPV Lc2HHH lines
                  'LcpToKmPpPipTurbo'        : [MassFilter('LcpToKmPpPip', nickname = 'Lc2HHH',
                                                    inputs=[LcXic2HHH_LcpToKmPpPip], shared = True,
                                                           reFitPVs = True)],
                  'LcpToKmPpKpTurbo'         : [MassFilter('Lc2HHH',inputs=[LcXic2HHH_LcpToKmPpKp], reFitPVs = True)],
                  'LcpToPimPpPipTurbo'       : [MassFilter('Lc2HHH',inputs=[LcXic2HHH_LcpToPimPpPip], reFitPVs = True)],
                  'LcpToPimPpKpTurbo'        : [MassFilter('Lc2HHH',inputs=[LcXic2HHH_LcpToPimPpKp], reFitPVs = True)], 
                  ## Xi_c+ -> p K- pi+, DO NOT SEND TO TURBO.
                  'XicpToKmPpPip'        : [MassFilter('XicpToKmPpPip', nickname = 'Xic2HHH',
                                                    inputs=[LcXic2HHH_LcpToKmPpPip], reFitPVs = True)],
                  # Now the three CF lifetime unbiased lines
                  'D2KPiPi_SS_LTUNBTurbo'    : [MassFilter('Dpm2HHH_LTUNB',inputs=[D2KPiPi_SS_LTUNB('Dpm2HHH_LTUNB')], 
                                                           reFitPVs = True)],
                  'D2KKPi_OS_LTUNBTurbo'     : [MassFilter('Ds2HHH_LTUNB',inputs=[D2KKPi_OS_LTUNB('Ds2HHH_LTUNB')], 
                                                           reFitPVs = True)],
                  'LcpToKmPpPip_LTUNBTurbo' : [MassFilter('Lc2HHH_LTUNB',inputs=[Lc2KPPi_LTUNB('Lc2HHH_LTUNB')],
                                                          reFitPVs = True)],
                  # Now the KPi asymmetry line
                  'Dpm2KPiPi_ForKPiAsym'     : [MassFilter('Dpm2KPiPi_ForKPiAsym',
                                                            inputs=[DetAsym_DpToKmPipPip], reFitPVs = True)]
            }

        stages['DspToKmKpPip'] = stages['DspToKmKpPipTurbo']
        
        return stages
