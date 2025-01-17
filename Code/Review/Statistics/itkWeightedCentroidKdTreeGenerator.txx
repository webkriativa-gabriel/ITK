/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    itkWeightedCentroidKdTreeGenerator.txx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWeightedCentroidKdTreeGenerator_txx
#define __itkWeightedCentroidKdTreeGenerator_txx

namespace itk {
namespace Statistics {

template< class TSample >
WeightedCentroidKdTreeGenerator< TSample >
::WeightedCentroidKdTreeGenerator()
{
}

template< class TSample >
void
WeightedCentroidKdTreeGenerator< TSample >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template< class TSample >
inline typename WeightedCentroidKdTreeGenerator< TSample >::KdTreeNodeType*
WeightedCentroidKdTreeGenerator< TSample >
::GenerateNonterminalNode(unsigned int beginIndex,
                          unsigned int endIndex,
                          MeasurementVectorType &lowerBound,
                          MeasurementVectorType &upperBound,
                          unsigned int level)
{
  MeasurementType dimensionLowerBound;
  MeasurementType dimensionUpperBound;
  MeasurementType partitionValue;
  unsigned int partitionDimension = 0;
  unsigned int i;
  unsigned int j;
  MeasurementType spread;
  MeasurementType maxSpread;
  unsigned int medianIndex;

  SubsamplePointer subsample = this->GetSubsample();

  // Sanity check. Verify that the subsample has measurement vectors of the
  // same length as the sample generated by the tree.
  if( this->GetMeasurementVectorSize() != subsample->GetMeasurementVectorSize() )
    {
    itkExceptionMacro( << "Measurement Vector Length mismatch" );
    }

  // calculates the weighted centroid which is the vector sum
  // of all the associated instances.
  typename KdTreeNodeType::CentroidType weightedCentroid;
  MeasurementVectorTraits::SetLength( weightedCentroid, this->GetMeasurementVectorSize() );
  MeasurementVectorType tempVector;
  weightedCentroid.Fill(NumericTraits< MeasurementType >::Zero);

  for (i = beginIndex; i < endIndex; i++)
    {
    tempVector = subsample->GetMeasurementVectorByIndex(i);
    for(j = 0; j < this->GetMeasurementVectorSize(); j++)
      {
      weightedCentroid[j] += tempVector[j];
      }
    }

  // find most widely spread dimension
  Algorithm::FindSampleBoundAndMean< SubsampleType >(this->GetSubsample(),
                                          beginIndex, endIndex,
                                          m_TempLowerBound, m_TempUpperBound,
                                          m_TempMean);

  maxSpread = NumericTraits< MeasurementType >::NonpositiveMin();
  for (i = 0; i < this->GetMeasurementVectorSize(); i++)
    {
    spread = m_TempUpperBound[i] - m_TempLowerBound[i];
    if (spread >= maxSpread)
      {
      maxSpread = spread;
      partitionDimension = i;
      }
    }


  medianIndex = (endIndex - beginIndex) / 2;

  //
  // Find the medial element by using the NthElement function
  // based on the STL implementation of the QuickSelect algorithm.
  //
  partitionValue =
    Algorithm::NthElement< SubsampleType >(this->GetSubsample(),
                                 partitionDimension,
                                 beginIndex, endIndex, 
                                 medianIndex);
             
  medianIndex += beginIndex;

  // save bounds for cutting dimension
  dimensionLowerBound = lowerBound[partitionDimension];
  dimensionUpperBound = upperBound[partitionDimension];

  upperBound[partitionDimension] = partitionValue;
  const unsigned int beginLeftIndex = beginIndex;
  const unsigned int endLeftIndex   = medianIndex;
  KdTreeNodeType* left = GenerateTreeLoop(beginLeftIndex, endLeftIndex, lowerBound, upperBound, level + 1);
  upperBound[partitionDimension] = dimensionUpperBound;

  lowerBound[partitionDimension] = partitionValue;
  const unsigned int beginRightIndex = medianIndex+1;
  const unsigned int endRighIndex    = endIndex;
  KdTreeNodeType* right = GenerateTreeLoop(beginRightIndex, endRighIndex, lowerBound, upperBound, level + 1);
  lowerBound[partitionDimension] = dimensionLowerBound;


  typedef KdTreeWeightedCentroidNonterminalNode< TSample >  KdTreeNonterminalNodeType;

  KdTreeNonterminalNodeType * nonTerminalNode =
    new KdTreeNonterminalNodeType( partitionDimension, 
                                   partitionValue,
                                   left, right,
                                   weightedCentroid,
                                   endIndex - beginIndex);

  nonTerminalNode->AddInstanceIdentifier(
    subsample->GetInstanceIdentifier( medianIndex ) );

  return nonTerminalNode;
}

} // end of namespace Statistics
} // end of namespace itk

#endif
