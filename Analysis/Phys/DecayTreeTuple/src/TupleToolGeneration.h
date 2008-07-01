// $Id: TupleToolGeneration.h,v 1.1 2008-07-01 14:50:02 pkoppenb Exp $
#ifndef JBOREL_TUPLETOOLGENERATION_H
#define JBOREL_TUPLETOOLGENERATION_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/IEventTupleTool.h"            // Interface

class ITupleTool;

/** @class TupleToolGeneration TupleToolGeneration.h jborel/TupleToolGeneration.h
 *
 * \brief b quark content and number of pp interactions for DecayTreeTuple
 *
 * Tuple columns:
 * - b-content, c-content, t-content
 * - ppColl : number of pp colliosns 
 *
 * Only data that can be extracted without accessing Sm/MCParticles to be added here
 *
 * \sa DecayTreeTuple
 *  @author P. Koppenburg
 *  @date   2008-07-01
 */
class TupleToolGeneration : public GaudiTool, virtual public IEventTupleTool {
public:
  /// Standard constructor
  TupleToolGeneration( const std::string& type,
		      const std::string& name,
		      const IInterface* parent);

  virtual ~TupleToolGeneration( ){}; ///< Destructor

  StatusCode fill( Tuples::Tuple& );
  virtual StatusCode initialize();

private :
  std::string m_etcTupleName;
};
#endif // JBOREL_TUPLETOOLGENERATION_H
