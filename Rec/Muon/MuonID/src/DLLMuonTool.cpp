/** Implementation of DLLMuonTool
 *
 * 2015-01-12: Ricardo Vazquez Gomez
 */
#include "MuonID/DLLMuonTool.h"
#include "MuonID/CommonMuonTool.h"

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "DetDesc/Condition.h"
#include "Event/MuonPID.h"
#include "Event/Track.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/ToolFactory.h"
#include "MuonDet/DeMuonDetector.h"

#include "MuonID/CommonMuonHitManager.h"

DECLARE_TOOL_FACTORY(DLLMuonTool)

/** C'tor
 * Also declares an interface to that the tool can be obtained in Gaudi via
 * tool<...>(...)
 */
DLLMuonTool::DLLMuonTool(const std::string& type, const std::string& name,
                               const IInterface* parent)
    : GaudiTool(type, name, parent), 
      foiFactor_{1.0} 
{
      declareInterface<DLLMuonTool>(this);

      // Same parameters for muons and non muons but different names
      // Muons 
      declareProperty( "MupBinsR1", m_MupBinsR1 );
      declareProperty( "MupBinsR2", m_MupBinsR2 );
      declareProperty( "MupBinsR3", m_MupBinsR3 );
      declareProperty( "MupBinsR4", m_MupBinsR4 ); 
      // Non muons
      declareProperty( "NonMupBinsR1", m_MupBinsR1);
      declareProperty( "NonMupBinsR2", m_MupBinsR2);
      declareProperty( "NonMupBinsR3", m_MupBinsR3);
      declareProperty( "NonMupBinsR4", m_MupBinsR4);
      // Landau parameters Muons R1
      declareProperty( "MuLandauParameterR1_1", m_MuLanParR1_1 );
      declareProperty( "MuLandauParameterR1_2", m_MuLanParR1_2 );
      declareProperty( "MuLandauParameterR1_3", m_MuLanParR1_3 );
      declareProperty( "MuLandauParameterR1_4", m_MuLanParR1_4 );
      declareProperty( "MuLandauParameterR1_5", m_MuLanParR1_5 );
      declareProperty( "MuLandauParameterR1_6", m_MuLanParR1_6 );
      declareProperty( "MuLandauParameterR1_7", m_MuLanParR1_7 );
      // Landau parameters Muons R2
      declareProperty( "MuLandauParameterR2_1", m_MuLanParR2_1 );
      declareProperty( "MuLandauParameterR2_2", m_MuLanParR2_2 );
      declareProperty( "MuLandauParameterR2_3", m_MuLanParR2_3 );
      declareProperty( "MuLandauParameterR2_4", m_MuLanParR2_4 );
      declareProperty( "MuLandauParameterR2_5", m_MuLanParR2_5 );
      // Landau parameters Muons R3
      declareProperty( "MuLandauParameterR3_1", m_MuLanParR3_1 );
      declareProperty( "MuLandauParameterR3_2", m_MuLanParR3_2 );
      declareProperty( "MuLandauParameterR3_3", m_MuLanParR3_3 );
      declareProperty( "MuLandauParameterR3_4", m_MuLanParR3_4 );
      declareProperty( "MuLandauParameterR3_5", m_MuLanParR3_5 );
      // Landau parameters Muons R4
      declareProperty( "MuLandauParameterR4_1", m_MuLanParR4_1 );
      declareProperty( "MuLandauParameterR4_2", m_MuLanParR4_2 );
      declareProperty( "MuLandauParameterR4_3", m_MuLanParR4_3 );
      declareProperty( "MuLandauParameterR4_4", m_MuLanParR4_4 );
      declareProperty( "MuLandauParameterR4_5", m_MuLanParR4_5 );
      // Landau parameters Non-Muons - Region 1-2-3-4:
      declareProperty( "NonMuLandauParameterR1", m_NonMuLanParR1 );
      declareProperty( "NonMuLandauParameterR2", m_NonMuLanParR2 );
      declareProperty( "NonMuLandauParameterR3", m_NonMuLanParR3 );
      declareProperty( "NonMuLandauParameterR4", m_NonMuLanParR4 );

      // hyperbolic tangent mapping of distances:

      // tanh scale factors
      declareProperty( "tanhScaleFactorsR1", m_tanhScaleFactorsR1);
      declareProperty( "tanhScaleFactorsR2", m_tanhScaleFactorsR2);
      declareProperty( "tanhScaleFactorsR3", m_tanhScaleFactorsR3);
      declareProperty( "tanhScaleFactorsR4", m_tanhScaleFactorsR4);

      // tanh(dist2) histograms contents
      declareProperty( "tanhCumulHistoMuonR1_1", m_tanhCumulHistoMuonR1_1);
      declareProperty( "tanhCumulHistoMuonR1_2", m_tanhCumulHistoMuonR1_2);
      declareProperty( "tanhCumulHistoMuonR1_3", m_tanhCumulHistoMuonR1_3);
      declareProperty( "tanhCumulHistoMuonR1_4", m_tanhCumulHistoMuonR1_4);
      declareProperty( "tanhCumulHistoMuonR1_5", m_tanhCumulHistoMuonR1_5);
      declareProperty( "tanhCumulHistoMuonR1_6", m_tanhCumulHistoMuonR1_6);
      declareProperty( "tanhCumulHistoMuonR1_7", m_tanhCumulHistoMuonR1_7);

      declareProperty( "tanhCumulHistoMuonR2_1", m_tanhCumulHistoMuonR2_1);
      declareProperty( "tanhCumulHistoMuonR2_2", m_tanhCumulHistoMuonR2_2);
      declareProperty( "tanhCumulHistoMuonR2_3", m_tanhCumulHistoMuonR2_3);
      declareProperty( "tanhCumulHistoMuonR2_4", m_tanhCumulHistoMuonR2_4);
      declareProperty( "tanhCumulHistoMuonR2_5", m_tanhCumulHistoMuonR2_5);

      declareProperty( "tanhCumulHistoMuonR3_1", m_tanhCumulHistoMuonR3_1);
      declareProperty( "tanhCumulHistoMuonR3_2", m_tanhCumulHistoMuonR3_2);
      declareProperty( "tanhCumulHistoMuonR3_3", m_tanhCumulHistoMuonR3_3);
      declareProperty( "tanhCumulHistoMuonR3_4", m_tanhCumulHistoMuonR3_4);
      declareProperty( "tanhCumulHistoMuonR3_5", m_tanhCumulHistoMuonR3_5);

      declareProperty( "tanhCumulHistoMuonR4_1", m_tanhCumulHistoMuonR4_1);
      declareProperty( "tanhCumulHistoMuonR4_2", m_tanhCumulHistoMuonR4_2);
      declareProperty( "tanhCumulHistoMuonR4_3", m_tanhCumulHistoMuonR4_3);
      declareProperty( "tanhCumulHistoMuonR4_4", m_tanhCumulHistoMuonR4_4);
      declareProperty( "tanhCumulHistoMuonR4_5", m_tanhCumulHistoMuonR4_5);

}

