#include "ExistingAtlasRegistration.h"

ExistingAtlasRegistration::ExistingAtlasRegistration(QString module) : Script(module)
{

}

// SET // 
void ExistingAtlasRegistration::setRegistrationParameters(RegistrationParameters* parameters) 
{
   m_parameters = parameters;
}
void ExistingAtlasRegistration::setExistingAtlas(QString atlas_path) 
{
   m_atlas_path = atlas_path;
   m_atlas_dir = new::QDir(m_atlas_path);
}

void ExistingAtlasRegistration::initializeScript()
{
   definePython();
   importGeneralModules();

   m_script += "import re\n";
   m_script += "import csv\n";
   m_script += "import time\n";

   defineExecutable("ANTS");
   defineExecutable("ResampleScalarVectorDWIVolume");
   defineExecutable("ImageMath");
   /*defineExecutable("ImageStat"); ->Not used anymore???*/
   defineExecutable("unu");
   /*defineExecutable("CleanRingMask"); ->Not used anymore???*/
 
   m_script += "logger = logging.getLogger('NeosegPipeline')\n\n"; 

   m_script += "runningProcess = None\n";  
}

QString ExistingAtlasRegistration::getFilePath(QDir* dir, QString name)
{
   name = "*" + name + "*";
   QStringList* filter = new QStringList(name);
   QFileInfoList foundFile_list = dir->entryInfoList(*filter);

   return (foundFile_list[0]).absoluteFilePath();
} 

