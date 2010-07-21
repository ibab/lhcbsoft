// $Id: TriggerSelectionTisTosInHlt.cpp,v 1.9 2010-07-21 21:22:17 tskwarni Exp $
// Include files 
#include <algorithm>
#include <vector>
#include <sstream>

// from Gaudi    
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/StringKey.h"
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/IIncidentSvc.h"

// local
#include "TriggerSelectionTisTosInHlt.h"
#include "ParticleTisTos.h"

#include "Event/HltDecReports.h"

#include "HltBase/HltSelection.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : TriggerSelectionTisTosInHlt
//
// 2009-10-22 : Tomasz Skwarnicki
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( TriggerSelectionTisTosInHlt );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TriggerSelectionTisTosInHlt::TriggerSelectionTisTosInHlt( const std::string& type,
                                                const std::string& name,
                                                const IInterface* parent )
  : ParticleTisTos ( type, name , parent )
  , m_hltDecReports(0)
  , m_hltDataSvc(0)
  , m_hltInspectorSvc(0)
  , m_objectSummaries(0)
{
  declareInterface<ITriggerSelectionTisTos>(this);
  
   declareProperty("HltDecReportsLocation",
                   m_HltDecReportsLocation = LHCb::HltDecReportsLocation::Default); 
   
  m_cached_SelectionNames.reserve(500);
  m_cached_tisTosTob.reserve(500);

}


//=============================================================================
// Destructor
//=============================================================================
TriggerSelectionTisTosInHlt::~TriggerSelectionTisTosInHlt() {} 

//=============================================================================
// Initialization
//=============================================================================
StatusCode TriggerSelectionTisTosInHlt::initialize() {
  StatusCode sc = ParticleTisTos::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  debug() << "==> Initialize" << endmsg;

  IIncidentSvc*                incidentSvc(0);     
  if (!service( "IncidentSvc", incidentSvc).isSuccess()) return StatusCode::FAILURE;
  // add listener to be triggered by first BeginEvent
  bool rethrow = false; bool oneShot = false; long priority = 0;
  incidentSvc->addListener(this,IncidentType::BeginEvent,priority,rethrow,oneShot);
  incidentSvc->release();

  m_hltDataSvc = svc<Hlt::IData>("Hlt::Service",true);
  m_hltInspectorSvc = svc<Hlt::IInspector>("Hlt::Service",true);

  m_newEvent =true;
   
  setOfflineInput();
  
  return StatusCode::SUCCESS;

}

void TriggerSelectionTisTosInHlt::handle(const Incident& ) 
{
  m_hltDecReports=0;

  setOfflineInput( );

  m_newEvent=true;  

  m_objectSummaries =0;
  
}

// ------------------------------------------------------------------------------------
void TriggerSelectionTisTosInHlt::getHltSummary()
{
  if( !m_hltDecReports ){
    if( exist<HltDecReports>(m_HltDecReportsLocation) ){    
      m_hltDecReports = get<HltDecReports>(m_HltDecReportsLocation);
    } else {
      Error( " No HltDecReports at "+m_HltDecReportsLocation.value(), StatusCode::FAILURE, 2 ).setChecked();
      m_hltDecReports =0;
    }    
  }  
}

//=============================================================================
// -------------- offline inputs -----------------------------------------------
//=============================================================================

// erase previous input ---------------------------------------------------------
void TriggerSelectionTisTosInHlt::setOfflineInput( )
{
  setSignal();  clearCache();
}
   

//    hit list input ---------------------------------------------------------------
void TriggerSelectionTisTosInHlt::addToOfflineInput( const std::vector<LHCb::LHCbID> & hitlist )
{
  if( addToSignal( hitlist ) )clearCache();
}

//    Track input ---------------------------------------------------------------
void TriggerSelectionTisTosInHlt::addToOfflineInput( const LHCb::Track & track )
{
  if( addToSignal( track ) )clearCache();
}

//    Proto-particle input -----------------------------------------------------------------------
void TriggerSelectionTisTosInHlt::addToOfflineInput( const LHCb::ProtoParticle & protoParticle )
{
  if( addToSignal( protoParticle ) )clearCache();
}

//    Particle input -----------------------------------------------------------------------
void TriggerSelectionTisTosInHlt::addToOfflineInput( const LHCb::Particle & particle )
{
  if( addToSignal( particle ) )clearCache();
}


 
//=============================================================================


//=============================================================================
// -------------- outputs:
//=============================================================================



