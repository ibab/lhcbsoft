// $Header: /afs/cern.ch/project/cvs/reps/lhcb/GaudiObjDesc/src/DaDiCppDict.h,v 1.4 2002-03-27 16:56:36 mato Exp $
#ifndef DADICPPDICT_H 
#define DADICPPDICT_H 1

// Include files
#include "DaDiTools.h"
#include "DaDiFrontEnd.h"
#include "DaDiPackage.h"

/** @class DaDiCppHeader DaDiCppHeader.h
 *  
 *
 *  @author Stefan Roiser
 *  @date   13/06/2001
 */

class DDBEdict
{
public:
  DDBEdict() {};
  ~DDBEdict() {};
  
  static void printCppDictionary(DaDiPackage* gddPackage, 
                                 char* envXmlDB, 
                                 const char* envOut, 
                                 bool additionalImports,
                                 std::string dothDir);

};

#endif // DADICPPHEADER_H

