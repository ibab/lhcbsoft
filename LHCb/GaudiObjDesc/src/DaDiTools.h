// $Header: /afs/cern.ch/project/cvs/reps/lhcb/GaudiObjDesc/src/DaDiTools.h,v 1.6 2002-02-22 16:52:22 mato Exp $
#ifndef DADITOOLS_H 
#define DADITOOLS_H 1

// Include files
#include "GaudiKernel/Kernel.h"

#include <iostream>
#include <string>
#include <vector>

#include <sax/SAXParseException.hpp>
#include <sax/ErrorHandler.hpp>
#include <dom/DOMString.hpp>
#include <util/XercesDefs.hpp>


/** @class DaDiTools DaDiTools.h
 *  
 *
 *  @author Stefan Roiser
 *  @date   13/06/2001
 */


class DaDiTools : public ErrorHandler
{

public:
  DaDiTools() {}
  ~DaDiTools() {} 

  void warning(const SAXParseException& toCatch);
  void error(const SAXParseException& toCatch);
  void fatalError(const SAXParseException& toCatch);
  void resetErrors();

    
  static std::string chooseAccess(const std::string& str);
  static bool isSimple(std::string value);
  static bool isFundamental(std::string value);
  static bool isPointer(std::string value);
  static bool isRef(std::string value);
  static bool isArray(std::string value);
  static bool isStdContainer(std::string value);
  static std::string containerType(std::string value);
  static std::string firstUp(const std::string& s);

  static void pushAddImport(std::string value);
  static const std::vector<std::string>& addImport();

private:
  static std::vector<std::string> additionalImports;
  static std::string name;

};





#endif // DADITOOLS_H
