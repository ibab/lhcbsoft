// Include files 
#include "GaudiKernel/DeclareFactoryEntries.h"
// from DaVinci
#include "Kernel/StringUtils.h"
// local
#include "BTagging.h"

//-----------------------------------------------------------------------
// Implementation file for class : BTagging
//
// Author: Marco Musy
//-----------------------------------------------------------------------

using namespace LHCb ;
using namespace Gaudi::Units;

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( BTagging );

//=======================================================================
BTagging::BTagging(const std::string& name,
                   ISvcLocator* pSvcLocator):
  DVAlgorithm(name, pSvcLocator){
  
  declareProperty( "TagOutputLocation", 
                   m_TagLocation = FlavourTagLocation::Default );
}

//=======================================================================
StatusCode BTagging::initialize() { return DVAlgorithm::initialize() ; }

BTagging::~BTagging() {}

//=======================================================================
// Main execution
//=======================================================================
StatusCode BTagging::execute() {

  setFilterPassed( false );

  //look in location where Selection has put the B candidates
  const LHCb::Particle::ConstVector& parts = desktop()->particles();
  if( parts.empty() ) return StatusCode::SUCCESS;
  debug() << "BTagging will tag "<< parts.size() << " B hypos!" <<endreq;

  //-------------- loop on signal B candidates from selection
  FlavourTags*  tags = new FlavourTags;
  Particle::ConstVector::const_iterator icandB;
  for ( icandB = parts.begin(); icandB != parts.end(); icandB++){
    if((*icandB)->particleID().hasBottom()) {
      debug() << "About to tag candidate B of mass=" 
              << (*icandB)->momentum().M()/GeV <<" GeV"<<endreq;

      FlavourTag* theTag = new FlavourTag;

      //--------------------- TAG IT ---------------------
      //use tool for tagging by just specifing the signal B
      StatusCode sc = flavourTagging() -> tag( *theTag, *icandB );

      //use tool for tagging if you want to specify the Primary Vtx
      //StatusCode sc = flavourTagging() -> tag( *theTag, *icandB, PVertex );

      //use tool for tagging if you want to specify a list of particles
      //StatusCode sc = flavourTagging() -> tag( *theTag, *icandB, PVertex, vtags );
      //--------------------------------------------------
      if (!sc) {
        err() <<"Tagging Tool returned error."<< endreq;
        delete theTag;
      } else tags->insert(theTag);

      //--- PRINTOUTS ---
      //print the information in theTag
      int tagdecision = theTag->decision();
      if(tagdecision) debug() << "Flavour guessed: " 
                              << (tagdecision>0 ? "b":"bbar")<<endreq;
      debug() << "estimated omega= " << theTag->omega() <<endreq;
      const Particle* tagB = theTag->taggedB();
      if( tagB ) debug() << "taggedB p="<< tagB->p()/GeV <<endreq;

      ///print Taggers information
      std::vector<Tagger> mytaggers = theTag->taggers();
      std::vector<Tagger>::iterator itag;
      for(itag=mytaggers.begin(); itag!=mytaggers.end(); ++itag) {
        std::string tts;
        switch ( itag->type() ) {
        case Tagger::none        : tts="none";        break;
        case Tagger::unknown     : tts="unknown";     break;
        case Tagger::OS_Muon     : tts="OS_Muon";     break;
        case Tagger::OS_Electron : tts="OS_Electron"; break;
        case Tagger::OS_Kaon     : tts="OS_Kaon";     break;
        case Tagger::SS_Kaon     : tts="SS_Kaon";     break;
        case Tagger::SS_Pion     : tts="SS_Pion";     break;
        case Tagger::jetCharge   : tts="jetCharge";   break;
        case Tagger::OS_jetCharge: tts="OS_jetCharge";break;
        case Tagger::SS_jetCharge: tts="SS_jetCharge";break;
        case Tagger::VtxCharge   : tts="VtxCharge";   break;
        case Tagger::Topology    : tts="Topology";    break;
        }
        debug() << "--> tagger type: " << tts <<endreq;
        debug() << "    decision = "
                << (itag->decision() > 0? "b":"bbar") <<endreq;
        debug() << "    omega    = " << itag->omega() <<endreq;
        std::vector<const Particle*> taggerparts = itag->taggerParts();
        std::vector<const Particle*>::iterator kp;
        for(kp=taggerparts.begin(); kp!=taggerparts.end(); kp++) {
          verbose() << "    ID:" <<std::setw(4)<< (*kp)->particleID().pid() 
                    << " p= "  << (*kp)->p()/GeV << endreq;
        }
      }
    }
  }

  ///Output to TES (for backward compatibility) 
  if(! (tags->empty()) ) put(tags, m_TagLocation);

  setFilterPassed( true );
  return StatusCode::SUCCESS;
}

//=========================================================================
StatusCode BTagging::finalize() { return DVAlgorithm::finalize(); }

//==========================================================================
