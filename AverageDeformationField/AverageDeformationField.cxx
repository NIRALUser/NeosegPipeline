//Filter to average deformation field
#include "AverageDeformationFieldCLP.h"

#include "itkImage.h"
#include <iostream>
#include <string>
#include <itkImageFileReader.h>
#include <itkImageFileWriter.h>
#include "itkVector.h"

int main(int argc, char *argv[])
{
  PARSE_ARGS;

  typedef itk::Vector< float, 3 >       PixelType;

  typedef itk::Image< PixelType, 3 >    ImageType;
  typedef itk::ImageFileReader< ImageType > ReaderType;

  ReaderType::Pointer reader1= ReaderType::New();
  ReaderType::Pointer reader2= ReaderType::New();

  reader1->SetFileName(InputImage1.c_str());
  reader1->Update();
  reader2->SetFileName(InputImage2.c_str());
  reader2->Update();
  ImageType::Pointer image1 = reader1->GetOutput();
  ImageType::Pointer image2 = reader2->GetOutput();
  ImageType::Pointer image_result = ImageType::New();

  // The image region should be initialized
  ImageType::RegionType region1 = image1->GetLargestPossibleRegion(); 
  ImageType::RegionType region2 = image2->GetLargestPossibleRegion();
  ImageType::SizeType size1 = region1.GetSize();
  ImageType::SizeType size2 = region2.GetSize();

  
if(size1!=size2)
 {
  std::cout<<"The 2 deformation fields are not the same size"<< std::endl;
  std::cout <<"Size of the 1rst deformation field :"<< size1 << std::endl;
  std::cout <<"Size of the 2nd deformation field :"<< size2 << std::endl;
 }
else
 {
  const ImageType::IndexType start = {{0,0,0}}; //First index at {X,Y,Z}
  region1.SetSize( size1 );
  region1.SetIndex( start );
  image1->SetRegions( region1 );
  image1->Allocate();

  region2.SetSize( size2 );
  region2.SetIndex( start );
  image2->SetRegions( region2 );
  image2->Allocate();

  image_result->SetRegions( region1 );
  image_result->Allocate();

  int x=0,y=0,z=0;
  ImageType::IndexType pixelIndex;
  ImageType::PixelType pixelValue;

  for(z=0;z<size1[2];z++)
  {
  	for(y=0;y<size1[1];y++)
  	{
  		for(x=0;x<size1[0];x++)
  		{  			
  			pixelIndex[0]=x;
  			pixelIndex[1]=y;
  			pixelIndex[2]=z;
   			pixelValue=(image1->GetPixel(pixelIndex)+image2->GetPixel(pixelIndex))/2.0;
  			image_result->SetPixel(pixelIndex,pixelValue);
  		}
  	}
  }

  typedef  itk::ImageFileWriter< ImageType  > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetFileName(OutputImage.c_str());
  writer->SetInput(image_result);
  writer->Update();
 }

return EXIT_SUCCESS;
}