// single complete Trigger Selection TisTos  (define Offline Input before calling)
unsigned int TriggerSelectionTisTosInHlt::tisTosSelection( const std::string & selectionName )
{
  unsigned int result=0;
  if( findInCache( selectionName, result ) )return result;

  getHltSummary();

  bool decision(false);  
  // get decision from HltDecReports if can find it
  bool decFound(false);  
  if( m_hltDecReports ){
    const HltDecReport* rep=m_hltDecReports->decReport( selectionName );
    if( rep ){
      decision = rep->decision();
      decFound = true;
    }    
  }
  if( decision )result |= kDecision;  

  const Gaudi::StringKey name(selectionName);
  // find producer of the selection
  const IAlgorithm* producer = m_hltInspectorSvc->producer(name);
  if( !producer ){storeInCache(selectionName,result); return result;}
  const Hlt::Selection* sel = m_hltDataSvc->retrieve(producer,name);
  if( !sel ){ storeInCache(selectionName,result); return result;}
  // unsuccessful selections not allowed to be TisTossed
  if( !(sel->decision()) ){ storeInCache(selectionName,result); return result;}

  // it is possible that decision was not stored in HltDecReports if this was an intermediate step, thus
  //   if not found in HltDecReports, set it here
  // do not overwrite decisions from HltDecReports, since positive selection could have been killed by postscale
  if( !decFound )decision=true;

  // classify the decision
  if( sel->size() > 0 ){

    if( sel->classID() == LHCb::Track::classID() ) {

      const Hlt::TrackSelection& tsel = dynamic_cast<const Hlt::TrackSelection&>(*sel);   
      const std::vector<Track*> tracks = tsel.vct();
      if (tracks.size() >0) {
        result = IParticleTisTos::tisTos<Track>(tracks);
      }
      
    } else if( sel->classID() == LHCb::RecVertex::classID() ) {

      const Hlt::VertexSelection& tsel = dynamic_cast<const Hlt::VertexSelection&>(*sel);     
      const std::vector<RecVertex*> vertices = tsel.vct();
      if (vertices.size() >0) {
        result = IParticleTisTos::tisTos<RecVertex>(vertices);
      }

    }  else if( sel->classID() == LHCb::Particle::classID() ) {

      const Hlt::TSelection<LHCb::Particle>& tsel = dynamic_cast<const Hlt::TSelection<LHCb::Particle>&>(*sel);   
      const std::vector<Particle*> particles = tsel.vct();
      if( particles.size() > 0 ){
        result = IParticleTisTos::tisTos<Particle>(particles);
      }

    } else {
      std::vector<std::string> dependencies( sel->inputSelectionsIDs().begin(), sel->inputSelectionsIDs().end());
      if (!dependencies.empty()) {
        result = ITriggerSelectionTisTos::tisTosSelection(dependencies);        
      } else {
        // @TODO: warning: hltDataSvc doesn't know about selInput...
      }

    }
  }

  if( decision )result |= kDecision; 
  storeInCache(selectionName,result);
  return result;  
}



