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
#include "otbWrapperApplication.h"
#include "otbWrapperApplicationFactory.h"

#include "itkMeanImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "otbPerBandVectorImageFilter.h"

namespace otb
{
namespace Wrapper
{

enum
{
  Smoothing_Mean,
  Smoothing_Gaussian,
  Smoothing_Anisotropic
};

typedef otb::Image<FloatVectorImageType::InternalPixelType, 2>  ImageType;

class Smoothing : public Application
{
public:
  /** Standard class typedefs. */
  typedef Smoothing                      Self;
  typedef Application                   Superclass;
  typedef itk::SmartPointer<Self>       Pointer;
  typedef itk::SmartPointer<const Self> ConstPointer;

  /** Standard macro */
  itkNewMacro(Self);

  itkTypeMacro(Smoothing, otb::Application);

private:
  void DoInit()
  {
    SetName("Smoothing");
    SetDescription("Apply a smoothing filter to an image");

    SetDocName("Smoothing");
    SetDocLongDescription("This application applies smoothing filter to an image."
      " Either gaussian, mean, or anisotropic diffusion are available.");
    SetDocLimitations("None");
    SetDocAuthors("OTB-Team");
    SetDocSeeAlso(" ");

    AddDocTag(Tags::Filter);

    AddParameter(ParameterType_InputImage,  "in",   "Input Image");
    SetParameterDescription("in", "Input image to smooth.");
    AddParameter(ParameterType_OutputImage, "out",  "Output Image");
    SetParameterDescription("out", "Output smoothed image.");

    AddRAMParameter();

    AddParameter(ParameterType_Choice,      "type", "Smoothing Type");
    SetParameterDescription("type", "Smoothing kernel to apply");

    AddChoice("type.mean",     "Mean");

    AddParameter(ParameterType_Radius, "type.mean.radius", "Radius");
    SetParameterDescription("type.mean.radius", "Mean radius (in pixels)");
    SetDefaultParameterInt("type.mean.radius", 2);

    AddChoice("type.gaussian", "Gaussian");

    AddParameter(ParameterType_Float, "type.gaussian.radius", "Radius");
    SetParameterDescription("type.gaussian.radius", "Gaussian radius (in pixels)");
    SetDefaultParameterFloat("type.gaussian.radius", 2.0);

    AddChoice("type.anidif",   "Anisotropic Diffusion");

    AddParameter(ParameterType_Float,  "type.anidif.timestep", "Time Step");
    SetParameterDescription("type.anidif.timestep", "Diffusion equation time step");

    AddParameter(ParameterType_Int,  "type.anidif.nbiter", "Nb Iterations");
    SetParameterDescription("type.anidif.nbiter", "Number of iterations");

    AddParameter(ParameterType_Float,  "type.anidif.conductance", "Conductance");
    SetParameterDescription("type.anidif.nbiter", "Controls the sensitivity of the conductance term");

    SetDefaultParameterFloat("type.anidif.timestep",   0.125);
    SetDefaultParameterInt("type.anidif.nbiter",     10);
    SetDefaultParameterInt("type.anidif.conductance",     1.);

    SetParameterString("type", "anidif");

    // Doc example parameter settings
    SetExampleComment("Image smoothing using a mean filter.", 0);
    SetDocExampleParameterValue("in", "Romania_Extract.tif");
    SetDocExampleParameterValue("out", "smoothedImage_mean.png uchar");
    SetDocExampleParameterValue("type", "mean");

    unsigned int exId = AddExample( "Image smoothing using an anisotropic diffusion filter." );
    SetDocExampleParameterValue("in", "Romania_Extract.tif", exId);
    SetDocExampleParameterValue("out", "smoothedImage_ani.png float", exId);
    SetDocExampleParameterValue("type", "anidif", exId);
    SetDocExampleParameterValue("type.anidif.timestep", "0.1", exId);
    SetDocExampleParameterValue("type.anidif.nbiter", "5", exId);
    SetDocExampleParameterValue("type.anidif.conductance", "1.5", exId);
  }

  void DoUpdateParameters()
  {
    // Nothing to do here : all parameters are independent
  }

  void DoExecute()
  {
    GetLogger()->Debug("Entering DoExecute\n");


    FloatVectorImageType::Pointer inImage = GetParameterImage("in");

    switch ( GetParameterInt("type") )
      {
      case Smoothing_Mean:
        {
        GetLogger()->Debug("Using mean");

        typedef itk::MeanImageFilter<ImageType, ImageType>         MeanFilterType;
        typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, MeanFilterType>
          PerBandMeanFilterType;

        PerBandMeanFilterType::Pointer perBand
          = PerBandMeanFilterType::New();
        perBand->SetInput(inImage);

        MeanFilterType::InputSizeType radius;
        radius.Fill( GetParameterInt("type.mean.radius") );
        perBand->GetFilter()->SetRadius(radius);
        perBand->UpdateOutputInformation();
        m_FilterRef = perBand;
        SetParameterOutputImage("out", perBand->GetOutput());
        }
        break;
      case Smoothing_Gaussian:
        {
        GetLogger()->Debug("Using gaussian");

        typedef itk::DiscreteGaussianImageFilter<ImageType, ImageType>  DiscreteGaussianFilterType;
        typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, DiscreteGaussianFilterType>
          PerBandDiscreteGaussianFilterType;

        PerBandDiscreteGaussianFilterType::Pointer perBand
          = PerBandDiscreteGaussianFilterType::New();
        perBand->SetInput(inImage);

        double radius = GetParameterFloat("type.gaussian.radius");
        double variance = radius * radius;
        perBand->GetFilter()->SetVariance(variance);
        perBand->GetFilter()->SetUseImageSpacing(false);
        perBand->UpdateOutputInformation();
        m_FilterRef = perBand;
        SetParameterOutputImage("out", perBand->GetOutput());
        }
        break;
      case Smoothing_Anisotropic:
        {
        GetLogger()->Debug("Using anisotropic diffusion");

        typedef itk::GradientAnisotropicDiffusionImageFilter<ImageType, ImageType>  GradientAnisotropicDiffusionFilterType;
        typedef otb::PerBandVectorImageFilter<FloatVectorImageType, FloatVectorImageType, GradientAnisotropicDiffusionFilterType>
          PerBandGradientAnisotropicDiffusionFilterType;

        PerBandGradientAnisotropicDiffusionFilterType::Pointer perBand
          = PerBandGradientAnisotropicDiffusionFilterType::New();
        perBand->SetInput(inImage);

        const int aniDifNbIter = GetParameterInt("type.anidif.nbiter");
        perBand->GetFilter()->SetNumberOfIterations(static_cast<unsigned int>(aniDifNbIter));

        const float aniDifTimeStep = GetParameterFloat("type.anidif.timestep");
        perBand->GetFilter()->SetTimeStep(static_cast<double>(aniDifTimeStep));

        perBand->GetFilter()->SetConductanceParameter(GetParameterFloat("type.anidif.conductance"));
        perBand->GetFilter()->SetUseImageSpacing(false);
        perBand->UpdateOutputInformation();
        
        m_FilterRef = perBand;
        SetParameterOutputImage("out", perBand->GetOutput());
        }
        break;
      }
  }

  itk::LightObject::Pointer m_FilterRef;
};


}
}

OTB_APPLICATION_EXPORT(otb::Wrapper::Smoothing)

