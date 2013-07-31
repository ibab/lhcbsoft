#include "LoKi/DictTransformTool.h"
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/MsgStream.h"

#include "TMVA/Reader.h"
#include "TXMLEngine.h"
#include "TXMLDocument.h"
#include "TXMLNode.h"

#include <boost/lexical_cast.hpp>

#include "Options.h"

/** @class TMVATransform 
 *  Policy class to be used by the DictTransformTool template
 *  Implementing the TMVA Reader backend
 *
 *  @author Sam Hall
 *  @date   2013-07-29
 */


typedef std::map<std::string, std::string> optmap;

//==============================================================================
// Forward decralration of class -- TMVATransform
//==============================================================================
class TMVATransform {
private:
  bool m_setup_success;
  bool m_keep_all_vars;
  std::string m_reader_opts;
  std::string m_weightfile;
  std::string m_name;
  std::string m_branchname;
  std::vector<std::string> m_spectator;
  //std::vector<double> m_values; // because of const this is defined within operator()
 
  //std::map<std::string, OptionBase*> m_options;

  // TMVA bits
  TMVA::Reader* m_reader;

  // Functions
  void readWeightsFile(MsgStream&);
  void setupReader(MsgStream&);
  bool parseOpts(optmap&, MsgStream&);


public:
  std::vector<std::string> m_variables;
  TMVATransform();
  ~TMVATransform();

  bool Init(optmap options, MsgStream& info);
  bool operator()(const IParticleDictTool::DICT& in, IParticleDictTool::DICT& out) const;

};


//==============================================================================
// Costructor and Destructor
//==============================================================================
TMVATransform::TMVATransform()
  : m_setup_success(false)
  , m_reader_opts("!Color") //Silent : none,  V : verbose, Color : colored
  , m_weightfile("")
  , m_name("")
  , m_branchname("")
  , m_reader(NULL)
{
  return;
}

TMVATransform::~TMVATransform() {
  delete m_reader;
  return;
}


//==============================================================================
// Implementation of required functions to fulfill the DictTransform policy
//==============================================================================
bool
TMVATransform::Init(optmap options, MsgStream& info){
  // parse options into member variables, also empties map
  m_setup_success = parseOpts(options, info);
  if (!m_setup_success) { return false ; }

  readWeightsFile(info);
  if (!m_setup_success) { return false ; }

  setupReader(info);
  if (!m_setup_success) { return false ; }
  if (m_reader == NULL) {
    m_setup_success &= false;
    return false;
  }

  return true;
}


// Works for std::transform
//double map2vec2(IParticleDictTool::DICT::const_reference in) {
//  return in.second;
//}


bool
TMVATransform::operator()(const IParticleDictTool::DICT& in, IParticleDictTool::DICT& out) const {
  // Check initialization
  if (!m_setup_success) {
    return false;
  }
  // This works, but the ordering is missing, so use for loop
  //std::transform(in.begin(), in.end(), std::back_inserter(values), map2vec2);
  std::vector<double> values(m_variables.size()); // vector with correct size
  std::vector<double>::iterator value = values.begin();
  std::vector<std::string>::const_iterator varname = m_variables.begin();
  for (; value!=values.end(); ++varname, ++value) { *value = in[*varname]; }
  if (m_keep_all_vars) { out = in; }
  else { out.clear(); }
  out.insert(out.end(), m_name, m_reader->EvaluateMVA(values, m_name));
  return true;
}