/** Initialises the tool.
 * Loads helpers and extracts detector information and variables from the
 * CondDB.
 */
StatusCode DLLMuonTool::initialize() {
  auto sc = GaudiTool::initialize();
  if (sc.isFailure()) {
    return sc;
  }

  hitManager_ = tool<CommonMuonHitManager>("CommonMuonHitManager");
  muonTool_ = tool<CommonMuonTool>("CommonMuonTool");
  det_ = getDet<DeMuonDetector>(DeMuonLocation::Default);

  // Load station information from detector description
  for (unsigned s = 0; s < nStations; ++s) {
    regionInner_[s] = std::make_pair(det_->getInnerX(s), det_->getInnerY(s));
    regionOuter_[s] = std::make_pair(det_->getOuterX(s), det_->getOuterY(s));
    stationZ_[s] = det_->getStationZ(s);
    for (unsigned r = 0; r < nRegions; ++r) {
      padSize_[s * nRegions + r] =
          std::make_pair(det_->getPadSizeX(s, r), det_->getPadSizeY(s, r));
    }
  }

  // Prepare to load information from conditions data base
  Condition* condFoiFactor = nullptr;
  Condition* condPreSelMomentum = nullptr;
  Condition* condFoiParametersX = nullptr;
  Condition* condFoiParametersY = nullptr;
  Condition* condMomentumCuts = nullptr;
  Condition* condNumberMupBins = nullptr;
  Condition* condMupBins = nullptr;
  Condition* condMuLandauParameterR1 = nullptr;
  Condition* condMuLandauParameterR2 = nullptr;
  Condition* condMuLandauParameterR3 = nullptr;
  Condition* condMuLandauParameterR4 = nullptr;
  Condition* condNonMuLandauParameterR1 = nullptr;
  Condition* condNonMuLandauParameterR2 = nullptr;
  Condition* condNonMuLandauParameterR3 = nullptr;
  Condition* condNonMuLandauParameterR4 = nullptr;
  Condition* condtanhScaleFactors = nullptr;
  Condition* condtanhCumulHistoMuonR1 = nullptr;
  Condition* condtanhCumulHistoMuonR2 = nullptr;
  Condition* condtanhCumulHistoMuonR3 = nullptr;
  Condition* condtanhCumulHistoMuonR4 = nullptr; 

  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/FOIfactor", condFoiFactor);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/PreSelMomentum", condPreSelMomentum);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/XFOIParameters", condFoiParametersX);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/YFOIParameters", condFoiParametersY);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/MomentumCuts", condMomentumCuts);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/nMupBins", condNumberMupBins);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/MupBins", condMupBins);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/MuLandauParameterR1", condMuLandauParameterR1);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/MuLandauParameterR2", condMuLandauParameterR2);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/MuLandauParameterR3", condMuLandauParameterR3);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/MuLandauParameterR4", condMuLandauParameterR4);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/NonMuLandauParameterR1",condNonMuLandauParameterR1);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/NonMuLandauParameterR2",condNonMuLandauParameterR2);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/NonMuLandauParameterR3",condNonMuLandauParameterR3);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/NonMuLandauParameterR4",condNonMuLandauParameterR4);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/tanhScaleFactors", condtanhScaleFactors);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/tanhCumulHistoMuonR1", condtanhCumulHistoMuonR1);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/tanhCumulHistoMuonR2", condtanhCumulHistoMuonR2);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/tanhCumulHistoMuonR3", condtanhCumulHistoMuonR3);
  registerCondition<DLLMuonTool>("Conditions/ParticleID/Muon/tanhCumulHistoMuonR4", condtanhCumulHistoMuonR4);

  // Update conditions
  sc = runUpdate();
  if (sc.isFailure()) {
    Error("Could not update conditions from the CondDB.");
    return sc;
  }
  // Extract parameters
  foiFactor_ = condFoiFactor->param<double>("FOIfactor");
  preSelMomentum_ = condPreSelMomentum->param<double>("PreSelMomentum");
  foiParamX_[0] = condFoiParametersX->paramVect<double>("XFOIParameters1");
  foiParamX_[1] = condFoiParametersX->paramVect<double>("XFOIParameters2");
  foiParamX_[2] = condFoiParametersX->paramVect<double>("XFOIParameters3");
  foiParamY_[0] = condFoiParametersY->paramVect<double>("YFOIParameters1");
  foiParamY_[1] = condFoiParametersY->paramVect<double>("YFOIParameters2");
  foiParamY_[2] = condFoiParametersY->paramVect<double>("YFOIParameters3");
  momentumCuts_ = condMomentumCuts->paramVect<double>("MomentumCuts");
  m_nMupBinsR1 = condNumberMupBins->param<int>("nMupBinsR1");
  m_nMupBinsR2 = condNumberMupBins->param<int>("nMupBinsR2");
  m_nMupBinsR3 = condNumberMupBins->param<int>("nMupBinsR3");
  m_nMupBinsR4 = condNumberMupBins->param<int>("nMupBinsR4");
  m_MupBinsR1 = condMupBins->paramVect<double>("MupBinsR1");
  m_MupBinsR2 = condMupBins->paramVect<double>("MupBinsR2");
  m_MupBinsR3 = condMupBins->paramVect<double>("MupBinsR3");
  m_MupBinsR4 = condMupBins->paramVect<double>("MupBinsR4");
  // Muons - Region1:
  m_MuLanParR1_1 = condMuLandauParameterR1->paramVect<double>("MuLandauParameterR1_1");
  m_MuLanParR1_2 = condMuLandauParameterR1->paramVect<double>("MuLandauParameterR1_2");
  m_MuLanParR1_3 = condMuLandauParameterR1->paramVect<double>("MuLandauParameterR1_3");
  m_MuLanParR1_4 = condMuLandauParameterR1->paramVect<double>("MuLandauParameterR1_4");
  m_MuLanParR1_5 = condMuLandauParameterR1->paramVect<double>("MuLandauParameterR1_5");
  m_MuLanParR1_6 = condMuLandauParameterR1->paramVect<double>("MuLandauParameterR1_6");
  m_MuLanParR1_7 = condMuLandauParameterR1->paramVect<double>("MuLandauParameterR1_7");

  // Muons - Region2:
  m_MuLanParR2_1 = condMuLandauParameterR2->paramVect<double>("MuLandauParameterR2_1");
  m_MuLanParR2_2 = condMuLandauParameterR2->paramVect<double>("MuLandauParameterR2_2");
  m_MuLanParR2_3 = condMuLandauParameterR2->paramVect<double>("MuLandauParameterR2_3");
  m_MuLanParR2_4 = condMuLandauParameterR2->paramVect<double>("MuLandauParameterR2_4");
  m_MuLanParR2_5 = condMuLandauParameterR2->paramVect<double>("MuLandauParameterR2_5");

  // Muons - Region3:
  m_MuLanParR3_1 = condMuLandauParameterR3->paramVect<double>("MuLandauParameterR3_1");
  m_MuLanParR3_2 = condMuLandauParameterR3->paramVect<double>("MuLandauParameterR3_2");
  m_MuLanParR3_3 = condMuLandauParameterR3->paramVect<double>("MuLandauParameterR3_3");
  m_MuLanParR3_4 = condMuLandauParameterR3->paramVect<double>("MuLandauParameterR3_4");
  m_MuLanParR3_5 = condMuLandauParameterR3->paramVect<double>("MuLandauParameterR3_5");

  // Muons - Region4:
  m_MuLanParR4_1 = condMuLandauParameterR4->paramVect<double>("MuLandauParameterR4_1");
  m_MuLanParR4_2 = condMuLandauParameterR4->paramVect<double>("MuLandauParameterR4_2");
  m_MuLanParR4_3 = condMuLandauParameterR4->paramVect<double>("MuLandauParameterR4_3");
  m_MuLanParR4_4 = condMuLandauParameterR4->paramVect<double>("MuLandauParameterR4_4");
  m_MuLanParR4_5 = condMuLandauParameterR4->paramVect<double>("MuLandauParameterR4_5");

  // Non-Muons - Region 1-2-3-4:
  m_NonMuLanParR1 = condNonMuLandauParameterR1->paramVect<double>("NonMuLandauParameterR1");
  m_NonMuLanParR2 = condNonMuLandauParameterR2->paramVect<double>("NonMuLandauParameterR2");
  m_NonMuLanParR3 = condNonMuLandauParameterR3->paramVect<double>("NonMuLandauParameterR3");
  m_NonMuLanParR4 = condNonMuLandauParameterR4->paramVect<double>("NonMuLandauParameterR4");

  m_tanhScaleFactorsR1 = condtanhScaleFactors->paramVect<double>("tanhScaleFactorsR1");
  m_tanhScaleFactorsR2 = condtanhScaleFactors->paramVect<double>("tanhScaleFactorsR2");
  m_tanhScaleFactorsR3 = condtanhScaleFactors->paramVect<double>("tanhScaleFactorsR3");
  m_tanhScaleFactorsR4 = condtanhScaleFactors->paramVect<double>("tanhScaleFactorsR4");

  m_tanhCumulHistoMuonR1_1 = condtanhCumulHistoMuonR1->paramVect<double>("tanhCumulHistoMuonR1_1");
  m_tanhCumulHistoMuonR1_2 = condtanhCumulHistoMuonR1->paramVect<double>("tanhCumulHistoMuonR1_2");
  m_tanhCumulHistoMuonR1_3 = condtanhCumulHistoMuonR1->paramVect<double>("tanhCumulHistoMuonR1_3");
  m_tanhCumulHistoMuonR1_4 = condtanhCumulHistoMuonR1->paramVect<double>("tanhCumulHistoMuonR1_4");
  m_tanhCumulHistoMuonR1_5 = condtanhCumulHistoMuonR1->paramVect<double>("tanhCumulHistoMuonR1_5");
  m_tanhCumulHistoMuonR1_6 = condtanhCumulHistoMuonR1->paramVect<double>("tanhCumulHistoMuonR1_6");
  m_tanhCumulHistoMuonR1_7 = condtanhCumulHistoMuonR1->paramVect<double>("tanhCumulHistoMuonR1_7");

  m_tanhCumulHistoMuonR2_1 = condtanhCumulHistoMuonR2->paramVect<double>("tanhCumulHistoMuonR2_1");
  m_tanhCumulHistoMuonR2_2 = condtanhCumulHistoMuonR2->paramVect<double>("tanhCumulHistoMuonR2_2");
  m_tanhCumulHistoMuonR2_3 = condtanhCumulHistoMuonR2->paramVect<double>("tanhCumulHistoMuonR2_3");
  m_tanhCumulHistoMuonR2_4 = condtanhCumulHistoMuonR2->paramVect<double>("tanhCumulHistoMuonR2_4");
  m_tanhCumulHistoMuonR2_5 = condtanhCumulHistoMuonR2->paramVect<double>("tanhCumulHistoMuonR2_5");

  m_tanhCumulHistoMuonR3_1 = condtanhCumulHistoMuonR3->paramVect<double>("tanhCumulHistoMuonR3_1");
  m_tanhCumulHistoMuonR3_2 = condtanhCumulHistoMuonR3->paramVect<double>("tanhCumulHistoMuonR3_2");
  m_tanhCumulHistoMuonR3_3 = condtanhCumulHistoMuonR3->paramVect<double>("tanhCumulHistoMuonR3_3");
  m_tanhCumulHistoMuonR3_4 = condtanhCumulHistoMuonR3->paramVect<double>("tanhCumulHistoMuonR3_4");
  m_tanhCumulHistoMuonR3_5 = condtanhCumulHistoMuonR3->paramVect<double>("tanhCumulHistoMuonR3_5");

  m_tanhCumulHistoMuonR4_1 = condtanhCumulHistoMuonR4->paramVect<double>("tanhCumulHistoMuonR4_1");
  m_tanhCumulHistoMuonR4_2 = condtanhCumulHistoMuonR4->paramVect<double>("tanhCumulHistoMuonR4_2");
  m_tanhCumulHistoMuonR4_3 = condtanhCumulHistoMuonR4->paramVect<double>("tanhCumulHistoMuonR4_3");
  m_tanhCumulHistoMuonR4_4 = condtanhCumulHistoMuonR4->paramVect<double>("tanhCumulHistoMuonR4_4");
  m_tanhCumulHistoMuonR4_5 = condtanhCumulHistoMuonR4->paramVect<double>("tanhCumulHistoMuonR4_5");

  m_tanhScaleFactors.push_back(&m_tanhScaleFactorsR1);
  m_tanhScaleFactors.push_back(&m_tanhScaleFactorsR2);
  m_tanhScaleFactors.push_back(&m_tanhScaleFactorsR3);
  m_tanhScaleFactors.push_back(&m_tanhScaleFactorsR4);

  m_tanhCumulHistoMuonR1.push_back(&m_tanhCumulHistoMuonR1_1);
  m_tanhCumulHistoMuonR1.push_back(&m_tanhCumulHistoMuonR1_2);
  m_tanhCumulHistoMuonR1.push_back(&m_tanhCumulHistoMuonR1_3);
  m_tanhCumulHistoMuonR1.push_back(&m_tanhCumulHistoMuonR1_4);
  m_tanhCumulHistoMuonR1.push_back(&m_tanhCumulHistoMuonR1_5);
  m_tanhCumulHistoMuonR1.push_back(&m_tanhCumulHistoMuonR1_6);
  m_tanhCumulHistoMuonR1.push_back(&m_tanhCumulHistoMuonR1_7);

  m_tanhCumulHistoMuonR2.push_back(&m_tanhCumulHistoMuonR2_1);
  m_tanhCumulHistoMuonR2.push_back(&m_tanhCumulHistoMuonR2_2);
  m_tanhCumulHistoMuonR2.push_back(&m_tanhCumulHistoMuonR2_3);
  m_tanhCumulHistoMuonR2.push_back(&m_tanhCumulHistoMuonR2_4);
  m_tanhCumulHistoMuonR2.push_back(&m_tanhCumulHistoMuonR2_5);

  m_tanhCumulHistoMuonR3.push_back(&m_tanhCumulHistoMuonR3_1);
  m_tanhCumulHistoMuonR3.push_back(&m_tanhCumulHistoMuonR3_2);
  m_tanhCumulHistoMuonR3.push_back(&m_tanhCumulHistoMuonR3_3);
  m_tanhCumulHistoMuonR3.push_back(&m_tanhCumulHistoMuonR3_4);
  m_tanhCumulHistoMuonR3.push_back(&m_tanhCumulHistoMuonR3_5);

  m_tanhCumulHistoMuonR4.push_back(&m_tanhCumulHistoMuonR4_1);
  m_tanhCumulHistoMuonR4.push_back(&m_tanhCumulHistoMuonR4_2);
  m_tanhCumulHistoMuonR4.push_back(&m_tanhCumulHistoMuonR4_3);
  m_tanhCumulHistoMuonR4.push_back(&m_tanhCumulHistoMuonR4_4);
  m_tanhCumulHistoMuonR4.push_back(&m_tanhCumulHistoMuonR4_5);

  m_tanhCumulHistoMuon.push_back(&m_tanhCumulHistoMuonR1);
  m_tanhCumulHistoMuon.push_back(&m_tanhCumulHistoMuonR2);
  m_tanhCumulHistoMuon.push_back(&m_tanhCumulHistoMuonR3);
  m_tanhCumulHistoMuon.push_back(&m_tanhCumulHistoMuonR4);

  if (msgLevel(MSG::VERBOSE)){ 
      verbose()  << "==> nMupBinsR1:" << m_nMupBinsR1 << endmsg;
      verbose()  << "==> nMupBinsR2:" << m_nMupBinsR2 << endmsg;
      verbose()  << "==> nMupBinsR3:" << m_nMupBinsR3 << endmsg;
      verbose()  << "==> nMupBinsR4:" << m_nMupBinsR4 << endmsg;
      verbose()  << "==> MupBinsR1:" << m_MupBinsR1 << endmsg;
      verbose()  << "==> MupBinsR2:" << m_MupBinsR2 << endmsg;
      verbose()  << "==> MupBinsR3:" << m_MupBinsR3 << endmsg;
      verbose()  << "==> MupBinsR4:" << m_MupBinsR4 << endmsg;
  }    
  return sc;
}

