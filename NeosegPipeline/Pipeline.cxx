#include "Pipeline.h"

Pipeline::Pipeline()
{
   m_processing_name = "Processing";
   m_indent = "  " ; // 2 white-spaces
   m_neosegExecution = 0;
   m_ABCExecution = 0;
}

Pipeline::~Pipeline()
{

}

void Pipeline::setIndent(QString indent)
{
    if( Script::checkIndent(indent) )
    {
        m_indent = indent ;
    }
}

void Pipeline::setPipelineParameters(PipelineParameters* parameters)
{
   m_parameters = parameters; 
}
void Pipeline::setPlainTextEdit(QPlainTextEdit* plainTextEdit)
{
   m_plainTextEdit = plainTextEdit; 
}

QProcess* Pipeline::getMainScriptProcess()
{
   return m_mainScriptProcess;
}

void Pipeline::createProcessingDirectory()
{
   QDir output_dir(m_parameters->getOutput());
   if(!output_dir.exists(m_processing_name))
   {
      output_dir.mkdir(m_processing_name);
   } 
   m_processing_path = output_dir.filePath(m_processing_name);
}

QString Pipeline::createModuleDirectory(QString directory_name)
{
   QDir output_dir(m_parameters->getOutput());
   if(!output_dir.exists(directory_name))
   {
      output_dir.mkdir(directory_name);
   }  
   return output_dir.filePath(directory_name);
}

void Pipeline::writePreProcessingData()
{
   QString directory_name = "1.PreProcessingData";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "preProcessingData";
   m_preProcessingData = new::PreProcessingData(module_name);
   m_preProcessingData->setIndent(m_indent);
   m_preProcessingData->setNeo(m_parameters->getNeo()); 
   m_preProcessingData->setModuleDirectory(directory_path);
   m_preProcessingData->setProcessingDirectory(m_processing_path);
   m_preProcessingData->setExecutablePaths(m_parameters->getExecutablePaths());
   m_preProcessingData->setOverwriting(m_parameters->getOverwriting());    
   m_preProcessingData->setSuffix(m_parameters->getSuffix()); 
   m_preProcessingData->setStoppingIfError(m_parameters->getStoppingIfError());

   m_preProcessingData->setUsingSmoothedMask((m_parameters->getAntsParametersDTI())->getUsingSmoothedMask() || (m_parameters->getAntsParametersAtlas())->getUsingSmoothedMask());
   m_preProcessingData->setSkullStripping(m_parameters->getSkullStripping());
   m_preProcessingData->setCorrecting(m_parameters->getCorrecting());   

   m_preProcessingData->update(); 
   m_importingModules += "import " + module_name + "\n"; 
   m_runningModules += module_name + ".run()\n"; 


   m_parameters->setNeo(m_preProcessingData->getOutput());
}

void Pipeline::writeDTIRegistration()
{
   QString directory_name = "2.DTIRegistration";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "DTIRegistration";
   m_dtiRegistration = new::DtiRegistration(module_name);
   m_dtiRegistration->setIndent(m_indent);
   m_dtiRegistration->setNeo(m_parameters->getNeo());
   m_dtiRegistration->setModuleDirectory(directory_path);
   m_dtiRegistration->setProcessingDirectory(m_processing_path);
   m_dtiRegistration->setAntsParameters(m_parameters->getAntsParametersDTI());
   m_dtiRegistration->setExecutablePaths(m_parameters->getExecutablePaths());
   m_dtiRegistration->setSuffix(m_parameters->getSuffix()); 
   m_dtiRegistration->setOverwriting(m_parameters->getOverwriting()); 
   m_dtiRegistration->setStoppingIfError(m_parameters->getStoppingIfError());

   m_dtiRegistration->update();
   m_importingModules += "import " + module_name + "\n"; 
   m_runningModules += module_name + ".run()\n"; 

   m_parameters->setNeo(m_dtiRegistration->getOutput());
}

