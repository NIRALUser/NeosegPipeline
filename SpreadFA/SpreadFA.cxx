// Generate CLP 
#include "SpreadFACLP.h"

// General Libraries // 
#include <iostream>

// ITK Librairies //
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkUnaryFunctorImageFilter.h"

// My Specific Librairies // 
#include "itkSpreadFA.h"


int main( int argc , char* argv[] )
{
   PARSE_ARGS ;
   if( input.empty() || output.empty() )
   {
      std::cerr << "input and output filename necessary" << std::endl ;
      return EXIT_FAILURE ;
   }
   // Types //
   typedef itk::Image< float , 3 > ImageType ;
   typedef itk::ImageFileReader< ImageType > ReaderType ;
   typedef itk::ImageFileWriter< ImageType > WriterType ;


   // Spread FA Filter //
   typedef itk::SpreadFA < ImageType::PixelType, ImageType::PixelType >  SpreadFAFunctorType ;
   typedef itk::UnaryFunctorImageFilter < ImageType, ImageType, SpreadFAFunctorType > SpreadFAFilterType ;

   SpreadFAFilterType::Pointer spreadFAFilter = SpreadFAFilterType::New() ;

   // Input //
   ReaderType::Pointer reader = ReaderType::New() ;
   reader->SetFileName( input ) ;
   reader->Update() ;
   spreadFAFilter->SetInput( reader->GetOutput() ) ;

   // Update //
   ( spreadFAFilter->GetFunctor() ).SetShift( shift ) ;
   ( spreadFAFilter->GetFunctor() ).SetSigma( sigma ) ;
   spreadFAFilter->Update() ;

   // Output //
   WriterType::Pointer writer = WriterType::New() ;
   writer->SetInput( spreadFAFilter->GetOutput() ) ;
   writer->SetFileName( output ) ;
   writer->SetUseCompression( true ) ;
   writer->Update() ;

   return EXIT_SUCCESS ;
}
