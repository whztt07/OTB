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
#ifndef __otbImageLayer_txx
#define __otbImageLayer_txx

#include "itkImageRegionConstIterator.h"
#include "otbMacro.h"
#include "itkTimeProbe.h"
#include "otbStandardRenderingFunction.h"

namespace otb
{

template <class TImage, class TOutputImage>
ImageLayer<TImage,TOutputImage>
::ImageLayer() : m_Quicklook(), m_Image(), m_ListSample(), m_RenderingFunction(),
                 m_QuicklookRenderingFilter(), m_ExtractRenderingFilter(), m_ScaledExtractRenderingFilter(),
                 m_ExtractFilter(), m_ScaledExtractFilter()
{
 // Rendering filters
  m_QuicklookRenderingFilter = RenderingFilterType::New();
  m_ExtractRenderingFilter = RenderingFilterType::New();
  m_ScaledExtractRenderingFilter = RenderingFilterType::New();

  m_ListSample = ListSampleType::New();

  // Default rendering function
  typedef Function::StandardRenderingFunction<PixelType,
                                    typename TOutputImage::PixelType> DefaultRenderingFunctionType;
  m_RenderingFunction = DefaultRenderingFunctionType::New();
  m_QuicklookRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  m_ExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);
  m_ScaledExtractRenderingFilter->SetRenderingFunction(m_RenderingFunction);

  // Extract filters
  m_ExtractFilter = ExtractFilterType::New();
  m_ScaledExtractFilter = ExtractFilterType::New();

  // Wiring
  m_ExtractRenderingFilter->SetInput(m_ExtractFilter->GetOutput());
  m_ScaledExtractRenderingFilter->SetInput(m_ScaledExtractFilter->GetOutput());
}

template <class TImage, class TOutputImage>
ImageLayer<TImage,TOutputImage>
::~ImageLayer()
{}