/**Method to calculate the distance from the hit to the extrapolated position
*/
double DLLMuonTool::calcDist( const CommonMuonTool::MuonTrackExtrapolation& extrapolation, CommonConstMuonHits& hits ) const{

  //CommonConstMuonHits hits;
  //std::array<unsigned, CommonMuonTool::nStations> occupancies;
  //std::tie(hits, occupancies) = muonTool_->hitsAndOccupancies(track, extrapolation);

  double m_dist = -1.;
  double mCoordX[5] = {0.,0.,0.,0.,0.};
  double mCoordY[5] = {0.,0.,0.,0.,0.};
  double mCoordDX[5] = {0.,0.,0.,0.,0.};
  double mCoordDY[5] = {0.,0.,0.,0.,0.};

  std::for_each(std::begin(hits), std::end(hits), [&](const CommonMuonHit* hit){
    for (unsigned s = 0; s < 5; ++s) {
      if(s == hit->station()){ // same station for hit and extrapolation
        if(mCoordX[s] == 0) { // first coord found. Doubt that this is needed RVG 
          mCoordX[s] = hit->x();
          mCoordY[s] = hit->y();
          // difference between x,y and the extrapolated x,y
          mCoordDX[s] = ( hit->x() - extrapolation[s].first ) / hit->dx();
          mCoordDY[s] = ( hit->y() - extrapolation[s].second ) / hit->dy();
        } else{
          // get best match in X and Y
          if( pow((hit->x() - extrapolation[s].first),2) + 
              pow((hit->y() - extrapolation[s].second),2) <
              pow((mCoordX[s]*hit->dx() - extrapolation[s].first),2) + 
              pow((mCoordY[s]*hit->dy() - extrapolation[s].second),2) ){
            // this Coord is a better match
            mCoordX[s] = hit->x();
            mCoordY[s] = hit->y();
            mCoordDX[s] = ( hit->x() - extrapolation[s].first ) / hit->dx();
            mCoordDY[s] = ( hit->y() - extrapolation[s].second ) / hit->dy();
          }
        } 
      }
    } 
  });

  // calculate the square of the distances
  int nstn = 0;
  for( int stn = 0 ; stn < 5; stn++ ){  // No M1 case: 5 here refers to dimension of mCoordDX arrays JHL 12/Oct./2013
    if (msgLevel(MSG::DEBUG) ) {
      debug() << "calcDist: station: " << stn << " mCoordDX = " << mCoordDX[stn] << " mCoordDY = " << mCoordDY[stn] << endmsg;
    }
    if ( mCoordDX[stn] != 0. ) {
      if(msgLevel(MSG::DEBUG)) debug() << "nstn = " << nstn << endmsg;
      if(msgLevel(MSG::DEBUG)) debug() << "mCoordDX[stn]*mCoordDX[stn] = " << mCoordDX[stn]*mCoordDX[stn] << endmsg;
      if(msgLevel(MSG::DEBUG)) debug() << "mCoordDY[stn]*mCoordDY[stn] = " << mCoordDY[stn]*mCoordDY[stn] << endmsg;
      nstn++;
      m_dist += mCoordDX[stn]*mCoordDX[stn] + mCoordDY[stn]*mCoordDY[stn];
    }
  }
  if(msgLevel(MSG::DEBUG)) debug() << "m_dist = " << m_dist <<endmsg;
  m_dist = m_dist / nstn; //divide by the number of station

  return m_dist;
}

