// $Id: RegisterFactory.h,v 1.4 2004-12-21 14:33:03 ltocco Exp $

#ifndef L0MUONKERNEL_REGISTERFACTORY_H
#define L0MUONKERNEL_REGISTERFACTORY_H     1

/** @class RegisterFactory RegisterFactory.h L0MuonKernel/RegisterFactory.h

   Class representing a factory for creating and 
   owning registers
   
   
*/ 

#include <string>
#include <map>
#include "L0mConf/L0MPuNodeBase.h"
#include "L0MuonKernel/Register.h"
#include "L0MuonKernel/TileRegister.h"

namespace L0Muon {

class RegisterFactory {

protected:

  /// Constructor
  RegisterFactory();
public:

  /// Destructor
  virtual ~RegisterFactory();
  
  /// single instance for the factory
  static RegisterFactory* instance();
  
  /**
     Create a register.
     If the register was already created, it 
     return the existing one.
     
     @param name : name of the register
     @param size : number of bits
   */
  Register* createRegister(std::string name, int size);

  /**
     Create a tileregister.
     If the register was alredy created, it return the 
     existing one
     
     @param name : name of the register
     @param size : number of bits
   */
  TileRegister* createTileRegister(std::string name, int size);
  TileRegister* searchTileRegister(std::string name);
 
  /// Crate an alias for the name of the register
  int createAlias(std::string name, std::string alias);

  /// Delete the registers named name 
  void deleteRegister(std::string name);
  std::map<std::string,Register*> getRegisters() { return m_registers ; }   

  /// True if the register is found
  bool found(){ return m_found;}
  
    

private:

  static RegisterFactory* m_instance;
  std::map<std::string,Register*> m_registers;    // registers repository    
  bool m_found;

};

};  // namespace L0Muon

#endif      // L0MUONKERNEL_REGISTERFACTORY_H  
