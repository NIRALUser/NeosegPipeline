// Generate CLP // 
#include <ReassignWhiteMatterCLP.h>

// General libraries // 
#include <iostream>
#include <string>

// ITK librairies //
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkBinaryImageToLabelMapFilter.h"
#include "itkBinaryThresholdImageFilter.h"

#include "itkXMLFile.h"

// XML Librairies // 
#include "Parameters.h"
#include "ParametersXMLFile.h"

using namespace std;

int executeRelabelOld(Parameters::Pointer parameters){
    // Image Type
    typedef itk::Image< float, 3>    ImageType;
    typedef ImageType::IndexType                 ImageIndexType;
    typedef ImageType::PixelType                 ImagePixelType;
    typedef ImageType::Pointer                   ImagePointerType;

    typedef itk::ImageFileReader<ImageType> ReaderType;
    typedef itk::ImageFileWriter<ImageType> WriterType;

    // Input Reader
    ReaderType::Pointer reader_input = ReaderType::New();
    reader_input->SetFileName(parameters->GetInput());
    reader_input->Update();
    ImagePointerType input = reader_input->GetOutput();

    // White Reader
    ReaderType::Pointer reader_white = ReaderType::New();
    reader_white->SetFileName(parameters->GetWhite());
    reader_white->Update();
    ImagePointerType white = reader_white->GetOutput();

    // Gray Reader
    ReaderType::Pointer reader_gray = ReaderType::New();
    reader_gray->SetFileName(parameters->GetGray());
    reader_gray->Update();
    ImagePointerType gray = reader_gray->GetOutput();

    // Caf Reader
    ReaderType::Pointer reader_csf = ReaderType::New();
    reader_csf->SetFileName(parameters->GetCsf());
    reader_csf->Update();
    ImagePointerType csf = reader_csf->GetOutput();

    // Input Reader
    ImagePointerType output = input;

    // Label Map Filters
    typedef itk::BinaryImageToLabelMapFilter<ImageType> BinaryImageToLabelMapFilterType;
    BinaryImageToLabelMapFilterType::Pointer labelMapFilters[3];

    // Number of Connected Components
    int numberOfConnectedComponents[3];

    for(int label = 0; label < 3; label++)
    {
       typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;
       BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
       thresholdFilter->SetInput(reader_input->GetOutput());
       thresholdFilter->SetLowerThreshold(label+1);
       thresholdFilter->SetUpperThreshold(label+1);
       thresholdFilter->SetInsideValue(255);
       thresholdFilter->SetOutsideValue(0);

       BinaryImageToLabelMapFilterType::Pointer labelMapFilter = BinaryImageToLabelMapFilterType::New();
       labelMapFilter->SetInput(thresholdFilter->GetOutput());
       labelMapFilter->SetOutputBackgroundValue(0);
       labelMapFilter->SetInputForegroundValue(255);
       labelMapFilter->SetFullyConnected(true);
       labelMapFilter->Update();

       labelMapFilters[label] = labelMapFilter;
       numberOfConnectedComponents[label] = (labelMapFilter->GetOutput())->GetNumberOfLabelObjects();
    }

    // Connected Component Size
    int connectedComponentSize[3];

    // Max Number of Connected Components
    int max = *std::max_element(numberOfConnectedComponents, numberOfConnectedComponents + 3);

    for(int i = 1; i <= max; i++)
    {
       for(int label = 0; label < 3; label++)
       {
          if(i < numberOfConnectedComponents[label])
          {
             connectedComponentSize[label] = ((labelMapFilters[label]->GetOutput())->GetLabelObject(i))->Size();
          }
          else
          {
             connectedComponentSize[label] = 0;
          }
       }

       if(connectedComponentSize[0] < parameters->GetThreshold())
       {
          for(int voxel = 0; voxel < connectedComponentSize[0]; voxel++)
          {
             ImageIndexType index;

             index = ((labelMapFilters[0]->GetOutput())->GetLabelObject(i))->GetIndex(voxel);

             ImagePixelType probability_gray = gray->GetPixel(index);
             ImagePixelType probability_csf = csf->GetPixel(index);

             if(probability_gray > probability_csf)
             {
                output->SetPixel(index, 2);
             }
             else
             {
                output->SetPixel(index, 3);
             }
          }
       }
    }

    // Output Writer
    WriterType::Pointer writer_output = WriterType::New();
    writer_output->SetFileName(parameters->GetOutput());
    writer_output->SetInput(output);
    writer_output->Update();

    return 0;
}

