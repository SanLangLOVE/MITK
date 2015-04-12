/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/


#ifndef mitkAbstractClassifier_h
#define mitkAbstractClassifier_h

#include <MitkClassificationCoreExports.h>

#include <mitkBaseData.h>

// Eigen
#include <Eigen/Dense>

// STD Includes

// MITK includes
#include <mitkConfigurationHolder.h>

namespace mitk
{
class MITKCLASSIFICATIONCORE_EXPORT AbstractClassifier : public BaseData
{
public:
  ///
  /// @brief MatrixType
  ///
  typedef Eigen::MatrixXd MatrixType;

  ///
  /// @brief VectorType
  ///
//  typedef Eigen::VectorXd VectorType;

  ///
  /// @brief Build a forest of trees from the training set (X, y).
  /// @param X, The training input samples. Matrix of shape = [n_samples, n_features]
  /// @param Y, The target values (class labels in classification, real numbers in regression). Array of shape = [n_samples]
  ///
  virtual void Train(const MatrixType &X, const MatrixType &Y) = 0;

  ///
  /// @brief Predict class for X.
  /// @param X, The input samples.
  /// @return The predicted classes. Y array of shape = [n_samples]
  ///
  virtual MatrixType Predict(const MatrixType &X) = 0;


  // * --------------- *
  // PointWiseWeight
  // * --------------- *

  ///
  /// @brief SupportsPointWiseWeight
  /// @return True if the classifier supports pointwise weighting else false
  ///
  virtual bool SupportsPointWiseWeight() = 0;

  ///
  /// @brief GetPointWiseWeightCopy
  /// @return Create and return a copy of W
  ///
  MatrixType & GetPointWiseWeight()
  {
    return m_PointWiseWeight;
  }

  ///
  /// @brief SetPointWiseWeight
  /// @param W, The pointwise weights. W array of shape = [n_samples]
  ///
  void SetPointWiseWeight(const MatrixType& W)
  {
    this->m_PointWiseWeight = W;
  }

  ///
  /// @brief UsePointWiseWeight
  /// @param toggle weighting on/off
  ///
  void UsePointWiseWeight(bool value)
  {
    this->m_IsUsingPointWiseWeight = value;
  }

  ///
  /// @brief IsUsingPointWiseWeight
  /// @return true if pointewise weighting is enabled.
  ///
  bool IsUsingPointWiseWeight()
  {
    return this->m_IsUsingPointWiseWeight;
  }

protected:
  MatrixType m_PointWiseWeight;
  bool m_IsUsingPointWiseWeight;

  // * --------------- *
  // PointWiseProbabilities
  // * --------------- *

public:
  ///
  /// @brief SupportsPointWiseProbability
  /// @return True if the classifier supports pointwise class probability calculation else false
  ///
  virtual bool SupportsPointWiseProbability() = 0;

  ///
  /// @brief GetPointWiseWeightCopy
  /// @return Create and return probability matrix
  ///
  MatrixType & GetPointWiseProbabilities()
  {
    return m_OutProbability;
  }

  ///
  /// \brief UsePointWiseProbabilities
  /// \param value
  ///
  void UsePointWiseProbability(bool value)
  {
    m_IsUsingPointWiseProbability = value;
  }

  ///
  /// \brief IsUsingPointWiseProbabilities
  /// \return
  ///
  bool IsUsingPointWiseProbability()
  {
    return m_IsUsingPointWiseProbability;
  }

protected:
  MatrixType m_OutProbability;
  MatrixType m_OutLabel;
  bool m_IsUsingPointWiseProbability;

public:

#ifndef DOXYGEN_SKIP

  virtual void SetRequestedRegionToLargestPossibleRegion(){}
  virtual bool RequestedRegionIsOutsideOfTheBufferedRegion(){return true;}
  virtual bool VerifyRequestedRegion(){return false;}
  virtual void SetRequestedRegion(const itk::DataObject */*data*/){}

#endif // Skip Doxygen

};
}

#endif //mitkAbstractClassifier_h