/** Calculate the number of tracks that share hits 
 */

int DLLMuonTool::calcNShared( LHCb::MuonPID* muonid, LHCb::MuonPIDs* pMuids, CommonConstMuonHits& hits, const CommonMuonTool::MuonTrackExtrapolation& extr, std::map< LHCb::MuonPID*, CommonConstMuonHits > m_muonMap) {
  
  int nNSH = 100;

  double dsquare1 = calcDist(extr, hits);
  if(msgLevel(MSG::DEBUG)) debug() << "For track with momentum p = " << (muonid->idTrack())->p() << ". Dist 1 = " << dsquare1 << endmsg;
  if( dsquare1<0. ) {
    muonid->setNShared(100);
    return Warning( "calcDist 1 failure", false );
  }
  // loop over the muonIDs
  LHCb::MuonPIDs::const_iterator iMuon;
  for( iMuon = pMuids->begin(); pMuids->end() != iMuon; ++iMuon ) {
    // skip if this muonID is the same as the input or if IsMuonLoose is false
    if ( *iMuon == muonid ) continue;
    // see if there are shared hits between the muonIDs
    if ( compareHits( muonid, *iMuon, m_muonMap ) ){
    // need to check better the function
    //
      // get dist for this muonID
      const LHCb::Track* track2 = (*iMuon)->idTrack(); // get the track of the muonID
      const auto extr2 = muonTool_->extrapolateTrack(*track2);
      double dsquare2 = calcDist(extr2, hits);
      if(msgLevel(MSG::DEBUG)) debug() << " For track with p = " << track2->p() << ". Dist 2 = " << dsquare2 << endmsg;
      if( dsquare2<0. ) {
        Warning(" calcDist 2 failure",StatusCode::SUCCESS,0).ignore();
        continue;
      }
      
      // the muonID which gets the number of shared hits is the one
      // which has the biggest dist
      if( dsquare2 < dsquare1 ) {
        nNSH = muonid->nShared();
        muonid->setNShared(nNSH+1);
      }
      else{
        nNSH = (*iMuon)->nShared();
        (*iMuon)->setNShared(nNSH+1);
      }
    }
  }
  if (msgLevel(MSG::DEBUG)) debug() << "nShared = " << muonid->nShared() << endmsg;

  return nNSH;
}

