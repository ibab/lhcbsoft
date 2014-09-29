// Include files 
#include <iostream>
#include <fstream>
#include <list>
#include <utility>

// from ROOT
#include "TMath.h"

// from LOKI
#include "LoKi/LoKi.h"
#include "LoKi/ParticleContextCuts.h"

// from local
#include "TaggerCharmTool.h"
#include "CharmD0KpiWrapper.h"
#include "CharmD0KpipipiWrapper.h"
#include "CharmD0Kpipi0Wrapper.h"
#include "CharmDpKpipiWrapper.h"
#include "CharmD0KpiXWrapper.h"
#include "CharmD0KeXWrapper.h"
#include "CharmD0KmuXWrapper.h"
//#include "CharmD0KspipiWrapper.h"
//#include "CharmDpKspiWrapper.h"
//#include "CharmDstD0KspipiWrapper.h"
//#include "CharmDpKpiXWrapper.h"
//#include "CharmDpKeXWrapper.h"
//#include "CharmDpKmuXWrapper.h"

using namespace std;
using namespace LoKi::Cuts;
using namespace LoKi::Types;
using namespace LoKi::Particles;
using namespace LHCb ;
using namespace Gaudi::Units;

//--------------------------------------------------------------------
// Implementation file for class : TaggerCharmTool
//
// Author: Riccardo Cenci, Jack Wimberley
// 2013-01-23
//--------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_TOOL_FACTORY( TaggerCharmTool )

//====================================================================
TaggerCharmTool::TaggerCharmTool( const std::string& type,
                                  const std::string& name,
                                  const IInterface* parent ) :
  GaudiTool ( type, name, parent ),
  m_myDATAreader( NULL ),
  m_util(0),
  m_pLifetimeFitter(0),
  m_descend(0)
                                //  m_nnet(0)
{
  declareInterface<ITagger>(this);

  std::vector<std::string> def_CharmTagLocations;
  def_CharmTagLocations.push_back("Phys/Tag_StdD02KPi/Particles");
  def_CharmTagLocations.push_back("Phys/Tag_StdD02KPiPiPi/Particles");
  def_CharmTagLocations.push_back("Phys/Tag_StdD02KPiPi0/Particles");
  def_CharmTagLocations.push_back("Phys/Tag_StdDp2KPiPi/Particles");
  declareProperty( "CharmTagLocations", m_CharmTagLocations = def_CharmTagLocations);

  std::vector<std::string> def_CharmInclTagLocations;
  def_CharmInclTagLocations.push_back("Phys/Tag_StdD2KPipart/Particles");
  def_CharmInclTagLocations.push_back("Phys/Tag_StdD2Kepart/Particles");
  def_CharmInclTagLocations.push_back("Phys/Tag_StdD2Kmupart/Particles");
  declareProperty( "CharmInclTagLocations", m_CharmInclTagLocations = def_CharmInclTagLocations);

  std::vector<std::string> def_CharmLambdaTagLocations;
  def_CharmLambdaTagLocations.push_back("Phys/Tag_StdLambdaC2PKPi/Particles");
  declareProperty( "CharmLambdaTagLocations", m_CharmLambdaTagLocations = def_CharmLambdaTagLocations);

  declareProperty( "Charm_P0_Cal",           m_P0_Cal_charm   = 0.3371);
  declareProperty( "Charm_P1_Cal",           m_P1_Cal_charm   = 0.9111);
  declareProperty( "Charm_Eta_Cal",          m_Eta_Cal_charm  = 0.3121);

  declareProperty( "P0_charm_scale", m_P0charm =  0.3206);
  declareProperty( "P1_charm_scale", m_P1charm =  -0.8247);
  declareProperty( "P2_charm_scale", m_P2charm =  0.6732);
  declareProperty( "P3_charm_scale", m_P3charm =  0.);

  // initialize decay map
  //                                                                       Idx   K  E   Mu  D* M-    M+      MVA Cut     Purity
  CharmDecayModeMap["D0_Kpi"]          = CharmDecayMode("D0_Kpi",	         0,    1,	0,	0,	0, 1.82, 1.915, -0.0223261, 0.630175 );
  CharmDecayModeMap["D0_Kpipipi"]      = CharmDecayMode("D0_Kpipipi",	     1,    1,	0,	0,	0, 1.82, 1.915, -0.0118087, 0.420859 );
  CharmDecayModeMap["D0_Kpipi0"]       = CharmDecayMode("D0_Kpipi0",	     3,    1,	0,	0,	0, 1.82, 1.915, -0.0199824, 0.0592187);
  CharmDecayModeMap["Dp_Kpipi"]        = CharmDecayMode("Dp_Kpipi",	       4,    1,	0,	0,	0, 1.82, 1.915, -0.0120574, 0.525746 );
  CharmDecayModeMap["D0_KpiX"]         = CharmDecayMode("D0_KpiX",	       6,    1,	0,	0,	0, 0.8,  1.915, -0.0587858, 0.235058 );
  CharmDecayModeMap["D0_KeX"]          = CharmDecayMode("D0_KeX",	         7,    1,	1,	0,	0, 0.8,  1.915, -0.0483723, 0.291827 );
  CharmDecayModeMap["D0_KmuX"]         = CharmDecayMode("D0_KmuX",	       8,    1,	0,	1,	0, 0.8,  1.915, -0.0207928, 0.39514  );
  
}


