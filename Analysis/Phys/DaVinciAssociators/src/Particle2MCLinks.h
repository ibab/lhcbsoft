// $Id: Particle2MCLinks.h,v 1.8 2004-06-11 15:26:17 phicharp Exp $
#ifndef Particle2MCLinks_H 
#define Particle2MCLinks_H 1

// Include files
// from STL
#include <string>


// local
#include "DaVinciAssociators/Particle2MCLinksAsct.h"
#include "DaVinciAssociators/Particle2MCLink.h"
#include "AsctAlgorithm.h"

/** @class Particle2MCLinks Particle2MCLinks.h
 *  
 *
 *  @author Philippe Charpentier
 *  @date   10/05/2002
 */
class Particle2MCLinks : public AsctAlgorithm {
  friend class AlgFactory<Particle2MCLinks>;
  
public:
  /// Standard constructor
  Particle2MCLinks( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~Particle2MCLinks( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute   ();    ///< Algorithm execution
  virtual StatusCode finalize  ();    ///< Algorithm finalization

protected:

private:
  Object2MCLink*   m_chargedLink;
  Object2MCLink*   m_neutralLink;
  std::vector<std::string>  m_chargedPPLocation;
  std::vector<std::string>  m_neutralPPLocation;
};
#endif // Particle2MCLinks_H