// Get the bin for the given P
int DLLMuonTool::GetPbin(double p, int region){
  std::vector<double> *pBins=0;
  switch( region+1 ){
  case 1:{ pBins = &m_MupBinsR1; break;}
  case 2:{ pBins = &m_MupBinsR2; break;}
  case 3:{ pBins = &m_MupBinsR3; break;}
  case 4:{ pBins = &m_MupBinsR4; break;}
  }
  if (msgLevel(MSG::VERBOSE)) verbose() << "GetPbin: region+1 " << region+1 << " p " << p << " pBins vector address: " << pBins << endmsg;
  if(0 == pBins) throw GaudiException("GetPbin: No match to a pBins vector. Null pBins pointer", "",StatusCode::FAILURE);
  for(unsigned int iBin=0; iBin<pBins->size();++iBin){
    if (msgLevel(MSG::VERBOSE)) verbose() << "GetPbin:\tiBin " << iBin << " pBins[iBin] " << (*pBins)[iBin] << endmsg;
    if(p < (*pBins)[iBin]) return iBin;
  }
  return pBins->size();  // last bin. npBins goes from 0 to (Gaias npBin)+1
}

std::vector<double> DLLMuonTool::loadNonMuLandauParam(const CommonMuonTool::MuonTrackExtrapolation& extrapolation){
  std::vector<double> parNonMu;   
  int region=findTrackRegions(extrapolation)[1]; // M2 JHLJHL Check M2 and M3 indices if no M1 30/08/2013
  if (region<0) region=findTrackRegions(extrapolation)[2]; // M3
  if (region==0){//Region 1
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR1[i]; }
  }else if (region==1){// Region 2
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR2[i]; }
  }else if (region==2){// Region 3 
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR3[i]; }
  }else if (region==3){// Region 4
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR4[i]; }
  } else throw GaudiException("Not valid region", "",StatusCode::FAILURE);
  return parNonMu;
}

std::pair<std::vector<double>,std::vector<double>> DLLMuonTool::loadLandauParam(double p, const CommonMuonTool::MuonTrackExtrapolation& extrapolation){
  std::vector<double> parMu, parNonMu;
  // Track region is defined in M2 or M3
  int region=findTrackRegions(extrapolation)[1]; // M2 JHLJHL Check M2 and M3 indices if no M1 30/08/2013
  if (region<0) region=findTrackRegions(extrapolation)[2]; // M3
  if (region==0){//Region 1
    if (p>preSelMomentum_ && p<m_MupBinsR1[0]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR1_1[i]; } }
    if (p>m_MupBinsR1[0] && p<m_MupBinsR1[1]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR1_2[i]; } }
    if (p>m_MupBinsR1[1] && p<m_MupBinsR1[2]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR1_3[i]; } }
    if (p>m_MupBinsR1[2] && p<m_MupBinsR1[3]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR1_4[i]; } }
    if (p>m_MupBinsR1[3] && p<m_MupBinsR1[4]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR1_5[i]; } }
    if (p>m_MupBinsR1[4] && p<m_MupBinsR1[5]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR1_6[i]; } }
    if (p>m_MupBinsR1[5]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR1_7[i]; } }
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR1[i]; }
  }else if (region==1){// Region 2
    if (p>preSelMomentum_  && p<m_MupBinsR2[0]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR2_1[i]; } }
    if (p>m_MupBinsR2[0] && p<m_MupBinsR2[1]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR2_2[i]; } }
    if (p>m_MupBinsR2[1] && p<m_MupBinsR2[2]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR2_3[i]; } }
    if (p>m_MupBinsR2[2] && p<m_MupBinsR2[3]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR2_4[i]; } }
    if (p>m_MupBinsR2[3] ) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR2_5[i]; } }
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR2[i]; }
  }else if (region==2){// Region 3
    if (p>preSelMomentum_  && p<m_MupBinsR3[0]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR3_1[i]; } }
    if (p>m_MupBinsR3[0] && p<m_MupBinsR3[1]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR3_2[i]; } }
    if (p>m_MupBinsR3[1] && p<m_MupBinsR3[2]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR3_3[i]; } }
    if (p>m_MupBinsR3[2] && p<m_MupBinsR3[3]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR3_4[i]; } }
    if (p>m_MupBinsR3[3] ) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR3_5[i]; } }
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR3[i]; }
  }else if (region==3){// Region 4
    if (p>preSelMomentum_  && p<m_MupBinsR4[0]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR4_1[i]; } }
    if (p>m_MupBinsR4[0]  && p<m_MupBinsR4[1]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR4_2[i]; } }
    if (p>m_MupBinsR4[1]  && p<m_MupBinsR4[2]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR4_3[i]; } }
    if (p>m_MupBinsR4[2]  && p<m_MupBinsR4[3]) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR4_4[i]; } }
    if (p>m_MupBinsR4[3] ) {for (int i=0;i<6;i++){parMu[i] = m_MuLanParR4_5[i]; } }
    for (int i=0;i<3;i++){parNonMu[i] = m_NonMuLanParR4[i]; }
  } else throw GaudiException("Not valid region", "",StatusCode::FAILURE);
  return {parMu, parNonMu};
}

