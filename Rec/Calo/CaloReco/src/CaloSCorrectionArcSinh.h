#ifndef CALOTOOLS_CALOSCORRECTIONARCSINH_H
#define CALOTOOLS_CALOSCORRECTIONARCSINH_H 1
// Include files
#include "GaudiKernel/AlgTool.h"
// from CaloKernel
#include "CaloKernel/CaloTool.h"
// from CaloInterfaces
#include "CaloInterfaces/ICaloSCorrectionFunction.h"

/** @class CaloSCorrectionArcSinh CaloSCorrectionArcSinh.h
 *
 *  S-Correction reduced function
 *  here: ArcSinh / 1 parameter
 *
 *  @author Richard Beneyton beneyton@in2p3.fr
 *  @date   20/06/2002
 */
class CaloSCorrectionArcSinh : public CaloTool,
                               virtual public ICaloSCorrectionFunction {
  /// friend factory for instantiation
  friend class ToolFactory<CaloSCorrectionArcSinh>;

public:
  /** The main processing method
   *  @see ICaloSCorrectionFunction
   *  @param  energies to be processed
   *  @param  seed position as option
   *  @param  result value
   *  @return status code 
   */  
  virtual StatusCode calculate( double, double , double,
                                double, double, double,
                                double& );

  /** The main error processing method
   *  @see ICaloSCorrectionFunction
   *  @param  energies to be processed
   *  @param  seed position as option
   *  @param  result value
   *  @return status code 
   */  
  virtual StatusCode calculateprime( double, double , double,
                                     double, double, double,
                                     double& );

  /** standard initialization method
   *  @see CaloTool 
   *  @see  AlgTool
   *  @see IAlgTool 
   *  @return status code 
   */
  virtual StatusCode initialize () ;

  /** standard finalization method
   *  @see CaloTool 
   *  @see  AlgTool
   *  @see IAlgTool 
   *  @return status code 
   */
  virtual StatusCode finalize   () ;

protected:
  
  /** Standard constructor
   *  @see   CaloTool
   *  @see    AlgTool 
   *  @param type    type of the tool  (?)
   *  @param name    name of the concrete instance 
   *  @param parent  pointer to parent object (algorithm, service or tool)
   */
  CaloSCorrectionArcSinh(const std::string& type,
                         const std::string& name,
                         const IInterface* parent);
  
  
  /// destructor (virtual and protected)
  virtual ~CaloSCorrectionArcSinh() ;
  
private:
  std::vector<double> m_Coeff;
};
#endif // CALOTOOLS_CALOSCORRECTIONARCSINH_H