void Pipeline::writeAtlasRegistration()
{
   QString module_name = "atlasRegistration";
   m_atlasRegistration = new::AtlasRegistration(module_name); 
   m_atlasRegistration->setIndent(m_indent);
   m_atlasRegistration->setNeo(m_parameters->getNeo());
   m_atlasRegistration->setSuffix(m_parameters->getSuffix()); 
   m_atlasRegistration->setProcessingDirectory(m_processing_path);
   m_atlasRegistration->setLog(m_log_path);
   m_atlasRegistration->setRegistrationParameters(m_parameters->getRegistrationParameters());
   m_atlasRegistration->setExecutablePaths(m_parameters->getExecutablePaths());
   m_atlasRegistration->setOverwriting(m_parameters->getOverwriting());
   m_atlasRegistration->setStoppingIfError(m_parameters->getStoppingIfError());

   m_atlasRegistration->update(); 
}

void Pipeline::writeAtlasPopulationRegistration()
{
   QString directory_name = "3.AtlasRegistration";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "atlasPopulationRegistration";
   m_atlasPopulationRegistration = new::AtlasPopulationRegistration(module_name); 
   m_atlasPopulationRegistration->setIndent(m_indent);
   m_atlasPopulationRegistration->setNeo(m_parameters->getNeo());
   m_atlasPopulationRegistration->setAtlasPopulation(m_parameters->getAtlasPopulation());
   m_atlasPopulationRegistration->setModuleDirectory(directory_path);
   m_atlasPopulationRegistration->setProcessingDirectory(m_processing_path);
   m_atlasPopulationRegistration->setComputingSystem(m_parameters->getComputingSystem());
   m_atlasPopulationRegistration->setExecutablePaths(m_parameters->getExecutablePaths());
   m_atlasPopulationRegistration->setSuffix(m_parameters->getSuffix()); 
   m_atlasPopulationRegistration->setOverwriting(m_parameters->getOverwriting()); 
   m_atlasPopulationRegistration->setStoppingIfError(m_parameters->getStoppingIfError());
   m_atlasPopulationRegistration->setRegistrationParameters(m_parameters->getRegistrationParameters()); 
   m_atlasPopulationRegistration->update();
   m_importingModules += "import " + module_name + "\n"; 
   m_runningModules += module_name + ".run()\n"; 
   
   m_parameters->setAtlasPopulation(m_atlasPopulationRegistration->getOutput());
}

void Pipeline::writeAtlasGeneration()
{
   QString directory_name = "4.AtlasGeneration";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "atlasGeneration";
   m_atlasGeneration = new::AtlasGeneration(module_name);
   m_atlasGeneration->setIndent(m_indent);
   m_atlasGeneration->setNeo(m_parameters->getNeo());
   m_atlasGeneration->setAtlasPopulation(m_parameters->getAtlasPopulation());
   m_atlasGeneration->setModuleDirectory(directory_path);
   m_atlasGeneration->setProcessingDirectory(m_processing_path);
   m_atlasGeneration->setSmoothing(m_parameters->getSmoothingFlag());
   m_atlasGeneration->setSmoothingSize(m_parameters->getSmoothingSize());
   m_atlasGeneration->setComputingWeights(m_parameters->getComputingWeights());
   m_atlasGeneration->setNeightborhoodRadius(m_parameters->getWeightsRadius());
   m_atlasGeneration->setNeightborhoodRadiusUnit(m_parameters->getWeightsRadiusUnit());
   m_atlasGeneration->setIncludingFA(m_parameters->getIncludingFA());
   m_atlasGeneration->setFAShift(m_parameters->getFAShift());
   m_atlasGeneration->setFASigmaScale(m_parameters->getFASigmaScale());
   m_atlasGeneration->setFAWeight(m_parameters->getFAWeight());
   m_atlasGeneration->setFASmoothingSize(m_parameters->getFASmoothingSize());
   m_atlasGeneration->setExecutablePaths(m_parameters->getExecutablePaths());
   m_atlasGeneration->setSuffix(m_parameters->getSuffix()); 
   m_atlasGeneration->setOverwriting(m_parameters->getOverwriting()); 
   m_atlasGeneration->setStoppingIfError(m_parameters->getStoppingIfError());

   if(m_parameters->getTissueSegmentationType() == TISSUE_SEG_TYPE_ABC){
       m_atlasGeneration->setABCPipelineModeOn();
   }

   m_atlasGeneration->setUseT1(m_parameters->getWeightsModalityIndex() == 0);

   m_atlasGeneration->setABCWhiteLabelOutputIndexString(m_parameters->getABCWhiteImageIndex());

   m_atlasGeneration->update();
   m_importingModules += "import " + module_name + "\n"; 
   m_runningModules += module_name + ".run()\n"; 

   m_parameters->setExistingAtlas(m_atlasGeneration->getOutput() , false );
}

