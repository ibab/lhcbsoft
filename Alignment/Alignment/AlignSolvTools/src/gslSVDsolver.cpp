// $Id: gslSVDsolver.cpp,v 1.1 2007-10-26 12:00:21 graven Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 

// local
#include "gslSVDsolver.h"

//-----------------------------------------------------------------------------
// Implementation file for class : gslSVDsolver
//
// 2007-07-24 : Jan Amoraal
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_TOOL_FACTORY( gslSVDsolver );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
gslSVDsolver::gslSVDsolver( const std::string& type,
                            const std::string& name,
                            const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<IAlignSolvTool>(this);
  declareProperty("SVDJacobi" ,           m_svdJacobi  = false);
  declareProperty("SVDEpsilon",           m_svdEpsilon = 0.0  );
  declareProperty("SVDSetSmallestToZero", m_nZero      = 0    );
}
//=============================================================================
// Destructor
//=============================================================================
gslSVDsolver::~gslSVDsolver() {} 


bool gslSVDsolver::compute(AlSymMat& symMatrix, AlVec& vector) const {
  
  size_t size = symMatrix.size();

  debug() << "Size of AlSymMat A = " << size << endmsg;
  /// Allocate matrix A
  gsl_matrix* matrixA = gsl_matrix_alloc(size, size);
  debug() << "Size of gsl_matrix A = " << matrixA->size1 << endmsg;
  /// Fill matrix A
  for (unsigned(i) = 0; i < size; ++i) {
    for (unsigned(j) = 0; j < size; ++j) {
      debug() << "Element (i,j) of AlSymMat A = " << symMatrix[i][j] << endmsg;
      gsl_matrix_set(matrixA, i, j, symMatrix[i][j]);
      debug() << "Element (i,j) of gsl_matrix A = " << gsl_matrix_get(matrixA, i, j) << endmsg;
    }
  }

  /// Allocate required matrix, vector and workspace
  gsl_matrix* matrixV = gsl_matrix_alloc(size, size);
  gsl_vector* vectorS = gsl_vector_alloc(size);
  gsl_vector* vectorW = gsl_vector_alloc(size);

  info() << "==> Matrix A  = " << (*matrixA) << endmsg;
 
  debug() << "Factorising matrix A" << endmsg;
  /// Factorise A into the SVD A = USV^T. Note matrix A is replaced with matrix U.
  /// GSL returns 0 if successful
  int factorised = 1;
  if (!m_svdJacobi) {
    info() << "==> SVD standard" << endmsg;
    factorised = gsl_linalg_SV_decomp(matrixA, matrixV, vectorS, vectorW);
  } else {
    info() << "==> SVD Jacobi" << endmsg;
    factorised = gsl_linalg_SV_decomp_jacobi(matrixA, matrixV, vectorS);
  }
  if (factorised != 0) {
    error() << "==> Couldn't factorise  matrix" << endmsg;
    return false;
  }
  debug() << "Done factorising matrix A" << endmsg;

  info() << "==> Matrix U  = " << (*matrixA) << endmsg;
  info() << "==> Vector S  = " << (*vectorS) << endmsg;
  info() << "==> Matrix V  = " << (*matrixV) << endmsg;

  /// Regularise by zeroing singular values below threshold
  if (m_svdEpsilon > 0) {
    /// Threshold is epsilon times max singular value
    double threshold = m_svdEpsilon * (*gsl_vector_const_ptr(vectorS, 0));
    for (unsigned(i) = 0; i < size; ++i) {
      double* s = gsl_vector_ptr(vectorS, i);
      if ((*s) < threshold) (*s) = 0; 
    }
  }
  if (m_nZero > 0) { /// regularise by removing the m_nzero smallest eigenvalues
    for (unsigned(i) = 0; i < m_nZero; ++i) {
       (*gsl_vector_ptr(vectorS, size-1-i)) = 0;
    }
  }

  info() << "==> Regularised Vector S  = " << (*vectorS) << endmsg;

  gsl_vector* vectorB = gsl_vector_alloc(size);
  for (unsigned(i) = 0; i < size; ++i) gsl_vector_set(vectorB, i, vector[i]);
  gsl_vector* vectorX = gsl_vector_alloc(size);
  /// Solve Ax = b
  int solved = gsl_linalg_SV_solve(matrixA, matrixV, vectorS, vectorB, vectorX);
  if (solved != 0) {
    error() << "==> Couldn't solve system Ax=b" << endmsg;
    return false;
  }

  /// Fill AlVec
  for (unsigned(i) = 0; i < size; ++i) vector[i] = (*gsl_vector_const_ptr(vectorX, i));

  info() << "==> Vector x = " << (*vectorX) << endmsg;
  
  /// free gsl vector and matrices
  gsl_matrix_free(matrixA);
  gsl_matrix_free(matrixV);
  gsl_vector_free(vectorS);
  gsl_vector_free(vectorW);
  gsl_vector_free(vectorB);
  gsl_vector_free(vectorX);

  return true;
}

  
//=============================================================================
