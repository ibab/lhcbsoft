// $Id: TsaConfirmTool.h,v 1.1.1.1 2007-02-08 17:17:48 hernando Exp $
#ifndef TSACONFIRMTOOL_H 
#define TSACONFIRMTOOL_H 1

// Include files
#include <string>
#include <vector>

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "HltBase/ITrackConfirmTool.h"            // Interface


/** @class TsaConfirmTool TsaConfirmTool.h
 *  
 *
 *  @author Johannes Albrecht
 *  @date   2007-01-26
 */

// forward declaration
//from initialization
class IITDataSvc;
class IOTDataSvc;
//from collector
class ITsaCollector;
class ITsaSeedStep;
//class ITsaSeedAddHits;
class ITsaStubFind;
class ITsaStubLinker;
class ITsaStubExtender;

namespace LHCb
{
  class Track;
}




class TsaConfirmTool : public GaudiTool, virtual public ITrackConfirmTool {
public: 
  /// Standard constructor
  TsaConfirmTool( const std::string& type, 
                  const std::string& name,
                  const IInterface* parent);

  virtual ~TsaConfirmTool( ); ///< Destructor

  // Initialize method
  virtual StatusCode initialize();

  StatusCode tracks(const LHCb::State& seedState, std::vector<LHCb::Track*>& outputTracks );
                                 
  
protected:

private:
  //variables declared in job options
  int m_nsigma;
  bool m_debugInfo;
  
  //from initialization
  std::string m_otDataSvcType;
  std::string m_itDataSvcType;
  std::string m_otDataSvcName;
  std::string m_itDataSvcName;
  IOTDataSvc* m_otDataSvc; 
  IITDataSvc* m_itDataSvc; 
  bool m_initIT;
  bool m_initOT;

  // Tsa Collector
  ITsaCollector*    m_tsacollector;


  std::string m_seedTrackLocation;
  std::string m_seedHitLocation;
  std::string m_seedStubLocation;

  std::string m_selectorType;
  double m_maxNumHits;
  bool m_calcLikelihood; 

  std::vector<ITsaSeedStep*> m_xSearchStep;
  std::vector<ITsaSeedStep*> m_stereoStep;
  ITsaSeedStep* m_xSelection;
  ITsaSeedStep* m_finalSelection;
  ITsaSeedStep* m_likelihood;
  ITsaStubFind* m_stubFind;
  ITsaStubLinker* m_stubLinker;
  ITsaStubExtender* m_extendStubs;

  //from TsaSeedTrackCnv
  double m_EX2;
  double m_EY2;
  double m_ETx2;
  double m_ETy2;
  double m_EQdivP2;
  double m_likCut;
  double m_curvFactor;
  bool m_pFromCurvature;
  bool m_largeErrors;
  //  std::string m_inputLocation;
  //std::string m_outputLocation;
  LHCb::Track* convert(const SeedTrack* aTrack) const;
  void addState(const SeedTrack* aTrack, 
                LHCb::Track* lTrack, const double z) const;
  ITrackPtKick* m_ptKickTool;

  };
#endif // TSACONFIRMTOOL_H
