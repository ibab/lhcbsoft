//====================================================================
// FlavourTagging_load.cpp
//--------------------------------------------------------------------
//
//	Package   : Tagging
//
//	Author    : O. Dormond
//  History   :
// +---------+----------------------------------------------+---------
// |    Date |                 Comment                      | Who     
// +---------+----------------------------------------------+---------
// | 15/06/01| Initial version                              | OD
// +---------+----------------------------------------------+---------
//====================================================================


#include "GaudiKernel/DeclareFactoryEntries.h"
DECLARE_FACTORY_ENTRIES(FlavourTagging) {
  DECLARE_ALGORITHM(FlavourTaggingAlgorithm);
  DECLARE_ALGORITHM(FlavourTaggingDebug);
  DECLARE_ALGORITHM(FlavourMonitorAlgorithm);
  DECLARE_ALGORITHM(FlavourTaggingSummary);
  DECLARE_ALGORITHM(CheatedReco);
  DECLARE_TOOL(ParticleTaggerCriterion);
  DECLARE_TOOL(SingleParticleTaggingTool);
  DECLARE_TOOL(KaonSameSideTaggingTool);
  DECLARE_TOOL(VertexChargeTaggingTool);
  DECLARE_TOOL(CategoryTaggingTool);
  DECLARE_TOOL(OrderedTaggingTool);
  DECLARE_TOOL(FTCombinedParticleMaker);

  DECLARE_ALGORITHM(BTagging);
  DECLARE_ALGORITHM(CheatedSelection);

}