int main(int argc, char* argv[])
{
   PARSE_ARGS;

   ParametersXMLFileReader::Pointer reader_XML = ParametersXMLFileReader::New();
   reader_XML->SetFilename(parameters_file);
   reader_XML->GenerateOutputInformation();
   Parameters::Pointer parameters = reader_XML->GetOutputObject();

   if (!parameters->CheckValues())
   {
      std::cout<<"Invalid Parameters"<<std::endl; 
      return -1;
   }

   if(parameters->GetProbabilityMaps().size() == 0){
       return executeRelabelOld(parameters);
   }

   // Image Type
   typedef itk::Image< float, 3>    ImageType;
   typedef ImageType::IndexType                 ImageIndexType;
   typedef ImageType::PixelType                 ImagePixelType;
   typedef ImageType::Pointer                   ImagePointerType;
   typedef itk::ImageRegionIterator< ImageType > ImageRegionIteratorType;

   typedef itk::ImageFileReader<ImageType> ReaderType;
   typedef itk::ImageFileWriter<ImageType> WriterType;

   // Input Reader
   ReaderType::Pointer reader_input = ReaderType::New();
   reader_input->SetFileName(parameters->GetInput());
   reader_input->Update();
   ImagePointerType input = reader_input->GetOutput();

   typedef map< ImagePixelType , int > InputImageLabelIndexMapType;
   typedef InputImageLabelIndexMapType::iterator InputImageLabelIndexMapIteratorType;
   InputImageLabelIndexMapType inputImageLabelIndexMap;

   //Get the values of the input labels
   ImageRegionIteratorType it(input, input->GetLargestPossibleRegion());
   it.GoToBegin();
   int indexOfLabel = 0;
   while(!it.IsAtEnd()){
       ImagePixelType p = it.Get();

       if(p != 0 && inputImageLabelIndexMap.find(p) == inputImageLabelIndexMap.end()){//We will check only for the first atlas, the rest of the atlas must have the same index (number) of labels
           inputImageLabelIndexMap[p] = indexOfLabel;
           indexOfLabel++;
       }
       ++it;
   }

   //Reorder the labels and assign index
   //Read probability maps, BIG assumption: The input probability maps are ordered in the same way as the labels ordered by value
   InputImageLabelIndexMapIteratorType itmap;

   vector< string > vectorprobmaps = parameters->GetProbabilityMaps();
   map< ImagePixelType, ImagePointerType > vectorprobimages;

   indexOfLabel = 0;
   for(itmap = inputImageLabelIndexMap.begin(); itmap != inputImageLabelIndexMap.end(); ++itmap){
       itmap->second = indexOfLabel;

       if(itmap->second < (int) vectorprobmaps.size()){
           ReaderType::Pointer reader = ReaderType::New();
           reader->SetFileName(vectorprobmaps[itmap->second]);
           reader->Update();
           vectorprobimages[itmap->first] = reader->GetOutput();
       }

       indexOfLabel++;
   }

   // Input Reader
   ImagePointerType output = input;
   ImagePixelType inputLabelValue = parameters->GetLabelValue();

   if(inputLabelValue == -1){
       cerr<<"Error: The specified label is: " <<inputLabelValue<<endl;
       return 1;
   }

   // Label Map Filters
   typedef itk::BinaryImageToLabelMapFilter<ImageType> BinaryImageToLabelMapFilterType;
   typedef BinaryImageToLabelMapFilterType::Pointer BinaryImageToLabelMapFilterPointerType;

   typedef itk::BinaryThresholdImageFilter <ImageType, ImageType> BinaryThresholdImageFilterType;
   BinaryThresholdImageFilterType::Pointer thresholdFilter = BinaryThresholdImageFilterType::New();
   thresholdFilter->SetInput(reader_input->GetOutput());
   thresholdFilter->SetLowerThreshold(inputLabelValue);
   thresholdFilter->SetUpperThreshold(inputLabelValue);
   thresholdFilter->SetInsideValue(255);
   thresholdFilter->SetOutsideValue(0);

   BinaryImageToLabelMapFilterType::Pointer labelMapFilter = BinaryImageToLabelMapFilterType::New();
   labelMapFilter->SetInput(thresholdFilter->GetOutput());
   labelMapFilter->SetOutputBackgroundValue(0);
   labelMapFilter->SetInputForegroundValue(255);
   labelMapFilter->SetFullyConnected(true);
   labelMapFilter->Update();


   for(unsigned int labelObj = 1; labelObj < (labelMapFilter->GetOutput())->GetNumberOfLabelObjects(); labelObj++){
       int objectSize = (labelMapFilter->GetOutput())->GetLabelObject(labelObj)->Size();
       if(objectSize < parameters->GetThreshold()){

           if(parameters->GetVoxelByVoxel()){
               for(int voxel = 0; voxel < objectSize; voxel++)
               {
                  ImageIndexType index = (labelMapFilter->GetOutput())->GetLabelObject(labelObj)->GetIndex(voxel);

                  ImagePixelType maxProbLabel = -1;
                  float maxProbValue = -1;

                  for(itmap = inputImageLabelIndexMap.begin(); itmap != inputImageLabelIndexMap.end(); ++itmap){
                      if(itmap->first != inputLabelValue && vectorprobimages.find(itmap->first) != vectorprobimages.end()){
                          ImagePixelType probValue = vectorprobimages[itmap->first]->GetPixel(index);
                          if(maxProbValue < probValue){
                              maxProbValue = probValue;
                              maxProbLabel = itmap->first;
                          }
                      }
                  }


                  if(maxProbLabel != -1){
                     output->SetPixel(index, maxProbLabel);
                  }
               }
           }else{

               map< ImagePixelType, float > maxProbMap;

               for(itmap = inputImageLabelIndexMap.begin(); itmap != inputImageLabelIndexMap.end(); ++itmap){
                   if(itmap->first != inputLabelValue && vectorprobimages.find(itmap->first) != vectorprobimages.end()){

                       maxProbMap[itmap->first] = 0;
                       for(int voxel = 0; voxel < objectSize; voxel++){

                           ImageIndexType index = (labelMapFilter->GetOutput())->GetLabelObject(labelObj)->GetIndex(voxel);

                           maxProbMap[itmap->first] += vectorprobimages[itmap->first]->GetPixel(index);

                       }
                   }

               }

               map< ImagePixelType, float >::iterator itprobmap;
               ImagePixelType maxLabel = -1;
               float maxLabelValue = -1;
               for(itprobmap = maxProbMap.begin(); itprobmap != maxProbMap.end(); ++itprobmap){
                   if(max(maxLabelValue, itprobmap->second)){
                       maxLabelValue = itprobmap->second;
                       maxLabel = itprobmap->first;
                   }
               }

               if(maxLabel != -1){
                   for(int voxel = 0; voxel < objectSize; voxel++){
                       ImageIndexType index = (labelMapFilter->GetOutput())->GetLabelObject(labelObj)->GetIndex(voxel);
                       output->SetPixel(index, maxLabel);
                   }
               }
           }
       }
   }

   // Output Writer
   WriterType::Pointer writer_output = WriterType::New();
   writer_output->SetFileName(parameters->GetOutput());
   writer_output->SetInput(output);
   writer_output->Update();




   return 0; 
}
