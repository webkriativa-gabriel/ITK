#ifndef __itkQuadEdgeMeshDiscreteMinCurvatureEstimator_h
#define __itkQuadEdgeMeshDiscreteMinCurvatureEstimator_h

#include "itkQuadEdgeMeshDiscretePrincipalCurvaturesEstimator.h"

namespace itk
{
/**
 * \class QuadEdgeMeshDiscreteMinCurvatureEstimator
 * \brief
*/
template< class TInputMesh, class TOutputMesh >
class QuadEdgeMeshDiscreteMinCurvatureEstimator :
  public QuadEdgeMeshDiscretePrincipalCurvaturesEstimator< TInputMesh, TOutputMesh >
{
public:
  typedef QuadEdgeMeshDiscreteMinCurvatureEstimator Self;
  typedef SmartPointer< Self > Pointer;
  typedef SmartPointer< const Self > ConstPointer;
  typedef QuadEdgeMeshDiscretePrincipalCurvaturesEstimator< TInputMesh, TOutputMesh > 
    Superclass;
    
  typedef typename Superclass::InputMeshType    InputMeshType;
  typedef typename Superclass::InputMeshPointer InputMeshPointer;

  typedef typename Superclass::OutputMeshType   OutputMeshType;
  typedef typename Superclass::OutputMeshPointer OutputMeshPointer;
  typedef typename Superclass::OutputPointsContainerPointer 
    OutputPointsContainerPointer;
  typedef typename Superclass::OutputPointsContainerIterator
    OutputPointsContainerIterator;
  typedef typename Superclass::OutputPointType OutputPointType;
  typedef typename Superclass::OutputVectorType OutputVectorType;
  typedef typename Superclass::OutputCoordType OutputCoordType;
  typedef typename Superclass::OutputPointIdentifier OutputPointIdentifier;
  typedef typename Superclass::OutputCellIdentifier OutputCellIdentifier;
  typedef typename Superclass::OutputQEType OutputQEType;
  typedef typename Superclass::OutputMeshTraits OutputMeshTraits;
  typedef typename Superclass::OutputCurvatureType OutputCurvatureType;
  
  typedef typename Superclass::TriangleType TriangleType;
  
  /** Run-time type information (and related methods).   */
  itkTypeMacro( QuadEdgeMeshDiscreteMinCurvatureEstimator,
    QuadEdgeMeshDiscretePrincipalCurvaturesEstimator );
  /** New macro for creation of through a Smart Pointer   */
  itkNewMacro( Self );

protected:
  QuadEdgeMeshDiscreteMinCurvatureEstimator() : Superclass() {}
  ~QuadEdgeMeshDiscreteMinCurvatureEstimator() {}

  virtual OutputCurvatureType EstimateCurvature( const OutputPointType& iP )
    {
    this->ComputeMeanAndGaussianCurvatures( iP );
    return this->m_Mean - vcl_sqrt( this->ComputeDelta() );
    }
    
private:
  QuadEdgeMeshDiscreteMinCurvatureEstimator( const Self& );
  void operator = ( const Self& );
};
}
#endif