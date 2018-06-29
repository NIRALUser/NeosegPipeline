#include <typeinfo>
#include <iostream>
#include "AtlasRegistration.h"

AtlasRegistration::AtlasRegistration(QString module) : Script(module)
{

}

// SET //  
void AtlasRegistration::setLog(QString log_path)
{
   m_log_path = log_path; 
}

void AtlasRegistration::setRegistrationParameters(RegistrationParameters* parameters) 
{
   m_parameters = parameters;
}

void AtlasRegistration::initializeScript()
{
   definePython();
   importGeneralModules();

   m_script += "import re\n";
   m_script += "import time\n";

   defineExecutable("ANTS");
   defineExecutable("ResampleScalarVectorDWIVolume");
   defineExecutable("ITKTransformTools");
   defineExecutable("ImageMath");
   defineExecutable("unu");
   defineExecutable("python");

   m_script += "logger = None\n";

   m_script += "runningProcess = None\n";  
   m_script += "logFile = None\n\n";  
}

void AtlasRegistration::initializeLogging()
{
   m_script += "def initializeLogging(log):\n";

   m_script += m_indent + "global logger\n";
   m_script += m_indent + "logger = logging.getLogger('NeosegPipelineRegistrations')\n";
   m_script += m_indent + "logger.setLevel(logging.DEBUG)\n";

   m_script += m_indent + "fileHandler = logging.FileHandler(log)\n";
   m_script += m_indent + "fileHandler.setLevel(logging.DEBUG)\n";
   m_script += m_indent + "fileFormatter = logging.Formatter('%(message)s')\n";
   m_script += m_indent + "fileHandler.setFormatter(fileFormatter)\n";

   m_script += m_indent + "logger.addHandler(fileHandler)\n\n";
}

