// $Header: /afs/cern.ch/project/cvs/reps/lhcb/Det/DetDesc/src/component/XmlParserSvc.h,v 1.4 2001-11-20 15:22:25 sponce Exp $
#ifndef DETDESC_XMLPARSERSVC_H
#define DETDESC_XMLPARSERSVC_H

// Include files
#include <map>

#include <sax/ErrorHandler.hpp>
#include <sax/SAXParseException.hpp>

#include <GaudiKernel/Service.h> 

#include "DetDesc/IXmlParserSvc.h"

// Forward and external declarations
template <class TYPE> class SvcFactory;


/** @class XmlParserSvc XmlParserSvc.h DetDesc/XmlParserSvc.h
 *
 * A parsing service for Xml files. Besides pure parsing, it also sperforms
 * some caching.
 *
 * @author Sebastien Ponce
 */
class XmlParserSvc : public Service,
                     virtual public IXmlParserSvc,
                     virtual public ErrorHandler {
  
  /// Friend needed
  friend class SvcFactory<XmlParserSvc>;
  
public:

  /**
   * Standard Constructor
   * @param name   String with service name
   * @param svc    Pointer to service locator interface
   */
  XmlParserSvc (const std::string& name, ISvcLocator* svc);

  /**
   * default destructor
   */
  virtual ~XmlParserSvc();

  /**
   * Queries interfaces of Interface.
   * @param riid ID of Interface to be retrieved
   * @param ppvInterface Pointer to Location for interface pointer
   * @return status depending on the completion of the call
   */
  virtual StatusCode queryInterface (const IID& riid, void** ppvInterface);

  ///////////////////////////////////////////////////
  // implementation of the IXmlParserSvc interface //
  ///////////////////////////////////////////////////

  /**
   * This method parses an xml file and produces the corresponding DOM
   * document.
   * @param fileName the name of the file to parse
   * @return the document issued from the parsing
   */
  virtual DOM_Document parse (const char* fileName);

  /**
   * This method parses XML from a string and produces the corresponding DOM
   * document.
   * @param source the string to parse
   * @return the document issued from the parsing
   */
  virtual DOM_Document parseString (std::string source);

  /**
   * This clears the cache of previously parsed xml files.
   */
  virtual void clearCache();

  //////////////////////////////////////////////////////
  // implementation of the SAX ErrorHandler interface //
  //////////////////////////////////////////////////////

  /**
   * Receives notification of a warning. 
   * The parser will use this method to report conditions that are not errors
   * or fatal errors as defined by the XML 1.0 recommendation. The default
   * behaviour is to display the warning via the Message service.
   * @param exception the warning information encapsulated in a SAX parse
   * exception.
   */
  virtual void warning( const SAXParseException& exception );

  /**
   * Receives notification of a recoverable error. 
   * This corresponds to the definition of "error" in section 1.2 of the W3C
   * XML 1.0 Recommendation. For example, a validating parser would use
   * this callback to report the violation of a validity constraint. The default
   * behaviour is to display the error via the Message service.
   * @param exception the error information encapsulated in a SAX parse
   * exception.
   */
  virtual void error( const SAXParseException& exception );

  /**
   * Receives notification of a non-recoverable error. 
   * This corresponds to the definition of "fatal error" in section 1.2 of the
   * W3C XML 1.0 Recommendation. For example, a parser would use this callback
   * to report the violation of a well-formedness constraint. The default
   * behaviour is to display the error via the Message service.
   * @param exception the error information encapsulated in a SAX parse
   * exception. 
   */
  virtual void fatalError( const SAXParseException& exception );

  /**
   * Resets the Error handler object on its reuse. 
   * This method helps in reseting the Error handler object implementational
   * defaults each time the Error handler is begun.
   * The default implementation does nothing
   */
  virtual void resetErrors ();


private:
  
  /**
   * Caches the new document, parsed from the given file
   * Since this adds an item into the cache, this may remove another
   * item if the cache was full
   * @param fileName the name of the file that was just parsed
   * @param document the document that is the result of the parsing
   */
  void cacheItem (std::string fileName, DOM_Document document);

  /**
   * this only increases the age of the cache.
   * It also checks that the age don't go back to 0. If it is the case,
   * it puts back every item birthDate to 0 also
   */
  void increaseCacheAge ();

  
private:

  /// the actual DOM parser
  DOMParser* m_parser;

  /**
   * this is a parameter that defines the cache behavior.
   * the default behavior is that an item is released if there is no more space
   * and if it has the smallest birthDate+cacheAgressivity*utility score.
   * Thus, a 0 value allows to have a FIFO cache behavior, while a bigger
   * value tends to keep only reused items.
   */
  unsigned int m_cacheBehavior;

  /**
   * a structure containing a cached document, its birthDate and its utility.
   * The birthDate is the age of the cache when this item arrived.
   * The utility is the number of times this item was retrieved since it is
   * in the cache.
   * The rule is that an item is released if there is no more space
   * and if it has the smallest birthDate+cacheBehavior*utility score.
   */
  typedef struct _cachedItem {
    DOM_Document document;
    unsigned int birthDate, utility;
  } cachedItem;

  /**
   * This is the type of the cache : a map of cachedItems,
   * indexed by a fileName, given as a standard string
   */
  typedef std::map<std::string, cachedItem> cacheType;

  /// a map for caching DOMDocuments
  cacheType m_cache;

  /**
   * The age of the cache : this is the number of operations done
   * on it, provided that an operation is either retrieving an element
   * or puting a new one (eventually removing another one but this is
   * not taken into account
   */
  unsigned int m_cacheAge;
  
  /// The maximum number of cached documents
  unsigned int m_maxDocNbInCache;

};

#endif    // DETDESC_XMLPARSERSVC_H