void Pipeline::writeExistingAtlasRegistration()
{
   QString directory_name = "2.ExistingAtlasRegistration";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "existingAtlasRegistration";
   m_existingAtlasRegistration = new::ExistingAtlasRegistration(module_name);
   m_existingAtlasRegistration->setIndent(m_indent);
   m_existingAtlasRegistration->setNeo(m_parameters->getNeo());
   m_existingAtlasRegistration->setModuleDirectory(directory_path);
   m_existingAtlasRegistration->setProcessingDirectory(m_processing_path);
   m_existingAtlasRegistration->setExistingAtlas(m_parameters->getExistingAtlas());
   m_existingAtlasRegistration->setAntsParameters(m_parameters->getAntsParametersAtlas());
   m_existingAtlasRegistration->setExecutablePaths(m_parameters->getExecutablePaths());
   m_existingAtlasRegistration->setSuffix(m_parameters->getSuffix());
   m_existingAtlasRegistration->setOverwriting(m_parameters->getOverwriting());
   m_existingAtlasRegistration->setStoppingIfError(m_parameters->getStoppingIfError());

   m_existingAtlasRegistration->update();
   m_importingModules += "import " + module_name +"\n"; 
   m_runningModules += module_name + ".run()\n"; 

   m_parameters->setExistingAtlas(m_existingAtlasRegistration->getOutput() , false );
}

void Pipeline::writeABCExecution(){

    QString directory_name = "5.ABCExecution";
    QString directory_path = createModuleDirectory(directory_name);

    QString module_name = "abcExecution";
    m_ABCExecution = new::ABCExecution(module_name);
    m_ABCExecution->setIndent(m_indent);
    m_ABCExecution->SetPipelineParameters(m_parameters);
    m_ABCExecution->setModuleDirectory(directory_path);
    m_ABCExecution->setProcessingDirectory(m_processing_path);
    m_ABCExecution->setExecutablePaths(m_parameters->getExecutablePaths());
    m_ABCExecution->setSuffix(m_parameters->getSuffix());
    m_ABCExecution->setOverwriting(m_parameters->getOverwriting());
    m_ABCExecution->setStoppingIfError(m_parameters->getStoppingIfError());

    m_ABCExecution->update();

    m_importingModules += "import " + module_name +"\n";
    m_runningModules += module_name + ".run()\n";

}

