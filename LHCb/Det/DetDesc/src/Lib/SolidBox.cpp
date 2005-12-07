// $Id: SolidBox.cpp,v 1.12 2005-12-07 07:33:50 cattanem Exp $ 
// ===========================================================================
// CVS tag $Name: not supported by cvs2svn $ 
// ===========================================================================
// GaudiKernel
#include "GaudiKernel/IInspector.h"
// DetDesc 
#include "DetDesc/DetDesc.h" 
#include "DetDesc/SolidBox.h" 
#include "DetDesc/SolidTicks.h" 
#include "DetDesc/SolidException.h" 
// LHCbDefinitions
#include "Kernel/SystemOfUnits.h"

// ============================================================================
/** @file SolidBox.cpp
 *
 *  implementation of class SolidBox 
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru  
 *  @date xx/xx/xxxx
 */
// ============================================================================

// ============================================================================
/** standard(public)  constructor 
 *  @param      Name           name of this box  
 *  @param      xHalf          half-size of the box in x-direction
 *  @param      yHalf          half-size of the box in y-direction
 *  @param      zHalf          half-size of the box in z-direction
 *  @exception  SolidException wrong parameters range 
 */
// ============================================================================
SolidBox::SolidBox
( const std::string& Name  ,
  const double       xHalf , 
  const double       yHalf , 
  const double       zHalf )
  : SolidBase        ( Name  ) 
  , m_box_xHalfLength( xHalf )
  , m_box_yHalfLength( yHalf )
  , m_box_zHalfLength( zHalf )
{
  if( 0 >= xHalfLength() )
    { throw SolidException("SolidBox(): XHalfLength is non positive! "); }
  if( 0 >= yHalfLength() )
    { throw SolidException("SolidBox(): YHalfLength is non positive! "); }
  if( 0 >= zHalfLength() )
    { throw SolidException("SolidBox(): ZHalfLength is non positive! "); }
  /// set bounding paramters od SolidBase class
  setBP () ;
};
// ============================================================================

// ============================================================================
/** set parameters for bounding solids (box, sphere and cylinder)
 *  @return status code 
 */
// ============================================================================
StatusCode SolidBox::setBP() 
{
  /// set bounding paramters od SolidBase class
  setXMin   ( -xHalfLength() );
  setXMax   (  xHalfLength() );
  setYMin   ( -yHalfLength() );
  setYMax   (  yHalfLength() );
  setZMin   ( -zHalfLength() );
  setZMax   (  zHalfLength() );
  setRMax   ( sqrt( xMax() * xMax() + yMax() * yMax() + zMax() * zMax() ) );
  setRhoMax ( sqrt( xMax() * xMax() + yMax() * yMax()                   ) );
  //
  return checkBP () ;
};
// ============================================================================

// ============================================================================
/** default (protected) constructor 
 */
// ============================================================================
SolidBox::SolidBox()
  : SolidBase        ("anonymous box")
  , m_box_xHalfLength( 100000.0 ) 
  , m_box_yHalfLength( 100000.0 ) 
  , m_box_zHalfLength( 100000.0 ) 
{};
// ============================================================================

// ============================================================================
/// destructor 
// ============================================================================
SolidBox::~SolidBox(){ if ( this == m_cover ) { m_cover = 0 ; } ; };
// ============================================================================
/** - serialization for reading
 *  - implementation of ISerialize abstract interface 
 *  - reimplementation of SolidBase::serialize 
 *  @see ISerialize 
 *  @see ISolid  
 *  @see SolidBase   
 *  @param      s               reference to stream buffer
 *  @exception  SolidException  wrong parameters range 
 *  @return reference to stream buffer
 */
// ============================================================================
StreamBuffer& SolidBox::serialize( StreamBuffer& s )       
{
  /// 
  reset();
  // serialize the base class 
  SolidBase::serialize( s ) ;  
  // serialize own data 
  s >> m_box_xHalfLength 
    >> m_box_yHalfLength 
    >> m_box_zHalfLength ;
  ///
  if( 0 >= xHalfLength() )
    { throw SolidException("SolidBox: XHalfLength is non positive!"); }
  if( 0 >= yHalfLength() )
    { throw SolidException("SolidBox: YHalfLength is non positive!"); }
  if( 0 >= zHalfLength() )
    { throw SolidException("SolidBox: ZHalfLength is non positive!"); }
  // set bounding paramters od SolidBase class
  setBP () ;
  //
  return s ;
};
// ============================================================================

// ============================================================================
/** - serialization for writing
 *  - implementation of ISerialize abstract interface 
 *  - reimplementation of SolidBase::serialize 
 *  @see ISerialize 
 *  @see ISolid  
 *  @see SolidBase   
 *  @param s reference to stream buffer
 *  @return reference to stream buffer
 */
// ============================================================================
StreamBuffer& SolidBox::serialize( StreamBuffer& s ) const 
{
  /// serialize the base class 
  SolidBase::serialize( s ) ;
  /// serialize own data 
  return s << xHalfLength() 
           << yHalfLength() 
           << zHalfLength() ; 
};
// ============================================================================

// ============================================================================
/** calculate the intersection points("ticks") with a given line. 
 *  Input - line, paramterised by  x_vect = Point + Vector * T 
 *  "tick" is just a value of T, at which the intersection occurs
 *  @param Point initial point for the line
 *  @param Vector vector along the line
 *  @param ticks output container of "Ticks"
 *  @return the number of intersection points (=size of Ticks container)
 */