template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os,indent);
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::Render()
{
  // Render the histogram
  this->UpdateListSample();

  // Render images
  this->RenderImages();
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::RenderImages()
{
//   if (m_ListSample.IsNull()) //FIXME make sure that it is not necessary
//   {
//     this->UpdateListSample();
//   }
  // Render quicklook
  if(this->GetHasQuicklook())
    {
    itk::TimeProbe probe;
    probe.Start();
    m_QuicklookRenderingFilter->Update();
    this->SetRenderedQuicklook(m_QuicklookRenderingFilter->GetOutput());
    probe.Stop();
    otbMsgDevMacro(<<"ImageLayer::RenderImages():"<<" ("<<this->GetName()<<")"<< " quicklook regenerated ("<<probe.GetMeanTime()<<" s.)");
    }
  // If there are pixels to render
  if(this->GetExtractRegion().GetNumberOfPixels() > 0)
    {
    itk::TimeProbe probe;
    probe.Start();
    //std::cout<<"Extent: "<<this->GetExtent()<<" Largest: "<<m_Image->GetLargestPossibleRegion()<<" ExtractRegion: "<<this->GetExtractRegion()<<std::endl;
    m_ExtractRenderingFilter->GetOutput()->SetRequestedRegion(this->GetExtractRegion());
    m_ExtractRenderingFilter->Update();
    this->SetRenderedExtract(m_ExtractRenderingFilter->GetOutput());
    probe.Stop();
    otbMsgDevMacro(<<"ImageLayer::RenderImages():"<<" ("<<this->GetName()<<")"<< " extract regenerated ("<<probe.GetMeanTime()<<" s.)");
    this->SetHasExtract(true);
    }
  else
    {
    this->SetHasExtract(false);
    }
  // Render scaled extract
  // If there are pixels to render
  if(this->GetScaledExtractRegion().GetNumberOfPixels() > 0)
      {
      itk::TimeProbe probe;
      probe.Start();
      m_ScaledExtractRenderingFilter->GetOutput()->SetRequestedRegion(this->GetScaledExtractRegion());
      m_ScaledExtractRenderingFilter->Update();
      this->SetRenderedScaledExtract(m_ScaledExtractRenderingFilter->GetOutput());
      this->SetHasScaledExtract(true);
      probe.Stop();
      otbMsgDevMacro(<<"ImageLayer::RenderImages():"<<" ("<<this->GetName()<<")"<< " scaled extract regenerated ("<<probe.GetMeanTime()<<" s.)");
      }
  else
    {
    this->SetHasScaledExtract(false);
    }
}

template <class TImage, class TOutputImage>
void
ImageLayer<TImage,TOutputImage>
::UpdateListSample()
{
  otbMsgDevMacro(<<"ImageLayer::UpdateListSample():"<<" ("<<this->GetName()<<")"<< " Entering method");
  // Declare the source of the histogram
  ImagePointerType histogramSource;

  // if there is a quicklook, use it for histogram generation
  if(m_Quicklook.IsNotNull())
    {
    histogramSource = m_Quicklook;
    }
  else
    {
    // Else use the full image (update the data)
    // REVIEW: Not sure the region is right here. Should be the
    // largest ?
    // REPLY: might be... didn't change anything
    //
    histogramSource = m_Image;
    histogramSource->SetRequestedRegion(this->GetExtractRegion());
    }

  // Check if we need to generate the histogram again
  if( m_ListSample.IsNull() || m_ListSample->Size() == 0 || (histogramSource->GetUpdateMTime() < histogramSource->GetPipelineMTime()) )
    {
    otbMsgDevMacro(<<"ImageLayer::UpdateListSample():"<<" ("<<this->GetName()<<")"<< " Regenerating histogram due to pippeline update.");

    // Update the histogram source
    histogramSource->Update();

    // Iterate on the image
    itk::ImageRegionConstIterator<ImageType> it(histogramSource,histogramSource->GetBufferedRegion());

    // declare a list to store the samples
    m_ListSample->Clear();

    unsigned int sampleSize = VisualizationPixelTraits::PixelSize(it.Get());
    m_ListSample->SetMeasurementVectorSize(sampleSize);

    // Fill the samples list
    it.GoToBegin();
    while(!it.IsAtEnd())
    {
      SampleType sample(sampleSize);
      VisualizationPixelTraits::Convert( it.Get(), sample );
      m_ListSample->PushBack(sample);
      ++it;
    }
    otbMsgDevMacro(<<"ImageLayer::UpdateListSample()"<<" ("<<this->GetName()<<")"<< " Sample list generated ("<<m_ListSample->Size()<<" samples, "<< sampleSize <<" bands)");

    m_RenderingFunction->SetListSample(m_ListSample);

    }
}


template <class TImage, class TOutputImage>
std::string
ImageLayer<TImage,TOutputImage>
::GetPixelDescription(const IndexType & index)
{
  //FIXME only if necessary
    this->UpdateListSample();

  // Ensure rendering function intialization
  m_RenderingFunction->Initialize(); //FIXME check, but the call must be done in the generator. To be moved to the layer?
  // The ouptut stringstream
  itk::OStringStream oss;
  oss<<"Layer: "<<this->GetName();
  // If we are inside the buffered region
  if(m_Image->GetBufferedRegion().IsInside(index))
    {
    oss<<std::endl<<m_RenderingFunction->Describe(m_Image->GetPixel(index));
    }
  else if(m_Quicklook.IsNotNull())
    // Else we extrapolate the value from the quicklook
    {
    IndexType ssindex = index;
    ssindex[0]/=this->GetQuicklookSubsamplingRate();
    ssindex[1]/=this->GetQuicklookSubsamplingRate();

    if(m_Quicklook->GetBufferedRegion().IsInside(ssindex))
      {
      oss<<" (ql)"<<std::endl<<m_RenderingFunction->Describe(m_Quicklook->GetPixel(ssindex));
      }
    }
  return oss.str();
}

}

#endif
