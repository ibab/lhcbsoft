#ifndef STRIPPINGREPORT_H 
#define STRIPPINGREPORT_H 1

// Include files
// from Gaudi
//#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GaudiTupleAlg.h"
#include "GaudiKernel/IChronoStatSvc.h"

/** @class StrippingReport StrippingReport.h
 *  
 *
 *  @author Anton Poluektov
 *  @date   2010-03-15
 */

class IAlgManager;

/// Report structure
class ReportStat {

  public: 
  
    std::string name;      /// Selection name
    int decisions;         /// Number of selected events 
    int candidates;        /// Number of candidates
    double avgtime;        /// Average time per event
    int errors;            /// Number of errors produced by the line 
    int incidents;         /// Number of incidents produced by the line (usually, too many candidates per event)
    int slow_events;       /// Number of slow events
}; 


class StrippingReport : public GaudiTupleAlg {
public: 
  /// Standard constructor
  StrippingReport( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~StrippingReport( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute   ();    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization

protected:

private:

  void report(bool onlyPositive);

  std::string m_hdrLocation;     ///< Location of the stripping HltDecReport
  std::vector < std::string > m_selections;  ///< List of selections to report
  bool m_onlyPositive;           ///< If true, show only selections with positive decision
  bool m_everyEvent;             ///< If true, show selection statistics for every event
  int m_reportFreq;              ///< Frequency of reports during run processing 
  bool m_printNonResponding;     ///< If true, show non-responding (with zero events selected) lines 
  bool m_printHot;               ///< If true, show hot (with rate above threshold) lines
  double m_hotThreshold;         ///< Rate threshold for hot lines
  
  std::vector < ReportStat > m_stat;
  
  IAlgManager* m_algMgr;                 ///< Pointer to algorithm manager
  IChronoStatSvc* m_chronoSvc; 
  
  int m_event;                   ///< Event count
  
};

#endif // STRIPPINGREPORT_H
