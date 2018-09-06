/*
 * Copyright (C) 2017 National Research Institute of Science and
 * Technology for Environment and Agriculture (IRSTEA)
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

#include "itkFixedArray.h"
#include "itkObjectFactory.h"

// Elevation handler
#include "otbWrapperElevationParametersHandler.h"
#include "otbWrapperApplicationFactory.h"

// Application engine
#include "otbStandardFilterWatcher.h"

// Process objects
#include "otbVectorDataToLabelImageFilter.h"
#include "otbVectorDataIntoImageProjectionFilter.h"
#include "otbStreamingStatisticsMapFromLabelImageFilter.h"
#include "otbStatisticsXMLFileWriter.h"

namespace otb
{

namespace Wrapper
{

class ZonalStatistics : public Application
{
public:
  /** Standard class typedefs. */
  typedef ZonalStatistics               Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /* Typedefs */
  typedef Int32ImageType                LabelImageType;
  typedef LabelImageType::ValueType     LabelValueType;
  typedef otb::VectorData<double, 2>    VectorDataType;
  typedef otb::VectorDataIntoImageProjectionFilter<VectorDataType,
      FloatVectorImageType>             VectorDataReprojFilterType;
  typedef otb::VectorDataToLabelImageFilter<VectorDataType,
      LabelImageType>                   RasterizeFilterType;
  typedef VectorDataType::DataTreeType  DataTreeType;
  typedef itk::PreOrderTreeIterator<DataTreeType>
                                        TreeIteratorType;
  typedef VectorDataType::DataNodeType  DataNodeType;
  typedef DataNodeType::PolygonListPointerType
                                        PolygonListPointerType;
  typedef otb::StreamingStatisticsMapFromLabelImageFilter<FloatVectorImageType,
      LabelImageType>                   StatsFilterType;
  typedef otb::StatisticsXMLFileWriter<FloatVectorImageType::PixelType>
                                        StatsWriterType;

  /** Standard macro */
  itkNewMacro(Self);
  itkTypeMacro(ZonalStatistics, Application);

  void DoInit()
  {

    SetName("ZonalStatistics");
    SetDescription("This application computes zonal statistics");

    // Documentation
    SetDocName("ZonalStatistics");
    SetDocLongDescription("This application computes zonal statistics from label image, or vector data. "
        "The application inputs one input multiband image, and a label input. "
        "If the label input is a raster, the output statistics are exported in a XML file. If the label "
        "input is a vector data, the output statistics are exported in a new vector data with statistics "
        "in the attribute table. The computed statistics are mean, min, max, and standard deviation.");
    SetDocLimitations("The shapefile must fit in memory");
    SetDocAuthors("Remi Cresson");

    AddDocTag(Tags::Manip);
    AddDocTag(Tags::Analysis);

    // Input image
    AddParameter(ParameterType_InputImage, "in",  "Input Image");

    // Processing mode
    AddParameter(ParameterType_Choice, "mode", "Processing mode");
    AddChoice("mode.vector",     "Input objects from vector data");
    AddChoice("mode.labelimage", "Input objects from label image");

    // Input for vector mode
    AddParameter(ParameterType_InputVectorData,  "mode.vector.in",        "Input vector data");
    AddParameter(ParameterType_Bool,             "mode.vector.reproject", "Reproject the input vector");
    AddParameter(ParameterType_OutputVectorData, "mode.vector.out",       "Output vector data");

    // Input for label image mode
    AddParameter(ParameterType_InputImage, "mode.labelimage.in",     "Input label image");
    AddParameter(ParameterType_Int,        "mode.labelimage.nodata", "No-data value for the input label image");
    SetDefaultParameterInt                ("mode.labelimage.nodata", 0);
    MandatoryOff                          ("mode.labelimage.nodata");
    AddParameter(ParameterType_InputImage, "mode.labelimage.outxml", "Output XML file");

    AddRAMParameter();

    // Doc example parameter settings
    SetDocExampleParameterValue("in", "input.tif");
    SetDocExampleParameterValue("mode.vector.in", "myvector.shp");
    SetDocExampleParameterValue("mode.vector.out", "myvector_with_stats.shp");


  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  // Returns a string of the kind "prefix_i"
  const std::string CreateFieldName(const std::string & prefix, const unsigned int i)
  {
    std::stringstream ss;
    ss << prefix << "_" << i;
    return ss.str();
  }

  // Returns a null pixel which has the same number of components per pixels as img
  FloatVectorImageType::PixelType NullPixel(FloatVectorImageType::Pointer & img)
  {
    const unsigned int nBands = img->GetNumberOfComponentsPerPixel();
    FloatVectorImageType::PixelType pix;
    pix.SetSize(nBands);
    pix.Fill(0);
    return pix;
  }

  void DoExecute()
  {

    // Get input image
    FloatVectorImageType::Pointer img = GetParameterImage("in");

    // Statistics filter
    m_StatsFilter = StatsFilterType::New();
    m_StatsFilter->SetInput(img);
    m_StatsFilter->GetStreamer()->SetAutomaticAdaptativeStreaming(GetParameterInt("ram"));
    AddProcess(m_StatsFilter->GetStreamer(), "Computing statistics");

    // Select mode
    if (GetParameterAsString("mode") == "vector")
      {
      otbAppLogINFO("Processing mode: vector");

      otbAppLogINFO("Loading vector data...");
      VectorDataType* shp = GetParameterVectorData("mode.vector.in");

      // Reproject vector data
      if (GetParameterInt("mode.vector.reproject") != 0)
        {
        otbAppLogINFO("Vector data reprojection enabled");
        m_VectorDataReprojectionFilter = VectorDataReprojFilterType::New();
        m_VectorDataReprojectionFilter->SetInputVectorData(shp);
        m_VectorDataReprojectionFilter->SetInputImage(img);
        AddProcess(m_VectorDataReprojectionFilter, "Reproject vector data");
        m_VectorDataReprojectionFilter->Update();
        m_VectorDataSrc = m_VectorDataReprojectionFilter->GetOutput();
        }
      else
        {
        m_VectorDataSrc = shp;
        }

      // Internal no-data value
      const LabelValueType intNoData = itk::NumericTraits<LabelValueType>::max();

      // Rasterize vector data
      m_RasterizeFilter = RasterizeFilterType::New();
      m_RasterizeFilter->AddVectorData(m_VectorDataSrc);
      m_RasterizeFilter->SetOutputOrigin(img->GetOrigin());
      m_RasterizeFilter->SetOutputSpacing(img->GetSignedSpacing());
      m_RasterizeFilter->SetOutputSize(img->GetLargestPossibleRegion().GetSize());
      m_RasterizeFilter->SetOutputProjectionRef(img->GetProjectionRef());
      m_RasterizeFilter->SetBurnAttribute("________");
      m_RasterizeFilter->SetDefaultBurnValue(intNoData);
      m_RasterizeFilter->SetGlobalWarningDisplay(false);
      m_RasterizeFilter->SetBackgroundValue(intNoData);

      // Computing stats
      m_StatsFilter->SetInputLabelImage(m_RasterizeFilter->GetOutput());
      m_StatsFilter->Update();

      // Remove the no-data entry
      StatsFilterType::LabelPopulationMapType countMap = m_StatsFilter->GetLabelPopulationMap();
      StatsFilterType::PixelValueMapType meanMap = m_StatsFilter->GetMeanValueMap();
      StatsFilterType::PixelValueMapType stdMap = m_StatsFilter->GetStandardDeviationValueMap();
      StatsFilterType::PixelValueMapType minMap = m_StatsFilter->GetMinValueMap();
      StatsFilterType::PixelValueMapType maxMap = m_StatsFilter->GetMaxValueMap();
      countMap.erase(intNoData);
      meanMap.erase(intNoData);
      stdMap.erase(intNoData);
      minMap.erase(intNoData);
      maxMap.erase(intNoData);

      // Add a statistics fields
      otbAppLogINFO("Writing output vector data");
      LabelValueType internalFID = 0;
      m_NewVectorData = VectorDataType::New();
      DataNodeType::Pointer root = m_NewVectorData->GetDataTree()->GetRoot()->Get();
      DataNodeType::Pointer document = DataNodeType::New();
      document->SetNodeType(otb::DOCUMENT);
      m_NewVectorData->GetDataTree()->Add(document, root);
      DataNodeType::Pointer folder = DataNodeType::New();
      folder->SetNodeType(otb::FOLDER);
      m_NewVectorData->GetDataTree()->Add(folder, document);
      m_NewVectorData->SetProjectionRef(m_VectorDataSrc->GetProjectionRef());
      TreeIteratorType itVector(m_VectorDataSrc->GetDataTree());
      itVector.GoToBegin();

      while (!itVector.IsAtEnd())
        {
        if (!itVector.Get()->IsRoot() && !itVector.Get()->IsDocument() && !itVector.Get()->IsFolder())
          {

          DataNodeType::Pointer currentGeometry = itVector.Get();

          // Add the geometry with the new fields
          if (countMap.count(internalFID) > 0)
            {
            currentGeometry->SetFieldAsDouble("count", countMap[internalFID] );
            for (unsigned int band = 0 ; band < img->GetNumberOfComponentsPerPixel() ; band++)
              {
              currentGeometry->SetFieldAsDouble(CreateFieldName("mean",  band), meanMap[internalFID][band] );
              currentGeometry->SetFieldAsDouble(CreateFieldName("stdev", band), stdMap [internalFID][band] );
              currentGeometry->SetFieldAsDouble(CreateFieldName("min",   band), minMap [internalFID][band] );
              currentGeometry->SetFieldAsDouble(CreateFieldName("max",   band), maxMap [internalFID][band] );
              }
            m_NewVectorData->GetDataTree()->Add(currentGeometry, folder);
            }

          internalFID++;
          }
        ++itVector;
        } // next feature

      SetParameterOutputVectorData("mode.vector.out", m_NewVectorData);
      }
    else if (GetParameterAsString("mode") == "labelimage")
      {
      otbAppLogINFO("Processing mode: label image");

      // Computing stats
      m_StatsFilter->SetInputLabelImage(GetParameterInt32Image("mode.labelimage.in"));
      m_StatsFilter->Update();

      // Remove the no-data entry
      StatsFilterType::LabelPopulationMapType countMap = m_StatsFilter->GetLabelPopulationMap();
      StatsFilterType::PixelValueMapType meanMap = m_StatsFilter->GetMeanValueMap();
      StatsFilterType::PixelValueMapType stdMap = m_StatsFilter->GetStandardDeviationValueMap();
      StatsFilterType::PixelValueMapType minMap = m_StatsFilter->GetMinValueMap();
      StatsFilterType::PixelValueMapType maxMap = m_StatsFilter->GetMaxValueMap();
      if (HasUserValue("mode.labelimage.nodata"))
        {
        // Internal no-data value
        const LabelValueType intNoData = GetParameterInt("mode.labelimage.nodata");

        otbAppLogINFO("Using no-data value: " << intNoData);

        countMap.erase(intNoData);
        meanMap.erase(intNoData);
        stdMap.erase(intNoData);
        minMap.erase(intNoData);
        maxMap.erase(intNoData);
        }

      // Write stats
      const std::string outXMLFile = this->GetParameterString("mode.labelimage.outxml");
      otbAppLogINFO("Writing " + outXMLFile)
      StatsWriterType::Pointer statWriter = StatsWriterType::New();
      statWriter->SetFileName(outXMLFile);
      statWriter->AddInputMap<StatsFilterType::LabelPopulationMapType>("count",countMap);
      statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("mean",meanMap);
      statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("std",stdMap);
      statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("min",minMap);
      statWriter->AddInputMap<StatsFilterType::PixelValueMapType>("max",maxMap);
      statWriter->Update();

      }
    else
      {
      otbAppLogFATAL("Unknow processing mode");
      }

  }

  VectorDataType::Pointer m_VectorDataSrc;
  VectorDataType::Pointer m_NewVectorData;
  VectorDataReprojFilterType::Pointer m_VectorDataReprojectionFilter;
  RasterizeFilterType::Pointer m_RasterizeFilter;
  StatsFilterType::Pointer m_StatsFilter;

};
}
}

OTB_APPLICATION_EXPORT( otb::Wrapper::ZonalStatistics )
