// $Log: not supported by cvs2svn $
// Revision 1.4  2001/06/29 10:26:18  ibelyaev
// update to use new features of DetDesc v7 package
// 

// CaloDet
#include "CaloDet/DeSubCalorimeter.h"

//------------------------------------------------------------------------------
//
// Implementation of class :  DeSubCalorimeter
//
//------------------------------------------------------------------------------

/// ===========================================================================
/// Standard Constructors
/// ===========================================================================
DeSubCalorimeter::DeSubCalorimeter( const std::string& name ) 
  : DetectorElement     ( name )
  , m_size( 0.0 )
{};
  
/// ===========================================================================
/// Destructor
/// ===========================================================================
DeSubCalorimeter::~DeSubCalorimeter(){};


/// ===========================================================================
/// object identification 
/// ===========================================================================
const CLID& DeSubCalorimeter::clID() const 
{ return DeSubCalorimeter::classID(); } 

/// ===========================================================================
/// standard initialization 
/// ===========================================================================
StatusCode DeSubCalorimeter::initialize()
{
  /// initialize the base class 
  StatusCode sc = DetectorElement::initialize();
  if( sc.isFailure() ) { return sc ; }
  ///
  typedef std::vector<std::string> Parameters;
  typedef Parameters::iterator     Iterator;
  ///
  Parameters pars( userParameters() );
  ///
  { /// cell size 
    Iterator it = std::find( pars.begin() , pars.end () , "CellSize" );
    if( pars.end() != it ) 
      {
        setSize( userParameterAsDouble(*it) ) ;
        pars.erase( it );
      }
    else 
      { return StatusCode::FAILURE ; } 
  }
  if( !pars.empty() ) {
    // some "extra" parameters.
    // should be an error??
  }
  ///
  return StatusCode::SUCCESS;
  ///
};

/// ===========================================================================
/// standard printout to STL standard stream 
/// ===========================================================================
std::ostream& DeSubCalorimeter::printOut( std::ostream& os ) const 
{
  os << "\tDeSubCalorimeter" 
     << " fullname ='" << fullpath() 
     << std::endl; 
  os << "\t\t\tCellSize=" << std::setw(12) << m_size
     << std::endl;    
  return os;
};

/// ===========================================================================
/// standard printout to Gaudi standard stream 
/// ===========================================================================
MsgStream&    DeSubCalorimeter::printOut( MsgStream&    os ) const 
{
  os << "\tDeSubCalorimeter" 
     << " fullname ='" << fullpath() 
     << endreq   ; 
  os << "\t\t\tCellSize=" << std::setw(12) << m_size
     << endreq   ;    
  return os;
};

/// ===========================================================================