std::vector<int> DLLMuonTool::findTrackRegions(const CommonMuonTool::MuonTrackExtrapolation& extrapolation){
  //=====================================================================
  // comment: Returns the muon detector region of the extrapolated track;
  // authors: G. Lanfranchi & S. Furcas & X. Cid Vidal,
  // date:    10/3/11
  //=====================================================================

  // RVG For the moment do not include the kalman FOI part

  std::vector<int> trackRegion(nStations,-1);
  // Region of the track extrapolated:

/*
  if (m_kalman_foi)
  {
    StatusCode sc = DistMuIDTool()->findTrackRegions(m_mutrack,trackRegion);
    if (sc.isFailure() && msgLevel(MSG::DEBUG) ) debug() << " Error finding track regions "<< endmsg;
    return trackRegion;
  }
*/

  for (unsigned sta=0; sta<nStations; sta++){
    int chnum = -1;
    int regnum = -1;
    det_->Pos2StChamberNumber(extrapolation[sta].first, extrapolation[sta].second, sta, chnum, regnum).ignore();
    trackRegion[sta] = regnum;
    if (trackRegion[sta]<0 && msgLevel(MSG::DEBUG)){
      debug() << format(" Track extrapolation in station %d gives not-existent region ",sta) << endmsg;
    }
  }

  if (msgLevel(MSG::DEBUG)) debug() << "TrackRegions" << trackRegion << endmsg;
  return trackRegion;
}

//=====================================================================
double DLLMuonTool::calc_closestDist(const LHCb::Track& track, const CommonMuonTool::MuonTrackExtrapolation& extrapolation, CommonConstMuonHits& hits, std::array<unsigned, CommonMuonTool::nStations> occupancies){
  //=====================================================================
  // comment: Returns the squared distance calculated with closest hit
  // authors: G. Lanfranchi & S. Furcas,
  // date:    10/5/09
  //=====================================================================

  double closest_dist=0;

  // occupancy with crossings still missing
  //CommonConstMuonHits hits;
  //std::array<unsigned, CommonMuonTool::nStations> occupancies;
  //std::tie(hits, occupancies) = muonTool_->hitsAndOccupancies(track, extrapolation);

/*  /// RVG so far not consider the kalman FOI case
  if (m_kalman_foi){

    StatusCode sc = DistMuIDTool()->muonQuality(m_mutrack, closest_dist);
    if (sc.isFailure())
    {
      Warning("DistMuonIDTool: ComputeDistance failed ",sc).ignore();
      return -1;
    }
    return closest_dist;
  }
*/

  // Get closest hits
  double foiXDim,foiYDim;

  for (int ista=0; ista<5; ista++){  // No M1 case: 5 here is OK, these arrays have dimension 5 JHL 12/Oct./2013
    closest_x[ista] = -1;
    closest_y[ista] = -1;
    small_dist[ista] = 10000000.;
    Fdist[ista] = 0;
    closest_region[ista] = -1;
  }
  double p = track.p();
  std::for_each(std::begin(hits), std::end(hits), [&](const CommonMuonHit* hit){ // hits in FOI
    unsigned s = hit->tile().station();
    unsigned r = hit->tile().region();
    foiXDim = hit->dx()*foiFactor_*muonTool_->foi(s, r, p).first;
    foiYDim = hit->dy()*foiFactor_*muonTool_->foi(s, r, p).second;
    if(msgLevel(MSG::DEBUG)) debug() << "The p = " << p << ", with foiFactor = " << foiFactor_ << ", foiX = " << muonTool_->foi(s, r, p).first << ", and foiY = " << muonTool_->foi(s, r, p).second << endmsg;
    if(msgLevel(MSG::DEBUG)) debug() << "For hit in x = " << hit->x() << ", and y= " << hit->y() << ". Extrapolation in x = " << extrapolation[s].first << ", y = " << extrapolation[s].second << endmsg;  
    if(msgLevel(MSG::DEBUG)) debug() << "The FOI dimension is, foiXDim  = " << foiXDim << ", foiYDim = " << foiYDim << endmsg;
    if (s >= 1 ) {  // JHL: station == 0 is OK in the case of no M1  12/Oct./2013
      if( ( fabs(hit->x() - extrapolation[s].first) < foiXDim ) && ( fabs(hit->y() - extrapolation[s].second) < foiYDim ) ) {
        if(msgLevel(MSG::DEBUG)) debug() << "residX = " << fabs(hit->x() - extrapolation[s].first) << ", and residY = " << fabs(hit->y() - extrapolation[s].second) << endmsg;

        Fdist[s] =pow((hit->x() - extrapolation[s].first)/hit->dx(),2) + pow((hit->y() - extrapolation[s].second)/hit->dy(),2);
        if(msgLevel(MSG::DEBUG)) debug() << "FDist in station " << s << " = " << Fdist[s] << endmsg;
        if(Fdist[s] < small_dist[s]){
          small_dist[s] = Fdist[s];
          closest_x[s]  = (hit->x() - extrapolation[s].first) / hit->dx();
          closest_y[s]  = (hit->y() - extrapolation[s].second) / hit->dy();
          closest_region[s] = r;
        }  //Fdist
      }  //foi
    } //station
  }); //hit

  if (msgLevel(MSG::DEBUG) ){
    debug() << "calc_closestDist: station closest_region \t closest_x \t closest_y" << endmsg;
    for(int ist=0;ist<5;++ist){
        debug() << ist << " " << closest_region[ist] << "\t" << closest_x[ist] << "\t" << closest_y[ist] << endmsg;
    }
  }  

  // calculate distance
  unsigned iM2=1, iM3=2, iM4=3, iM5=4;

  if(p>preSelMomentum_ && p<momentumCuts_[0]){ // JHLJHL: Check station indices whith no M1 configuration 30/08/2013
    //3 or 2 stations
    if(occupancies[iM2]>0 && occupancies[iM3]>0 && occupancies[iM4]>0){//M2 &&M3 && M4
      closest_dist = pow(closest_x[iM2],2)+pow(closest_y[iM2],2) 
                   + pow(closest_x[iM3],2)+pow(closest_y[iM3],2) 
                   + pow(closest_x[iM4],2)+pow(closest_y[iM4],2);
      closest_dist = closest_dist/3.;
      return closest_dist;
    }
    if(occupancies[iM2]>0 && occupancies[iM3]>0){//M2 &&M3
      closest_dist = pow(closest_x[iM2],2)+pow(closest_y[iM2],2) 
                   + pow(closest_x[iM3],2)+pow(closest_y[iM3],2);
      closest_dist = closest_dist/2.;
      return closest_dist;
    }
    if(occupancies[iM2]>0 && occupancies[iM4]>0){//M2 &&M4
      closest_dist = pow(closest_x[iM2],2)+pow(closest_y[iM2],2) 
                   + pow(closest_x[iM4],2)+pow(closest_y[iM4],2);
      closest_dist = closest_dist/2.;
      return closest_dist;
    }
    if(occupancies[iM3]>0 && occupancies[iM4]>0){//M3 &&M4
      closest_dist = pow(closest_x[iM3],2)+pow(closest_y[iM3],2) 
                   + pow(closest_x[iM4],2)+pow(closest_y[iM4],2);
      closest_dist = closest_dist/2.;
      return closest_dist;
    }
  }//3-6
  
  if(p>momentumCuts_[0]){
    if(occupancies[iM2]>0 && occupancies[iM3]>0 && occupancies[iM4]>0 && occupancies[iM5]>0){
      //M2 &&M3&&M4&&M5
      closest_dist = pow(closest_x[iM2],2)+pow(closest_y[iM2],2) 
                   + pow(closest_x[iM3],2)+pow(closest_y[iM3],2)
                   + pow(closest_x[iM4],2)+pow(closest_y[iM4],2)
                   + pow(closest_x[iM5],2)+pow(closest_y[iM5],2);
      closest_dist = closest_dist/4.;
      return closest_dist;
    }
    if(occupancies[iM2]>0 && occupancies[iM3]>0 && occupancies[iM4]){//M2 && M3 && M4
      closest_dist = pow(closest_x[iM2],2)+pow(closest_y[iM2],2)
                   + pow(closest_x[iM3],2)+pow(closest_y[iM3],2)
                   + pow(closest_x[iM4],2)+pow(closest_y[iM4],2);
      closest_dist = closest_dist/3.;
      return closest_dist;
    }
    if(occupancies[iM2]>0 && occupancies[iM3]>0 && occupancies[iM5]){//M2 && M3 && M5
      closest_dist = pow(closest_x[iM2],2)+pow(closest_y[iM2],2)
                   + pow(closest_x[iM3],2)+pow(closest_y[iM3],2)
                   + pow(closest_x[iM5],2)+pow(closest_y[iM5],2);
      closest_dist = closest_dist/3.;
      return closest_dist;
    }
    if(occupancies[iM3]>0 && occupancies[iM4]>0 && occupancies[iM5]>0){//M3 &&M4 && M5
      closest_dist = pow(closest_x[iM3],2)+pow(closest_y[iM3],2)
                   + pow(closest_x[iM4],2)+pow(closest_y[iM4],2)
                   + pow(closest_x[iM5],2)+pow(closest_y[iM5],2);
      closest_dist = closest_dist/3.;
      return closest_dist;
    }
    if(occupancies[iM2]>0 && occupancies[iM4]>0 && occupancies[iM5]>0){//M2 &&M4 && M5
      closest_dist = pow(closest_x[iM2],2)+pow(closest_y[iM2],2)
                   + pow(closest_x[iM4],2)+pow(closest_y[iM4],2)
                   + pow(closest_x[iM5],2)+pow(closest_y[iM5],2);
      closest_dist = closest_dist/3.;
      return closest_dist;    
    }
  }
  if(!closest_dist) return 0;
  return closest_dist;
}