void AtlasRegistration::implementRegisterAtlas(bool probabilistic)
{
   if(probabilistic)
   {
      m_script += "def main(name, T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, output, log):\n\n";
   } 
   else
   {      
      m_script += "def main(name, T1Atlas, T2Atlas, segAtlas, output, log, parcAtlas, roi0Atlas, roi1Atlas, roi2Atlas, roi3Atlas, roi4Atlas, roi5Atlas, roi6Atlas, roi7Atlas, roi8Atlas, roi9Atlas):\n\n";
   }

   m_script += m_indent + "signal.signal(signal.SIGINT, stop)\n";
   m_script += m_indent + "signal.signal(signal.SIGTERM, stop)\n\n";

   m_script += m_indent + "logFile = open(log, \"w\")\n";
   m_script += m_indent + "initializeLogging(log)\n\n";

   m_script += m_indent + "outbase = output + '/' + name + '_to_" + m_neo.prefix + "'\n";
   m_script += m_indent + "finalT1 = outbase + '-T1.nrrd'\n";
   m_script += m_indent + "finalT2 = outbase + '-T2.nrrd'\n";

   if(probabilistic)
   {
      m_script += m_indent + "finalWhite = outbase + '-white.nrrd'\n";
      m_script += m_indent + "finalGray = outbase + '-gray.nrrd'\n";
      m_script += m_indent + "finalCSF = outbase + '-csf.nrrd'\n";
      m_script += m_indent + "if checkFileExistence(finalT1)==True and checkFileExistence(finalT2) and checkFileExistence(finalWhite) and checkFileExistence(finalGray) and checkFileExistence(finalCSF) :\n";
   }
   else
   {
      m_script += m_indent + "finalSeg = outbase + '-seg.nrrd'\n\n";
      
      m_script += m_indent + "if parcAtlas != '""':\n";
      m_script += m_indent + m_indent + "finalParc = outbase + '-parc.nrrd'\n\n";

      m_script += m_indent + "finalRoi0='""'\n";
      m_script += m_indent + "finalRoi1='""'\n";
      m_script += m_indent + "finalRoi2='""'\n";
      m_script += m_indent + "finalRoi3='""'\n";
      m_script += m_indent + "finalRoi4='""'\n";
      m_script += m_indent + "finalRoi5='""'\n";
      m_script += m_indent + "finalRoi6='""'\n";
      m_script += m_indent + "finalRoi7='""'\n";
      m_script += m_indent + "finalRoi8='""'\n";
      m_script += m_indent + "finalRoi9='""'\n\n";

      m_script += m_indent + "finalRoiX=[finalRoi0,finalRoi1,finalRoi2,finalRoi3,finalRoi4,finalRoi5,finalRoi6,finalRoi7,finalRoi8,finalRoi9]\n";
      m_script += m_indent + "roiXAtlas=[roi0Atlas, roi1Atlas, roi2Atlas, roi3Atlas, roi4Atlas, roi5Atlas, roi6Atlas, roi7Atlas, roi8Atlas, roi9Atlas]\n\n";

      m_script += m_indent + "for i in range(0,10):\n";
      m_script += m_indent + m_indent + "if roiXAtlas[i] != '""':\n";
      m_script += m_indent + m_indent + m_indent + "finalRoiX[i] = outbase + '-roi'+str(i)+'.nrrd'\n\n";

      m_script += m_indent + "if checkFileExistence(finalT1)==True and checkFileExistence(finalT2) and checkFileExistence(finalSeg) :\n";
   }

   //m_script += m_indent + m_indent + "time.sleep(0.5)\n";
   m_script += m_indent + m_indent + "mainLog = open('" + m_log_path + "','a')\n";
   m_script += m_indent + m_indent + "mainLog.write(name + ' Registration -> Skipped\\n')\n";
   m_script += m_indent + m_indent + "mainLog.close()\n";
   m_script += m_indent + m_indent + "return\n";

   m_script += m_indent + "mainLog = open('" + m_log_path + "','a')\n";
   m_script += m_indent + "mainLog.write(name + ' Registration...\\n')\n";
   m_script += m_indent + "mainLog.close()\n";

   m_script += m_indent + "T1 = '" + m_neo.T1 + "'\n";
   m_script += m_indent + "T2 = '" + m_neo.T2 + "'\n";
   m_script += m_indent + "mask = '" + m_neo.mask + "'\n\n"; 

   bool registrationSoftware=m_parameters->getRegistrationSoftware();
   if(registrationSoftware==true)
   {
   // ANTS Registration 

      QString modality1 = "CC[' + T1 + ',' + T1Atlas + '," + QString::number(m_parameters->getWeight1()) + "," + QString::number(m_parameters->getRadius1()) + "]";
      QString modality2 = "CC[' + T2 + ',' + T2Atlas + '," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]";
      QString iterations = QString::number(m_parameters->getIterationsJ()) + "x" + QString::number(m_parameters->getIterationsK()) + "x" + QString::number(m_parameters->getIterationsL());
      QString transformation = m_parameters->getTransformationType() + "[" + QString::number(m_parameters->getGradientStepLength()) + "]"; //"," + QString::number(m_parameters->getNumberOfTimeSteps()) + "," + QString::number(m_parameters->getDeltaTime()) + "]";
      QString regularization = m_parameters->getRegularizationType() + "[" + QString::number(m_parameters->getGradientFieldSigma()) + "," + QString::number(m_parameters->getDeformationFieldSigma()) + "]";

      QString output = "' + outbase + '_.nii.gz";
      QString affine = "' + outbase + '_Affine.txt";
      QString warp = "' + outbase + '_Warp.nii.gz";

      m_log = "Calculating transformations";

      m_inputs.insert("modality1", modality1); 
      m_inputsTests.insert("modality1", "T1Atlas");
      m_inputs.insert("modality2", modality2); 
      m_inputsTests.insert("modality2", "T2Atlas");
      m_inputs.insert("iterations", iterations); 
      m_inputs.insert("transformation", transformation);
      m_inputs.insert("regularization", regularization);  
      m_inputs.insert("output", output );

      m_outputs.insert("affine", affine);
      m_outputs.insert("warp", warp);  

      QString mask; 
      if(m_parameters->getUsingMask())
      {
         mask = "mask";
         m_inputs.insert("mask", m_neo.mask); 
      }

      else if(m_parameters->getUsingSmoothedMask())
      {
         mask = "smoothedMask";
         m_inputs.insert("mask", m_neo.smoothedMask); 
      }

      m_argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";

      QStringList argsTests_T1; 
      argsTests_T1 << "ANTS" << "'3'" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";

      QStringList argsTests_T2; 
      argsTests_T2 << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";

      if(m_parameters->getUsingMask() || m_parameters->getUsingSmoothedMask())
      {
         m_argumentsList << "'-x'" << mask; 
         argsTests_T1 << "'-x'" << mask; 
         argsTests_T2 << "'-x'" << mask; 
      }

      m_argsTests.insert("T1Atlas", argsTests_T1); 
      m_argsTests.insert("T2Atlas", argsTests_T2); 
      execute(); 

      // Normalizing affine file
      m_log = "Normalizing affine file";
      m_argumentsList << "ITKTransformTools" << "'MO2Aff'" << "affine" << "affine"; 
      execute();

      // Applying transformations to T1
      QString T1Reg = "' + outbase + '-T1.nrrd";

      m_log = "Applying transformations to T1";
      m_test = "T1Atlas";
      m_outputs.insert("T1Reg", T1Reg); 

      m_argumentsList << "ResampleScalarVectorDWIVolume" << "T1Atlas" << "T1Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute();

      // Applying transformations to T2
      QString T2Reg = "' + outbase + '-T2.nrrd";

      m_log = "Applying transformations to T2";
      m_test = "T2Atlas";
      m_outputs.insert("T2Reg", T2Reg); 
      m_argumentsList << "ResampleScalarVectorDWIVolume" << "T2Atlas" << "T2Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
      execute(); 

      if(probabilistic)
      {
         // Applying transformations to the white probability 
         QString whiteReg = "' + outbase + '-white.nrrd";

         m_log = "Applying transformations to the white probability";
         m_outputs.insert("whiteReg", whiteReg); 
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "whiteAtlas" << "whiteReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
         execute(); 

         // Applying transformations to the gray probability 
         QString grayReg = "' + outbase + '-gray.nrrd";

         m_log = "Applying transformations to the gray probability";
         m_outputs.insert("grayReg", grayReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "grayAtlas" << "grayReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
         execute(); 

         // Applying transformations to the CSF probability 
         QString csfReg = "' + outbase + '-csf.nrrd";

         m_log = "Applying transformations to the CSF probability";
         m_outputs.insert("csfReg", csfReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "csfAtlas" << "csfReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
         execute(); 
      }

      else
      {
         // Applying transformations to the segmentation
         m_test = "segAtlas";
         QString segReg = "' + outbase + '-seg.nrrd";
         m_log = "Applying transformations to the tissue segmentation";
         m_outputs.insert("segReg", segReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "segAtlas" << "segReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
         execute(); 

         // Applying transformations to brain parcellation
         m_test = "parcAtlas"; 
         QString parcReg = "' + outbase + '-parc.nrrd";
         m_log = "Applying transformations to the brain parcellation";                     
         m_outputs.insert("parcReg", parcReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "parcAtlas" << "parcReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
         execute();    

         // Applying transformations to brain Rois
         QString roiXAtlas[10]={"roi0Atlas","roi1Atlas","roi2Atlas","roi3Atlas","roi4Atlas","roi5Atlas","roi6Atlas","roi7Atlas","roi8Atlas","roi9Atlas"};
         for (int k=0 ; k<10 ; k++)
         {
            m_test=roiXAtlas[k];

            m_log = "Applying transformations to the ROI"+QString::number(k);
            QString roiReg = "' + outbase + '-roi"+QString::number(k)+".nrrd";
            m_outputs.insert("roiReg", roiReg);

            m_argumentsList << "ResampleScalarVectorDWIVolume" << "roi"+QString::number(k)+"Atlas" << "roiReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
            execute(); 
            
         }
      }
   }
   else
   {
      //QUICKSILVER Registration

      m_log = "Calculating transformations";

      m_log = "Applying transformations to T1";

      //Read the registration script resource and write the new one with arguments
      QResource resources(":/quicksilver_script/Resources/quicksilver_script/registration.py");
      QFile quicksilver_scriptResource( resources.absoluteFilePath() );
      quicksilver_scriptResource.open( QIODevice::ReadOnly ) ;
      QString registration_script = quicksilver_scriptResource.readAll() ;
      quicksilver_scriptResource.flush();
      quicksilver_scriptResource.close();

      QString output_dir=m_parameters->getOutputDir();
      m_outputs.insert("output_dir",output_dir);
      QDir().mkdir(output_dir);
      QString main_script = QDir::cleanPath(output_dir + QString("/quicksilver_exe.py"));
      QFile quicksilver_script(main_script);
      quicksilver_script.open( QIODevice::WriteOnly ) ;
      QTextStream out( &quicksilver_script );
      out << registration_script ;
      quicksilver_script.flush();
      quicksilver_script.close();

      QString warp = "'+outbase+'_Warp.nrrd";
      //m_outputs.insert("warp", warp);
      m_script += m_indent + "warp = '" + warp + "'\n\n";

      QString T1Reg = "'+outbase+'-T1.nrrd";
      m_test = "T1Atlas";
      m_outputs.insert("T1Reg", T1Reg); 

      QString container = m_parameters->getContainerId();
      m_inputs.insert("container", container);

      m_argumentsList << "python" << "output_dir+'/quicksilver_exe.py'" << "'--target-images'" << "T1" << "'--moving-images'" << "T1Atlas" << "'--output-prefix'" << "T1Reg" << "'--container-id'" << "container" << "'--output-deffield'" << "warp";
      execute();

      QString T2Reg = "' + outbase + '-T2.nrrd";
      m_log = "Applying transformations to T2";
      m_test = "T2Atlas";
      m_outputs.insert("T2Reg", T2Reg); 

      m_argumentsList << "ResampleScalarVectorDWIVolume" << "T2Atlas" << "T2Reg" << "'--Reference'" << "T2" << "'-H'" << "warp" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'";
      execute(); 

      if(probabilistic)
      {
         // Applying transformations to the white probability 
         QString whiteReg = "' + outbase + '-white.nrrd";

         m_log = "Applying transformations to the white probability";
         m_outputs.insert("whiteReg", whiteReg); 
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "whiteAtlas" << "whiteReg" << "'--Reference'" << "T2" << "'-H'" << "warp" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'";
         execute(); 

         // Applying transformations to the gray probability 
         QString grayReg = "' + outbase + '-gray.nrrd";

         m_log = "Applying transformations to the gray probability";
         m_outputs.insert("grayReg", grayReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "grayAtlas" << "grayReg" << "'--Reference'" << "T2" << "'-H'" << "warp" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'";
         execute(); 

         // Applying transformations to the CSF probability 
         QString csfReg = "' + outbase + '-csf.nrrd";

         m_log = "Applying transformations to the CSF probability";
         m_outputs.insert("csfReg", csfReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "csfAtlas" << "csfReg" << "'--Reference'" << "T2" << "'-H'" << "warp" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'";
         execute(); 
      }
      else
      {
         // Applying transformations to the segmentation
         m_test = "segAtlas";
         QString segReg = "' + outbase + '-seg.nrrd";
         m_log = "Applying transformations to the tissue segmentation";
         m_outputs.insert("segReg", segReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "segAtlas" << "segReg" << "'--Reference'" << "T2" << "'-H'" << "warp" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'";
         execute(); 
         
         // Applying transformations to brain parcellation
         m_test = "parcAtlas"; 
         QString parcReg = "' + outbase + '-parc.nrrd";
         m_log = "Applying transformations to the brain parcellation";
         m_outputs.insert("parcReg", parcReg);
         m_argumentsList << "ResampleScalarVectorDWIVolume" << "parcAtlas" << "parcReg" << "'--Reference'" << "T2" << "'-H'" << "warp" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'";
         execute(); 
         
         // Applying transformations to brain Rois
         QString roiXAtlas[10]={"roi0Atlas", "roi1Atlas", "roi2Atlas", "roi3Atlas", "roi4Atlas", "roi5Atlas", "roi6Atlas", "roi7Atlas", "roi8Atlas", "roi9Atlas"};
         for (int k=0;k<10;k++)
         {
            m_test=roiXAtlas[k];

            m_log = "Applying transformations to the ROI"+QString::number(k);
            QString roiReg = "' + outbase + '-roi"+QString::number(k)+".nrrd";
            m_outputs.insert("roiReg", roiReg);

            m_argumentsList << "ResampleScalarVectorDWIVolume" << "roi"+QString::number(k)+"Atlas" << "roiReg" << "'--Reference'" << "T2" << "'-H'" << "warp" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'";
            execute(); 
         }

      }
   }
}

void AtlasRegistration::writeRegisterAtlas() // args = (T1Atlas, T2Atlas, segAtlas, output, log, parcAtlas, roi0Atlas, ..., roi9Atlas)
{
   initializeScript();

   implementStop();
   implementCheckFileExistence();
   initializeLogging(); 
   implementExecute();

   implementRegisterAtlas(0);
   
   m_script += "if __name__ == '__main__':\n";
   m_script += m_indent + "main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7], sys.argv[8], sys.argv[9], sys.argv[10], sys.argv[11], sys.argv[12], sys.argv[13], sys.argv[14], sys.argv[15], sys.argv[16], sys.argv[17])\n";

   QString script_path = m_processing_dir->filePath("atlasRegistration.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << m_script.toStdString() << std::endl; 
}

void AtlasRegistration::writeRegisterProbabilisticAtlas() // args = (T1Atlas, T2Atlas, whiteAtlas, grayAtlas, csfAtlas, outbase, log)
{
   m_script = ""; 

   initializeScript();

   implementStop();
   implementCheckFileExistence();
   initializeLogging(); 
   implementExecute();

   implementRegisterAtlas(1);

   m_script += "if __name__ == '__main__':\n";
   m_script += m_indent + "main(sys.argv[1], sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5], sys.argv[6], sys.argv[7], sys.argv[8])\n";

   QString script_path = m_processing_dir->filePath("probabilisticAtlasRegistration.py");

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << m_script.toStdString() << std::endl; 
}

void AtlasRegistration::update()
{  
   writeRegisterAtlas(); 
   writeRegisterProbabilisticAtlas();
}