// single complete Trigger Selection TisTos  (define Offline Input before calling)
std::string TriggerSelectionTisTosInHlt::analysisReportSelection( const std::string & selectionName )
{


  std::ostringstream report;
  report << offset() << " Selection " + selectionName + " ";  

  unsigned int result=0;

  getHltSummary();

  bool decision(false);  
  // get decision from HltDecReports if can find it
  bool decFound(false);  
  if( m_hltDecReports ){
    const HltDecReport* rep=m_hltDecReports->decReport( selectionName );
    if( rep ){
      decision = rep->decision();
      decFound = true;
      report << " HltDecReport decision=" << decision;   
    }    
  }
  if( decision )result |= kDecision;  

  const Gaudi::StringKey name(selectionName);
  // find producer of the selection
  const IAlgorithm* producer = m_hltInspectorSvc->producer(name);
  if( !producer ){ report << "not known to HltInspectorSvc " << std::endl; return report.str();}
  const Hlt::Selection* sel = m_hltDataSvc->retrieve(producer,name);
  if( !sel ){ report << "not found in HltDataSvc " << std::endl; return report.str();}
  // unsuccessful selections not allowed to be TisTossed
  if( !(sel->decision()) ){ report << "decision=false " << std::endl; return report.str();}

  // it is possible that decision was not stored in HltDecReports if this was an intermediate step, thus
  //   if not found in HltDecReports, set it here
  // do not overwrite decisions from HltDecReports, since positive selection could have been killed by postscale
  if( !decFound )decision=true;

  report << " size " << sel->size() << std::endl;

  if( sel->size() > 0 ){

    if( sel->classID() == LHCb::Track::classID() ) {

      const Hlt::TrackSelection& tsel = dynamic_cast<const Hlt::TrackSelection&>(*sel);   
      const std::vector<Track*> tracks = tsel.vct();
      if (tracks.size() >0) {
        report << analysisReport<Track>(tracks);
        result = IParticleTisTos::tisTos<LHCb::Track>(tracks);
      }
      
    } else if( sel->classID() == LHCb::RecVertex::classID() ) {

      const Hlt::VertexSelection& tsel = dynamic_cast<const Hlt::VertexSelection&>(*sel);     
      const std::vector<RecVertex*> vertices = tsel.vct();
      if (vertices.size() >0) {
        report<< analysisReport<RecVertex>(vertices);
        result = IParticleTisTos::tisTos<RecVertex>(vertices);
      }

    }  else if( sel->classID() == LHCb::Particle::classID() ) {

      const Hlt::TSelection<LHCb::Particle>& tsel = dynamic_cast<const Hlt::TSelection<LHCb::Particle>&>(*sel);   
      const std::vector<Particle*> particles = tsel.vct();
      if( particles.size() > 0 ){
        report<< analysisReport<Particle>(particles);
        result = IParticleTisTos::tisTos<Particle>(particles);
      }

    } else {
      std::vector<std::string> dependencies( sel->inputSelectionsIDs().begin(), sel->inputSelectionsIDs().end());
      if (!dependencies.empty()) {
        report << offset() << " Input Selections Vector size=" << dependencies.size() << std::endl;  
        ++m_reportDepth;
        for( std::vector<std::string>::const_iterator iSel=dependencies.begin();iSel!=dependencies.end();++iSel){
          report << analysisReportSelection( *iSel );
        }
        --m_reportDepth;
        result = ITriggerSelectionTisTos::tisTosSelection(dependencies);        
      } else {
        report << " not a Track/RecVertex/Particle selection and no Input Selections " << std::endl;        
      }

    }
  }
  if( decision )result |= kDecision;  
  TisTosTob res( result );
  report << offset() << " Selection " + selectionName + " "  
         << " TIS= " << res.tis() << " TOS= " << res.tos() << " TPS= " << res.tps() 
         << " decision= " << res.decision() << std::endl;
  return report.str();
}

 
// fast check for TOS
#define TOSTISTPS( FUN )\
{\
  getHltSummary();\
  if( m_hltDecReports ){\
    const HltDecReport* rep=m_hltDecReports->decReport( selectionName );\
    if( rep ){\
      if( !(rep->decision()) )return false;      \
    }    \
  }\
  const Gaudi::StringKey name(selectionName);\
  const IAlgorithm* producer = m_hltInspectorSvc->producer(name);\
  if( !producer )return false;\
  const Hlt::Selection* sel = m_hltDataSvc->retrieve(producer,name);\
  if( !sel )return false;\
  if( !(sel->decision()) )return false;\
  if( !(sel->size()) )return false;\
  if( sel->classID() == LHCb::Track::classID() ) {\
    const Hlt::TrackSelection& tsel = dynamic_cast<const Hlt::TrackSelection&>(*sel);   \
    const std::vector<Track*> tracks = tsel.vct();\
    if (tracks.size() >0) {\
      return IParticleTisTos::FUN<LHCb::Track>(tracks);\
    }      \
  } else if( sel->classID() == LHCb::RecVertex::classID() ) {\
    const Hlt::VertexSelection& tsel = dynamic_cast<const Hlt::VertexSelection&>(*sel);     \
    const std::vector<RecVertex*> vertices = tsel.vct();\
    if (vertices.size() >0) {\
      return IParticleTisTos::FUN<RecVertex>(vertices);\
    }\
  }  else if( sel->classID() == LHCb::Particle::classID() ) {\
    const Hlt::TSelection<LHCb::Particle>& tsel = dynamic_cast<const Hlt::TSelection<LHCb::Particle>&>(*sel);   \
    const std::vector<Particle*> particles = tsel.vct();\
    if( particles.size() > 0 ){\
      return IParticleTisTos::FUN<Particle>(particles);\
    }\
  } else {\
    std::vector<std::string> dependencies( sel->inputSelectionsIDs().begin(), sel->inputSelectionsIDs().end());\
    if (!dependencies.empty()) {\

bool TriggerSelectionTisTosInHlt::tosSelection( const std::string & selectionName )
TOSTISTPS( tos )
      return ITriggerSelectionTisTos::tosSelection(dependencies);        
    }
  }
  return false;  
}

bool TriggerSelectionTisTosInHlt::tisSelection( const std::string & selectionName )
TOSTISTPS( tis )
      return ITriggerSelectionTisTos::tisSelection(dependencies);        
    }
  }
  return false;  
}

