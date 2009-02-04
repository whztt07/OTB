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
#include "itkExceptionObject.h"


#include "otbEntropyTextureImageFunction.h"
#include "otbImage.h"


int otbEntropyTextureImageFunctionNew(int argc, char * argv[])
{
  //const char * inputFileName  = argv[1];
  //const char * outputFileName = argv[2];

  typedef double                               InputPixelType;
  const int Dimension = 2;
  typedef otb::Image<InputPixelType,Dimension> ImageType;
  typedef otb::EntropyTextureImageFunction<ImageType> EntropyTextureImageFunctionType;

  EntropyTextureImageFunctionType::Pointer entropy = EntropyTextureImageFunctionType::New();


  return EXIT_SUCCESS;
}