TaggerCharmTool::~TaggerCharmTool() {}
//=====================================================================
StatusCode TaggerCharmTool::initialize()
{
  const StatusCode sc = GaudiTool::initialize();
  if ( sc.isFailure() ) return sc;

  //
  m_util = tool<ITaggingUtils> ( "TaggingUtils", this );
  if( ! m_util ) {
    fatal() << "Unable to retrieve TaggingUtils tool "<< endreq;
    return StatusCode::FAILURE;
  }
  m_pLifetimeFitter = tool<ILifetimeFitter>("PropertimeFitter", this);
  if(!m_pLifetimeFitter){
    fatal() << "Unable to retrieve LifetimeFitter tool" << endreq;
    return StatusCode::FAILURE;
  }
  m_descend = tool<IParticleDescendants> ( "ParticleDescendants", this );
  if( ! m_descend ) {
    fatal() << "Unable to retrieve ParticleDescendants tool "<< endreq;
    return StatusCode::FAILURE;
  }

  //  m_nnet = tool<INNetTool> ( "NNetTool_MLP", this);

  //   //load tmva readers
  //   int nFoundReaders = 0;

  //   if ( msgLevel(MSG::DEBUG) )  debug() << " Number of TMVA readers found: " << nFoundReaders << endreq;

  std::vector<std::string> variable_names;

  variable_names.push_back("mult"                    );
  variable_names.push_back("nnkrec"                  );
  variable_names.push_back("ptB"                     );
  variable_names.push_back("charm_mass"              );
  variable_names.push_back("charm_mode"              );
  variable_names.push_back("charm_bpvdira"           );
  variable_names.push_back("charm_probchi2"          );
  variable_names.push_back("charm_tau"               );
  variable_names.push_back("charm_flightdistchi2"    );
  variable_names.push_back("charm_kprobnnk"          );
  variable_names.push_back("charm_kippvchi2"         );
  variable_names.push_back("charm_maxprobnnghostdaus");

  m_myDATAreader = new CharmOSWrapper(variable_names);

  return sc;
}

//=====================================================================

StatusCode TaggerCharmTool::finalize()
{
  delete m_myDATAreader; 
  m_myDATAreader = NULL;
  for ( auto i : m_classifiers ) { delete i.second; }
  m_classifiers.clear();
  return GaudiTool::finalize();
}

