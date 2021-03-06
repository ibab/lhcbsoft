###########################################################################################
##                          S T R I P P I N G  2 4                                       ##
##                                                                                       ##
##  Configuration for Charm WG                                                           ##
##  Contact person: Eva Gersabeck & Jinlin Fu (evelina.gersabeck@cern.ch & j.fu@cern.ch) ##
###########################################################################################

from GaudiKernel.SystemOfUnits import *

#StrippingB2ppipiSigmacmm_Lcpi.py
B2ppipiSigmacmm_Lcpi = {
    "BUILDERTYPE": "StrippingB2ppipiSigmacmm_Lcpi_Conf", 
    "CONFIG": {
        "ApplyDeltaMCut": False, 
        "ApplyGECs": False, 
        "ApplyGhostProbCut": True, 
        "ApplyPionPIDK": True, 
        "BDIRA": 0.9995, 
        "BFDChi2": 64.0, 
        "BMAXDOCA": 0.2, 
        "BMassWind": 200.0, 
        "BVtxChi2DOF": 5.0, 
        "CheckPV": True, 
        "DelmLower": 0.0, 
        "DelmUpper": 3000.0, 
        "GhostProbCut": 0.5, 
        "Hlt2TOS": {
            "Hlt2.*Decision%TOS": 0
        }, 
        "LcFDChi2": 64.0, 
        "LcMassWind": 100.0, 
        "LcVtxChi2DOF": 5.0, 
        "MaxLongTracks": 200, 
        "PionPIDK": 0.0, 
        "PostscaleComplete": 1.0, 
        "PostscaleCompleteNorm": 1.0, 
        "Postscalefourpart": 1.0, 
        "PrescaleComplete": 1.0, 
        "PrescaleCompleteNorm": 1.0, 
        "Prescalefourpart": 1.0, 
        "SigmaFDChi2": 64.0, 
        "SigmaLcDeltaMhigh": 1000.0, 
        "SigmaLcDeltaMlow": 0.0, 
        "SigmaMAXDOCA": 0.2, 
        "SigmaPT": 150.0, 
        "SigmaVtxChi2DOF": 5.0, 
        "TrackChi2DOF": 3.0, 
        "UseTOS": True, 
        "fourpartFDChi2": 49.0, 
        "fourpartMAXDOCA": 0.15, 
        "fourpartMassHigh": 3000.0, 
        "fourpartMassLow": 1800.0, 
        "fourpartMinIPChi2": 6.0, 
        "fourpartPT": 1000.0, 
        "fourpartVtxChi2DOF": 5.0, 
        "piSigmaMAXP": 10000000.0, 
        "piSigmaMAXPT": 1000000.0, 
        "piSigmaMINIPCHI2": 8.0, 
        "piSigmaMINP": 2000.0, 
        "piSigmaMINPT": 150.0, 
        "pionMINIPCHI2": 8.0, 
        "pionMINP": 5000.0, 
        "pionMINPT": 400.0, 
        "protonMINIPCHI2": 8.0, 
        "protonMINP": 10000.0, 
        "protonMINPIDp": 5.0, 
        "protonMINPIDp_K": -5.0, 
        "protonMINPT": 500.0, 
        "threepartFDChi2": 49.0, 
        "threepartMAXDOCA": 0.15, 
        "threepartMassHigh": 2800.0, 
        "threepartMassLow": 1500.0, 
        "threepartMinIPChi2": 6.0, 
        "threepartPT": 1000.0, 
        "threepartVtxChi2DOF": 5.0
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingChargedHyperons.py
ChargedHyperons = {
    "BUILDERTYPE": "StrippingChargedHyperonsConf", 
    "CONFIG": {
        "Bachelor_BPVIPCHI2_MIN": 100.0, 
        "Bachelor_PT_MIN": 100.0, 
        "Hyperon_BPVLTIME_MIN": 0.005, 
        "KaonPIDK": -5.0, 
        "LambdaDDMassWin": 5.7, 
        "LambdaDDMaxVZ": 2275.0, 
        "LambdaDDMinVZ": 400.0, 
        "LambdaDDVtxChi2Max": 5.0, 
        "LambdaDeltaZ_MIN": 5.0, 
        "LambdaLLMassWin": 5.7, 
        "LambdaLLMaxVZ": 400.0, 
        "LambdaLLMinDecayTime": 0.005, 
        "LambdaLLMinVZ": -100.0, 
        "LambdaLLVtxChi2Max": 5.0, 
        "LambdaPi_PT_MIN": 100.0, 
        "LambdaPr_PT_MIN": 500.0, 
        "OmegaMassWindow": 25.0, 
        "PionPIDK": 5.0, 
        "PostVertexChi2_MIN": 5.0, 
        "ProbNNkMin": 0.2, 
        "ProbNNpMinDD": 0.05, 
        "ProbNNpMinLL": 0.2, 
        "TRCHI2DOFMax": 3.0, 
        "TrGhostProbMax": 0.25, 
        "XiMassWindow": 25.0
    }, 
    "STREAMS": [ "CharmCompleteEvent" ], 
    "WGs": [ "Charm" ]
}

#StrippingCharmForVub.py
CharmForVub = {
    "BUILDERTYPE": "CharmForVubConf", 
    "CONFIG": {
        "B_DIRA": 0.9994, 
        "B_FDCHI2": 100, 
        "BtoD_DeltaMass_MAX": 3700.0, 
        "BtoD_DeltaMass_MIN": 2700.0, 
        "D0_FD": 4.0, 
        "D_BPVVDZ": 0.0, 
        "D_DOCA": 0.07, 
        "D_MCORR_MAX": 2700.0, 
        "D_MCORR_MIN": 1400.0, 
        "D_MassMax": 1950.0, 
        "D_MinP": 20000.0, 
        "D_VtxChi2": 9.0, 
        "Dplus_FD": 4.0, 
        "Dstar_DOCA": 0.4, 
        "Dstar_DeltaMass": 200.0, 
        "Dstar_MassMin": 300.0, 
        "Dstar_VtxChi2": 9.0, 
        "HLT_FILTER_HMuNu": "(HLT_PASS_RE('Hlt2CharmSemilepD02HMuNu_.*Decision') | HLT_PASS_RE('Hlt2CharmHadD02HHXDst_.*Decision'))", 
        "HLT_FILTER_VMuNu": "(HLT_PASS_RE('Hlt2CharmSemilepD02HMuNu_.*Decision') | HLT_PASS_RE('Hlt2Topo.*Decision'))", 
        "HLinePrescale": 1.0, 
        "KAON_MinPT": 600.0, 
        "KAON_PIDK": 3.0, 
        "Kstar_MassMax": 1040.0, 
        "Kstar_MassMin": 750.0, 
        "Kstar_MinPT": 600.0, 
        "MUON_MinPT": 800.0, 
        "MUON_PIDmu": 0, 
        "PAIR_SumPTMin": 2800.0, 
        "PION_MinPT": 600.0, 
        "PION_PIDK": 0, 
        "PION_PIDmu": -1.0, 
        "Rho_MassMax": 1020, 
        "Rho_MassMin": 530.0, 
        "Rho_MinPT": 600.0, 
        "TRACK_MINIPCHI2": 6, 
        "VLinePrescale": 1.0
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingCharmFromBSemi.py
CharmFromBSemi = {
    "BUILDERTYPE": "CharmFromBSemiAllLinesConf", 
    "CONFIG": {
        "BDIRA": 0.999, 
        "BVCHI2DOF": 6.0, 
        "CombMassHigh_HH": 1820, 
        "CombMassLow_HH": 0, 
        "D02HHGammaAMassWin": 220, 
        "D02HHGammaMassWin": 210, 
        "D02HHPi0AMassWin": 220, 
        "D02HHPi0DocaCut": 6.0, 
        "D02HHPi0MassWin": 210, 
        "D02KSHHPi0AMassWin": 220, 
        "D02KSHHPi0MassWin": 210, 
        "D02KSHHPi0_D0PT": 1000, 
        "D02KSHHPi0_D0PTComb": 1000, 
        "D02KSHHPi0_PTSUMLoose": 1000, 
        "DDocaChi2Max": 20, 
        "DZ": 0, 
        "DsAMassWin": 100.0, 
        "DsDIRA": 0.99, 
        "DsFDCHI2": 100.0, 
        "DsIP": 7.4, 
        "DsMassWin": 80.0, 
        "DsVCHI2DOF": 6.0, 
        "Dstar_Chi2": 8.0, 
        "Dstar_SoftPion_PIDe": 2.0, 
        "Dstar_SoftPion_PT": 80.0, 
        "Dstar_wideDMCutLower": 0.0, 
        "Dstar_wideDMCutUpper": 170.0, 
        "Dto4hADocaChi2Max": 7, 
        "Dto4h_AMassWin": 42.0, 
        "Dto4h_MassWin": 40.0, 
        "DtoXgammaADocaChi2Max": 10, 
        "GEC_nLongTrk": 250, 
        "HLT2": "HLT_PASS_RE('Hlt2.*SingleMuon.*Decision') | HLT_PASS_RE('Hlt2Topo.*Decision')", 
        "KPiPT": 250.0, 
        "KSCutDIRA": 0.99, 
        "KSCutZFDFromD": 10.0, 
        "KSDDCutFDChi2": 100, 
        "KSDDCutMass": 30, 
        "KSDDPMin": 3000, 
        "KSDDPTMin": 250, 
        "KSDaugTrackChi2": 4, 
        "KSLLCutFDChi2": 100, 
        "KSLLCutMass": 30, 
        "KSLLPMin": 2000, 
        "KSLLPTMin": 250, 
        "KSVertexChi2": 6, 
        "KaonPIDK": 4.0, 
        "KaonPIDKloose": -5, 
        "LambdaCutDIRA": 0.99, 
        "LambdaDDCutFDChi2": 100, 
        "LambdaDDCutMass": 30, 
        "LambdaDDPMin": 3000, 
        "LambdaDDPTMin": 800, 
        "LambdaDaugTrackChi2": 4, 
        "LambdaLLCutFDChi2": 100, 
        "LambdaLLCutMass": 30, 
        "LambdaLLPMin": 2000, 
        "LambdaLLPTMin": 500, 
        "LambdaVertexChi2": 6, 
        "MINIPCHI2": 9.0, 
        "MINIPCHI2Loose": 4.0, 
        "MassHigh_HH": 1810, 
        "MassLow_HH": 0, 
        "MaxBMass": 6000, 
        "MaxConvPhDDChi": 9, 
        "MaxConvPhDDMass": 100, 
        "MaxConvPhLLChi": 9, 
        "MaxConvPhLLMass": 100, 
        "MaxVCHI2NDOF_HH": 10.0, 
        "MinBMass": 2500, 
        "MinConvPhDDPT": 800, 
        "MinConvPhLLPT": 800, 
        "MinVDCHI2_HH": 1000.0, 
        "MuonIPCHI2": 4.0, 
        "MuonPT": 800.0, 
        "PIDmu": -0.0, 
        "PTSUM": 1800.0, 
        "PTSUMLoose": 1400.0, 
        "PTSUM_HHGamma": 1800.0, 
        "PTSUM_HHPi0": 1800.0, 
        "PhotonCL": 0.25, 
        "PhotonPT": 1500, 
        "Pi0PMin": 3000, 
        "Pi0PtMin": 1000, 
        "PionPIDK": 10.0, 
        "PionPIDKTight": 4.0, 
        "TRCHI2": 4.0, 
        "TTSpecs": {
            "Hlt1.*Track.*Decision%TOS": 0, 
            "Hlt2.*SingleMuon.*Decision%TOS": 0, 
            "Hlt2Global%TIS": 0, 
            "Hlt2Topo.*Decision%TOS": 0
        }, 
        "TrGhostProbMax": 0.5
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD02KSKS.py
D02KSKS = {
    "BUILDERTYPE": "D02KSKSConf", 
    "CONFIG": {
        "D0_DOCA_DD": 4.0, 
        "D0_DOCA_LL": 1.0, 
        "D0_IPchi2_DD": 10.0, 
        "D0_IPchi2_LD": 10.0, 
        "D0_IPchi2_LL": 50.0, 
        "D0_KSLL_signedFLchi2_LD": 500.0, 
        "D0_KS_nu2prime": 4.0, 
        "D0_KS_signedFLchi2_LL": 200.0, 
        "D0_MassWindow": 100.0, 
        "D0_MassWindowBeforeFit": 150.0, 
        "D0_lnPt_DD": 21.0, 
        "D0_signedFLchi2_LD": 1.0, 
        "D0_signedFLchi2_LL": -1.0, 
        "D0_vertexChi2_DD": 20.0, 
        "D0_vertexChi2_LD": 20.0, 
        "D0_vertexChi2_LL": 20.0, 
        "DstPostscaleDD": 1.0, 
        "DstPostscaleLD": 1.0, 
        "DstPostscaleLL": 1.0, 
        "DstPrescaleDD": 1.0, 
        "DstPrescaleLD": 1.0, 
        "DstPrescaleLL": 1.0, 
        "Dst_massDiffHigh_DD": 160, 
        "Dst_massDiffHigh_LD": 160, 
        "Dst_massDiffHigh_LL": 160, 
        "Dst_massDiffLow_DD": 135, 
        "Dst_massDiffLow_LD": 135, 
        "Dst_massDiffLow_LL": 135, 
        "Dst_mass_DD": 200, 
        "Dst_mass_LD": 200, 
        "Dst_mass_LL": 200, 
        "Dst_vertexChi2_DD": 40.0, 
        "Dst_vertexChi2_LD": 40.0, 
        "Dst_vertexChi2_LL": 40.0, 
        "KS_DD_TRGHOSTPROB": 1.0, 
        "KS_DD_nu2prime": 0.0, 
        "KS_DD_signedFLchi2": 50.0, 
        "KS_LL_TRGHOSTPROB": 1.0, 
        "KS_LL_nu2prime": 0.0, 
        "KS_LL_signedFLchi2": 50.0, 
        "PostscaleDD": 1.0, 
        "PostscaleLD": 1.0, 
        "PostscaleLL": 1.0, 
        "PrescaleDD": 1.0, 
        "PrescaleLD": 1.0, 
        "PrescaleLL": 1.0, 
        "slowPi_DDL_TRGHOSTPROB": 1.0, 
        "slowPi_DDU_TRGHOSTPROB": 1.0, 
        "slowPi_IPchi2_DD": 40.0, 
        "slowPi_IPchi2_LD": 40.0, 
        "slowPi_IPchi2_LL": 40.0, 
        "slowPi_LDL_TRGHOSTPROB": 1.0, 
        "slowPi_LDU_TRGHOSTPROB": 1.0, 
        "slowPi_LLL_TRGHOSTPROB": 1.0, 
        "slowPi_LLU_TRGHOSTPROB": 1.0
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2PiPi0.py
D2EtaEEGH = {
    "BUILDERTYPE": "StrippingD2EtaEEGHConf", 
    "CONFIG": {
         # Minimum best primary vertex IP chi^2 for charged D daughters
                'Bachelor_IPCHI2_MIN'      : 25.0,        
                # Minimum PT for charged D daughters 
                'Bachelor_PT_MIN'          : 350  *MeV,   
                # Minimum momentum for charged D daughters 
                'Bachelor_P_MIN'           : 1000 *MeV,
                # Bachelor particle PID requirement 
                'Bachelor_PIDK_MIN'        : -999.0, # not used if < -10
                # Minimum best primary vertex IP chi^2 for charged D daughters
                'Daug_IPCHI2_MIN'      : 25.0,     
                # Minimum PT for charged D daughters 
                'Daug_PT_MIN'          : 350  *MeV,   # 600 --> 300
                # Minimum momentum for charged D daughters 
                'Daug_P_MIN'           : 1000 *MeV,
                # Maximum momentum for charged D daughters 
                'Daug_P_MAX'           : 100000 *MeV,
                # Minimum pseudorapidity for charged D daughters 
                'Daug_ETA_MIN'         : 2.0,
                # Maximum pseudorapidity for charged D daughters 
                'Daug_ETA_MAX'         : 5.0,
                # Track quality requirement for charged D daughters
                'Daug_TRCHI2DOF_MAX'   : 5,    
                # Maximum ghost probability for charged D daughters
                'Daug_TRGHOSTPROB_MAX' : 0.5,
                # Minimum PT for intermediate resonance neutral daughters
                'Neut_PT_MIN'        : 350  *MeV,
                # Maximum mass for dielectron                
                'DiElectron_Mass_MAX'       : 650  *MeV,
                # Minimum PT for dielectron                
                'DiElectron_PT_MIN'       : 0  *MeV, 
                # Minimum mass for intermediate resonance
                'Res_Mass_MIN'       : 400  *MeV, 
                # Maximum mass for intermediate resonance
                'Res_Mass_MAX'       : 700  *MeV, 
                # Minimum D PT
                'D_PT_Min'           : 2000 *MeV, 
                # Minimum D mass
                'D_Mass_MIN'          : 1600 *MeV,
                # Maximum D mass
                'D_Mass_MAX'          : 2500 *MeV,
                # Maximum chi^2 on D end vertex
                'D_VCHI2PDOF_MAX'         : 5,
                # minimum flight time                
                'D_BPVLTIME_MIN'       : 0.15*picosecond,
                # max chi^2 per DOF of DTF fit
                'DTF_CHI2NDOF_MAX'   : 5,
                # HLT filters, only process events firing triggers matching the RegEx
                'Hlt1Filter'         : None,
                'Hlt2Filter'      : "HLT_PASS_RE('Hlt2CharmHadDp2Eta.*_Eta2EmEpG.*Decision')",
                # prescale and postscale
                'PrescaleD2PiEtaEEG' : 1,
                'PrescaleD2KEtaEEG'  : 1,
                'PostscaleD2PiEtaEEG': 1,
                'PostscaleD2KEtaEEG' : 1,
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2PiPi0.py
D2EtaH = {
    "BUILDERTYPE": "StrippingD2EtaHConf", 
    "CONFIG": {
        # Minimum best primary vertex IP chi^2 for charged D daughters
                'Bachelor_IPCHI2_MIN'      : 16.0,        # 25 --> 16
                # Minimum PT for charged D daughters
                'Bachelor_PT_MIN'          : 600  *MeV,   # 600
                # Minimum momentum for charged D daughters
                'Bachelor_P_MIN'           : 1000 *MeV,
                # Bachelor particle PID requirement 
                'Bachelor_PIDK_MIN'        : -999.0, # not used
                # Minimum best primary vertex IP chi^2 for charged D daughters
                'Daug_IPCHI2_MIN'      : 16.0,        # 25 --> 4
                # Minimum PT for charged D daughters
                'Daug_PT_MIN'          : 500  *MeV,   # 600 --> 300
                # Minimum momentum for charged D daughters
                'Daug_P_MIN'           : 1000 *MeV,
                # Maximum momentum for charged D daughters
                'Daug_P_MAX'           : 100000 *MeV,
                # Minimum pseudorapidity for charged D daughters
                'Daug_ETA_MIN'         : 2.0,
                # Maximum pseudorapidity for charged D daughters
                'Daug_ETA_MAX'         : 5.0,
                # Track quality requirement for charged D daughters
                'Daug_TRCHI2DOF_MAX'   : 5,
                # Maximum ghost probability for charged D daughters
                'Daug_TRGHOSTPROB_MAX' : 0.5,
                # Maximum DLLK of intermediate resonance daughter (pion)
                'Pi_PIDK_MAX'        : 0.0,
                # Minimum PT for intermediate resonance neutral daughters
                'Neut_PT_MIN'        : 600  *MeV,
                # Mass window for intermediate resonance neutral daughters (pi0, eta)
                'Neut_Mass_Win'      :  50  *MeV,
                # Minimum dipion mass
                'Dipion_Mass_MIN'    : 200 *MeV,
                # Maximum dipion mass
                'Dipion_Mass_MAX'    : 750 *MeV,
                # Maximum DOCA chi2 for dipion combination
                'Dipion_DOCACHI2_MAX': 15,
                # Minimum mass for intermediate resonance
                'Res_Mass_MIN'       : 350   *MeV,  # 500 --> 400
                # Maximum mass for intermediate resonance
                'Res_Mass_MAX'       : 750  *MeV,   # 600 --> 700
                # Minimum D PT
                'D_PT_Min'           : 2000 *MeV,   # 
                # Minimum D mass
                'D_Mass_MIN'          : 1600 *MeV,
                # Maximum D mass
                'D_Mass_MAX'          : 2200 *MeV,
                # Maximum chi^2 on D end vertex
                'D_VCHI2PDOF_MAX'         : 4,
                # minimum flight time
                'D_BPVLTIME_MIN'       : 0.25*picosecond,
                # max chi^2 per DOF of DTF fit
                'DTF_CHI2NDOF_MAX'   : 5,    # not used
                # HLT filters, only process events firing triggers matching the RegEx
                'Hlt1Filter'         : None,
                #'Hlt2Filter'         : None,
                #'Hlt2Filter'      : "(HLT_PASS_RE('Hlt2CharmHadDp2EtaPip_.*Decision') | HLT_PASS_RE('Hlt2CharmHadDp2EtaKp_.*Decision'))",
                #'Hlt2Filter'      : "HLT_PASS_RE('Hlt2CharmHadDp2Eta.*Decision') | HLT_PASS_RE('Hlt2Topo.*Decision')",
                'Hlt2Filter'      : "HLT_PASS_RE('Hlt2CharmHadDp2Eta.*Decision')",
                # prescale and postscale
                'PrescaleD2PiEtaPPG' : 1,
                'PrescaleD2PiEta3HM' : 1,
                'PrescaleD2PiEta3HR' : 1,
                'PrescaleD2KEtaPPG'  : 1,
                'PrescaleD2KEta3HM'  : 1,
                'PrescaleD2KEta3HR'  : 1,
                'PostscaleD2PiEtaPPG': 1,
                'PostscaleD2PiEta3HM': 1,
                'PostscaleD2PiEta3HR': 1,
                'PostscaleD2KEtaPPG' : 1,
                'PostscaleD2KEta3HM' : 1,
                'PostscaleD2KEta3HR' : 1
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2PiPi0.py
D2EtaPrimeH = {
    "BUILDERTYPE": "StrippingD2EtaPrimeHConf", 
    "CONFIG": {
        # Minimum best primary vertex IP chi^2 for charged D daughters
                'Bachelor_IPCHI2_MIN'      : 25.0,        # 25 
                # Minimum PT for charged D daughters
                'Bachelor_PT_MIN'          : 600  *MeV,   # 600
                # Minimum momentum for charged D daughters
                'Bachelor_P_MIN'           : 1000 *MeV,
                # Bachelor particle PID requirement 
                'Bachelor_PIDK_MIN'        : -999.0, # not used 
                # Minimum best primary vertex IP chi^2 for charged D daughters
                'Daug_IPCHI2_MIN'      : 25.0,        # 25 --> 16
                # Minimum PT for charged D daughters
                'Daug_PT_MIN'          : 600  *MeV,   # 600 --> 500
                # Minimum momentum for charged D daughters
                'Daug_P_MIN'           : 1000 *MeV,
                # Maximum momentum for charged D daughters
                'Daug_P_MAX'           : 100000 *MeV,
                # Minimum pseudorapidity for charged D daughters
                'Daug_ETA_MIN'         : 2.0,
                # Maximum pseudorapidity for charged D daughters
                'Daug_ETA_MAX'         : 5.0,
                # Track quality requirement for charged D daughters
                'Daug_TRCHI2DOF_MAX'   : 5,
                # Maximum ghost probability for charged D daughters
                'Daug_TRGHOSTPROB_MAX' : 0.5,
                # Maximum DLLK of intermediate resonance daughter (pion)
                'Pi_PIDK_MAX'        : 0,
                # Minimum PT for intermediate resonance neutral daughters
                'Neut_PT_MIN'        : 1000  *MeV,
                # Mass window for intermediate resonance neutral daughters (pi0, eta)
                'Neut_Mass_Win'      :  50  *MeV,
                # Minimum dipion mass
                'Dipion_Mass_MIN'    : 200 *MeV,
                # Maximum dipion mass
                'Dipion_Mass_MAX'    : 1200 *MeV,
                # Maximum DOCA chi2 for dipion combination
                'Dipion_DOCACHI2_MAX': 15,
                # Minimum mass for intermediate resonance
                'Res_Mass_MIN'       : 700   *MeV,  # 900 --> 800
                # Maximum mass for intermediate resonance
                'Res_Mass_MAX'       : 1150  *MeV,   # 990 --> 1050
                # Minimum D PT
                'D_PT_Min'           : 2000 *MeV,   # 2000
                # Minimum D mass
                'D_Mass_MIN'          : 1600 *MeV,
                # Maximum D mass
                'D_Mass_MAX'          : 2200 *MeV,
                # Maximum chi^2 on D end vertex
                'D_VCHI2PDOF_MAX'         : 4,
                # minimum flight time
                'D_BPVLTIME_MIN'       : 0.25*picosecond,
                # max chi^2 per DOF of DTF fit
                'DTF_CHI2NDOF_MAX'   : 5,
                # HLT filters, only process events firing triggers matching the RegEx
                'Hlt1Filter'         : None,
                #'Hlt2Filter'         : None, 
                'Hlt2Filter'      : "HLT_PASS_RE('Hlt2CharmHadDp2Etap.*Decision') | HLT_PASS_RE('Hlt2Topo.*Decision')",
                # prescale and postscale
                'PrescaleD2PiEtaPrimePPG' : 1,
                'PrescaleD2PiEtaPrime3HR' : 1,
                'PrescaleD2KEtaPrimePPG'  : 1,
                'PrescaleD2KEtaPrime3HR'  : 1,
                'PostscaleD2PiEtaPrimePPG': 1,
                'PostscaleD2PiEtaPrime3HR': 1,
                'PostscaleD2KEtaPrimePPG' : 1,
                'PostscaleD2KEtaPrime3HR' : 1
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2HHHKs.py
D2HHHKs = {
    "BUILDERTYPE": "D2HHHKsLines", 
    "CONFIG": {
        "CombMassHigh": 2100.0, 
        "CombMassLow": 1750.0, 
        "HighPIDK": 0, 
        "Hlt1Filter": None, 
        "Hlt2Filter": None, 
        "KSCutDIRA_DD": 0.999, 
        "KSCutDIRA_LL": 0.999, 
        "KSCutFDChi2_DD": 5, 
        "KSCutFDChi2_LL": 5, 
        "KSCutMass_DD": 50.0, 
        "KSCutMass_LL": 35.0, 
        "LowPIDK": 0, 
        "MassHigh": 2050.0, 
        "MassLow": 1790.0, 
        "MaxADOCACHI2": 10.0, 
        "MaxDz_DD": 9999.0, 
        "MaxDz_LL": 9999.0, 
        "MaxKsVCHI2NDOF_DD": 20.0, 
        "MaxKsVCHI2NDOF_LL": 20.0, 
        "MaxVCHI2NDOF": 12.0, 
        "MinBPVDIRA": 0.9998, 
        "MinBPVTAU": 0.0001, 
        "MinCombPT": 1.5, 
        "MinDz_DD": 250.0, 
        "MinDz_LL": 0.0, 
        "MinKsIpChi2_DD": 3, 
        "MinKsIpChi2_LL": 3, 
        "MinKsPT_DD": 300.0, 
        "MinKsPT_LL": 300.0, 
        "MinTrkIPChi2": 3, 
        "MinTrkPT": 250.0, 
        "PrescaleD2KKPiKsDD": 1, 
        "PrescaleD2KKPiKsLL": 1, 
        "PrescaleD2KPiPiKsDD": 1, 
        "PrescaleD2KPiPiKsLL": 1, 
        "PrescaleD2PiPiPiKsDD": 1, 
        "PrescaleD2PiPiPiKsLL": 1, 
        "TrChi2": 4, 
        "TrGhostProb": 0.5
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2HHHPi0.py
D2HHHPi0 = {
    "BUILDERTYPE": "D2HHHPi0Lines", 
    "CONFIG": {
        "CombMassHigh": 2100.0, 
        "CombMassLow": 1750.0, 
        "HighPIDK": 0, 
        "Hlt1Filter": None, 
        "Hlt2Filter": None, 
        "LowPIDK": 0, 
        "MassHigh": 2060.0, 
        "MassLow": 1790.0, 
        "MaxADOCACHI2": 10.0, 
        "MaxMass_Pi0R": 150, 
        "MaxVCHI2NDOF": 12.0, 
        "MinBPVDIRA": 0.9998, 
        "MinBPVTAU": 0.0001, 
        "MinCombPT": 3000.0, 
        "MinMass_Pi0R": 120, 
        "MinPT_Pi0M": 2000.0, 
        "MinPT_Pi0R": 1500.0, 
        "MinTrkIPChi2": 3, 
        "MinTrkPT": 300.0, 
        "PrescaleD2KKPiPi0M": 1, 
        "PrescaleD2KKPiPi0R": 1, 
        "PrescaleD2KPiPiPi0M": 1, 
        "PrescaleD2KPiPiPi0R": 1, 
        "PrescaleD2PiPiPiPi0M": 1, 
        "PrescaleD2PiPiPiPi0R": 1, 
        "TrChi2": 4, 
        "TrGhostProb": 0.5
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2KS0H_conf.py
D2KS0H = {
    "BUILDERTYPE": "StrippingD2KS0HConf", 
    "CONFIG": {
        "BachKaonMIPChi2": 15, 
        "BachKaonP": 2000.0, 
        "BachKaonPIDK": 0, 
        "BachKaonPT": 200.0, 
        "BachKaonTrackChi2": 3, 
        "BachPionMIPChi2": 15, 
        "BachPionP": 2000.0, 
        "BachPionPIDK": 0, 
        "BachPionPT": 200.0, 
        "BachPionTrackChi2": 3, 
        "DMesonADOCAChi2": 11, 
        "DMesonAPT": 1800.0, 
        "DMesonComboDOCA": 0.6, 
        "DMesonComboHighMass": 2080.0, 
        "DMesonComboLowMass": 1760.0, 
        "DMesonFlightDistChi2": 10, 
        "DMesonMotherHighMass": 2070.0, 
        "DMesonMotherLowMass": 1770.0, 
        "DMesonMotherMIPChi2": 15, 
        "DMesonMotherPT": 2000.0, 
        "DMesonMotherVertexChi2": 10, 
        "D_BPVLTIME_MIN": 0.00025, 
        "KS0BPVVDCHI2": 300, 
        "KS0BPVVDCHI2DD": 200, 
        "KS0DaugMIPChi2": 40, 
        "KS0DaugP": 2000.0, 
        "KS0DaugPIDK": 10, 
        "KS0DaugPT": 200.0, 
        "KS0DaugTrackChi2": 4, 
        "KS0MIPChi2": 7, 
        "KS0MIPChi2DD": 0, 
        "KS0MassWindow": 35.0, 
        "KS0PT": 500.0, 
        "KS0VertexChi2": 10, 
        "KS0ZDiff": 10.0, 
        "KaonLinePostscale": 1, 
        "KaonLinePostscaleDD": 1, 
        "KaonLinePrescale": 1, 
        "KaonLinePrescaleDD": 1, 
        "KsDD_VZ_MAX": 2275.0, 
        "KsDD_VZ_MIN": 300.0, 
        "KsLL_BPVLTIME_MIN": 0.0005, 
        "PionLinePostscale": 1, 
        "PionLinePostscaleDD": 1, 
        "PionLinePrescale": 1, 
        "PionLinePrescaleDD": 1, 
        "TisTosSpecs_KS0K": {}, 
        "TisTosSpecs_KS0KDD": {}, 
        "TisTosSpecs_KS0Pi": {}, 
        "TisTosSpecs_KS0PiDD": {}, 
        "TrGhostProbMax": 0.5, 
        "UseTOS": False
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2PiPi0.py
D2PhiH = {
    "BUILDERTYPE": "StrippingD2PhiHConf", 
    "CONFIG": {
        # Minimum best primary vertex IP chi^2 for charged D daughters
                'Bachelor_IPCHI2_MIN'      : 25.0,        # 25 --> 16
                # Minimum PT for charged D daughters
                'Bachelor_PT_MIN'          : 600  *MeV,   # 600 
                # Minimum momentum for charged D daughters
                'Bachelor_P_MIN'           : 1000 *MeV,
                # Bachelor particle PID requirement 
                'Bachelor_PIDK_MIN'        : -999.0, # not used 
                # Minimum best primary vertex IP chi^2 for charged D daughters
                'Daug_IPCHI2_MIN'      : 25.0,        # 25 --> 4
                # Minimum PT for charged D daughters
                'Daug_PT_MIN'          : 600  *MeV,   # 600 --> 300
                # Minimum momentum for charged D daughters
                'Daug_P_MIN'           : 1000 *MeV,
                # Maximum momentum for charged D daughters
                'Daug_P_MAX'           : 100000 *MeV,
                # Minimum pseudorapidity for charged D daughters
                'Daug_ETA_MIN'         : 2.0,
                # Maximum pseudorapidity for charged D daughters
                'Daug_ETA_MAX'         : 5.0,
                # Track quality requirement for charged D daughters
                'Daug_TRCHI2DOF_MAX'   : 5,
                # Maximum ghost probability for charged D daughters
                'Daug_TRGHOSTPROB_MAX' : 0.5,
                # Maximum DLLK of intermediate resonance daughter (pion)
                'Pi_PIDK_MAX'        : 0,
                # Minimum PT for intermediate resonance neutral daughters
                'Neut_PT_MIN'        : 600  *MeV,
                # Mass window for intermediate resonance neutral daughters (pi0, eta)
                'Neut_Mass_Win'      :  50  *MeV,
                # Minimum dipion mass
                'Dipion_Mass_MIN'    : 200 *MeV,
                # Maximum dipion mass
                'Dipion_Mass_MAX'    : 1200 *MeV,
                # Maximum DOCA chi2 for dipion combination
                'Dipion_DOCACHI2_MAX': 15,
                # Minimum mass for intermediate resonance
                'Res_Mass_MIN'       : 680   *MeV,  #  includes omega
                # Maximum mass for intermediate resonance
                'Res_Mass_MAX'       : 890  *MeV,   #
                # Minimum D PT
                'D_PT_Min'           : 2000 *MeV,   #
                # Minimum D mass
                'D_Mass_MIN'          : 1600 *MeV,
                # Maximum D mass
                'D_Mass_MAX'          : 2200 *MeV,
                # Maximum chi^2 on D end vertex
                'D_VCHI2PDOF_MAX'         : 4,
                # minimum flight time
                'D_BPVLTIME_MIN'       : 0.25*picosecond,
                # max chi^2 per DOF of DTF fit
                'DTF_CHI2NDOF_MAX'   : 5,
                # HLT filters, only process events firing triggers matching the RegEx
                'Hlt1Filter'         : None,
                'Hlt2Filter'      : "HLT_PASS_RE('Hlt2CharmHad.*Decision') | HLT_PASS_RE('Hlt2Topo.*Decision')",
                # prescale and postscale
                'PrescaleD2PiPhi3HM' : 1,
                'PrescaleD2PiPhi3HR' : 1,
                'PrescaleD2KPhi3HM'  : 1,
                'PrescaleD2KPhi3HR'  : 1,
                'PostscaleD2PiPhi3HM': 1,
                'PostscaleD2PiPhi3HR': 1,
                'PostscaleD2KPhi3HM' : 1,
                'PostscaleD2KPhi3HR' : 1
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}


#StrippingD2PiPi0.py
D2Pi0H = {
    "BUILDERTYPE": "StrippingD2Pi0HConf", 
    "CONFIG": {
        # Minimum best primary vertex IP chi^2 for charged D daughters
                'Bachelor_IPCHI2_MIN'      : 25.0,        
                # Minimum PT for charged D daughters 
                'Bachelor_PT_MIN'          : 350  *MeV,   
                # Minimum momentum for charged D daughters 
                'Bachelor_P_MIN'           : 1000 *MeV,
                # Bachelor particle PID requirement 
                'Bachelor_PIDK_MIN'        : -999.0, # not used if < -10
                # Minimum best primary vertex IP chi^2 for charged D daughters
                'Daug_IPCHI2_MIN'      : 25.0,     
                # Minimum PT for charged D daughters 
                'Daug_PT_MIN'          : 350  *MeV,   # 600 --> 300
                # Minimum momentum for charged D daughters 
                'Daug_P_MIN'           : 1000 *MeV,
                # Maximum momentum for charged D daughters 
                'Daug_P_MAX'           : 100000 *MeV,
                # Minimum pseudorapidity for charged D daughters 
                'Daug_ETA_MIN'         : 2.0,
                # Maximum pseudorapidity for charged D daughters 
                'Daug_ETA_MAX'         : 5.0,
                # Track quality requirement for charged D daughters
                'Daug_TRCHI2DOF_MAX'   : 5,    
                # Maximum ghost probability for charged D daughters
                'Daug_TRGHOSTPROB_MAX' : 0.5,
                # Minimum PT for intermediate resonance neutral daughters
                'Neut_PT_MIN'        : 350  *MeV,
                # Maximum mass for dielectron                
                'DiElectron_Mass_MAX'       : 210  *MeV,
                # Minimum PT for dielectron                
                'DiElectron_PT_MIN'       : 0  *MeV, 
                # Minimum mass for intermediate resonance
                'Res_Mass_MIN'       : 70   *MeV, 
                # Maximum mass for intermediate resonance
                'Res_Mass_MAX'       : 210  *MeV, 
                # Minimum D PT
                'D_PT_Min'           : 2000 *MeV, 
                # Minimum D mass
                'D_Mass_MIN'          : 1600 *MeV,
                # Maximum D mass
                'D_Mass_MAX'          : 2500 *MeV,
                # Maximum chi^2 on D end vertex
                'D_VCHI2PDOF_MAX'         : 5,
                # minimum flight time                
                'D_BPVLTIME_MIN'       : 0.15*picosecond,
                # max chi^2 per DOF of DTF fit
                'DTF_CHI2NDOF_MAX'   : 5,
                # Track quality requirement for soft pion from Dstar
                'Soft_TRCHI2DOF_MAX'   : 5,
                # Maximum Dstar-D0 mass difference
                'Dstar_AMDiff_MAX': 165. * MeV,
                'Dstar_MDiff_MAX': 160. * MeV,                
                # Maximum chi^2 on Dstar vertex
                'Dstar_VCHI2VDOF_MAX': 100.,
                # HLT filters, only process events firing triggers matching the RegEx
                'Hlt1Filter'      : None,
                'Hlt2Filter'      : "(HLT_PASS_RE('Hlt2CharmHadD.*EmEp.*Decision') | HLT_PASS_RE('Hlt2Topo.*Decision'))",
                # prescale and postscale
                'PrescaleD2PiPi0EEG'  : 1,
                'PrescaleD2KPi0EEG'   : 1,
                'PrescaleDst2D0PiEEG' : 1,
                'PostscaleD2PiPi0EEG' : 1,
                'PostscaleD2KPi0EEG'  : 1,
                'PostscaleDst2D0PiEEG': 1
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2XMuMuSS.py
D2XMuMuSS = {
    "BUILDERTYPE": "StrippingD2XMuMuSSConf", 
    "CONFIG": {
        "D02K3PiLinePostscale": 1, 
        "D02K3PiLinePrescale": 0.005, 
        "D02KKMuMuLinePostscale": 1, 
        "D02KKMuMuLinePrescale": 1, 
        "D02KKPiPiLinePostscale": 1, 
        "D02KKPiPiLinePrescale": 0.01, 
        "D02KPiMuMuLinePostscale": 1, 
        "D02KPiMuMuLinePrescale": 1, 
        "D02PiPiMuMuLinePostscale": 1, 
        "D02PiPiMuMuLinePrescale": 1, 
        "D02PiPiPiPiLinePostscale": 1, 
        "D02PiPiPiPiLinePrescale": 0.01, 
        "D22KPiLinePostscale": 1, 
        "D22KPiLinePrescale": 0.005, 
        "D2K2PiLinePostscale": 1, 
        "D2K2PiLinePrescale": 0.005, 
        "D2KEECalLinePostscale": 1, 
        "D2KEECalLinePrescale": 1, 
        "D2KEEOSLinePostscale": 1, 
        "D2KEEOSLinePrescale": 1, 
        "D2KEESSLinePostscale": 1, 
        "D2KEESSLinePrescale": 1, 
        "D2KEMuCalLinePostscale": 1, 
        "D2KEMuCalLinePrescale": 1, 
        "D2KEMuOSLinePostscale": 1, 
        "D2KEMuOSLinePrescale": 1, 
        "D2KMuEOSLinePostscale": 1, 
        "D2KMuEOSLinePrescale": 1, 
        "D2KMuESSLinePostscale": 1, 
        "D2KMuESSLinePrescale": 1, 
        "D2KMuMuCalLinePostscale": 1, 
        "D2KMuMuCalLinePrescale": 1, 
        "D2KMuMuOSLinePostscale": 1, 
        "D2KMuMuOSLinePrescale": 1, 
        "D2KMuMuSSLinePostscale": 1, 
        "D2KMuMuSSLinePrescale": 1, 
        "D2PiEECalLinePostscale": 1, 
        "D2PiEECalLinePrescale": 1, 
        "D2PiEEOSLinePostscale": 1, 
        "D2PiEEOSLinePrescale": 1, 
        "D2PiEESSLinePostscale": 1, 
        "D2PiEESSLinePrescale": 1, 
        "D2PiEMuCalLinePostscale": 1, 
        "D2PiEMuCalLinePrescale": 1, 
        "D2PiEMuOSLinePostscale": 1, 
        "D2PiEMuOSLinePrescale": 1, 
        "D2PiMuEOSLinePostscale": 1, 
        "D2PiMuEOSLinePrescale": 1, 
        "D2PiMuESSLinePostscale": 1, 
        "D2PiMuESSLinePrescale": 1, 
        "D2PiMuMuCalLinePostscale": 1, 
        "D2PiMuMuCalLinePrescale": 1, 
        "D2PiMuMuOSLinePostscale": 1, 
        "D2PiMuMuOSLinePrescale": 1, 
        "D2PiMuMuSSLinePostscale": 1, 
        "D2PiMuMuSSLinePrescale": 1, 
        "D2PiPiPiCalLinePostscale": 1, 
        "D2PiPiPiCalLinePrescale": 0.01, 
        "DDIRA": 0.9999, 
        "DFDCHI2_hhmumu": 36, 
        "DIPCHI2": 25, 
        "DIPCHI2_4bodyCS": 16, 
        "DIPCHI2_hhmumu": 20, 
        "DMAXDOCA": 0.15, 
        "DMAXDOCA_hhmumu": 0.2, 
        "DMassLow": 1763.0, 
        "DMassWin": 200.0, 
        "DMassWin_CS_hhmumu": 100.0, 
        "DPT_hhmumu": 2500.0, 
        "DVCHI2DOF": 5, 
        "DVCHI2DOF_hhmumu": 8, 
        "DdauMAXIPCHI2_hhmumu": 15, 
        "DimuonMass": 250.0, 
        "EleMINIPCHI2": 6, 
        "EleP": 3000.0, 
        "ElePT": 500.0, 
        "EleTRCHI2": 5, 
        "GhostProbCut_hhmumu": 0.5, 
        "KaonMINIPCHI2": 6, 
        "KaonP": 2000.0, 
        "KaonPIDK": -1.0, 
        "KaonPT": 300.0, 
        "KaonTRCHI2": 5, 
        "Lambdac2PEELinePostscale": 1, 
        "Lambdac2PEELinePrescale": 1, 
        "Lambdac2PEMuLinePostscale": 1, 
        "Lambdac2PEMuLinePrescale": 1, 
        "Lambdac2PMuELinePostscale": 1, 
        "Lambdac2PMuELinePrescale": 1, 
        "Lambdac2PMuMuLinePostscale": 1, 
        "Lambdac2PMuMuLinePrescale": 1, 
        "Lambdac2PPiPiLinePostscale": 1, 
        "Lambdac2PPiPiLinePrescale": 0.01, 
        "LambdacMassWin": 100.0, 
        "MINIPCHI2_4bodyCS": 3.0, 
        "MINIPCHI2_hhmumu": 2.0, 
        "MuonMINIPCHI2": 6, 
        "MuonP": 3000.0, 
        "MuonPIDmu_hhmumu": -1, 
        "MuonPT": 500.0, 
        "MuonTRCHI2": 5, 
        "PT_hhmumu": 300, 
        "PionMINIPCHI2": 6, 
        "PionP": 2000.0, 
        "PionPT": 300.0, 
        "PionTRCHI2": 5, 
        "ProtonMINIPCHI2": 6, 
        "ProtonP": 2000.0, 
        "ProtonPT": 300.0, 
        "ProtonTRCHI2": 5, 
        "RelatedInfoTools": [
            {
                "Location": "RelInfoVertexIsolation", 
                "Type": "RelInfoVertexIsolation"
            }, 
            {
                "Location": "RelInfoVertexIsolationBDT", 
                "Type": "RelInfoVertexIsolationBDT"
            }, 
            {
                "ConeAngle": 1.0, 
                "DaughterLocations": None, 
                "Location": "RelInfoConeVariables_1.0", 
                "Type": "RelInfoConeVariables"
            }, 
            {
                "ConeAngle": 1.5, 
                "DaughterLocations": None, 
                "Location": "RelInfoConeVariables_1.5", 
                "Type": "RelInfoConeVariables"
            }, 
            {
                "ConeAngle": 2.0, 
                "DaughterLocations": None, 
                "Location": "RelInfoConeVariables_2.0", 
                "Type": "RelInfoConeVariables"
            }, 
            {
                "ConeAngle": 0.0, 
                "DaughterLocations": None, 
                "Location": "RelInfoConeVariablesForEW_0.0", 
                "Type": "RelInfoConeVariablesForEW"
            }, 
            {
                "ConeAngle": 0.5, 
                "DaughterLocations": None, 
                "Location": "RelInfoConeVariablesForEW_0.5", 
                "Type": "RelInfoConeVariablesForEW"
            }, 
            {
                "ConeAngle": 1.0, 
                "DaughterLocations": None, 
                "Location": "RelInfoConeVariablesForEW_1.0", 
                "Type": "RelInfoConeVariablesForEW"
            }, 
            {
                "ConeAngle": 1.5, 
                "DaughterLocations": None, 
                "Location": "RelInfoConeVariablesForEW_1.5", 
                "Type": "RelInfoConeVariablesForEW"
            }, 
            {
                "DaughterLocations": None, 
                "Location": "RelInfoTrackIsolationBDT", 
                "Type": "RelInfoTrackIsolationBDT"
            }, 
            {
                "Location": "RelInfoBstautauCDFIso", 
                "Type": "RelInfoBstautauCDFIso"
            }, 
            {
                "DaughterLocations": None, 
                "IsoTwoBody": False, 
                "Location": "RelInfoBs2MuMuTrackIsolations", 
                "Type": "RelInfoBs2MuMuTrackIsolations", 
                "angle": 0.27, 
                "doca_iso": 0.13, 
                "fc": 0.6, 
                "ips": 3.0, 
                "makeTrackCuts": False, 
                "pvdis": 0.5, 
                "pvdis_h": 40.0, 
                "svdis": -0.15, 
                "svdis_h": 30.0, 
                "tracktype": 3
            }
        ]
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2hh.py
D2hh = {
    "BUILDERTYPE": "D2hhBuilder", 
    "CONFIG": {
        "AddPartialD": False, 
        "D0BPVDira": 0.9999, 
        "D0DOCA": 0.07, 
        "D0FDChi2": 40.0, 
        "D0KKMassWindowWidthHigh": 200.0, 
        "D0KKMassWindowWidthLow": -100.0, 
        "D0KPiMassWindowWidthHigh": 200.0, 
        "D0KPiMassWindowWidthLow": -100.0, 
        "D0MassWindowCentre": 1865.0, 
        "D0MassWindowWidth": 100.0, 
        "D0P": 5000.0, 
        "D0PiPiMassWindowWidthHigh": 200.0, 
        "D0PiPiMassWindowWidthLow": -75.0, 
        "D0Pt": 2000.0, 
        "D0PtLoose": 1500.0, 
        "D0Tau": 0.0, 
        "D0VtxChi2Ndof": 10.0, 
        "DaugIPChi2": 9.0, 
        "DaugIPChi2Loose": 4.0, 
        "DaugP": 5000.0, 
        "DaugPLoose": 3000.0, 
        "DaugPtLoose": 500.0, 
        "DaugPtMax": 1500.0, 
        "DaugPtMin": 800.0, 
        "DaugTrkChi2": 3.0, 
        "DaugTrkChi2Loose": 4.0, 
        "Daug_TRCHI2DOF_MAX": 5.0, 
        "Dstar_AMDiff_MAX": 165.0, 
        "Dstar_MDiff_MAX": 160.0, 
        "Dstar_VCHI2VDOF_MAX": 100.0, 
        "HighPIDK": 5.0, 
        "Hlt1TOS": {
            "Hlt1TrackAllL0Decision%TOS": 0
        }, 
        "Hlt2TOSKK": {
            "Hlt2CharmHadD02HH_D02KKDecision%TOS": 0, 
            "Hlt2CharmHadD02HH_D02KKWideMassDecision%TOS": 0, 
            "Hlt2CharmHadD02KKDecision%TOS": 0, 
            "Hlt2CharmHadD02KKWideMassDecision%TOS": 0
        }, 
        "Hlt2TOSKPi": {
            "Hlt2CharmHadD02HH_D02KPiDecision%TOS": 0, 
            "Hlt2CharmHadD02HH_D02KPiWideMassDecision%TOS": 0, 
            "Hlt2CharmHadD02KPiDecision%TOS": 0, 
            "Hlt2CharmHadD02KPiWideMassDecision%TOS": 0
        }, 
        "Hlt2TOSPiPi": {
            "Hlt2CharmHadD02HH_D02PiPiDecision%TOS": 0, 
            "Hlt2CharmHadD02HH_D02PiPiWideMassDecision%TOS": 0, 
            "Hlt2CharmHadD02PiPiDecision%TOS": 0, 
            "Hlt2CharmHadD02PiPiWideMassDecision%TOS": 0
        }, 
        "LowPIDK": 0.0, 
        "RunDefault": True, 
        "RunSameSign": True, 
        "RunTISLines": True, 
        "TaggedRSLinePostscale": 1.0, 
        "TaggedRSLinePrescale": 1.0, 
        "TaggedRSSSLinePostscale": 1.0, 
        "TaggedRSSSLinePrescale": 1.0, 
        "TaggedSCSLinePostscale": 1.0, 
        "TaggedSCSLinePrescale": 1.0, 
        "TaggedSCSSSLinePostscale": 1.0, 
        "TaggedSCSSSLinePrescale": 1.0, 
        "TaggedWSLinePostscale": 1.0, 
        "TaggedWSLinePrescale": 1.0, 
        "UntaggedCFLinePostscale": 1.0, 
        "UntaggedCFLinePrescale": 1.0, 
        "UntaggedKpiOnly": False, 
        "UntaggedSCSLinePostscale": 1.0, 
        "UntaggedSCSLinePrescale": 1.0, 
        "UntaggedTISLinePostscale": 1.0, 
        "UntaggedTISLinePrescale": 1.0, 
        "UseTOSFilter": False
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingD2hhh_FTcalib.py
D2hhhFTCalib = {
    "BUILDERTYPE": "D2hhhFTCalibConf", 
    "CONFIG": {
        "D2KKKDaughter2IPChi2": 4.0, 
        "D2KKKDaughterIPChi2": 4.0, 
        "D2KKPLinePostscale": 1.0, 
        "D2KKPLinePrescale": 0.2, 
        "DDIRA": 0.99995, 
        "DIPChi2": 1500.0, 
        "DOCAChi2": 50.0, 
        "DOCAMax": 0.5, 
        "DPVLTIME": 0.2, 
        "DPt": 2000.0, 
        "DVXChi2NDOF": 6.0, 
        "Daughter2IPChi2": 10.0, 
        "DaughterIPChi2": 4.0, 
        "DaughterP": 2000.0, 
        "DaughterPT": 250.0, 
        "DdcaFDChi2": 150.0, 
        "KPIDK": 7.0, 
        "MaxMassPosFit": 2040.0, 
        "MaxTracksInEvent": 500, 
        "MinMassPosFit": 1920.0, 
        "PTSum": 3200.0, 
        "piPIDK": 3.0
    }, 
    "STREAMS": [ "CharmCompleteEvent" ], 
    "WGs": [ "ALL" ]
}

#StrippingD2hhh_conf.py
D2hhhLines = {
    "BUILDERTYPE": "D2hhhConf", 
    "CONFIG": {
        "D2HHHIncLinePostscale": 1.0, 
        "D2HHHIncLinePrescale": 0.03, 
        "D2HHHIncMaxMassPosFit": 2070.0, 
        "D2HHHIncMinMassPosFit": 1100.0, 
        "D2KKKDaughter2IPChi2": 4.0, 
        "D2KKKDaughterIPChi2": 4.0, 
        "D2KKKLinePostscale": 1.0, 
        "D2KKKLinePrescale": 1.0, 
        "D2KKPLinePostscale": 1.0, 
        "D2KKPLinePrescale": 1.0, 
        "D2KPPLinePostscale": 1.0, 
        "D2KPPLinePrescale": 0.5, 
        "D2KPPMaxMassPosFit": 1940.0, 
        "D2KPPosLinePostscale": 1.0, 
        "D2KPPosLinePrescale": 1.0, 
        "D2PPPLinePostscale": 1.0, 
        "D2PPPLinePrescale": 0.5, 
        "DDIRA": 0.99995, 
        "DIPChi2": 1500.0, 
        "DOCAChi2": 50.0, 
        "DOCAMax": 0.5, 
        "DPVLTIME": 0.2, 
        "DPt": 2000.0, 
        "DVXChi2NDOF": 6.0, 
        "Daughter2IPChi2": 10.0, 
        "DaughterIPChi2": 4.0, 
        "DaughterP": 2000.0, 
        "DaughterPT": 250.0, 
        "DdcaFDChi2": 150.0, 
        "Ds2KKPosLinePostscale": 1.0, 
        "Ds2KKPosLinePrescale": 1.0, 
        "Ds2KKPosMinMassPosFit": 1900.0, 
        "KPIDK": 7.0, 
        "MaxMassPosFit": 2040.0, 
        "MaxTracksInEvent": 500, 
        "MinMassPosFit": 1800.0, 
        "PTSum": 3200.0, 
        "piPIDK": 3.0
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}


#StrippingD2PiPi0.py
Dst2PiD0EEG = {
    "BUILDERTYPE": "StrippingDst2PiD0EEGConf", 
    "CONFIG": {
        # Minimum PT for gamma in D0-> e+ e- gamma
                'Neut_PT_MIN'        : 600  *MeV, # 350
                # Maximum mass for dielectron in D0-> e+ e- gamma                
                'DiElectron_Mass_MAX'       : 1914  *MeV,
                # Minimum PT for dielectron in D0-> e+ e- gamma              
                'DiElectron_PT_MIN'       : 2600  *MeV, 
                # Minimum mass for intermediate resonance
                'Res_Mass_MIN'       : 1814   *MeV, 
                # Maximum mass for intermediate resonance
                'Res_Mass_MAX'       : 1914  *MeV, 
                # Track quality requirement for soft pion from Dstar
                'Soft_TRCHI2DOF_MAX'   : 3,
                # Maximum Dstar-D0 mass difference
                'Dstar_AMDiff_MAX': 165. * MeV,
                'Dstar_MDiff_MAX': 160. * MeV,                
                # Maximum chi^2 on Dstar vertex
                'Dstar_VCHI2VDOF_MAX': 9.,
                # Minimum P for electrons in D0-> e+ e-
                'Electron_P_MIN'       : 1000  *MeV,
                # Minimum PT for electrons in D0-> e+ e-                
                'Electron_PT_MIN'       : 350  *MeV,
                # Track quality requirement for electrons in in D0-> e+ e-
                'Electron_TRCHI2DOF_MAX'   : 6,    
                # Maximum ghost probability for electrons in in D0-> e+ e-
                'Electron_TRGHOSTPROB_MAX' : 0.5,
                # Minimum best primary vertex IP chi^2 for electrons in in D0-> e+ e-
                'Electron_IPCHI2_MIN'      : 16.0,   
                # HLT filters, only process events firing triggers matching the RegEx
                'Hlt1Filter'         : None,
                'Hlt2Filter'      : "HLT_PASS_RE('Hlt2CharmHadDstp2.*EmEp.*Decision') | HLT_PASS_RE('Hlt2Topo.*Decision')",
                 # prescale and postscale
                'PrescaleDst2PiD0EEG'  : 1,
                'PostscaleDst2PiD0EEG' : 1,
                'PrescaleDst2PiD0EE'   : 1,
                'PostscaleDst2PiD0EE'  : 1 
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarD02xx.py
DstarD02xx = {
    "BUILDERTYPE": "StrippingDstarD02xxConf", 
    "CONFIG": {
        "ConeAngles": {
            "08": 0.8, 
            "10": 1.0, 
            "12": 1.2, 
            "14": 1.4
        }, 
        "ConeVariables": [
            "CONEANGLE", 
            "CONEMULT", 
            "CONEPTASYM"
        ], 
        "D0MinPT": 1800.0, 
        "DDira": 0.9997, 
        "DMassWin": 70.0, 
        "DMassWinEMu": 300, 
        "DMassWinMuMuHigh": 300, 
        "DMassWinMuMuLow": -150.0, 
        "DMaxIPChi2": 15.0, 
        "DMinFlightChi2": 20.0, 
        "DVChi2": 10.0, 
        "DstD0DMWin": 10.0, 
        "DstD0DMWinMuMu": 30.0, 
        "DstMassWin": 300.0, 
        "HLT1MB": "HLT_PASS_RE('Hlt1(MB|L0).*Decision')", 
        "HLT2MB": "HLT_PASS_RE('Hlt2CharmHadMinBiasD02KPiDecision')", 
        "HLT2String": "Hlt2RareCharmD02LAB1LAB2*Decision", 
        "PiMaxIPCHI2": 10.0, 
        "PiMinPT": 110.0, 
        "PrescaleKmuBox": 1.0, 
        "PrescaleKmu_untagged_Box": 0.5, 
        "PrescaleKpiBox": 1.0, 
        "PrescaleKpi_untagged_Box": 1.0, 
        "PrescaleKpi_untagged_BoxMB": 0.0, 
        "PrescaleKpi_untagged_BoxMBTrEff": 0.0, 
        "PrescaleeKBox": 1.0, 
        "PrescaleemuBox": 1.0, 
        "PrescaleepiBox": 1.0, 
        "PrescalemumuBox": 1.0, 
        "Prescalemumu_untagged_Box": 1.0, 
        "PrescalepimuBox": 1.0, 
        "Prescalepimu_untagged_Box": 1.0, 
        "PrescalepipiBox": 1.0, 
        "Prescalepipi_untagged_Box": 1.0, 
        "Prescalepipi_untagged_BoxMB": 0.0, 
        "RequireHlt": 1, 
        "XTrackChi2": 5.0, 
        "XTrackChi2Pi": 7.0, 
        "XmaxIPChi2": 8, 
        "XmaxPT": 1100.0, 
        "XminIPChi2": 3, 
        "XminP": 4000.0, 
        "XminPT": 750.0, 
        "doca": 0.1, 
        "ghostProbCut": 0.5, 
        "prefix": ""
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarD0ToHHPi0.py
DstarD0ToHHPi0 = {
    "BUILDERTYPE": "DstarD0ToHHPi0AllLinesBuilder", 
    "CONFIG": {
        "D0_APT": 1400, 
        "D0_M_WINDOW": 150, 
        "DELTA_MASS_MAX": 180, 
        "Hlt2Filter": "", 
        "Kaon_PIDK": 7, 
        "Kaon_PT": 500, 
        "Kaon_TRGHOSTPROB": 0.35, 
        "Pi0M_PT": 1000, 
        "Pi0R_DMASS": 15, 
        "Pi0R_PT": 500, 
        "Pi0_MassConstraint": False, 
        "Pion_PIDK": 0, 
        "Pion_PT": 500, 
        "Pion_TRGHOSTPROB": 0.35, 
        "Slowpion_PIDe": 5, 
        "Slowpion_PT": 300, 
        "Slowpion_TRGHOSTPROB": 0.35, 
        "TOSFilter": {
            "Hlt2CharmHadInclDst2PiD02HHXBDTDecision%TOS": 0
        }, 
        "prescale_Pi0R_WIDEMASS": 1.0, 
        "useHLT2": False, 
        "useTOS": True
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarD2HHHH.py
DstarD2HHHH = {
    "BUILDERTYPE": "DstarD2HHHHLines", 
    "CONFIG": {
        "CombMassHigh": 2000.0, 
        "CombMassLow": 1730.0, 
        "Daug_TRCHI2DOF_MAX": 3, 
        "Dstar_AMDiff_MAX": 190.0, 
        "Dstar_MDiff_MAX": 170.0, 
        "Dstar_VCHI2VDOF_MAX": 15.0, 
        "HighPIDK": 0, 
        "Hlt1Filter": None, 
        "Hlt2Filter": None, 
        "LowPIDK": 0, 
        "MassHigh": 1940.0, 
        "MassLow": 1790.0, 
        "MaxADOCACHI2": 10.0, 
        "MaxVCHI2NDOF": 12.0, 
        "MinBPVDIRA": 0.9998, 
        "MinBPVTAU": 0.0001, 
        "MinCombP": 20000.0, 
        "MinCombPT": 1500.0, 
        "MinDP": 25000.0, 
        "MinDPT": 1600.0, 
        "MinTrkIPChi2": 3, 
        "MinTrkPT": 250.0, 
        "PrescaleDstarD2KKKPi": 1, 
        "PrescaleDstarD2KKPiPi": 1, 
        "PrescaleDstarD2KPiPiPi": 1, 
        "PrescaleDstarD2PiPiPiPi": 1, 
        "TrChi2": 4, 
        "TrGhostProb": 0.5
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarD2KSHHPi0.py
DstarD2KSHHPi0 = {
    "BUILDERTYPE": "DstarD2KSHHPi0Lines", 
    "CONFIG": {
        "CombMassHigh": 2100.0, 
        "CombMassLow": 1700.0, 
        "Daug_TRCHI2DOF_MAX": 3, 
        "Dstar_AMDiff_MAX": 190.0, 
        "Dstar_MDiff_MAX": 170.0, 
        "Dstar_VCHI2VDOF_MAX": 15.0, 
        "HighPIDK": 0, 
        "Hlt1Filter": None, 
        "Hlt2Filter": None, 
        "KSCutDIRA_DD": 0.999, 
        "KSCutDIRA_LL": 0.999, 
        "KSCutFDChi2_DD": 5, 
        "KSCutFDChi2_LL": 5, 
        "KSCutMass_DD": 50.0, 
        "KSCutMass_LL": 35.0, 
        "LowPIDK": 0, 
        "MassHigh": 1940.0, 
        "MassLow": 1790.0, 
        "MaxADOCACHI2": 10.0, 
        "MaxDz_DD": 9999.0, 
        "MaxDz_LL": 9999.0, 
        "MaxVCHI2NDOF": 12.0, 
        "MinBPVDIRA": 0.9998, 
        "MinBPVTAU": 0.0001, 
        "MinCombPT": 0.0, 
        "MinDz_DD": 0.0, 
        "MinDz_LL": 0.0, 
        "MinTrkIPChi2": 3, 
        "MinTrkPT": 250.0, 
        "PhotonCL": 0.25, 
        "Pi0MassWin": 16.0, 
        "Pi0PMin": 2500.0, 
        "Pi0PtMin": 1000.0, 
        "PrescaleDstarD2KKPi0KSDD": 1, 
        "PrescaleDstarD2KKPi0KSLL": 1, 
        "PrescaleDstarD2KPiPi0KSDD": 1, 
        "PrescaleDstarD2KPiPi0KSLL": 1, 
        "PrescaleDstarD2PiPiPi0KSDD": 1, 
        "PrescaleDstarD2PiPiPi0KSLL": 1, 
        "TrChi2": 4, 
        "TrGhostProb": 0.5
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarD2KShh.py
DstarD2KShh = {
    "BUILDERTYPE": "DstarD2KShhBuilder", 
    "CONFIG": {
        "DCutDIRA": 0.0, 
        "DCutTau": 0.2, 
        "DCutVtxChi2_KK": 21.0, 
        "DCutVtxChi2_KP": 15.0, 
        "DCutVtxChi2_PP": 13.0, 
        "DstarCutChi2NDOF_KK": 60.0, 
        "DstarCutChi2NDOF_KP": 20.0, 
        "DstarCutChi2NDOF_PP": 20.0, 
        "DstarCutPT_KK": 2200.0, 
        "DstarCutPT_KP": 2200.0, 
        "DstarCutPT_PP": 2200.0, 
        "KKDDPostscale": 1.0, 
        "KKDDPrescale": 1.0, 
        "KKLLPostscale": 1.0, 
        "KKLLPrescale": 1.0, 
        "KPDDPostscale": 1.0, 
        "KPDDPrescale": 1.0, 
        "KPLLPostscale": 1.0, 
        "KPLLPrescale": 1.0, 
        "KSCutZFDFromD": 10.0, 
        "KSDDCutDIRA": 0.99995, 
        "KSDDCutFDChi2": 100, 
        "KSDDCutMass": 40.0, 
        "KSLLCutDIRA": 0.9997, 
        "KSLLCutFDChi2": 100, 
        "KSLLCutMass": 20.0, 
        "LongTrackGEC": 150, 
        "PPDDPostscale": 1.0, 
        "PPDDPrescale": 1.0, 
        "PPLLPostscale": 1.0, 
        "PPLLPrescale": 1.0, 
        "SoftPionCutPIDe": 2.0, 
        "kaonFromDCutPIDpi": -3.0, 
        "kaonFromDCutTRCHI2DOF": 5.0, 
        "pionFromDCutPIDK": -1.0, 
        "pionFromDCutTRCHI2DOF": 4.0, 
        "preFitDCutPT": 1500.0, 
        "preFitDMassCut_DD": 270, 
        "preFitDMassCut_LL": 130, 
        "preFitDstarMassCut": 200.0, 
        "trackFromDCutP": 1500.0, 
        "trackFromDCutPIDe": 10.0, 
        "trackFromDCutPIDp": 15.0, 
        "wideDMCutLower": -10.0, 
        "wideDMCutUpper": 15.0, 
        "wideDMassCut_KKDD": 85.0, 
        "wideDMassCut_KKLL": 70.0, 
        "wideDMassCut_KPDD": 130.0, 
        "wideDMassCut_KPLL": 105.0, 
        "wideDMassCut_PPDD": 150.0, 
        "wideDMassCut_PPLL": 120.0
    }, 
    "STREAMS": [ "CharmCompleteEvent" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarD2XGamma.py
DstarD2XGamma = {
    "BUILDERTYPE": "DstarD2XGammaLines", 
    "CONFIG": {
        "CombMassHigh": 2075.0, 
        "CombMassHigh_HH": 1820.0, 
        "CombMassLow": 1665.0, 
        "CombMassLow_HH": 0.0, 
        "Daug_TRCHI2DOF_MAX": 3, 
        "Dstar_AMDiff_MAX": 180.0, 
        "Dstar_MDiff_MAX": 160.0, 
        "Dstar_VCHI2VDOF_MAX": 15.0, 
        "HighPIDK": 0, 
        "Hlt1Filter": None, 
        "Hlt2Filter": None, 
        "LowPIDK": 0, 
        "MassHigh": 2055.0, 
        "MassHigh_HH": 1810.0, 
        "MassLow": 1685.0, 
        "MassLow_HH": 0.0, 
        "MaxADOCACHI2": 10.0, 
        "MaxMass_CNV_DD": 100.0, 
        "MaxMass_CNV_LL": 100.0, 
        "MaxVCHI2NDOF": 12.0, 
        "MaxVCHI2NDOF_HH": 16.0, 
        "MaxVCHI2_CNV_DD": 9, 
        "MaxVCHI2_CNV_LL": 9, 
        "MinBPVDIRA": 0.9998, 
        "MinBPVTAU": 0.0001, 
        "MinCombPT": 1500.0, 
        "MinPT": 1800.0, 
        "MinPT_CNV_DD": 1000.0, 
        "MinPT_CNV_LL": 1000.0, 
        "MinTrkIPChi2": 3, 
        "MinTrkPT": 250.0, 
        "MinVDCHI2_HH": 1000.0, 
        "MinVDCHI2_HHComb": 1000.0, 
        "PrescaleDstarD2KKGamma": 1, 
        "PrescaleDstarD2KKGamma_CNVDD": 1, 
        "PrescaleDstarD2KKGamma_CNVLL": 1, 
        "PrescaleDstarD2KPiGamma": 1, 
        "PrescaleDstarD2KPiGamma_CNVDD": 1, 
        "PrescaleDstarD2KPiGamma_CNVLL": 1, 
        "PrescaleDstarD2PiPiGamma": 1, 
        "PrescaleDstarD2PiPiGamma_CNVDD": 1, 
        "PrescaleDstarD2PiPiGamma_CNVLL": 1, 
        "TrChi2": 4, 
        "TrGhostProb": 0.5, 
        "photonPT": 1800.0
    }, 
    "STREAMS": [ "CharmCompleteEvent" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarPromptWithD02HHMuMu.py
DstarPromptWithD02HHLL = {
    "BUILDERTYPE": "DstarPromptWithD02HHLLConf", 
    "CONFIG": {
        "ApplyElePIDe": False, 
        "ApplyGECs": False, 
        "ApplyGhostProbCut": False, 
        "ApplyKaonPIDK": False, 
        "ApplyPionPIDK": False, 
        "CheckPV": True, 
        "CombD0MassWin": 120.0, 
        "CombDelmLower": -9.0, 
        "CombDelmUpper": 20.0, 
        "ConeAngles": {
            "11": 1.1, 
            "13": 1.3, 
            "15": 1.5, 
            "17": 1.7, 
            "19": 1.9
        }, 
        "ConeVariables": [
            "CONEANGLE", 
            "CONEMULT", 
            "CONEPTASYM"
        ], 
        "D0DIRA": 0.9998, 
        "D0DauIPChi2": 3.0, 
        "D0DauMaxIPChi2": 9, 
        "D0DauP": 3000.0, 
        "D0DauPt": 300.0, 
        "D0FDChi2": 30, 
        "D0IPChi2": 36, 
        "D0MassWin": 100.0, 
        "D0MaxDOCA": 0.3, 
        "D0Pt": 2000.0, 
        "D0VtxChi2DOF": 20, 
        "DelmLower": -8.0, 
        "DelmUpper": 18.0, 
        "DstarDOCA": 0.3, 
        "DstarPt": 2000.0, 
        "DstarVtxChi2DOF": 20, 
        "ElePIDe": -5, 
        "GhostProbCut": 0.5, 
        "KaonPIDK": -100.0, 
        "MaxITClusters": None, 
        "MaxLongTracks": 250, 
        "MaxSpdDigits": None, 
        "MaxVeloTracks": None, 
        "PionPIDK": 100, 
        "Postscale": 1, 
        "Prescale": 1, 
        "SlowPionPt": 120.0, 
        "TrackChi2DOF": 3
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingDstarPromptWithD02HHHH.py
DstarPromptWithD02HHMuMuControl = {
    "BUILDERTYPE": "DstarPromptWithD02HHHHConf", 
    "CONFIG": {
        "ApplyGECs": False, 
        "ApplyGhostProbCut": False, 
        "ApplyKaonPIDK": False, 
        "ApplyPionPIDK": False, 
        "CheckPV": True, 
        "CombD0MassWin": 120.0, 
        "CombDelmLower": -9.0, 
        "CombDelmUpper": 20.0, 
        "ConeAngles": {
            "11": 1.1, 
            "13": 1.3, 
            "15": 1.5, 
            "17": 1.7, 
            "19": 1.9
        }, 
        "ConeVariables": [
            "CONEANGLE", 
            "CONEMULT", 
            "CONEPTASYM"
        ], 
        "D0DIRA": 0.9998, 
        "D0DauIPChi2": 3.0, 
        "D0DauMaxIPChi2": 9, 
        "D0DauP": 3000.0, 
        "D0DauPt": 300.0, 
        "D0FDChi2": 30, 
        "D0IPChi2": 36, 
        "D0MassWin": 100.0, 
        "D0MaxDOCA": 0.3, 
        "D0Pt": 2000.0, 
        "D0VtxChi2DOF": 20, 
        "DelmLower": -8.0, 
        "DelmUpper": 18.0, 
        "DstarDOCA": 0.3, 
        "DstarPt": 2000.0, 
        "DstarVtxChi2DOF": 20, 
        "GhostProbCut": 0.4, 
        "KaonPIDK": 0.0, 
        "MaxITClusters": None, 
        "MaxLongTracks": 250, 
        "MaxSpdDigits": None, 
        "MaxVeloTracks": None, 
        "PionPIDK": 3, 
        "Postscale": 1, 
        "Prescale": 0.05, 
        "Run2K2pi": True, 
        "Run3Kpi": True, 
        "Run4pi": True, 
        "RunK3pi": True, 
        "SlowPionPt": 120.0, 
        "TrackChi2DOF": 3
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingLambdac2V0H.py
Lambdac2V0H = {
    "BUILDERTYPE": "StrippingLambdac2V0HConf", 
    "CONFIG": {
        "Bach_All_BPVIPCHI2_MIN": 4.0, 
        "Bach_All_PT_MIN": 750.0, 
        "Bach_ETA_MAX": 5.0, 
        "Bach_ETA_MIN": 2.0, 
        "Bach_P_MAX": 100000.0, 
        "Bach_P_MIN": 2000.0, 
        "Comb_ADAMASS_WIN": 90.0, 
        "Comb_ADOCAMAX_MAX": 0.5, 
        "Comb_PT_MIN": 1000.0, 
        "Hlt1Filter": None, 
        "Hlt2Filter": None, 
        "KS0_FDCHI2_MIN": 100, 
        "KS0_PT_MIN": 500.0, 
        "KS0_P_MIN": 2000.0, 
        "KS0_VCHI2VDOF_MAX": 10.0, 
        "K_PIDK_MIN": 5.0, 
        "Lambda0_FDCHI2_MIN": 100, 
        "Lambda0_PT_MIN": 500.0, 
        "Lambda0_P_MIN": 2000.0, 
        "Lambda0_VCHI2VDOF_MAX": 10.0, 
        "Lambdac_PVDispCut": "(BPVVDCHI2 > 16.0)", 
        "Lambdac_VCHI2VDOF_MAX": 10.0, 
        "Lambdac_acosBPVDIRA_MAX": 0.14, 
        "Pi_PIDK_MAX": 3.0, 
        "PostscaleLambdac2Lambda0KDD": 1.0, 
        "PostscaleLambdac2Lambda0KLL": 1.0, 
        "PostscaleLambdac2Lambda0PiDD": 1.0, 
        "PostscaleLambdac2Lambda0PiLL": 1.0, 
        "PostscaleLambdac2pKS0DD": 1.0, 
        "PostscaleLambdac2pKS0LL": 1.0, 
        "PrescaleLambdac2Lambda0KDD": 1.0, 
        "PrescaleLambdac2Lambda0KLL": 1.0, 
        "PrescaleLambdac2Lambda0PiDD": 1.0, 
        "PrescaleLambdac2Lambda0PiLL": 1.0, 
        "PrescaleLambdac2pKS0DD": 1.0, 
        "PrescaleLambdac2pKS0LL": 1.0, 
        "Proton_PIDpPIDK_MIN": 2.0, 
        "Proton_PIDpPIDpi_MIN": 7.0
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

#StrippingXicc.py
Xicc = {
    "BUILDERTYPE": "XiccBuilder", 
    "CONFIG": {
        "DpHlt2TisTosSpec": {
            "Hlt2CharmHadDpToKmPipPip.*Decision%TOS": 0, 
            "Hlt2Global%TIS": 0
        }, 
        "DzHlt2TisTosSpec": {
            "Hlt2CharmHad.*Decision%TOS": 0, 
            "Hlt2Global.*%TIS": 0
        }, 
        "LcHlt2TisTosSpec": {
            "Hlt2CharmHadLcpToPpKmPip.*Decision%TOS": 0, 
            "Hlt2Global%TIS": 0
        }, 
        "LongTrackGEC": 150, 
        "XiHlt2TisTosSpec": {
            "Hlt2CharmHadXim2LamPim.*Decision%TOS": 0, 
            "Hlt2Global%TIS": 0
        }, 
        "controlPrescaleD0": 0.01, 
        "controlPrescaleDp": 0.01, 
        "controlPrescaleDsp": 0.01, 
        "controlPrescaleLc": 0.05, 
        "controlPrescaleXic": 0.05, 
        "signalPrescaleViaD0": 1.0, 
        "signalPrescaleViaD0DCS": 1.0, 
        "signalPrescaleViaD0WC": 1.0, 
        "signalPrescaleViaDp": 1.0, 
        "signalPrescaleViaDpWC": 1.0, 
        "signalPrescaleViaLc": 1.0, 
        "signalPrescaleViaLcDCS": 1.0, 
        "signalPrescaleViaLcWC": 1.0, 
        "signalPrescaleViaXic": 1.0, 
        "signalPrescaleViaXicWC": 1.0
    }, 
    "STREAMS": [ "Charm" ], 
    "WGs": [ "Charm" ]
}