void Pipeline::writeNeosegExecution()
{
   QString directory_name = "5.NeosegExecution";
   QString directory_path = createModuleDirectory(directory_name);

   QString module_name = "neosegExecution";
   m_neosegExecution = new::NeosegExecution(module_name);
   m_neosegExecution->setIndent(m_indent);
   m_neosegExecution->setNeo(m_parameters->getNeo());
   m_neosegExecution->setModuleDirectory(directory_path);
   m_neosegExecution->setProcessingDirectory(m_processing_path);
   m_neosegExecution->setNewAtlas(m_parameters->getNewAtlas());
   m_neosegExecution->setExistingAtlas(m_parameters->getExistingAtlas());
   m_neosegExecution->setAtlasFormat(m_parameters->getAtlasFormat());
   m_neosegExecution->setUsingFA(m_parameters->getUsingFA());
   m_neosegExecution->setUsingAD(m_parameters->getUsingAD());
   m_neosegExecution->setComputing3LabelsSeg(m_parameters->getComputing3LabelsSeg());
   m_neosegExecution->setReassigningWhiteMatter(m_parameters->getReassigningWhiteMatter());
   m_neosegExecution->setSizeThreshold(m_parameters->getSizeThreshold());
   m_neosegExecution->setNeosegParameters(m_parameters->getNeosegParameters());
   m_neosegExecution->setExecutablePaths(m_parameters->getExecutablePaths());
   m_neosegExecution->setSuffix(m_parameters->getSuffix());
   m_neosegExecution->setOverwriting(m_parameters->getOverwriting());
   m_neosegExecution->setStoppingIfError(m_parameters->getStoppingIfError());

   m_neosegExecution->update();
   m_importingModules += "import " + module_name +"\n"; 
   m_runningModules += module_name + ".run()\n"; 

   m_parameters->setNeo(m_neosegExecution->getOutput());
}

void Pipeline::initializeMainScript()
{
   m_script = "#!/usr/bin/env python\n\n";

   m_script += "import os\n";
   m_script += "import sys\n";
   m_script += "import signal\n";
   m_script += "import logging\n";
   m_script += "import subprocess\n";
   m_script += "import datetime\n";
   m_script += "import shutil\n\n";
}

void Pipeline::defineSignalHandler()
{
   m_script += "def signal_handler(signal, frame):\n";
   m_script += m_indent + "print '***************You pressed Ctrl+C!******************'\n";
   m_script += m_indent + "if runningProcess.poll!=1:\n";
   m_script += m_indent + m_indent + "runningProcess.terminate()\n";
   m_script += m_indent + "sys.exit(0)\n\n";

   m_script += "signal.signal(signal.SIGINT, signal_handler)\n";
   m_script += "signal.signal(signal.SIGTERM, signal_handler)\n\n";
}

void Pipeline::initializeLogging()
{
   m_script += "log = \"" + m_log_path + "\"\n";
   m_script += "logFile = open(log, \"w\") \n\n";

   m_script += "logger = logging.getLogger('NeosegPipeline')\n"; 
   m_script += "logger.setLevel(logging.DEBUG)\n\n";

   m_script += "fileHandler = logging.FileHandler(log)\n";
   m_script += "fileHandler.setLevel(logging.DEBUG)\n";
   m_script += "fileFormatter = logging.Formatter('%(message)s')\n";
   m_script += "fileHandler.setFormatter(fileFormatter)\n\n";

   m_script += "consoleHandler = logging.StreamHandler()\n";
   m_script += "consoleHandler.setLevel(logging.DEBUG)\n";     
   m_script += "consoleFormatter = logging.Formatter('%(message)s')\n";
   m_script += "consoleHandler.setFormatter(consoleFormatter)\n\n";

   m_script += "logger.addHandler(fileHandler)\n";
   m_script += "logger.addHandler(consoleHandler)\n\n";
}

void Pipeline::copySegmentations()
{
   QDir* output_dir = new QDir(m_parameters->getOutput());
   Neo neo = m_parameters->getNeo(); 

   m_script += "seg4Labels_src = \"" + neo.seg4Labels + "\"\n";
   QString seg4Labels_name = neo.prefix + "-seg-4Labels.nrrd";
   QString seg4Labels_path = output_dir->filePath(seg4Labels_name);
   m_script += "seg4Labels_dest = \"" + seg4Labels_path + "\"\n";

   m_script += "shutil.copyfile(seg4Labels_src, seg4Labels_dest)\n\n"; 

   if(m_parameters->getComputing3LabelsSeg())
   {
      m_script += "seg3Labels_src = \"" + neo.seg3Labels + "\"\n";
      QString seg3Labels_name = neo.prefix + "-seg-3Labels.nrrd";
      QString seg3Labels_path = output_dir->filePath(seg3Labels_name);
      m_script += "seg3Labels_dest = \"" + seg3Labels_path + "\"\n";

      m_script += "shutil.copyfile(seg3Labels_src, seg3Labels_dest)\n\n"; 
   }
}