//=====================================================================
Tagger TaggerCharmTool::tag( const Particle* signalB,
                             const RecVertex* RecVert,
                             const int nPV,
                             Particle::ConstVector& tagParticles )
{

  Tagger tcharm;

  std::vector< CharmParticle > cands;

  addCands(cands, m_CharmTagLocations    ,   *signalB, RecVert, 0);
  addCands(cands, m_CharmInclTagLocations,   *signalB, RecVert, 1);
  // Uncomment once I add in all the new trainings!
  //addCands(cands, m_CharmLambdaTagLocations, *signalB, RecVert, 3);

  if ( msgLevel(MSG::DEBUG) )  debug() << "Number of charm cands retrieved: "<<cands.size()<<endreq;

  // selection
  CharmParticle *thecharm = NULL;
  std::vector< CharmParticle >::iterator ipart;

  double max_mvaVar = -1., curr_purity = 0.;

  int ncands = 0, icand = 0, ipicked = -1;
  for (ipart=cands.begin(); ipart!=cands.end(); ++ipart) {

    CharmParticle *cpart = &(*ipart);
    std::string mode = cpart->mode;

    if ( msgLevel(MSG::DEBUG) )
      debug()<<"cand #"<<icand<<" flavour "<<cpart->flavour<<" mode "<<mode<<" ptr "<<cpart<<" thecharm "<<thecharm<<endreq;
    icand++;

    if (cpart->flavour==0) continue;

    ////////////
    // selection

    // mass
    if ( msgLevel(MSG::DEBUG) ) debug()<<"mass "<<cpart->mass<<endreq;
    if ( cpart->mass <= CharmDecayModeMap[mode].lowMcut || cpart->mass >= CharmDecayModeMap[mode].highMcut ) continue;

    // decay time
    if ( msgLevel(MSG::DEBUG) )
      debug()<<"tau "<<cpart->tau<<endreq;
    if ( cpart->tau <= -100 || cpart->tau >= 100 ) continue;

    if ( msgLevel(MSG::DEBUG) )    debug()<<"bpvdira "<<cpart->bpvdira<<endreq;
    if ( cpart->bpvdira <= 0.5 ) continue;

    // kaon vars
    if (CharmDecayModeMap[mode].hasK) {
      if ( msgLevel(MSG::DEBUG) )      debug()<<"kaonProbnnk "<<cpart->kaonProbnnk<<endreq;
      if ( cpart->kaonProbnnk <= 0.05 ) continue;
      if ( msgLevel(MSG::DEBUG) )      debug()<<" kaonIppvchi2 "<<cpart->kaonIppvchi2<<endreq;
      if ( cpart->kaonIppvchi2 <= 0 ) continue;
    }

    if ( msgLevel(MSG::DEBUG) )    debug()<<"maxProbGhostDaus "<<cpart->maxProbGhostDaus<<endreq;
    if ( cpart->maxProbGhostDaus >= 0.8 ) continue;

    if ( msgLevel(MSG::DEBUG) )    debug()<<"pchi2 "<<cpart->pchi2<<endreq;
    if( cpart->pchi2 <= 0.001 ) continue;

    // mva cut
    double mvaVar = getMvaVal(cpart, nPV, tagParticles.size(), *signalB);

    if ( msgLevel(MSG::DEBUG) )    debug()<<"mva "<<mvaVar<<endreq;
    if ( mvaVar < CharmDecayModeMap[mode].mvaCut ) continue;

    ncands++;

    if ( msgLevel(MSG::DEBUG) )    debug()<<"purity "<<CharmDecayModeMap[mode].purity<<endreq;

    if( mvaVar > max_mvaVar && CharmDecayModeMap[mode].purity >= curr_purity ) { //discrVar ordering
      thecharm = cpart;
      max_mvaVar = mvaVar;
      curr_purity = CharmDecayModeMap[mode].purity;
      ipicked = icand-1;
    }

  }

  if ( msgLevel(MSG::DEBUG) )
    debug() << "Number of charm cands selected: "<<ncands<<" picked idx "<<ipicked<<" thecharm"<<thecharm<<endreq;

  if(!thecharm) return tcharm;

  //calculate omega
  double omega = getOmega(thecharm, nPV, tagParticles.size(), *signalB);
  if ( msgLevel(MSG::DEBUG) )    debug()<<"omega "<<omega << endreq;
  if( omega < 0 ||  omega > 1 )  {
    if ( msgLevel(MSG::DEBUG) )    debug()<<"Something wrong with Charm Training "<<omega << endreq;
    return tcharm;
  }

  if ( msgLevel(MSG::DEBUG) )  debug()<<"the charm flavour "<<thecharm->flavour<<endreq;

  int decision = thecharm->flavour > 0 ? +1: -1; // double flip, one for opposite side, one for D from B

  tcharm.setOmega(omega);
  tcharm.setDecision(decision);
  tcharm.setType( Tagger::OS_Charm );

  // add charm candidate and daughters to tagger particles
  tcharm.addToTaggerParts(thecharm->part);
  const SmartRefVector<Particle>& charmDaus = thecharm->part->daughters();
  for( SmartRefVector<Particle>::const_iterator idau = charmDaus.begin(); idau != charmDaus.end(); ++idau)
    tcharm.addToTaggerParts(*idau);

  return tcharm;

}