//=====================================================================
double DLLMuonTool::calc_ProbMu_tanh(const double& tanhdist0, int pBin, int region){
  int nDistBins = (*(*m_tanhCumulHistoMuon[region])[pBin]).size();
  int itanhdist=int(tanhdist0*nDistBins);

  //if(itanhdist>nDistBins)itanhdist--;
  if(itanhdist>=nDistBins) itanhdist=nDistBins-1;

  if (msgLevel(MSG::DEBUG) ) debug() << "calc_ProbMu_tanh: region " << region << " pBin " << pBin << " tanh(dist) " << tanhdist0
          << " itanhdist " << itanhdist << " ProbMu " << (*((*(m_tanhCumulHistoMuon[region]))[pBin]))[itanhdist] << endmsg;
  return (*((*(m_tanhCumulHistoMuon[region]))[pBin]))[itanhdist];
}

double DLLMuonTool::calc_ProbNonMu(double& dist0, std::vector<double>& parNonMu){
  //=====================================================================
  // comment: Returns for a given track of dist0 the probability to be a non-muon;
  // authors: G. Lanfranchi & S. Furcas,
  // date:    10/5/09
  // Modified by Helder Lopes on January 2014 to solve https://sft.its.cern.ch/jira/browse/ROOT-5985
  //=====================================================================

  double Prob = ROOT::Math::landau_cdf(dist0, parNonMu[1], parNonMu[0]) - ROOT::Math::landau_cdf(0, parNonMu[1], parNonMu[0]);
  if (msgLevel(MSG::DEBUG)) debug() << "ProbNonMu, parNonMu[0], parNonMu[2] = " << Prob << ", " << parNonMu[0] << ", " << parNonMu[2] << endmsg; 
  if(parNonMu[2]>0){
    if (msgLevel(MSG::DEBUG) ) debug() << "probnmu, parNonMu[2] : "<< Prob <<","<< parNonMu[2] << endmsg;
    return Prob = Prob/parNonMu[2];
  }else{
    Warning("ProbNonMu: normalization out of control ",StatusCode::FAILURE).ignore();
    return -1.;
  }
}

StatusCode DLLMuonTool::calcLandauNorm(){
  //=====================================================================
  // comment: Normalizations for Landaus for Muons and Non-Muons
  // authors: G. Lanfranchi & S. Furcas,
  // date:    10/5/09
  //=====================================================================

  double parnmu[3];
  for(int i=1;i<3;i++){parnmu[i]=0;}
  double Norm = -1;

  //=============
  //  Non-Muons
  //=============

  for(int i=0;i<3;i++){parnmu[i]=m_NonMuLanParR1[i];}
  Norm = calcNorm_nmu(parnmu);
  if (Norm<0 || Norm==0) return Error("normalization of Non-Muon R1 out of control");
  m_NonMuLanParR1[2] = Norm;

  for(int i=0;i<3;i++){parnmu[i]=m_NonMuLanParR2[i];}
  Norm = calcNorm_nmu(parnmu);
  if (Norm<0 || Norm==0)  return Error("normalization of Non-Muon R2 out of control");
  m_NonMuLanParR2[2] = Norm;

  for(int i=0;i<3;i++){parnmu[i]=m_NonMuLanParR3[i];}
  Norm = calcNorm_nmu(parnmu);
  if (Norm<0 || Norm==0)  return Error("normalization of Non-Muon R3 out of control");
  m_NonMuLanParR3[2] = Norm;

  for(int i=0;i<3;i++){parnmu[i]=m_NonMuLanParR4[i];}
  Norm = calcNorm_nmu(parnmu);
  if (Norm<0 || Norm==0)  return Error("normalization of Non-Muon R4 out of control");
  m_NonMuLanParR4[2] = Norm;

  return StatusCode::SUCCESS;

}

double DLLMuonTool::calcNorm_nmu(double *par){
  //=====================================================================
  // comment: Calculate Normalizations for non-muons
  // authors: G. Lanfranchi & S. Furcas,
  // date:    10/5/09
  // Modified by Helder Lopes on January 2014 to solve https://sft.its.cern.ch/jira/browse/ROOT-5985
  //=====================================================================

  double m_x = 0.2;
  double m_nMax = 4000;  
  double Norm =  ROOT::Math::landau_cdf(m_x*m_nMax,par[1],par[0]) - ROOT::Math::landau_cdf(0.,par[1],par[0]);

  return Norm;
}