void Pipeline::writeMainScript()
{
   initializeMainScript();

   m_script += m_importingModules; 
   defineSignalHandler();
   initializeLogging();

   m_script += "os.environ['ITK_GLOBAL_DEFAULT_NUMBER_OF_THREADS'] = '1' \n";
   
   m_script += "logger.info(sys.executable)\n"; 

   m_script += "start = datetime.datetime.now()\n\n";

   m_script += m_runningModules + "\n"; 

   if(m_neosegExecution){
       copySegmentations();
   }

   m_script += "end = datetime.datetime.now()\n\n";

   m_script += "elapsedTime = (end - start).seconds\n";

   m_script += "hours = elapsedTime/(60*60)\n";
   m_script += "elapsedTime -= hours*(60*60)\n\n"; 

   m_script += "minutes = elapsedTime/60\n";
   m_script += "elapsedTime -= minutes*60\n\n"; 

   m_script += "seconds = elapsedTime\n\n";

   m_script += "logger.info('Pipeline took %s hour(s), %s minute(s) and %s second(s)', hours, minutes, seconds)\n"; 

   QDir* processing_dir = new QDir(m_processing_path); 
   m_main_path = processing_dir->filePath("main.py");

   std::ofstream* script_stream = new std::ofstream((m_main_path.toStdString()).c_str());
   *script_stream << m_script.toStdString() << std::endl;

   script_stream->close();

   chmod((m_main_path.toStdString()).c_str(), 0755);
}

void Pipeline::cleanUp()
{
   m_preProcessingData->cleanUp();
   
   if( (m_parameters->getNewAtlas() && m_parameters->getIncludingFA()) || m_parameters->getUsingFA() || m_parameters->getUsingAD())
   {
      m_dtiRegistration->cleanUp();
   }

   if(m_parameters->getNewAtlas()==true)
   {
      m_atlasRegistration->cleanUp(); 
      m_atlasPopulationRegistration->cleanUp();
      m_atlasGeneration->cleanUp(); 
   }
   else
   {
      m_existingAtlasRegistration->cleanUp(); 
   }

   if(m_neosegExecution){
       m_neosegExecution->cleanUp();
   }

   if(m_ABCExecution){
       m_ABCExecution->cleanUp();
   }

}

void Pipeline::writeXMLFiles()
{
   QDir* output_dir = new QDir(m_parameters->getOutput());

   XmlWriter* writer = new::XmlWriter();
   writer->setPipelineParameters(m_parameters);

   QString data_path = output_dir->filePath(m_parameters->getPrefix() + "-data.xml"); 
   writer->writeDataConfiguration(data_path);

   QString parameters_path = output_dir->filePath(m_parameters->getPrefix() + "-parameters.xml"); 
   writer->writeParametersConfiguration(parameters_path);

   QString executables_path = output_dir->filePath(m_parameters->getPrefix() + "-executables.xml");   
   writer->writeExecutablesConfiguration(executables_path);

}

void Pipeline::writePipeline()
{
   m_importingModules = ""; 
   m_runningModules = ""; 

   createProcessingDirectory();

   m_parameters->initializeNeo(); 
   m_parameters->initializeAtlasPopulation(); 

   QDir* output_dir = new QDir(m_parameters->getOutput());
   m_log_path = output_dir->filePath((m_parameters->getNeo()).prefix + ".log"); 

   writePreProcessingData();
   if( (m_parameters->getNewAtlas() && m_parameters->getIncludingFA()) || m_parameters->getUsingFA() || m_parameters->getUsingAD())
   {
      writeDTIRegistration(); 
   }

   if(m_parameters->getNewAtlas()==true)
   {
      writeAtlasRegistration();
      writeAtlasPopulationRegistration(); 
      writeAtlasGeneration(); 
   } 
   else
   {
      writeExistingAtlasRegistration(); 
   }
   if(m_parameters->getTissueSegmentationType() == TISSUE_SEG_TYPE_NEOSEG){
       writeNeosegExecution();
   }else{
       writeABCExecution();
   }

   writeMainScript();

}