//=====================================================================
int TaggerCharmTool::addCands(std::vector< CharmParticle >& cands, const std::vector<std::string>& locations,
                              const LHCb::Particle& signalB, const RecVertex* RecVert, const int type)
{

  for( std::vector<std::string>::const_iterator ilist = locations.begin(); ilist != locations.end(); ++ilist) {
    if (exist<Particle::Range>(*ilist)){
      LHCb::Particle::Range partsin  = get<Particle::Range>(*ilist) ;

      if ( msgLevel(MSG::DEBUG) )
        debug() << "Found "<<partsin.size()<<" charm cands for location "<<*ilist<<endreq;

      // removing candidates with daughters in common with signal B
      LHCb::Particle::ConstVector purgedParts = m_util->purgeCands(partsin, signalB);

      for (Gaudi::Range_<LHCb::Particle::ConstVector>::const_iterator icand = purgedParts.begin();
           icand != purgedParts.end(); ++icand) {

        const Particle *cand, *dstar_cand, *pisoft_cand;
        if (type==2) {
          dstar_cand = (*icand);
          cand = (*icand)->daughters().at(0);
          pisoft_cand = (*icand)->daughters().at(1);
        } else {
          dstar_cand = pisoft_cand = NULL;
          cand = (*icand);
        }

        // compute vars
        float mass = cand->measuredMass()/GeV;

        int charmId = cand->particleID().pid();
        int charmStarId = 0; if (dstar_cand) charmStarId = dstar_cand->particleID().pid();

        float dstarDm = dstar_cand ? dstar_cand->measuredMass()/GeV - mass : 0. ;

        double ct=0., ctErr=0., ctChi2=0.;
        double tau = -999.;
        StatusCode sc = m_pLifetimeFitter->fit(*RecVert, *cand, ct, ctErr, ctChi2);
        if(sc) tau = ct/picosecond;


        double pchi2 = TMath::Prob(cand->endVertex()->chi2(),cand->endVertex()->nDoF());

        // construct the functors
        Fun fDIRA = DIRA( RecVert );
        const float bpvdira = fDIRA( cand ) ;

        Fun fVD = VD ( RecVert );
        const float fd = fVD( cand ) ;

        Fun fVDCHI2 = VDCHI2 ( RecVert );
        const float fdchi2 = fVDCHI2( cand ) ;

        LHCb::VertexBase::ConstVector verts; verts.push_back(RecVert);
        Fun ipPvChi2 = MIPCHI2 ( verts , m_util->getDistanceCalculator() ) ;
        const MIPCHI2DV ipChi2Min  = MIPCHI2DV ( "" ) ;

        std::string mode = m_util->getCharmDecayMode(cand,type);

        if ( msgLevel(MSG::DEBUG) )        debug()<<"mode "<<mode<<endreq;

        float maxProbGhostDaus = 0.;
        float kaonId = 0, kaonProbnnk = 1., kaonIppvchi2 = -1, kaonIpMinchi2 = -1;
        float elecProbnne = 1., muonProbnnmu = 1.;

        const SmartRefVector<Particle>& daus = cand->daughters();
        if ( msgLevel(MSG::DEBUG) )
          debug() << "Charm cand, dau vector size = " << daus.size() << endreq;
        for( SmartRefVector<Particle>::const_iterator idau = daus.begin(); idau != daus.end(); ++idau) {
          const Particle *daucand = *idau;
          if (daucand->proto()) {
            float pghostnn = daucand->proto()->info(ProtoParticle::ProbNNghost, -1. );
            if(maxProbGhostDaus < pghostnn ) maxProbGhostDaus = pghostnn;
          }
          if ( daucand->particleID() == LoKi::Particles::_ppFromName("K+")->particleID() ||
               daucand->particleID() == LoKi::Particles::_ppFromName("K-")->particleID() ) {
            kaonId = daucand->particleID().pid();
            if (daucand->proto()) {
              kaonProbnnk = daucand->proto()->info(ProtoParticle::ProbNNk, -1. );
            }
            kaonIppvchi2 = ipPvChi2(daucand) ;
            kaonIpMinchi2 = ipChi2Min(daucand) ;        // use the functor:

          }

          if ( daucand->particleID() == LoKi::Particles::_ppFromName("e+")->particleID() ||
               daucand->particleID() == LoKi::Particles::_ppFromName("e-")->particleID() )
            if (daucand->proto())
              elecProbnne = daucand->proto()->info(ProtoParticle::ProbNNe, -1. );

          if ( daucand->particleID() == LoKi::Particles::_ppFromName("mu+")->particleID() ||
               daucand->particleID() == LoKi::Particles::_ppFromName("mu-")->particleID() )
            if (daucand->proto())
              muonProbnnmu = daucand->proto()->info(ProtoParticle::ProbNNmu, -1. );

        }

        // compute flavour charge
        int flavour = 0;
        if (mode=="D0_Kpi" || mode=="D0_Kpipipi" || mode=="D0_Kpipi0" || mode=="D0_KpiX" || mode=="D0_KeX" || mode=="D0_KmuX")
          flavour = -1 * int(kaonId)/abs(int(kaonId));
        else if (mode=="Dp_Kpipi" || mode=="Dp_Kspi" || mode=="Dp_KpiX" || mode=="Dp_KeX" || mode=="Dp_KmuX")
          flavour = int(charmId)/abs(int(charmId));
        else if (mode=="Dstar_D0_Kspipi")
          flavour = int(charmStarId)/abs(int(charmStarId));

        cands.push_back(CharmParticle(mode, cand, mass, flavour,
                                      pchi2, fd, fdchi2, tau, bpvdira,
                                      maxProbGhostDaus,
                                      kaonId, kaonProbnnk, kaonIppvchi2, kaonIpMinchi2,
                                      elecProbnne, muonProbnnmu,
                                      dstarDm));
      }

    } else {
      error() << "Location not found: "<<*ilist<<endreq;
    }

  }

  return 0;

}

