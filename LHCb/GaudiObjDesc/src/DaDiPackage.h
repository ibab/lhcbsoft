// $Header: /afs/cern.ch/project/cvs/reps/lhcb/GaudiObjDesc/src/DaDiPackage.h,v 1.1.1.1 2001-10-03 16:39:17 mato Exp $
#ifndef DADIPACKAGE_H 
#define DADIPACKAGE_H 1

// Include files
#include <list>

#include "dom/DOMString.hpp"

#include "DaDiClass.h"

/** @class DaDiPackage DaDiPackage.h
 *  
 *
 *  @author Stefan Roiser
 *  @date   14/06/2001
 */
class DaDiPackage {
public:
  /// Standard constructor
  DaDiPackage() {}; 

  virtual ~DaDiPackage() {}; ///< Standard destructor

  DOMString packageName();
  void setPackageName(DOMString value);

  std::string popImpSoftList();
  void pushImpSoftList(std::string value);
  int sizeImpSoftList();
  
  std::string popImpStdList();
  void pushImpStdList(std::string value);
  int sizeImpStdList();
  
  std::string popImportList();
  void pushImportList(std::string value);
  int sizeImportList();
  void remDblImportList();
  
  DaDiClass* popDaDiClass();
  void pushDaDiClass(DaDiClass* value);
  int sizeDaDiClass();

protected:

private:

  DOMString              m_packageName;
  std::list<std::string> m_impSoftList,
                         m_impStdList,
                         m_importList;
  std::list<DaDiClass*>  m_daDiClass;

};

inline DOMString DaDiPackage::packageName()
{
  return m_packageName;
}

inline void DaDiPackage::setPackageName(DOMString value)
{
  m_packageName = value;
}

inline std::string DaDiPackage::popImpSoftList()
{
  std::string pt =  m_impSoftList.front();
  m_impSoftList.push_back(pt);
  m_impSoftList.pop_front();
  return pt;
}

inline void DaDiPackage::pushImpSoftList(std::string value)
{
  m_impSoftList.push_front(value);
}

inline int DaDiPackage::sizeImpSoftList()
{
  return m_impSoftList.size();
}

inline std::string DaDiPackage::popImpStdList()
{
  std::string pt =  m_impStdList.front();
  m_impStdList.push_back(pt);
  m_impStdList.pop_front();
  return pt;
}

inline void DaDiPackage::pushImpStdList(std::string value)
{
  m_impStdList.push_front(value);
}

inline int DaDiPackage::sizeImpStdList()
{
  return m_impStdList.size();
}

inline std::string DaDiPackage::popImportList()
{
  std::string pt = m_importList.front();
  m_importList.push_back(pt);
  m_importList.pop_front();
  return pt;
}

inline void DaDiPackage::pushImportList(std::string value)
{

	std::string import;

	while (value != "")
	{
		int i = value.find_first_of(":,<>");

		if (i == -1)
		{
			import = value;
			value = "";
		}
		else
		{
			import = value.substr(0,i);
			value = value.substr(i+1,std::string::npos);
		}

		if ((import != "")       && (import != "long")     &&
			(import != "bool")   && (import != "short")    && 
			(import != "long")   && (import != "string")   &&
			(import != "int")    && (import != "float")    &&
			(import != "double") && (import != "unsigned") &&
			(import != "signed") && (import != "std")      &&
			(import != "pair")   && (import != "char"))
		{
			if ((import == "vector") || (import == "list")   ||
				(import == "deque")  || (import == "queue")  ||
				(import == "stack")  || (import == "map")    ||
				(import == "set")    || (import == "bitset"))
			{
				m_impStdList.push_front(import);
			}
			else
			{
				m_importList.push_front(import);
			}
		}
	}
}

inline int DaDiPackage::sizeImportList()
{
  return m_importList.size();
}

inline void DaDiPackage::remDblImportList()
{
	m_importList.sort();
	m_importList.unique();
}

inline DaDiClass* DaDiPackage::popDaDiClass()
{
  DaDiClass* pt = m_daDiClass.front();
  m_daDiClass.pop_front();
  return pt;
}

inline void DaDiPackage::pushDaDiClass(DaDiClass* value)
{
  m_daDiClass.push_back(value);
}

inline int DaDiPackage::sizeDaDiClass()
{
  return m_daDiClass.size();
}

#endif // DADIPACKAGE_H

