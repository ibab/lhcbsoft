// $Id: Unit.h,v 1.4 2004-12-21 14:33:03 ltocco Exp $

#ifndef L0MUONKERNEL_UNIT_H
#define L0MUONKERNEL_UNIT_H     1

/** @class Unit Unit.h L0MuonKernel/Unit.h

   Class representing a unit of data processing logic 
   of the level-0 muon trigger for hardware simulations
   
*/ 

#include <string>
#include <map>
#include "L0MuonKernel/Register.h"
#include "GaudiKernel/MsgStream.h"

namespace L0Muon {

class Unit {

public:

  /// Default constructor
  Unit();

  /// Destructor
  virtual ~Unit();
  
  /// Reset input/output registers
  void releaseRegisters();
  
  /// Print the registers contents
  void dumpRegisters(MsgStream & log);

  /// set the pointer to the parent unit
  virtual void setParent(L0Muon::Unit * unit);

  /// return the parent unit
  Unit * parent(){ return m_parent;}
  
  /// Search for subunit 
  Unit * subUnit(std::string name){
    std::map<std::string,L0Muon::Unit*>::iterator iunit;
    iunit = m_units.find(name);
    return (*iunit).second;
  };
  
  
  /// Add input register  
  virtual void addInputRegister(L0Muon::Register* in);

  /** Add input register

      @param rname   : name of the input register
  */
  virtual void addInputRegister(L0Muon::Register* in, std::string rname);
 
  /// Add output register  
  virtual void addOutputRegister(L0Muon::Register* out);

  /** Add output register

      @param rname   : name of the output register
  */
  virtual void addOutputRegister(L0Muon::Register* out, std::string rname);
 
  /// Add subunit
  void addUnit(L0Muon::Unit* unit);

  /** Add sununit

      @param uname   : name of the subunit
  */
  void addUnit(L0Muon::Unit* unit, std::string uname);
 
  
  /// Virtual method to initialize the hierarchy of units
  virtual void initialize() =0;

  /** Virtual method to initialize the hierarchy of units

      @param log   : MSG::DEBUG for running in Gaudi
  */
  virtual void initialize(MsgStream & log) =0;

  /// Virtual method to execute the hierarchy of units
  virtual void execute()    =0;

  /** Virtual method to execute the hierarchy of units

      @param log   : MSG::DEBUG for running in Gaudi
  */
  virtual void execute(MsgStream & log)    =0;

  /// Virtual method to finalize the hierarchy of units
  virtual void finalize()   =0;


protected:

  Unit * m_parent;
  std::map<std::string,L0Muon::Register*> m_inputs;    // input registers    
  std::map<std::string,L0Muon::Register*> m_outputs;   // output registers
  std::map<std::string,L0Muon::Unit*> m_units;         // subunits  
  

  static std::ostream m_log;
  

};

};  // namespace L0Muon

#endif      // L0MUONKERNEL_UNIT_H  