//=====================================================================
double TaggerCharmTool::getMvaVal(const CharmParticle *cpart, const int nPV, const int multiplicity,
                                  const LHCb::Particle& signalB)
{

  const Particle *part = cpart->part;
  const std::string mode = cpart->mode;

  CharmDecayMode decay = CharmDecayModeMap[mode];

  std::vector<double> inputVals;

  inputVals.push_back(nPV);
  inputVals.push_back(multiplicity);

  inputVals.push_back(signalB.pt()/GeV);

  inputVals.push_back(part->measuredMass()/GeV);
  inputVals.push_back(part->p()/GeV);
  inputVals.push_back(part->pt()/GeV);
  inputVals.push_back(part->momentum().Eta() - signalB.momentum().Eta());

  inputVals.push_back(safe_log(cpart->pchi2));

  inputVals.push_back(safe_log(cpart->tau));
  inputVals.push_back(safe_log(cpart->fd));

  inputVals.push_back(safe_log(cpart->fdchi2));
  inputVals.push_back(safe_log(1-cpart->bpvdira));
  inputVals.push_back(safe_log(cpart->maxProbGhostDaus));

  if (decay.hasK) {
    inputVals.push_back(safe_log(1-cpart->kaonProbnnk));
    inputVals.push_back(safe_log(cpart->kaonIppvchi2));
    inputVals.push_back(safe_log(cpart->kaonIpMinchi2));
  }

  if (decay.hasE)
    inputVals.push_back(safe_log(1-cpart->elecProbnne));

  if (decay.hasMu)
    inputVals.push_back(safe_log(1-cpart->muonProbnnmu));

  if (decay.hasDstar) {
    inputVals.push_back(cpart->dstarDm);
  }

  if ( msgLevel(MSG::DEBUG) )
    debug()<<"Set MvaCharm Var: recv "<<nPV<<" mult "<<multiplicity<<" sigpt "<<signalB.pt()/GeV
           <<" m "<<part->measuredMass()/GeV<<" p "<<part->p()/GeV<<" pt "<<part->pt()/GeV
           <<" deta "<<part->momentum().Eta() - signalB.momentum().Eta()
           <<" logpchi2 "<<safe_log(cpart->pchi2)<<" logtau "<<safe_log(cpart->tau)<<" logfd "<<safe_log(cpart->fd)
           <<" logfdchi2 "<<safe_log(cpart->fdchi2)<<" logdira "<<safe_log(1-cpart->bpvdira)<<" logmghost "<<safe_log(cpart->maxProbGhostDaus)
           <<" logkprobk "<<safe_log(1-cpart->kaonProbnnk)<<" logkipchi2 "<<safe_log(cpart->kaonIppvchi2)
           <<" logprobe "<<safe_log(1-cpart->elecProbnne)<<" logprobmu "<<safe_log(1-cpart->muonProbnnmu)
           <<" dstarm "<<cpart->dstarDm
           <<endreq;

  TMVAWrapper * mva = getMVA(mode);
  return ( mva ? mva->GetMvaValue(inputVals) : -10.0 );

}

