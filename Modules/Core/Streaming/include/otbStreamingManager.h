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

#ifndef otbStreamingManager_h
#define otbStreamingManager_h

#include "otbMacro.h"

#include "itkDataObject.h"
#include "itkImageRegionSplitterBase.h"
#include "otbPipelineMemoryPrintCalculator.h"
#include "OTBStreamingExport.h"

namespace otb
{

/** \class StreamingManager
 *  \brief This class handles the streaming process used in the writers implementation
 *
 *  The streaming mode can be chosen with either SetStrippedRAMStreamingMode, SetStrippedNumberOfLinesStreamingMode,
 *  SetTiledRAMStreamingMode, or SetTiledTileDimensionStreamingMode.
 *
 *  Then, PrepareStreaming must be called so that the stream type and dimensions are computed
 *  This involves passing the actual DataObject who will be written, since it will be used
 *  during memory estimation for some specific streaming modes.
 *
 *  After PrepareStreaming has been called, the actual number of splits and streaming mode which will be used
 *  can be retrieved with GetStreamingMode and GetNumberOfSplits.
 *  The different splits can be retrieved with GetSplit
 *
 * \sa ImageFileWriter
 * \sa StreamingImageVirtualFileWriter
 *
 * \ingroup OTBStreaming
 */
template<class TImage>
class OTBStreaming_EXPORT_TEMPLATE StreamingManager : public itk::Object
{
public:
  /** Standard class typedefs. */
  typedef StreamingManager              Self;
  typedef itk::LightObject              Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  typedef TImage                                ImageType;
  typedef typename ImageType::Pointer           ImagePointerType;
  typedef typename ImageType::RegionType        RegionType;
  typedef typename RegionType::IndexType        IndexType;
  typedef typename RegionType::SizeType         SizeType;
  typedef typename ImageType::InternalPixelType PixelType;

  typedef otb::PipelineMemoryPrintCalculator::MemoryPrintType MemoryPrintType;
  typedef itk::ImageRegionSplitterBase          AbstractSplitterType;

  /** Type macro */
  itkTypeMacro(StreamingManager, itk::LightObject);

  /** Dimension of input image. */
  itkStaticConstMacro(ImageDimension, unsigned int, ImageType::ImageDimension);

  const AbstractSplitterType * GetSplitter() const;

  /** Actually computes the stream divisions, according to the specified streaming mode,
   * eventually using the input parameter to estimate memory consumption */
  virtual void PrepareStreaming(itk::DataObject * input, const RegionType &region) = 0;

  /** Returns the actual number of pieces that will be used to process the image.
   * PrepareStreaming() must have been called before.
   * This can be different than the requested number */
  virtual unsigned int GetNumberOfSplits();

  /** Get a region definition that represents the ith piece a specified region.
   * The "numberOfPieces" must be equal to what
   * GetNumberOfSplits() returns. */
  virtual RegionType GetSplit(unsigned int i);

  itkSetMacro(DefaultRAM, MemoryPrintType);
  itkGetMacro(DefaultRAM, MemoryPrintType);

protected:
  StreamingManager();
  ~StreamingManager() override;

  virtual unsigned int EstimateOptimalNumberOfDivisions(itk::DataObject * input, const RegionType &region,
                                                        MemoryPrintType availableRAMInMB,
                                                        double bias = 1.0);

  /** The number of splits generated by the splitter */
  unsigned int m_ComputedNumberOfSplits;

  /** The region to stream */
  RegionType m_Region;

  /** The splitter used to compute the different strips */
  typedef typename AbstractSplitterType::Pointer AbstractSplitterPointerType;
  AbstractSplitterPointerType m_Splitter;

private:
  StreamingManager(const StreamingManager &) = delete;
  void operator =(const StreamingManager&) = delete;

  /** Compute the available RAM in Bytes from an input value in MByte.
   *  If the input value is 0, it uses the m_DefaultRAM value.
   *  If m_DefaultRAM is also 0, it uses the configuration settings */
  MemoryPrintType GetActualAvailableRAMInBytes(MemoryPrintType availableRAMInMB);

  /** Default available RAM in MB */
  MemoryPrintType m_DefaultRAM;
};

} // End namespace otb

#include "otbVectorImage.h"
#include "otbImage.h"

namespace otb {
extern template class OTBStreaming_EXPORT_TEMPLATE StreamingManager<VectorImage<float>>;
extern template class OTBStreaming_EXPORT_TEMPLATE StreamingManager<Image<float>>;
}

#ifndef OTB_MANUAL_INSTANTIATION
#include "otbStreamingManager.hxx"
#endif

#endif