//==============================================================================
// Implementation of other functions
//==============================================================================
void TMVATransform::readWeightsFile(MsgStream& info) {
  // Clear relavent variables
  m_variables.clear();
  m_spectator.clear();
  // Check that the WeightFile exists
  ifstream fin(m_weightfile.c_str());
  if (!fin.good()) { // check existence of WeightFile path
    if (m_weightfile == "") {
      info << "ERROR  ";
      info << "WeightFile not given." << endmsg;
    } else {
      info << "WeightFile \"" << m_weightfile << "\" not found." << endmsg;
      info << "  TMVA will not be run.  The output will be 0 for each event." << endmsg;
    }
    m_setup_success &= false;
    return;
  } else {
    m_setup_success &= true;
  }
  // Setup the XML parser
  info << "Reading WeightFile \"" << m_weightfile << "\"" << endmsg;
  TXMLEngine xmlparser;
  XMLDocPointer_t doc = xmlparser.ParseFile(m_weightfile.c_str());
  XMLNodePointer_t root = xmlparser.DocGetRootElement(doc);
  XMLNodePointer_t child = xmlparser.GetChild(root);
  // Initialization of variables
  XMLNodePointer_t gchild; // grandchild (grandnode)
  std::string nodename;    // Node name of current child (node)
  std::string name;        // name of current gchild
  std::string val;         // values assigned to current gchild
  std::string exp;         // expression assigned to current gchild
  unsigned nvar;
  // Loop over all children (nodes)
  while (child != NULL) {
    nodename = xmlparser.GetNodeName(child);
    if (nodename == "GeneralInfo") { // Get general info
      info << "Node name: " << nodename << endmsg;
      gchild = xmlparser.GetChild(child);
      while (gchild != NULL) {
        name = xmlparser.GetAttr(gchild, "name");
        val = xmlparser.GetAttr(gchild, "value");
        info << name << " : " << val << endmsg;
        gchild = xmlparser.GetNext(gchild);
      }
    } else if (nodename == "Variables") { // Get training variables
      info << "Node name: " << nodename << endmsg;
      nvar = atoi(xmlparser.GetAttr(child, "NVar"));
      info << "Identified " << nvar << " training variables:" << endmsg;
      gchild = xmlparser.GetChild(child);
      for (unsigned i=0; i<nvar; ++i) {
        exp = xmlparser.GetAttr(gchild, "Expression");
        info << "  " << exp; // << endmsg;
        m_variables.push_back(exp);
        gchild = xmlparser.GetNext(gchild);
      }
    } else if (nodename == "Spectators") { // Get spectator variables
      info << "Node name: " << nodename << endmsg;
      nvar = atoi(xmlparser.GetAttr(child, "NSpec"));
      info << "Identified " << nvar << " spectator variables:" << endmsg;
      gchild = xmlparser.GetChild(child);
      for (unsigned i=0; i<nvar; ++i) {
        exp = xmlparser.GetAttr(gchild, "Expression");
        info << "  " << exp; // << endmsg;
        m_spectator.push_back(exp);
        gchild = xmlparser.GetNext(gchild);
      }
    } // end if statement(s)
    child = xmlparser.GetNext(child);
  } // end loop over child nodes
  return;
}


//==============================================================================
void TMVATransform::setupReader(MsgStream& info) {
  // Check required variables
  if (m_variables.size() == 0) {
    info << "No variables set" << endmsg;
  }
  if (m_weightfile == "") {
    info << "No WeightFile set" << endmsg;
  }
  // Initialize new TMVA::Reader with training variables and reader options
  m_reader = new TMVA::Reader(m_variables, m_reader_opts.c_str());
  // Tell the reader about Spectator variables, but we can just set their values to 1
  //   - TMVA will never know.
  std::vector<std::string>::iterator it;
  for (it=m_spectator.begin(); it!=m_spectator.end(); ++it) {
    int* unity = new int(1.0);
    m_reader->AddSpectator(it->c_str(), unity);
  }
  m_reader->BookMVA(m_name.c_str(), m_weightfile);
  return;
}

//==============================================================================
bool TMVATransform::parseOpts(optmap& options, MsgStream& info) {
  bool pass = true;
  Options parse(options);
  parse.add<std::string>("Name", "Name of output branch (Required)",
      m_name, info);
  parse.add<std::string>("XMLFile", "Some XML file (Required)",
      m_weightfile, info);
  parse.add<std::string>("TMVAOptions", "Must be Color, Silent, V",
      m_reader_opts, info, "Color");
  parse.add<bool>("KeepVars", "Keep BDT input variables, \"1\" or \"0\"",
      m_keep_all_vars, info, false);
  pass = parse.check(info);
  return pass;
}


////==============================================================================
typedef LoKi::Hybrid::DictTransformTool<TMVATransform> TMVAClassifierTool;
DECLARE_TOOL_FACTORY(TMVAClassifierTool)
//==============================================================================
