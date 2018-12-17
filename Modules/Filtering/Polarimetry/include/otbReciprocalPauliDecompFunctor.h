/*
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef otbReciprocalPauliDecompFunctor_h
#define otbReciprocalPauliDecompFunctor_h

namespace otb
 {

namespace Functor {

/** \class ReciprocalPauliDecompFunctor
 *
 * \brief Evaluate the Pauli decomposition from the reciprocal Sinclair matrix image.
 *
 * \ingroup OTBPolarimetry
 */
template< class TInput, class TOutput>
class ReciprocalPauliDecompFunctor
{
public:

  typedef typename TInput::ValueType   InputValueType;
  typedef typename TOutput::ValueType  OutputValueType;


  inline void operator()( TOutput & result, const TInput & Sinclair ) const
    {
    InputValueType sqrt2(std::sqrt(2.0),0.0);

    InputValueType Shh = static_cast<InputValueType>(Sinclair[0]);
    InputValueType Shv = static_cast<InputValueType>(Sinclair[1]);
    InputValueType Svv = static_cast<InputValueType>(Sinclair[2]);

    result[0] = (Shh+Svv)/sqrt2;
    result[1] = (Shh-Svv)/sqrt2;
    result[2] = sqrt2*Shv;
    }

   constexpr size_t OutputSize(...) const
  {
    // Size of the result
    return 3;
  }

   /** Constructor */
   ReciprocalPauliDecompFunctor() {}

   /** Destructor */
   virtual ~ReciprocalPauliDecompFunctor() {}

private:
   static constexpr double m_Epsilon = 1e-6;
};
} // end namespace functor
} // end namespace otb

#endif