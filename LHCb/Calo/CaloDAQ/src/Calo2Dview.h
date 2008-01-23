// $Id: Calo2Dview.h,v 1.1 2008-01-23 19:05:17 odescham Exp $
#ifndef CALO2DVIEW_H 
#define CALO2DVIEW_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiHistoAlg.h"
//from LHCb
#include "CaloDet/DeCalorimeter.h"
#include "Kernel/CaloCellID.h"
#include "AIDA/IHistogram2D.h"
// from Event
#include "Event/MCCaloHit.h"
#include "Event/MCCaloDigit.h"
#include "Event/CaloDigit.h"
#include "Event/CaloAdc.h"
#include "Event/L0CaloAdc.h"
#include "Event/L0PrsSpdHit.h"
#include "Event/L0CaloCandidate.h"

/** @class Calo2Dview Calo2Dview.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2008-01-18
 */
class Calo2Dview : public GaudiHistoAlg {
public: 
  /// Standard constructor
  Calo2Dview( const std::string& name, ISvcLocator* pSvcLocator );
  virtual ~Calo2Dview( ); ///< Destructor

  virtual StatusCode initialize();    ///< Algorithm initialization
  virtual StatusCode finalize  ();    ///< Algorithm finalization
protected:
  AIDA::IHistogram2D* bookCalo2D(const HistoID unit,const std::string title, std::string name );
  AIDA::IHistogram2D* bookCalo2D(const HistoID unit,const std::string title, unsigned  int calo );
  AIDA::IHistogram2D*  Calo2Dview::fillCalo2D(const HistoID unit, LHCb::MCCaloHit mchit, const std::string title="");
  AIDA::IHistogram2D*  Calo2Dview::fillCalo2D(const HistoID unit, LHCb::MCCaloDigit mcdigit, const std::string title="");
  AIDA::IHistogram2D*  Calo2Dview::fillCalo2D(const HistoID unit, LHCb::CaloDigit digit, const std::string title="");
  AIDA::IHistogram2D*  Calo2Dview::fillCalo2D(const HistoID unit, LHCb::CaloAdc adc, const std::string title="");
  AIDA::IHistogram2D*  Calo2Dview::fillCalo2D(const HistoID unit, LHCb::L0CaloAdc l0adc, const std::string title="");
  AIDA::IHistogram2D*  Calo2Dview::fillCalo2D(const HistoID unit, LHCb::L0PrsSpdHit hit, const std::string title="");
  AIDA::IHistogram2D*  Calo2Dview::fillCalo2D(const HistoID unit, LHCb::L0CaloCandidate l0calo, const std::string title="");  
  AIDA::IHistogram2D* fillCalo2D(const HistoID unit, LHCb::CaloCellID id , double value, const std::string title="");
  AIDA::IHistogram2D* fillCaloPin2D(const HistoID unit,  LHCb::CaloCellID id , double value, const std::string title="");
  void reset(const HistoID unit,std::string title="");
  void resetTitle(const HistoID unit,std::string title);
  void getCaloParam(unsigned int calo);
  void setThreshold(int threshold){m_threshold=threshold;};
  void setPinView(bool pin){m_pin=pin ;};
  void setActualSize(bool dim){m_dim=dim ;};
  void setL0ClusterView(bool l0){m_l0=l0 ;};
  void setOffset(int offset){m_offset = offset;};


  // attributes
  long m_threshold;
  bool m_pin;
  int  m_offset;
  bool m_dim;
  bool m_l0;

private:
  std::map<int,DeCalorimeter*> m_caloMap;
  std::map<int,int> m_centreMap;
  std::map<int,int> m_regMap;
  std::map<int,double> m_xsizeMap;
  std::map<int,double> m_ysizeMap;
  //
  DeCalorimeter* m_calo;
  int m_caloType;
  unsigned int m_reg;
  unsigned int m_centre;
  double m_xsize;
  double m_ysize;
  //
  std::map<HistoID,unsigned int> caloViewMap;
  
};
#endif // CALO2DVIEW_H