TMVAWrapper * TaggerCharmTool::getMVA( const std::string& mode )
{
  Classifiers::const_iterator iC = m_classifiers.find(mode);

  // If found, just return
  if ( iC != m_classifiers.end() ) { return iC->second; }

  // Else, make the MVA

  // initialize classifier
  // CRJ : Only create as needed, as oppossed to all all the time, to reduce memory footprint....

  std::map<std::string, std::vector< std::string> > inputVarMap;
  for( std::map<std::string, CharmDecayMode>::iterator iter = CharmDecayModeMap.begin();
       iter !=CharmDecayModeMap.end(); ++iter )
  {
    CharmDecayMode& decay = iter->second;
    std::vector< std::string > inputVars;

    inputVars.push_back("RecVerts");
    inputVars.push_back("Ntrks");
    inputVars.push_back("SigBpt");
    inputVars.push_back("Mass");
    inputVars.push_back("P");
    inputVars.push_back("Pt");
    inputVars.push_back("DeltaEta");

    inputVars.push_back("log(Probchi2)");
    inputVars.push_back("log(Tau)");
    inputVars.push_back("log(FlightDist)");
    inputVars.push_back("log(FlightDistChi2)");
    inputVars.push_back("log(1-BpvDira)");
    inputVars.push_back("log(MaxProbNNGhostDaus)");

    if (decay.hasK) {
      inputVars.push_back("log(1-KaonProbNNk)");
      inputVars.push_back("log(KaonIppvChi2)");
      inputVars.push_back("log(KaonIpMinChi2)");
    }

    if (decay.hasE) {
      inputVars.push_back("log(1-ElecProbNNe)");
    }

    if (decay.hasMu) {
      inputVars.push_back("log(1-MuonProbNNmu)");
    }

    if (decay.hasDstar) {
      inputVars.push_back("DstarDeltaM");
    }

    inputVarMap[decay.mode] = inputVars;
  }

  if      ( mode == "D0_Kpi" )
  {
    m_classifiers[mode] = new CharmD0KpiWrapper(inputVarMap["D0_Kpi"]);
  }
  else if ( mode == "D0_Kpipipi" )
  {
    m_classifiers[mode] = new CharmD0KpipipiWrapper(inputVarMap["D0_Kpipipi"]);
  }
  else if ( mode == "D0_Kpipi0" )
  {
    m_classifiers[mode] = new CharmD0Kpipi0Wrapper(inputVarMap["D0_Kpipi0"]);
  }
  else if ( mode == "Dp_Kpipi" )
  {
    m_classifiers[mode] = new CharmDpKpipiWrapper(inputVarMap["Dp_Kpipi"]);
  }
  else if ( mode == "D0_KpiX" )
  {
    m_classifiers[mode] = new CharmD0KpiXWrapper(inputVarMap["D0_KpiX"]);
  }
  else if ( mode == "D0_KeX" )
  {
    m_classifiers[mode] = new CharmD0KeXWrapper(inputVarMap["D0_KeX"]);
  }
  else if ( mode == "D0_KmuX" )
  {
    m_classifiers[mode] = new CharmD0KmuXWrapper(inputVarMap["D0_KmuX"]);
  }
  else
  {
    Exception( "Unknown mode '"+mode+"'" );
  }

  return m_classifiers[mode];
}