void Pipeline::runPipeline()
{
   QString command; 
   if(!(m_parameters->getComputingSystem()).compare("local", Qt::CaseInsensitive) || !(m_parameters->getComputingSystem()).compare("killdevil interactive", Qt::CaseInsensitive))
   {
      command = m_main_path;
   }

   if(!(m_parameters->getComputingSystem()).compare("killdevil", Qt::CaseInsensitive))
   {
      command = "bsub -q day -M 4 -n 1 -R \"span[hosts=1]\" python " +  m_main_path;      
   }   

   QString python_path = (m_parameters->getExecutablePaths())->getExecutablePath("python"); 
   QString pythonDirectory_path = ((QFileInfo(python_path)).absoluteDir()).path();

   QString FSL_path = (m_parameters->getLibraryPaths()->getLibraryPath("FSL") );
                       //ExecutablePaths())->getExecutablePath("FSL");

   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   env.insert("PATH", pythonDirectory_path + ":" + env.value("PATH"));
   env.insert("PYTHONPATH", "");
   env.insert("FSLDIR", FSL_path);
   env.insert("FSLOUTPUTTYPE", "NIFTI_GZ");


   m_mainScriptProcess = new QProcess;
   m_mainScriptProcess->setProcessEnvironment(env);
   m_mainScriptProcess->start(command);

   m_mainScriptProcess->waitForStarted();   

   m_mainScriptProcess->waitForFinished( -1 ) ;

   if(!(m_parameters->getComputingSystem()).compare("killdevil", Qt::CaseInsensitive))
   {
      bool jobRunning = true; 

      QString output(m_mainScriptProcess->readAllStandardOutput());
      QRegExp regExp("(<{1})([0-9]{1,})(>{1})");
      regExp.indexIn(output);
      m_jobID = regExp.cap(2);   

      std::cout<<"jobID = "<<m_jobID.toStdString()<<std::endl; 

      QProcess* bjobs_process = new::QProcess(); 
      while(jobRunning)
      {
         bjobs_process->start("bjobs " + m_jobID); 
         bjobs_process->waitForFinished( -1 ) ;
         QString bjobs_output(bjobs_process->readAllStandardOutput());
         if(bjobs_output.contains("DONE") || bjobs_output.contains("EXIT"))
         {
            jobRunning = false;
         }

         sleep(1); 
      }
   }
   if( m_mainScriptProcess->exitCode() == 0 )
   {
      cleanUp() ;
   }
}

void Pipeline::stopPipeline()
{
   if(!(m_parameters->getComputingSystem()).compare("killdevil", Qt::CaseInsensitive))
   {
      QProcess* bkill_process = new::QProcess();
      bkill_process->start("bkill " + m_jobID);
      while (!bkill_process->waitForFinished())
      {
         sleep(1);
      }
   }

   m_mainScriptProcess->terminate();
/*    qDebug() << "wait for finished: "<<m_mainScriptProcess->waitForFinished(500);
    usleep(10000);
    qDebug() << "Still running: "<<(m_mainScriptProcess->state() == QProcess::Running);*/
//   m_mainScriptProcess->terminate();
   m_mainScriptProcess->waitForFinished(-1);
   //usleep(10000);
   /*while( !m_mainScriptProcess->waitForFinished(500) )
   {
       qDebug() << m_mainScriptProcess->readAllStandardOutput() ;
       qDebug() << m_mainScriptProcess->readAllStandardError() ;
   }*/
   usleep(10000);
   qDebug() << "Process is still running: " << (m_mainScriptProcess->state() == QProcess::Running ) ;
}

