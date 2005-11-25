
//-----------------------------------------------------------------------------
/** @file RichSmartID.cpp
 *
 *  Implementation file for RICH Channel ID class : RichSmartID
 *
 *  $Id: RichSmartID.cpp,v 1.6 2005-11-25 16:15:48 cattanem Exp $
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2005-01-06
 */
//-----------------------------------------------------------------------------

// Gaudi
#include "GaudiKernel/GaudiException.h"

// boost
#include "boost/lexical_cast.hpp"

// local
#include "Kernel/RichSmartID.h"

std::ostream& LHCb::RichSmartID::fillStream(std::ostream& s) const
{

  // Is this smart ID valid
  if ( isValid() ) {

    // If so, print out only the explicitly set fields
    s << "{";
    if ( richIsSet()     ) s << " "       << rich();
    if ( panelIsSet()    ) s << " Panel " << static_cast<unsigned int>(panel());
    if ( pdColIsSet()    ) s << " PDCol"  << format("%3i",pdCol());
    if ( pdRowIsSet()    ) s << " PDRow"  << format("%3i",pdRow());
    if ( pixelColIsSet() ) s << " pixCol" << format("%3i",pixelCol());
    if ( pixelRowIsSet() ) s << " pixRow" << format("%3i",pixelRow());
    if ( subPixelIsSet() ) s << " subPix" << format("%2i",subPixel());
    s << " }";

  } else {

    // This SmartID has no valid bits set. This is bad...
    s << "WARNING : Invalid RichSmartID";

  }

  return s;
}

void LHCb::RichSmartID::rangeError(const int value,
                                   const int max,
                                   const std::string& message) const
{
  throw GaudiException ( message+" value "+boost::lexical_cast<std::string>(value)
                         +" exceeds field maximum "+boost::lexical_cast<std::string>(max),
                         "*RichSmartID*", StatusCode::FAILURE );
}