/////////////////////////////////////////////
double TaggerCharmTool::getOmega(const CharmParticle* cpart, const int nPV, const int multiplicity, const LHCb::Particle& signalB)
{

  const Particle *part = cpart->part;
  std::vector<double> inputVals;
  inputVals.push_back( (double)multiplicity                        );
  inputVals.push_back( (double)nPV                                 );
  inputVals.push_back( (double)safe_log(signalB.pt()/GeV)          );
  inputVals.push_back( (double)part->measuredMass()/GeV            );
  inputVals.push_back( (double)CharmDecayModeMap[cpart->mode].index);
  inputVals.push_back( (double)cpart->bpvdira                      );
  inputVals.push_back( (double)cpart->pchi2                        );
  inputVals.push_back( (double)cpart->tau                          );
  inputVals.push_back( (double)cpart->fdchi2                       );
  inputVals.push_back( (double)cpart->kaonProbnnk                  );
  inputVals.push_back( (double)cpart->kaonIppvchi2                 );
  inputVals.push_back( (double)cpart->maxProbGhostDaus             );

  double rnet = m_myDATAreader->GetMvaValue(inputVals);
  double pn;
  
  if (rnet>=0 && rnet<=1) {
    pn = 1.0 - (m_P0charm + m_P1charm * (rnet - m_P2charm));
    verbose()<<"Using MLPBNN for charm - rnet "<<rnet<<" pn "<<pn<<endmsg;
  } else {
    debug()<<"**********************BAD TRAINING charm"<<rnet<<endmsg;
    pn = -1.;
  }

  double omega = 1 - pn;

  if (msgLevel(MSG::DEBUG)) {
    debug()<<"Set NNetCharm Omega Inputs:";
    for (unsigned int ivec = 0; ivec<inputVals.size();ivec++)
      debug()<<" "<<inputVals[ivec];    
    debug()<<endreq;
  }

  //Calibration (w=1-pn) w' = p0 + p1(w-eta)
  omega = m_P0_Cal_charm + m_P1_Cal_charm * ( omega-m_Eta_Cal_charm);
  if ( msgLevel(MSG::DEBUG) )  debug()<<" Vtx pn="<<1-omega<<" w="<<omega<<endmsg;

  return omega;
}
