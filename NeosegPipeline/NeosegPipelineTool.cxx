#include "NeosegPipelineTool.h"

NeosegPipelineTool::NeosegPipelineTool(QString programPath)
{
   m_parameters = new::PipelineParameters();
   m_parameters->setProgramPath(programPath);

   m_pipeline = new::Pipeline();
   m_pipeline->setPipelineParameters(m_parameters); 

   m_parametersErrors = ""; 
   m_executablesErrors = "";
   m_Force = false ;
}

void NeosegPipelineTool::setOutput(QString output)
{
   m_parameters->setOutput(output);
}

void NeosegPipelineTool::setPrefix(QString prefix)
{
   m_parameters->setPrefix(prefix);
}

void NeosegPipelineTool::setForce(bool force)
{
   m_Force = force ;
}

void NeosegPipelineTool::setSuffix(QString suffix)
{
   m_parameters->setSuffix(suffix);
}

void NeosegPipelineTool::setT1(QString T1)
{
   if(!T1.isEmpty())
   {
      if(m_parameters->checkT1(T1))
      {
         m_parameters->setT1(T1);
      }
      else
      {
         m_errors += T1 + " does not exist\n";
      }
   }
}
void NeosegPipelineTool::setT2(QString T2)
{
   if(!T2.isEmpty())
   {
      if(m_parameters->checkT2(T2))
      {
         m_parameters->setT2(T2);
      }
      else
      {
         m_errors += T2 + " does not exist\n";
      }
   }
}
void NeosegPipelineTool::setMask(QString mask)
{
   if(!mask.isEmpty())
   {
      if(m_parameters->checkMask(mask))
      {
         m_parameters->setMask(mask);
      }
      else
      {
         m_errors += mask + " does not exist\n";
      }
   }
}

void NeosegPipelineTool::setDWI(QString DWI)
{
   if(!DWI.isEmpty())
   {
      if(m_parameters->checkDWI(DWI))
      {
         m_parameters->setDWI(DWI);
      }
      else
      {
         m_errors += DWI + " does not exist\n";
      }
   }
}

void NeosegPipelineTool::setParametersFile(QString xmlFile) 
{
   if(!xmlFile.isEmpty())
   {
      XmlReader m_xmlReader;
      m_xmlReader.setPipelineParameters(m_parameters);
      m_parametersErrors = m_xmlReader.readParametersConfigurationFile(xmlFile); 
   }
}

void NeosegPipelineTool::setDataFile(QString xmlFile)
{
   if(!xmlFile.isEmpty())
   {
      XmlReader m_xmlReader;
      m_xmlReader.setPipelineParameters(m_parameters);
      m_dataErrors = m_xmlReader.readDataConfigurationFile(xmlFile);
   }
}

void NeosegPipelineTool::setExecutablesFile(QString xml_CLI) 
{
   XmlReader m_xmlReader;
   m_xmlReader.setPipelineParameters(m_parameters);

   if(!xml_CLI.isEmpty())
   {
      m_executablesErrors = m_xmlReader.readExecutablesConfigurationFile(xml_CLI);
   }
   else
   {
      QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
      QString xml_ENV = env.value("NEOSEG_PIPELINE_EXECUTABLES", QString::null);
      if(!xml_ENV.isEmpty())
      {
         m_executablesErrors = m_xmlReader.readExecutablesConfigurationFile(xml_ENV);
      }
   }
}

void NeosegPipelineTool::printErrors()
{
   if(!m_parametersErrors.isEmpty()
   || !m_executablesErrors.isEmpty()
   || !m_dataErrors.isEmpty()
     )
   {
       if( !m_dataErrors.isEmpty() )
       {
          m_errors += "Errors in the data configuration :\n" + m_dataErrors;
          m_errors += "\n";
       }
      if(!m_parametersErrors.isEmpty() )
      {
         m_errors = "Errors in the parameters configuration :\n" + m_parametersErrors;
         m_errors += "\n";
      }
      if( !m_executablesErrors.isEmpty() )
      {
         m_errors += "Errors in the software configuration :\n" + m_executablesErrors;
         m_errors += "\n";
      }

      m_errors += "\n";
      m_errors += "All the data or parameters or softwares that are nor valid, are left to their default value\n";
   }
}

int NeosegPipelineTool::launch(int argc, char *argv[], bool gui)
{
   printErrors();

   if(gui)
   {
      QApplication app(argc, argv);
      MainScriptThread* thread = new::MainScriptThread(); 

      DerivedWindow window;
      window.setPipelineParameters(m_parameters); 
      window.setPipeline(m_pipeline);
      window.setMainScriptThread(thread);
      window.show();

      if(!m_errors.isEmpty())
      {
         window.printErrors(m_errors);
      }

      app.exec(); 
      thread->wait();
   }
   else
   {
      if(!m_errors.isEmpty())
      {
         std::cerr << m_errors.toStdString() << std::endl ;
         if( !m_Force )
         {
             return 2 ;
         }
      }
      if( m_parameters->getT1() == ""
          || m_parameters->getT2() == ""
          || m_parameters->getMask() == ""
          || m_parameters->getOutput() == ""
        )
      {
         std::cerr << "When running from command line: T1, T2, mask and output need to be specified" << std::endl ;
         return 3 ;
      }
      QString output = m_parameters->getOutput();
      if(!(QFileInfo(output).exists()))
      { 
         std::cout << output.toStdString() << " does not exist. Creating it" << std::endl ; 
         bool result = (QDir::root()).mkpath( output ) ;
         if( !result )
         {
            std::cerr << output.toStdString() << " can not be created, Please try again with a new directory path" << std::endl ;
            return 1 ;
         }
      }
      QString imagesErrors = m_parameters->checkImages();
      if( imagesErrors.isEmpty() )
      {
          m_pipeline->writePipeline() ;
          m_pipeline->runPipeline() ;
      }
      else
      {
          std::cerr << imagesErrors.toStdString() << std::endl ;
      }
   }
   return 0 ;
}