// ============================================================================
inline unsigned int 
SolidBox::intersectionTicks
( const Gaudi::XYZPoint & point  ,
  const Gaudi::XYZVector& vect   ,
  ISolid::Ticks    & ticks  ) const
{  ///
  ticks.clear(); 
  ///
  // find intersection ticks with x-planes
  SolidTicks::LineIntersectsTheX( point , vect ,        xHalfLength() , 
                                  std::back_inserter( ticks ) ); 
  SolidTicks::LineIntersectsTheX( point , vect , -1.0 * xHalfLength() , 
                                  std::back_inserter( ticks ) ); 
  // find intersection ticks with y-planes
  SolidTicks::LineIntersectsTheY( point , vect ,        yHalfLength() , 
                                  std::back_inserter( ticks ) ); 
  SolidTicks::LineIntersectsTheY( point , vect , -1.0 * yHalfLength() , 
                                  std::back_inserter( ticks ) ); 
  // find intersection ticks with z-planes
  SolidTicks::LineIntersectsTheZ( point , vect ,        zHalfLength() , 
                                  std::back_inserter( ticks ) ); 
  SolidTicks::LineIntersectsTheZ( point , vect , -1.0 * zHalfLength() , 
                                  std::back_inserter( ticks ) ); 
  // sort and remove adjancent and some EXTRA ticks and return 
  return SolidTicks::RemoveAdjancentTicks( ticks , point , vect , *this );  
};
// ============================================================================


// ============================================================================
 /** calculate the intersection points("ticks") of the solid objects 
   *  with given line. 
   *  - Line is parametrized with parameter \a t : 
   *     \f$ \vec{x}(t) = \vec{p} + t \times \vec{v} \f$ 
   *      - \f$ \vec{p} \f$ is a point on the line 
   *      - \f$ \vec{v} \f$ is a vector along the line  
   *  - \a tick is just a value of parameter \a t, at which the
   *    intersection of the solid and the line occurs
   *  - both  \a Point  (\f$\vec{p}\f$) and \a Vector  
   *    (\f$\vec{v}\f$) are defined in local reference system 
   *   of the solid 
   *  Only intersection ticks within the range 
   *   \a tickMin and \a tickMax are taken into account.
   *  @see ISolid::intersectionTicks()
   *  @param Point initial point for the line
   *  @param Vector vector along the line
   *  @param tickMin minimum value of Tick 
   *  @param tickMax maximu value of Tick 
   *  @param ticks output container of "Ticks"
   *  @return the number of intersection points
   */
// ============================================================================
unsigned int
SolidBox::intersectionTicks 
( const Gaudi::XYZPoint & Point   ,
  const Gaudi::XYZVector& Vector  ,
  const Tick       & tickMin ,
  const Tick       & tickMax ,
  Ticks            & ticks   ) const  
{
  if( isOutBBox( Point , Vector  , tickMin , tickMax  ) ) { return 0 ; }
  if( !crossBSphere( Point , Vector )                   ) { return 0 ; }
  
  //
  return SolidBase::intersectionTicks ( Point   , 
                                        Vector  ,
                                        tickMin , 
                                        tickMax ,
                                        ticks   );
};
// ============================================================================

// ============================================================================
/** printout to STD/STL stream    
 *  @param os STD/STL stream
 *  @return reference to the stream 
 */ 
// ============================================================================
std::ostream& SolidBox::printOut  ( std::ostream&  os ) const
{
  /// serialize the base class 
  SolidBase::printOut( os );
  return
    os << "[" 
       << " xsize[mm]=" << DetDesc::print( xsize() / mm ) 
       << " ysize[mm]=" << DetDesc::print( ysize() / mm ) 
       << " zsize[mm]=" << DetDesc::print( zsize() / mm )  
       << "]" << std::endl ;
};
// ============================================================================

// ============================================================================
/** printout to Gaudi  stream    
 *  @param os Gaudi stream
 *  @return reference to the stream 
 */ 
// ============================================================================
MsgStream&    SolidBox::printOut  ( MsgStream&     os ) const
{
  /// serialize the base class 
  SolidBase::printOut( os );
  return 
    os << "[" 
       << " xsize[mm]=" << DetDesc::print( xsize() / mm ) 
       << " ysize[mm]=" << DetDesc::print( ysize() / mm ) 
       << " zsize[mm]=" << DetDesc::print( zsize() / mm )  
       << "]" << endreq ;
};
// ============================================================================

// ============================================================================
/** - retrieve the pointer to "simplified" solid - "cover"
 *  - implementation of ISolid abstract interface 
 *  The Box is the most simple shape
 *  @see ISolid 
 *  @return pointer to "simplified" solid - "cover"
 */
// ============================================================================
const ISolid* SolidBox::cover() const { return this; };

// ============================================================================
/** - retrieve the pointer to "the most simplified cover" 
 *  - implementation  of ISolid abstract interface 
 *  - reimplementation of SolidBase::coverTop() 
 *  The Box is the most simple shape
 *  @see ISolid 
 *  @see SolidBase 
 *  @return pointer to the most simplified cover 
 */
// ============================================================================
const ISolid* SolidBox::coverTop () const { return this; };


// ============================================================================
// The END 
// ============================================================================