bool TriggerSelectionTisTosInHlt::tusSelection( const std::string & selectionName )
TOSTISTPS( tus )
      return ITriggerSelectionTisTos::tusSelection(dependencies);        
    }
  }
  return false;  
}

// ------------ auxiliary output:  list of LHCbIDs corresponding to present offline input
std::vector<LHCb::LHCbID> TriggerSelectionTisTosInHlt::offlineLHCbIDs() 
{
  return signal();
}


#define TISTOSSELECTSTORE() \
      unsigned int result = tisTos(*object);\
      bool tis,tos,tps;\
      tis = result & kTIS;\
      tos = result & kTOS;\
      tps = result & kTPS;\
      if(    ((tisRequirement>=kAnything)||(tis==tisRequirement)) \
             && ((tosRequirement>=kAnything)||(tos==tosRequirement)) \
             && ((tpsRequirement>=kAnything)||(tps==tpsRequirement)) \
             ){\
        HltObjectSummary* hos(0);\
        for( HltObjectSummarys::iterator ppHos=m_objectSummaries->begin();\
             ppHos!=m_objectSummaries->end();++ppHos){\
          HltObjectSummary* pHos(*ppHos);    \
          if( pHos->summarizedObjectCLID() == object->clID() ){\
            if( pHos->summarizedObject() == object ) hos = pHos;\
          }\
        }\
        if( !hos ){          \
          hos = new HltObjectSummary();\
          m_objectSummaries->push_back(hos);\
          hos->setSummarizedObjectCLID( object->clID() );\
          hos->setSummarizedObject(object);\
        }        \
        matchedObjectSummaries.push_back( hos );\
      }\


// ------------  additional functionality:  lists of object summaries for tracks/vertices/particles from trigger selections
//               satisfying TIS, TOS requirements

std::vector<const LHCb::HltObjectSummary*> TriggerSelectionTisTosInHlt::hltSelectionObjectSummaries(
                      const std::string & selectionName,
                      unsigned int tisRequirement,
                      unsigned int tosRequirement,
                      unsigned int tpsRequirement)
{
  std::vector<const LHCb::HltObjectSummary*> matchedObjectSummaries;

  getHltSummary();
  if( !m_objectSummaries ){
    if( exist<HltObjectSummary::Container>("/Event/Hlt/TriggerSelectionTisTosInHltStore") ){
      m_objectSummaries =  get<HltObjectSummary::Container>("/Event/Hlt/TriggerSelectionTisTosInHltStore");
    } else  {      
      m_objectSummaries = new HltObjectSummary::Container();
      put( m_objectSummaries,"/Event/Hlt/TriggerSelectionTisTosInHltStore");
    }      
  }  


  const Gaudi::StringKey name(selectionName);
  // find producer of the selection
  const IAlgorithm* producer = m_hltInspectorSvc->producer(name);
  if( !producer )return matchedObjectSummaries;
  const Hlt::Selection* sel = m_hltDataSvc->retrieve(producer,name);
  if( !sel )return matchedObjectSummaries;
  if( !(sel->size()) )return matchedObjectSummaries;

  if( sel->classID() == LHCb::Track::classID() ) {

    const Hlt::TrackSelection& tsel = dynamic_cast<const Hlt::TrackSelection&>(*sel);   
    const std::vector<Track*> objects = tsel.vct();
    for( std::vector<Track*>::const_iterator obj=objects.begin();obj!=objects.end();++obj){
      Track* object= *obj; 
      TISTOSSELECTSTORE()
    }

  } else if( sel->classID() == LHCb::RecVertex::classID() ) {

    const Hlt::VertexSelection& tsel = dynamic_cast<const Hlt::VertexSelection&>(*sel);     
    const std::vector<RecVertex*> objects = tsel.vct();
    for( std::vector<RecVertex*>::const_iterator obj=objects.begin();obj!=objects.end();++obj){
      RecVertex* object= *obj; 
      TISTOSSELECTSTORE()
    }


  } else if( sel->classID() == LHCb::Particle::classID() ) {

    const Hlt::TSelection<LHCb::Particle>& tsel = dynamic_cast<const Hlt::TSelection<LHCb::Particle>&>(*sel);   
    const std::vector<Particle*> objects = tsel.vct();
    for( std::vector<Particle*>::const_iterator obj=objects.begin();obj!=objects.end();++obj){
      Particle* object= *obj; 
      TISTOSSELECTSTORE()      
    }
    
  }
  
  return matchedObjectSummaries;
}