std::pair<double,double> DLLMuonTool::calcMuonLL_tanhdist_landau(const LHCb::Track& track, const CommonMuonTool::MuonTrackExtrapolation& extrapolation, CommonConstMuonHits& hits, std::array<unsigned, CommonMuonTool::nStations> occupancies){
  //=============================================================================
  // comment: Calculate the muon DLL using cumulative histos of the hyperbolic
  //          tangent of the closest distance tuned on 2010 data for muon hypothesis 
  //          and previous landau fittings to 2009 MC for non-muon hypothesis, 
  //          per region and momentum bins:
  // authors: J. Helder Lopes
  // date:    16/02/2011
  //=============================================================================

  static const double atanh05 = gsl_atanh(0.5);

  double p = track.p();

  // RVG the set is done outside the funtion
  //pMuid->setMuonLLMu(-10000.);
  //pMuid->setMuonLLBg(-10000.);

  // RVG this should be done even before entering the function
  // calculate dll only for IsMuonLoose:
  //if ( !pMuid->IsMuonLoose() ) return StatusCode::SUCCESS;

  // Initialize some variables:
  double myDist=-1.;
  double ProbMu=-1.;
  double ProbNonMu = -1.;
  //double parMu[6];  // These won't be used here... Only parNonMu
  //double parNonMu[3];
  //for(int i=0;i<6;i++){parMu[i]=0;}
  //for(int i=0;i<3;i++){parNonMu[i]=0;}
  std::vector<int> regions = findTrackRegions(extrapolation);

  // Calculate Distance using the closest hit:
  myDist = calc_closestDist(track, extrapolation, hits, occupancies);
  if(msgLevel(MSG::DEBUG)) debug() << "The value of myDist is = " << myDist << endmsg;
  
  // RVG need to sort out the protection
/*  if (myDist<=0){
    Warning("Closest Distance < 0 ").ignore();
    break;
  }*/

  // RVG not yet exported
  //EP: Store dist to fill Muon Track extra info
  //m_dist_out=myDist;

  int region=regions[1]; // M2   JHLJHL Check indices ... 30/08/2013
  if (region<0) region=regions[2]; // M3

  // Find Landau's parameters for a given track:
  //std::vector<double> parMu (6,0);
  std::vector<double> parNonMu (3,0);
  
  // RVG TEST. HARDCODED IMPLEMENTATION
  //parNonMu = loadNonMuLandauParam(extrapolation);
  if(region==0) parNonMu = {36.5, 23.8, 0.919259}; //# region 1
  else if (region == 1) parNonMu = {40.5, 33.3, 0.867921}; //# region 2
  else if (region == 2) parNonMu = {22.0, 30.0, 0.800642}; //# region 3
  else if (region == 3) parNonMu = {10.0, 17.0, 0.795441}; //# region 4

  /*if (sc.isFailure()) {
    Warning(" Find Landau Parameters: no valid region",StatusCode::SUCCESS).ignore();
    return StatusCode::FAILURE;
  }*/

  // Determine the momentum bin for this region
  int pBin=GetPbin(p, region);
  double tanhdist;
  // Calculate tanh(dist). The effetive scale factor is after dividing by arc tanh(0.5)
  tanhdist = tanh(myDist/(*(m_tanhScaleFactors[region]))[pBin]*atanh05);
  if(msgLevel(MSG::DEBUG)) debug() << "The value of Tanhdist is = " << tanhdist << endmsg;

  // Calculate Prob(mu)  for a given track using tanh(dist);
  ProbMu = calc_ProbMu_tanh(tanhdist, pBin, region );
  if(msgLevel(MSG::DEBUG)) debug() << "The value of ProbMu is = " << ProbMu << endmsg;
  // RVG need to handle error properly
  //if (ProbMu<0) return Error("ProbMu <0");

  // Calculate ProbNonMu using Landau fits 
  ProbNonMu = calc_ProbNonMu(myDist, parNonMu);
  if(msgLevel(MSG::DEBUG)) debug() << "The value of ProbNonMu is = " << ProbNonMu << endmsg;
  // RVG need to handle error properly
  //if (ProbNonMu<0) throw GaudiException("ProbNonMu < 0", "",StatusCode::FAILURE);

  // Using histograms it's not unexpected that some bins are empty. Use very small prob as a protection for the log
  if(ProbMu<1.e-30) {
    if (msgLevel(MSG::DEBUG) ) debug() << "calcMuonLL_tanhdist_landau: Found null ProbMu: " << ProbMu << endmsg;
    ProbMu = 1.e-30;
  }
  if(ProbNonMu<1.e-30) {
    if (msgLevel(MSG::DEBUG) ) debug() << "calcMuonLL_tanhdist_landau: Found null ProbNonMu: " << ProbNonMu << endmsg;
    ProbNonMu = 1.e-30;
  }

  // Set in the MuonPID object the ProbMu & ProbNonMu (Note the Log!)
  // RVG the set is done outside the function    
  //pMuid->setMuonLLMu(log(ProbMu));
  //pMuid->setMuonLLBg(log(ProbNonMu));

  if (msgLevel(MSG::DEBUG) ) {
    debug() << "calcMuonLL_tanhdist_landau: region: " << region << " momentum: " << p
             << " pBin: " <<  pBin << " dist " << myDist << " tanh(dist2): "
             << tanhdist << " ProbMu: " << ProbMu << " ProbNonMu: " << ProbNonMu
             <<" DLL: " << log(ProbMu/ProbNonMu) << endmsg;
  }

  return {ProbMu, ProbNonMu};
}

/**  Method to compare the Coords of the muonIDs
* modified from the original MuonIDAlg.cpp
*/
bool DLLMuonTool::compareHits(LHCb::MuonPID* muonid1, LHCb::MuonPID* muonid2, std::map< LHCb::MuonPID*, CommonConstMuonHits > m_muonMap ){

  bool theSame = false;
  unsigned m_iM2 = 2;
  CommonConstMuonHits hits1 = m_muonMap[muonid1];
  CommonConstMuonHits hits2 = m_muonMap[muonid2];
  std::for_each(std::begin(hits1), std::end(hits1), [&](const CommonMuonHit* hit1){ // first hits
    std::for_each(std::begin(hits2), std::end(hits2), [&](const CommonMuonHit* hit2){ // second hits
      if(hit1->tile().key() == hit2->tile().key()){
        if(hit1->tile().station() >= m_iM2) theSame = true;
      }
    });
  });

  return theSame;
}

//=============================================================================
// Find out if st myst is in input array of stations
//=============================================================================
/*bool DLLMuonTool::stInStations(const int myst,const std::vector<int>& stations)
{

  for (std::vector<int>::const_iterator it = stations.begin();
       it != stations.end(); ++it)
  {
    int ist = *it;
    if (ist==myst) return true;
  }
  return false;
}*/

