/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.

  Some parts of this code are derived from ITK. See ITKCopyright.txt
  for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __otbPersistentImageToOGRDataFilter_txx
#define __otbPersistentImageToOGRDataFilter_txx

#include "otbPersistentImageToOGRDataFilter.h"
#include "ogrsf_frmts.h"
#include "itksys/SystemTools.hxx"
#include "otbSystem.h"

namespace otb
{




template<class TImage>
PersistentImageToOGRDataFilter<TImage>
::PersistentImageToOGRDataFilter() : m_FieldName("DN"), m_FileName(""), m_TileNum(0)
{
   // OGR factory registration
   OGRRegisterAll();
}

template<class TImage>
PersistentImageToOGRDataFilter<TImage>
::~PersistentImageToOGRDataFilter()
{
   if (m_DataSource != NULL)
    {
      OGRDataSource::DestroyDataSource(m_DataSource);
    }
}

template<class TImage>
std::string
PersistentImageToOGRDataFilter<TImage>
::GetOGRDriverName(std::string name) const
{
  std::string extension;
  std::string driverOGR;

  std::string upperName;
  upperName = name;
  std::transform(name.begin(), name.end(), upperName.begin(), (int (*)(int))toupper);
   
  //Test of PostGIS connection string
  if (upperName.substr(0, 3) == "PG:")
    {
    driverOGR = "PostgreSQL";
    }
  else
    {
    extension = System::GetExtension(upperName);
    if (extension == "SHP") driverOGR = "ESRI Shapefile";
    else if ((extension == "TAB")) driverOGR = "MapInfo File";
    else if (extension == "GML") driverOGR = "GML";
    else if (extension == "GPX") driverOGR = "GPX";
    else if (extension == "SQLITE") driverOGR = "SQLite";
//    else if (extension=="KML")
//      driverOGR="KML";
    else driverOGR = "NOT-FOUND";
    }
  return driverOGR;
}


template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::AllocateOutputs()
{
  // Nothing that needs to be allocated for the outputs : the output is not meant to be used
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::Reset()
{

}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::Synthetize()
{

}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::Initialize()
{
   if (m_DataSource != NULL)
    {
      OGRDataSource::DestroyDataSource(m_DataSource);
    }
   
   std::string projectionRefWkt = this->GetInput()->GetProjectionRef();
   bool projectionInformationAvailable = !projectionRefWkt.empty();
   OGRSpatialReference * oSRS = NULL;
   if (projectionInformationAvailable)
    {
    oSRS = static_cast<OGRSpatialReference *>(OSRNewSpatialReference(projectionRefWkt.c_str()));
    }
   OGRSFDriver * ogrDriver;
   std::string driverName = this->GetOGRDriverName(this->m_FileName).data();
   if(driverName != "NOT-FOUND")
   {
      ogrDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(driverName.c_str());
      m_DataSource = ogrDriver->CreateDataSource(this->m_FileName.c_str(), NULL);
      m_DataSource->CreateLayer("layer", oSRS ,wkbMultiPolygon, NULL);
      
      OGRFieldDefn field(m_FieldName.c_str(),OFTInteger);
      m_DataSource->GetLayer(0)->CreateField(&field, true);
   }
   else
   {
      itkExceptionMacro(<< "No OGR driver found to write file " << this->m_FileName);
   }

}


template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::GenerateData()
{
  // call the processing function for this tile
  OGRDataSourceObjectPointerType currentTileVD = this->ProcessTile();
  OGRLayer * poSrcLayer = currentTileVD->Get()->GetDataSource()->GetLayer(0);

  OGRLayer * poDstLayer;
  poDstLayer = m_DataSource->GetLayer(0);
  
  //Copy features in the output layer
  poSrcLayer->ResetReading();
  unsigned int nbFeatures = poSrcLayer->GetFeatureCount(true);
//  std::cout<< "pppppp"<<std::endl;
  unsigned int i = 0;
  OGRFeature  *poFeature;
  while (i<nbFeatures)
  {
      OGRFeature  *poDstFeature = NULL;
      poFeature = poSrcLayer->GetNextFeature();
      
      if( poFeature == NULL )
            break;
      
      poDstFeature = OGRFeature::CreateFeature( poDstLayer->GetLayerDefn() );
      poDstFeature->SetFrom( poFeature, TRUE );
      poDstLayer->CreateFeature( poDstFeature );
      
      //free memory
      OGRFeature::DestroyFeature( poDstFeature );
      OGRFeature::DestroyFeature( poFeature );
      
      i++;
  }
  
//   OGRDataSource::DestroyDataSource(currentTileVD->Get()->GetDataSource());
  
  
  m_TileNum++;
  
}

template<class TImage>
void
PersistentImageToOGRDataFilter<TImage>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

} // end namespace otb
#endif
