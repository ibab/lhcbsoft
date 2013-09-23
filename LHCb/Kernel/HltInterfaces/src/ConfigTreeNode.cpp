#include "Kernel/ConfigTreeNode.h"
#include "boost/regex.hpp"

#include "GaudiKernel/GaudiException.h"

using namespace std;
using Gaudi::Math::MD5;

ConfigTreeNode::ConfigTreeNode(const LeafRef& leaf, const NodeRefs& nodes, const std::string& label)
      : m_nodes(nodes)
      , m_leaf(leaf)
      , m_label(label)
      , m_digest( digest_type::createInvalid() )
{ 
 if (this->label().find(':')!=std::string::npos)
    throw GaudiException("':' is not allowed in label",label,StatusCode::FAILURE);
}

void ConfigTreeNode::updateCache() const { 
    std::ostringstream str; this->print(str);
    m_digest = digest_type::compute(str.str());
}

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/stream_translator.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/xml_parser.hpp>
using boost::property_tree::ptree;
namespace {
void read_custom(istream& is, ptree& top) {
    bool parsing_nodes = false;
    static boost::regex leaf("^Leaf: ([a-fA-F0-9]{32})$"),
                        label("^Label: (.*)$"),
                        nodestart("^Nodes: \\[$"),
                        node("^ ?([a-fA-F0-9]{32})$"),
                        nodeend("^\\]$");
    std::string s;
    boost::smatch what;
    ptree& nodes = top.put_child("Nodes",ptree());
    while (!is.eof()) {
        getline(is,s);
        if (s.empty()) break;
        if (parsing_nodes)  {
            if (boost::regex_match(s,what,nodeend) ) { 
                parsing_nodes = false;
            } else if (boost::regex_match(s,what,node) ) { 
                nodes.push_back(std::make_pair(std::string(),what[1].str()));
            } else {
                cout << "parsing error while looking for nodes!!! : [" << s << "]" << endl;
            }
        } else {
            if (boost::regex_match(s,what,leaf) ) { assert(leaf().invalid());
                top.put("Leaf", what[1].str() );
            } else if (boost::regex_match(s,what,nodestart) ) { assert(nodes().empty());
                parsing_nodes = true;
            } else if (boost::regex_match(s,what,label) ) { assert(label().empty());
                top.put("Label",what[1].str());
            } else {
                cout << "parsing error!!! : [" << s << "]" << endl;
            }
        }
    }
}

struct MD5Translator {
    typedef std::string                  internal_type;
    typedef ConfigTreeNode::digest_type  external_type;
    boost::optional<external_type> get_value( internal_type const & v) 
    {  return external_type::createFromStringRep(v); }
    boost::optional<internal_type> put_value( external_type const & v) 
    { return v.str(); }
};

}

namespace boost { namespace property_tree {
    template <> 
    struct translator_between<std::string,ConfigTreeNode::digest_type> {
        typedef MD5Translator type;
    };
} }


istream& ConfigTreeNode::read(istream& is) {
    m_digest = digest_type::createInvalid();
    ptree top;
    int fmt = is.peek();
    if (fmt=='{')      { read_json(is,top);   }
    else if (fmt=='<') { read_xml(is,top);    }
    else               { read_custom(is,top); }
    m_label = top.get<std::string>("Label"); // add default: empty string (most nodes have no label)
    m_leaf =  top.get<digest_type>("Leaf");  // add default: digest_type::createInvalid() (top level has no leaf)
    m_nodes.clear();
    const ptree& nodes = top.get_child("Nodes");
    for (ptree::const_iterator i=nodes.begin(); i!=nodes.end();++i) m_nodes.push_back( i->second.get_value<ConfigTreeNode::digest_type>() );
    return is;
}

// when switching to JSON (or XML) make Label and nodes optional (saves space!)
// ditto for Leaf: top level nodes have no leaf. (value if not present: MD5::createInvalid)
// NOTE: when switching, make sure that digest continues to use this representation...
ostream& ConfigTreeNode::print(ostream& os) const {
    os << "Label: " << label() << '\n'
       << "Leaf: " << leaf() << '\n'
       << "Nodes: [\n";
    for (NodeRefs::const_iterator i=nodes().begin();i!=nodes().end();++i ) os << " "<< *i <<'\n';
    return os << "]" << endl;
}

std::ostream& operator<<(std::ostream& os, const ConfigTreeNode& x) { return x.print(os); }
std::istream& operator>>(std::istream& is, ConfigTreeNode& x)       { return x.read(is); }
