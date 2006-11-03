// $Id: TrackPrepareVelo.cpp,v 1.6 2006-11-03 08:47:51 mneedham Exp $
//
// This File contains the implementation of the TsaEff
// C++ code for 'LHCb Tracking package(s)'
//

// Gaudi
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "gsl/gsl_math.h"

// track interfaces
#include "Event/State.h"

#include "TrackPrepareVelo.h"

using namespace LHCb;
using namespace Gaudi::Units;
using namespace Gaudi;

// Needed for the creation of TrackPrepareVelo objects.
static const AlgFactory<TrackPrepareVelo> s_factory;
const IAlgFactory& TrackPrepareVeloFactory = s_factory;

TrackPrepareVelo::TrackPrepareVelo(const std::string& name,
                       ISvcLocator* pSvcLocator):
  GaudiAlgorithm(name, pSvcLocator)
{
  // constructor
  declareProperty( "inputLocation", m_inputLocation = TrackLocation::Velo);
  declareProperty( "outputLocation", m_outputLocation = "/Event/Rec/Track/PreparedVelo");
  declareProperty( "bestLocation", m_bestLocation = TrackLocation::Default);
  declareProperty( "ptVelo", m_ptVelo = 400.*MeV);
}

TrackPrepareVelo::~TrackPrepareVelo()
{
  // destructor
}

StatusCode TrackPrepareVelo::initialize(){
  m_ignoreBest = ( m_bestLocation == "" ) ? true : false;
  return StatusCode::SUCCESS;
}

StatusCode TrackPrepareVelo::execute(){

  Tracks* inCont = get<Tracks>(m_inputLocation);
  Tracks* bestCont = 0 ;
  if ( !m_ignoreBest ) bestCont = get<Tracks>(m_bestLocation);
  Tracks* outCont = new Tracks();
  
  // loop 
  unsigned int i = 0;
  Tracks::const_iterator iterT = inCont->begin();
  for (; iterT != inCont->end(); ++iterT, ++i){
    if ( m_ignoreBest || (used(*iterT,bestCont) == false) ){
      Track* aTrack = (*iterT)->clone();
      int charge = 0;
      i % 2  == 0 ? charge = -1 : charge = 1;   
      prepare(aTrack, charge );
      outCont->insert(aTrack);
    }
  } // iterT
    
  put(outCont,m_outputLocation);

  return StatusCode::SUCCESS;
};

bool TrackPrepareVelo::used(const Track* aTrack, const Tracks* bestCont) const{

  // check if velo track is used.
  bool found = false;
  Tracks::const_iterator iterT =  bestCont->begin();
  while ((iterT != bestCont->end())&&(found == false)){
    const SmartRefVector<LHCb::Track>& parents = (*iterT)->ancestors();
    for ( SmartRefVector<LHCb::Track>::const_iterator iterP = parents.begin();
          iterP != parents.end(); ++iterP) {
      const Track* testTrack = *iterP;
      if ( testTrack == aTrack) found = true;
    } // iterP
    ++iterT;
  } // iterT

  return found;
}

void TrackPrepareVelo::prepare(Track* aTrack, const int charge) const{

 // do what we have to do...
 State& vState = aTrack->firstState();
 TrackVector& vec = vState.stateVector();
 double slope2 = GSL_MAX(vec(2)*vec(2) + vec(3)*vec(3), 1e-20);
 double curv = charge * sqrt( slope2 ) / (m_ptVelo * sqrt( 1. + slope2 ));
 vState.setQOverP(curv);
 vState.setErrQOverP2(1e-6);
}