void ExistingAtlasRegistration::implementRun()
{   
   m_script += "def run():\n\n";

   m_script += m_indent + "signal.signal(signal.SIGINT, stop)\n";
   m_script += m_indent + "signal.signal(signal.SIGTERM, stop)\n\n";

   QString finalTemplateT1_name = "atlas_to_" + m_neo.prefix + "-templateT1.nrrd";
   QString finalTemplateT1_path = m_module_dir->filePath(finalTemplateT1_name); 

   QString finalTemplateT2_name = "atlas_to_" + m_neo.prefix + "-templateT2.nrrd";
   QString finalTemplateT2_path = m_module_dir->filePath(finalTemplateT2_name); 

   QString finalWhite_name = "atlas_to_" + m_neo.prefix + "-white.nrrd";
   QString finalWhite_path = m_module_dir->filePath(finalWhite_name); 

   QString finalGray_name = "atlas_to_" + m_neo.prefix + "-gray.nrrd";
   QString finalGray_path = m_module_dir->filePath(finalGray_name); 

   QString finalCSF_name = "atlas_to_" + m_neo.prefix + "-CSF.nrrd";
   QString finalCSF_path = m_module_dir->filePath(finalCSF_name); 

   QString finalRest_name = "atlas_to_" + m_neo.prefix + "-rest.nrrd";
   QString finalRest_path = m_module_dir->filePath(finalRest_name); 

   m_outputs.insert("finalTemplateT1", finalTemplateT1_path);
   m_outputs.insert("finalTemplateT2", finalTemplateT2_path);
   m_outputs.insert("finalWhite", finalWhite_path);
   m_outputs.insert("finalGray", finalGray_path);
   m_outputs.insert("finalCSF", finalCSF_path);
   m_outputs.insert("finalRest", finalRest_path);
   checkFinalOutputs(); 

   QString templateT1_path = getFilePath(m_atlas_dir, "templateT1");
   QString templateT2_path = getFilePath(m_atlas_dir, "templateT2");

   m_script += m_indent + "T1 = '" + m_neo.T1 + "'\n";
   m_script += m_indent + "T2 = '" + m_neo.T2 + "'\n";
   m_script += m_indent + "templateT1 = '" + templateT1_path + "'\n";
   m_script += m_indent + "templateT2 = '" + templateT2_path + "'\n";



   // Smooth mask
   QString smoothedMask_path;
   if(m_parameters->getUsingSmoothedMask())
   {
      QString smoothedMask_name = m_prefix + "mask-smoothed" + m_suffix + ".nrrd";
      QString smoothedMask_path = m_module_dir->filePath(smoothedMask_name); 

      m_log = "Smoothing mask";
      m_inputs.insert("mask", m_neo.mask);   
      m_outputs.insert("smoothedMask", smoothedMask_path); 
      m_argumentsList << "ImageMath" << "mask" << "'-smooth'" << "'-gauss'" << "'-size'" << "'1'" << "'-outfile'" << "smoothedMask";
      execute(); 
   } 

   // Registration
   QString T1Atlas_path = getFilePath(m_atlas_dir, "templateT1");
   QString T2Atlas_path = getFilePath(m_atlas_dir, "templateT2");
   
   QString modality1 = "CC[' + T1 + ',' + templateT1 + '," + QString::number(m_parameters->getWeight1()) + "," + QString::number(m_parameters->getRadius1()) + "]";
   QString modality2 = "CC[' + T2 + ',' + templateT2 + '," + QString::number(m_parameters->getWeight2()) + "," + QString::number(m_parameters->getRadius2()) + "]";
   QString iterations = QString::number(m_parameters->getIterationsJ()) + "x" + QString::number(m_parameters->getIterationsK()) + "x" + QString::number(m_parameters->getIterationsL());
   QString transformation = m_parameters->getTransformationType() + "[" + QString::number(m_parameters->getGradientStepLength()) + "]"; //"," + QString::number(m_parameters->getNumberOfTimeSteps()) + "," + QString::number(m_parameters->getDeltaTime()) + "]";
   QString regularization = m_parameters->getRegularizationType() + "[" + QString::number(m_parameters->getGradientFieldSigma()) + "," + QString::number(m_parameters->getDeformationFieldSigma()) + "]";

   QString output_name = "atlas_to_" + m_neo.prefix + "_.nii.gz";
   QString output_path = m_module_dir->filePath(output_name); 

   QString affine_name = "atlas_to_" + m_neo.prefix + "_Affine.txt";
   QString affine_path = m_module_dir->filePath(affine_name); 

   QString warp_name = "atlas_to_" + m_neo.prefix + "_Warp.nii.gz";
   QString warp_path = m_module_dir->filePath(warp_name); 

   m_log = "Calculating transformations";
   m_inputs.insert("T1", m_neo.T1); 
   m_inputs.insert("T2", m_neo.T2); 
   m_inputs.insert("templateT1", T1Atlas_path); 
   m_inputs.insert("templateT2", T2Atlas_path); 
   m_inputs.insert("modality1", modality1); 
   m_inputs.insert("modality2", modality2); 
   m_inputs.insert("iterations", iterations); 
   m_inputs.insert("transformation", transformation);
   m_inputs.insert("regularization", regularization);  
   m_inputs.insert("output", output_path);

   m_outputs.insert("affine", affine_path);
   m_outputs.insert("warp", warp_path);  

   m_argumentsList << "ANTS" << "'3'" << "'-m'" << "modality1" << "'-m'" << "modality2" << "'-o'" << "output" << "'-i'" << "iterations" << "'-t'" << "transformation" << "'-r'" << "regularization";

   if(m_parameters->getUsingMask())
   {
      m_inputs.insert("mask", m_neo.mask); 
      m_argumentsList << "'-x'" << "mask"; 
   }
   if(m_parameters->getUsingSmoothedMask())
   {
      m_inputs.insert("smoothedMask", smoothedMask_path); 
      m_argumentsList << "'-x'" << "smoothedMask"; 
   }
   execute(); 

   // Normalizing affine file
   m_log = "Normalizing affine file";
   m_argumentsList << "'text_subst.pl'" << "'MatrixOffsetTransformBase_double_3_3'" << "'AffineTransform_double_3_3'" << "affine"; 
   execute();

   // Applying transformations to template T1
   QString templateT1Reg_name = "templateT1.nrrd";
   QString templateT1Reg_path = m_module_dir->filePath(templateT1Reg_name); 

   m_log = "Applying transformations to template T1";
   m_outputs.insert("T1Reg", templateT1Reg_path); 
   m_argumentsList << "ResampleScalarVectorDWIVolume" << "templateT1" << "T1Reg" << "'--Reference'" << "T1" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute();

   // Applying transformations to template T2
   QString templateT2Reg_name = "templateT2.nrrd";
   QString templateT2Reg_path = m_module_dir->filePath(templateT2Reg_name); 

   m_log = "Applying transformations to template T2";
   m_outputs.insert("T2Reg", templateT2Reg_path); 
   m_argumentsList << "ResampleScalarVectorDWIVolume" << "templateT2" << "T2Reg" << "'--Reference'" << "T2" << "'-i'" << "'bs'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(); 

   // Applying transformations to the white probability 
   QString white_path = getFilePath(m_atlas_dir, "white");
   QString whiteReg_name = "white.nrrd";
   QString whiteReg_path = m_module_dir->filePath(whiteReg_name);

   m_log = "Applying transformations to the white probability";
   m_inputs.insert("white", white_path);
   m_outputs.insert("whiteReg", whiteReg_path); 
   m_argumentsList << "ResampleScalarVectorDWIVolume" << "white" << "whiteReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(); 

   // Applying transformations to the gray probability 
   QString gray_path = getFilePath(m_atlas_dir, "gray");
   QString grayReg_name = "gray.nrrd";
   QString grayReg_path = m_module_dir->filePath(grayReg_name);
 
   m_log = "Applying transformations to the gray probability";
   m_inputs.insert("gray", gray_path);
   m_outputs.insert("grayReg", grayReg_path);
   m_argumentsList << "ResampleScalarVectorDWIVolume" << "gray" << "grayReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(); 

   // Applying transformations to the CSF probability 
   QString csf_path = getFilePath(m_atlas_dir, "csf");
   QString csfReg_name = "csf.nrrd";
   QString csfReg_path = m_module_dir->filePath(csfReg_name); 

   m_log = "Applying transformations to the CSF probability";
   m_inputs.insert("csf", csf_path);
   m_outputs.insert("csfReg", csfReg_path);
   m_argumentsList << "ResampleScalarVectorDWIVolume" << "csf" << "csfReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(); 

   // Applying transformations to the Rest probability 
   QString rest_path = getFilePath(m_atlas_dir, "rest"); 
   QString restReg_name = "rest.nrrd";
   QString restReg_path = m_module_dir->filePath(restReg_name);

   m_log = "Applying transformations to the CSF probability";
   m_inputs.insert("rest", rest_path); 
   m_outputs.insert("restReg", restReg_path);
   m_argumentsList << "ResampleScalarVectorDWIVolume" << "rest" << "restReg" << "'--Reference'" << "T2" << "'-i'" << "'nn'" << "'--hfieldtype'" << "'displacement'" << "'--defField'" << "warp" << "'--transformationFile'" << "affine";
   execute(); 
}

void ExistingAtlasRegistration::update()
{
   initializeScript();
   implementStop();
   implementCheckFileExistence();
   implementExecute();
   implementRun();

   writeScript();
}

QString ExistingAtlasRegistration::getOutput()
{
   return m_module_path; 
}


