// $Id: DaVinciKernelDict.h,v 1.5 2007-07-24 12:21:05 jpalac Exp $
#ifndef DICT_DAVINCIKERNELDICT_H 
#define DICT_DAVINCIKERNELDICT_H 1

// Include files

/** @file DaVinciKernelDict
 *  
 *
 *  @author Juan PALACIOS
 *  @date   2006-10-02
 */
// ============================================================================
// DaVinciKernel
// ============================================================================
#include "Kernel/IAlgorithm2ID.h"
#include "Kernel/IBestParticles.h"
#include "Kernel/IBremAdder.h"
#include "Kernel/IBTaggingTool.h"
#include "Kernel/ICaloParticleMaker.h"
#include "Kernel/IChangePIDTool.h"
#include "Kernel/ICheckOverlap.h"
#include "Kernel/ICheckSelResults.h"
#include "Kernel/IContextTool.h"
#include "Kernel/IDecayFinder.h"
#include "Kernel/IDecodeSimpleDecayString.h"
#include "Kernel/IDirectionFit.h"
#include "Kernel/IFilterCriteriaTESTool.h"
#include "Kernel/IFilterCriterion.h"
#include "Kernel/IFilterParticles.h"
#include "Kernel/IGeomDispCalculator.h"
#include "Kernel/IJetMaker.h"
#include "Kernel/ILifetimeFitter.h"
#include "Kernel/IMassVertexFit.h"
#include "Kernel/IOnOffline.h"
#include "Kernel/IParticleArrayFilter.h"
#include "Kernel/IParticle2State.h"
#include "Kernel/IParticle2VertexAsct.h"
#include "Kernel/IParticleCombiner.h"
#include "Kernel/IParticleDescendants.h"
#include "Kernel/IParticleFilter.h"
#include "Kernel/IParticleMaker.h"
#include "Kernel/IParticleReFitter.h"
#include "Kernel/IParticleStuffer.h"
#include "Kernel/IParticleTransporter.h"
#include "Kernel/IPhysDesktop.h"
#include "Kernel/IPlotTool.h"
#include "Kernel/IProtoParticleFilter.h"
#include "Kernel/IPVReFitter.h"
#include "Kernel/IRelatedPVFinder.h"
#include "Kernel/IRelatedPV.h"
#include "Kernel/ISecondaryVertexTool.h"
#include "Kernel/ITagger.h"
#include "Kernel/IVertexFit.h"
#include "Kernel/DVAlgorithm.h" // Has to be last for compilation on Windows

#endif // DICT_DAVINCIKERNELDICT_H
