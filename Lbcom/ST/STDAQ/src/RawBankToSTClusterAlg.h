// $Id: RawBankToSTClusterAlg.h,v 1.5 2007-10-29 13:51:11 mneedham Exp $
#ifndef RAWBANKTOSTCLUSTERALG_H 
#define RAWBANKTOSTCLUSTERALG_H 1

#include "STDecodingBaseAlg.h"
#include "Event/RawBank.h"
#include "STDAQDefinitions.h"

#include "Event/STCluster.h"

#include <vector>
#include <string>

/** @class RawBankToSTClusterAlg RawBankToSTClusterAlg.h
 *  
 *  Algorithm to create STClusters from RawEvent object
 * 
 *  @author M. Needham
 *  @date   2004-01-07
 */


#include "STClusterWord.h"

class SiADCWord;
class STTell1Board;

namespace LHCb{
 class STChannelID;
 class STLiteCluster;
};

class RawBankToSTClusterAlg : public STDecodingBaseAlg {

public:

  /// Standard constructor
  RawBankToSTClusterAlg( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RawBankToSTClusterAlg( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode execute();    ///< Algorithm execution

private:

  StatusCode decodeBanks(LHCb::RawEvent* rawEvt, LHCb::STClusters* digitCont ) const;

  StatusCode createCluster(const STClusterWord& aWord,
                           const STTell1Board* aBoard,
                           const std::vector<SiADCWord>& adcValues,
		 	   LHCb::STClusters* clusCont,
			   const unsigned int version) const;
 
  double mean(const std::vector<SiADCWord>& adcValues) const;
   
  LHCb::STLiteCluster word2LiteCluster(STClusterWord aWord, 
				       LHCb::STChannelID chan,
				       const unsigned int fracStrip) const;

  double stripFraction(const double interStripPos) const;

  /// Output location for STClusters
  std::string m_clusterLocation;

  unsigned int m_nBits; 

  std::string m_rawEventLocation;

};

#endif // RAWBUFFERTOSTDIGITALG_H 
