// $Id: TriggerSelectionTisTosInHlt.h,v 1.4 2010-07-21 21:22:17 tskwarni Exp $
#ifndef TRIGGERSELECTIONTISTOSINHLT_H 
#define TRIGGERSELECTIONTISTOSINHLT_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/ITriggerSelectionTisTos.h"            // Interface
#include "HltBase/IHltData.h"
#include "HltBase/IHltInspector.h"

#include "ParticleTisTos.h"

#include "Event/HltObjectSummary.h"
#include "Event/Particle.h"
#include "Event/RecVertex.h"
#include "Event/Track.h"

#include "GaudiKernel/IIncidentListener.h"

namespace LHCb {
  class HltDecReports;
};

/** @class TriggerSelectionTisTosInHlt TriggerSelectionTisTosInHlt.h
 *  
 *  @author Tomasz Skwarnicki
 *  @date   2009-10-22
 *
 *  This is a special version intended for use in Hlt2 to Tis/Tos Hlt1 triggers 
 *     bypassing HltSelReports and going directly to HltDataSvc.
 *     Based on old implementation of the offline version.
 *
 *  Hit based implementation of Tis,Tos'ing Trigger Selection(s).
 *  @sa  ITriggerSelectionTisTos docs for more explanation.
 *  This interface also defines inlined shortcuts to set Offline Input and get an output in one call. 
 */
class TriggerSelectionTisTosInHlt :   public ParticleTisTos,
                               virtual public IIncidentListener, 
                               virtual public ITriggerSelectionTisTos {
public: 

  /// Standard constructor
  TriggerSelectionTisTosInHlt( const std::string& type, 
                            const std::string& name,
                            const IInterface* parent);

  virtual ~TriggerSelectionTisTosInHlt( ); ///< Destructor


  virtual StatusCode         initialize();
  virtual void handle(const Incident&);
 
  // ------------  various ways to define off-line input -------------------------

  /// erase previous input 
  void setOfflineInput( ); 
 
  ///    Detector hit input 
  void addToOfflineInput( const std::vector<LHCb::LHCbID> & hitlist );

  ///    Track input 
  void addToOfflineInput( const LHCb::Track & track);
  
  ///    Proto-particle input
  void addToOfflineInput( const LHCb::ProtoParticle & protoParticle);
  
  ///    Particle input; for composite particles loop over daughters will be executed
  void addToOfflineInput( const LHCb::Particle & particle);

  // ------------  TIS,TOS output functions  ------------------------

  /// single complete Trigger Selection TisTos  (define Offline Input before calling)
  unsigned int tisTosSelection( const std::string & selectionName );

  /// check for TOS  - may be faster than using tisTosSelection()
  bool tosSelection(const std::string & selectionName );

  /// check for TIS  - may be faster than using tisTosSelection()
  bool tisSelection(const std::string & selectionName );

  /// check for TUS (Trigger Used Signal: TPS or TOS) - may be faster than using tisTosSelection()
  bool tusSelection(const std::string & selectionName );
  
   /// analysis report
  std::string analysisReportSelection(const std::string & selectionName);

  template<class T>
  std::string analysisReport( const std::vector<T*> & list )
  {
    std::ostringstream report;
    report << offset() << " Vector of CLID=" << T::classID() << " size=" << list.size() << std::endl;  
    
    ++m_reportDepth;
    unsigned int result(0);
    BOOST_FOREACH( const T* obj, list )
      { 
        result |= tisTos( *obj );
        report << ParticleTisTos::analysisReport( *obj );      
        //      if( (result&kTPS) && (result&kTOS) && (result&kTIS) )break;        
      }
    --m_reportDepth;
    return report.str();    
  }

  // ------------ auxiliary outputs ---------------------------------

  /// list of LHCbIDs corresponding to present Offline Input (only hits used in matching are returned)
  std::vector<LHCb::LHCbID> offlineLHCbIDs(); 

  /// list of HltObjectSummaries from Selection Summary satisfying TOS,TIS requirements (define Offline Input before calling)
  std::vector<const LHCb::HltObjectSummary*> hltSelectionObjectSummaries( const std::string & selectionName,
                                                                          unsigned int tisRequirement      = kAnything,
                                                                          unsigned int tosRequirement      = kAnything,
                                                                          unsigned int tpsRequirement      = kAnything );



protected:

  /// get Hlt Summary and configuration
  void getHltSummary();
  
  /// Hlt summary reports
  LHCb::HltDecReports* m_hltDecReports;
  /// HltDataSvc
  Hlt::IData*        m_hltDataSvc;
  Hlt::IInspector*   m_hltInspectorSvc;
 
  /// Location of Hlt Dec Report
  StringProperty m_HltDecReportsLocation;

  bool m_newEvent;

private:

  // internal Cache of results used as long as the Offline Input remains the same (cache only full classifications)
 
  void clearCache()
  {
    m_cached_SelectionNames.clear();
    m_cached_tisTosTob.clear();
  }
  
  void storeInCache(const std::string & selectionName, unsigned int result)
  {
    m_cached_SelectionNames.push_back(selectionName);
    m_cached_tisTosTob.push_back(result);
  }
  
  bool findInCache(const std::string & selectionName, unsigned int & result)
  {
    std::vector< std::string >::iterator found =
      std::find( m_cached_SelectionNames.begin(),m_cached_SelectionNames.end(), selectionName);
    if( found == m_cached_SelectionNames.end() )return false;
    int index ( found - m_cached_SelectionNames.begin() );
    result = m_cached_tisTosTob[index];
    return true;
  }  


  //  -------------------------- data members --------------------
 
  /// Cache of results for the same Offline Input
  std::vector< std::string >  m_cached_SelectionNames;
  std::vector< unsigned int > m_cached_tisTosTob;
  
  
  /// HltObjectSummary container
  LHCb::HltObjectSummary::Container* m_objectSummaries;  

};
#endif // TRIGGERSELECTIONTISTOSINHLT_H
