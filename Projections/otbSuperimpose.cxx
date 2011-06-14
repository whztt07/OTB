/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/
#include "otbSuperimpose.h"

#include <iostream>

#include "otbImageFileReader.h"
#include "otbStreamingImageFileWriter.h"
#include "otbImage.h"
#include "otbVectorImage.h"
#include "otbGenericRSResampleImageFilter.h"
#include "otbBCOInterpolateImageFunction.h"
#include "itkExceptionObject.h"
#include "otbStandardWriterWatcher.h"

namespace otb
{

int Superimpose::Describe(ApplicationDescriptor* descriptor)
{
  descriptor->SetName("Superimpose");
  descriptor->SetDescription("Using available image metadata, project one image onto another one");
  descriptor->AddOption("DEMDirectory","Directory were to find the DEM tiles","dem", 1, false, otb::ApplicationDescriptor::DirectoryName);
  descriptor->AddOption("NumStreamDivisions","Number of streaming divisions (optional)","stream", 1, false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOption("LocMapSpacing","Generate a coarser deformation field with the given spacing.","lmSpacing", 1, false, otb::ApplicationDescriptor::Real);
  descriptor->AddOption("ReferenceInput","The reference input","inR", 1, true, otb::ApplicationDescriptor::InputImage);
  descriptor->AddOption("MovingInput","The image to reproject","inM", 1, true, otb::ApplicationDescriptor::InputImage);
  descriptor->AddOption("AvailableMemory","Set the maximum of available memory for the pipeline execution in mega bytes (optional, 256 by default)","ram", 1, false, otb::ApplicationDescriptor::Integer);
  descriptor->AddOutputImage();

  return EXIT_SUCCESS;
}

int Superimpose::Execute(otb::ApplicationOptionsResult* parseResult)
{
  try
    {

    typedef unsigned short int PixelType;

    typedef otb::VectorImage<PixelType, 2>                ImageType;
    typedef otb::ImageFileReader<ImageType>               ReaderType;
    typedef otb::StreamingImageFileWriter<ImageType>      WriterType;
    typedef otb::BCOInterpolateImageFunction<ImageType>   InterpolatorType;

    typedef otb::GenericRSResampleImageFilter<ImageType, ImageType>  ResamplerType;

    // Read input images information
    ReaderType::Pointer refReader = ReaderType::New();
    refReader->SetFileName(parseResult->GetParameterString("ReferenceInput"));
    refReader->GenerateOutputInformation();

    ReaderType::Pointer movingReader = ReaderType::New();
    movingReader->SetFileName(parseResult->GetParameterString("MovingInput"));
    movingReader->GenerateOutputInformation();
    
    // Resample filter
    ResamplerType::Pointer    resampler = ResamplerType::New();
    InterpolatorType::Pointer interpolator = InterpolatorType::New();
    resampler->SetInterpolator(interpolator);
    
    // Configure DEM directory
    if(parseResult->IsOptionPresent("DEMDirectory"))
      {
      resampler->SetDEMDirectory(parseResult->GetParameterString("DEMDirectory", 0));
      }
    else
      {
      if ( otb::ConfigurationFile::GetInstance()->IsValid() )
        {
        resampler->SetDEMDirectory(otb::ConfigurationFile::GetInstance()->GetDEMDirectory());
        }
      }
    
    // Set up output image informations
    ImageType::SpacingType spacing = refReader->GetOutput()->GetSpacing();
    ImageType::IndexType start = refReader->GetOutput()->GetLargestPossibleRegion().GetIndex();
    ImageType::SizeType size = refReader->GetOutput()->GetLargestPossibleRegion().GetSize();
    ImageType::PointType origin = refReader->GetOutput()->GetOrigin();

    if(parseResult->IsOptionPresent("LocMapSpacing"))
      {
      double defScalarSpacing = parseResult->GetParameterFloat("LocMapSpacing");
      std::cout<<"Generating coarse deformation field (spacing="<<defScalarSpacing<<")"<<std::endl;
      ImageType::SpacingType defSpacing;

      defSpacing[0] = defScalarSpacing;
      defSpacing[1] = defScalarSpacing;
      
      resampler->SetDeformationFieldSpacing(defSpacing);
      }
    else
      {
      ImageType::SpacingType defSpacing;
      defSpacing[0]=10*spacing[0];
      defSpacing[1]=10*spacing[1];
      resampler->SetDeformationFieldSpacing(defSpacing);
      }
    
    ImageType::PixelType defaultValue;
    itk::PixelBuilder<ImageType::PixelType>::Zero(defaultValue,
                                                  movingReader->GetOutput()->GetNumberOfComponentsPerPixel());

    resampler->SetInput(movingReader->GetOutput());
    resampler->SetOutputOrigin(origin);
    resampler->SetOutputSpacing(spacing);
    resampler->SetOutputSize(size);
    resampler->SetOutputStartIndex(start);
    resampler->SetOutputKeywordList(refReader->GetOutput()->GetImageKeywordlist());
    resampler->SetOutputProjectionRef(refReader->GetOutput()->GetProjectionRef());
    resampler->SetEdgePaddingValue(defaultValue);
    
    WriterType::Pointer writer = WriterType::New();
    writer->SetFileName(parseResult->GetOutputImage());
    writer->SetInput(resampler->GetOutput());
    writer->SetWriteGeomFile(true);

    unsigned int ram = 256;
    if (parseResult->IsOptionPresent("AvailableMemory"))
      {
      ram = parseResult->GetParameterUInt("AvailableMemory");
      }
    writer->SetAutomaticTiledStreaming(ram);

    otb::StandardWriterWatcher w4(writer, resampler,"Superimposition");
    writer->Update();
    }
  catch ( itk::ExceptionObject & err )
    {
    std::cout << "Exception itk::ExceptionObject raised !" << std::endl;
    std::cout << err << std::endl;
    return EXIT_FAILURE;
    }
  catch ( std::bad_alloc & err )
    {
    std::cout << "Exception bad_alloc : "<<(char*)err.what()<< std::endl;
    return EXIT_FAILURE;
    }
  catch ( ... )
    {
    std::cout << "Unknown exception raised !" << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;

}

}
