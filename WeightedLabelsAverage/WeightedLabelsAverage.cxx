// Generate CLP
#include "WeightedLabelsAverageCLP.h"

// General libraries // 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iterator>
#include <stdio.h>

// ITK librairies //
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWeightedLabelsAverageFilter.h"
#include "itkXMLFile.h"

// XML Librairies // 
#include "Parameters.h"
#include "ParametersXMLFile.h"

#include "Atlas.h"

int main(int argc, char* argv[])
{

   PARSE_ARGS;

   // Types // 
   typedef itk::Image<float, 3>                    FloatImageType;
   typedef itk::ImageFileReader<FloatImageType>    ReaderType; 
   typedef itk::ImageFileWriter<FloatImageType>    WriterType; 

   ParametersXMLFileReader::Pointer reader = ParametersXMLFileReader::New();
   reader->SetFilename(parameters_file);
   reader->GenerateOutputInformation();
   Parameters::Pointer parameters = reader->GetOutputObject();

   if (!parameters->CheckValues())
   {
      std::cout<<"Invalid Parameters"<<std::endl; 
   }

   // Tissue Probability Maps Filter 
   typedef itk::WeightedLabelsAverageFilter< FloatImageType, FloatImageType > WeightedLabelsAverageFilterType;
   WeightedLabelsAverageFilterType::Pointer WeightedLabelsAverageFilter = WeightedLabelsAverageFilterType::New();

   // Weights 
   if(parameters->GetComputingWeights())
   {
      WeightedLabelsAverageFilter->SetComputingWeightsOn();
      WeightedLabelsAverageFilter->SetRadiusValue(parameters->GetNeightborhoodRadius());
      if(parameters->GetNeightborhoodRadiusUnit()=="mm")
      {
         WeightedLabelsAverageFilter->SetRadiusValueInMillimetersOn();
      }
      else
      {
         WeightedLabelsAverageFilter->SetRadiusValueInMillimetersOff();         
      }
   }

   else
   {
      WeightedLabelsAverageFilter->SetComputingWeightsOff();
      //WeightedLabelsAverageFilter->SetNumberOfThreads(2);
   }


   std::cout<<"Reading Inputs "<<std::endl;

   std::string inputFilename = parameters->GetInput();
   std::string inputExtension = inputFilename.substr(inputFilename.find_last_of("."));
   if(inputExtension.compare(".gz") == 0){
    inputExtension = ".nii.gz";
   }
   // Input
   ReaderType::Pointer input_reader = ReaderType::New();
   input_reader->SetFileName(parameters->GetInput());
   input_reader->Update();
   WeightedLabelsAverageFilter->SetInput(input_reader->GetOutput());


   std::string maskfile = parameters->GetInputMask();
   if(maskfile.compare("") != 0){
     std::cout<<"Reading Input Mask "<<std::endl;
       ReaderType::Pointer input_mask_reader = ReaderType::New();
       input_mask_reader->SetFileName(maskfile);
       input_mask_reader->Update();
       WeightedLabelsAverageFilter->SetInputImageMask(input_mask_reader->GetOutput());
   }


   // Atlas Population
   std::vector<Atlas> atlasPopulation = parameters->GetAtlasPopulation();
   Atlas atlas;
   std::vector<Atlas>::iterator it;
   for( it=atlasPopulation.begin(); it!=atlasPopulation.end(); ++it)
   {
       Atlas atlas = *(it);

       // Image
       ReaderType::Pointer image_reader = ReaderType::New();
       image_reader->SetFileName(atlas.image);
       image_reader->Update();

       if(atlas.probabilistic)
       {
           ReaderType::Pointer white_reader = ReaderType::New();
           white_reader->SetFileName(atlas.white);
           white_reader->Update();

           ReaderType::Pointer gray_reader = ReaderType::New();
           gray_reader->SetFileName(atlas.gray);
           gray_reader->Update();

           ReaderType::Pointer csf_reader = ReaderType::New();
           csf_reader->SetFileName(atlas.csf);
           csf_reader->Update();

           WeightedLabelsAverageFilter->AddAtlas(image_reader->GetOutput(), white_reader->GetOutput(), gray_reader->GetOutput(), csf_reader->GetOutput());
       }
       else
       {
           ReaderType::Pointer seg_reader = ReaderType::New();
           seg_reader->SetFileName(atlas.seg);
           seg_reader->Update();
           WeightedLabelsAverageFilter->AddAtlas(image_reader->GetOutput(), seg_reader->GetOutput());
       }
   }
   std::cout<<"Reading Inputs Done "<<std::endl;

   // Update 
   try{
       WeightedLabelsAverageFilter->Update();
   }catch(itk::ExceptionObject& e){
       std::cerr<<e.GetDescription()<<std::endl;
      return -1;
   }


   WeightedLabelsAverageFilterType::InputImageLabelIndexMapType map = WeightedLabelsAverageFilter->GetInputImagePixelType();

   int start = 1;
   int addout = 0;
   if(map.find(0) == map.end()){//If the label 0 (background) is not found then we write all of the generated outputs. l
    start = 0;
    addout = 1;
   }

   std::cout<<"Writing Outputs: "<<std::endl;

   if(parameters->GetOutputDirectory().empty()){//The output folder is set when there is an unknown number of priors.
      // Weighted Averaged White // 
     WriterType::Pointer writer_white = WriterType::New(); 
     writer_white->SetInput(WeightedLabelsAverageFilter->GetOutput(1)); 
     writer_white->SetFileName(parameters->GetWhite());
     writer_white->SetUseCompression(true);
     writer_white->Update();
     
     // Weighted Averaged Gray // 
     WriterType::Pointer writer_gray = WriterType::New(); 
     writer_gray->SetInput(WeightedLabelsAverageFilter->GetOutput(2)); 
     writer_gray->SetFileName(parameters->GetGray());
     writer_gray->SetUseCompression(true);
     writer_gray->Update();
     
     // Weighted Averaged Csf // 
     WriterType::Pointer writer_csf = WriterType::New(); 
     writer_csf->SetInput(WeightedLabelsAverageFilter->GetOutput(3)); 
     writer_csf->SetFileName(parameters->GetCsf());
     writer_csf->SetUseCompression(true);
     writer_csf->Update();
   }else{
    for(int i = start; i < (int)WeightedLabelsAverageFilter->GetNumberOfOutputs(); i++){
      WriterType::Pointer writer_white = WriterType::New(); 
       writer_white->SetInput(WeightedLabelsAverageFilter->GetOutput(i)); 
       char buf[50];
       sprintf(buf, "%d", i + addout);
       std::string outfilename = parameters->GetOutputDirectory() + "/" + std::string(buf)  + inputExtension;
       std::cout<<"\t"<<outfilename<<std::endl;
       writer_white->SetFileName(outfilename.c_str());
       writer_white->SetUseCompression(true);
       writer_white->Update();
     }
   }
   
}


