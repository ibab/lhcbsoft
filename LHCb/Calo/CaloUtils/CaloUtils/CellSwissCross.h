// $Id: CellSwissCross.h,v 1.1 2001-11-07 17:24:48 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ 
// ============================================================================
// $Log: not supported by cvs2svn $ 
// ============================================================================
#ifndef CALOUTILS_CELLSWISSCROSS_H 
#define CALOUTILS_CELLSWISSCROSS_H 1
// Include files
// CaloUtils
#include "CaloUtils/CellMatrix.h"


/** @class CellSwissCross CellSwissCross.h CaloUtils/CellSwissCross.h
 *  
 *  The simple utility class to determine if 2 cells belongs 
 *  to the "swiss-cross" cluster 
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   07/11/2001
 */

class CellSwissCross : public CellMatrix
{

public:

  /** Standard constructor
   *  @param Det pointer to calorimeter detector 
   */
  CellSwissCross( const DeCalorimeter* Det )
    : CellMatrix( Det )
  {};
  
  virtual ~CellSwissCross(); ///< Destructor
  
  /** the major method. It returns "true" if the cell 
   *  belongs to "swiss-cross" sub-matrix around the seed 
   *  @param seed  the seed   cell
   *  @param cell  the cell to be tested
   *  @return fraction of area of the second cell which fits inside 
   *          "swiss-cross" area around the seed 
   */
  inline double operator() ( const CaloCellID& seed , 
                             const CaloCellID& cell ) const 
  { 
    /// the same cells
    if      ( seed             == cell             ) { return 1 ; } ///< RETURN
    /// the same calorimeter?
    if      ( seed.calo ()     != cell.calo ()     ) { return 0 ; } ///< RETURN 
    /// the same area ?
    if      ( seed.area ()     != cell.area ()     )  
      { return treatDifferentAreas( seed , cell ); }                ///< RETURN
    /// regular case 
    if      ( seed.col  ()     == cell.col  ()     ) 
      {
        if( cell.row  ()     >  seed.row  () + 1   ) { return 0 ; } ///< RETURN
        if( cell.row  () + 1 <  seed.row  ()       ) { return 0 ; } ///< RETURN
        /// ok
        return 1 ;                                                  ///< RETURN
      }
    else if ( seed.row  ()     == cell.row  ()     ) 
      {    
        if( cell.col  ()     >  seed.col  () + 1   ) { return 0 ; } ///< RETURN 
        if( cell.col  () + 1 <  seed.col  ()       ) { return 0 ; } ///< RETURN
        /// ok 
        return 1 ;                                                  ///< RETURN
      }
    ///
    return 0 ;
    
  };

protected:
  
  /** if cells belong to diffeent areas
   *  one need to treat them in a different way
   *  @param cell  the cell to be tested
   *  @return fraction of area of the second cell which fits inside 
   *          3x3 matrix aroud the seed cell
   */
  double treatDifferentAreas( const CaloCellID& seed ,
                              const CaloCellID& cell ) const ;
  
};

// ============================================================================
// The End 
// ============================================================================
#endif // CALOUTILS_CELLSWISSCROSS_H
// ============================================================================
