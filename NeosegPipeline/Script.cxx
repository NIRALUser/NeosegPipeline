#include "Script.h"

Script::Script(QString module)
{
   m_module_name = module; 
   m_script_name = module + ".py";   
   m_indent = "  " ; //2 white spaces
} 

void Script::setProcessingDirectory(QString processing_path) 
{
   m_processing_dir = new QDir(processing_path);
}
void Script::setModuleDirectory(QString module_path) 
{
   m_module_path = module_path;
   m_module_dir = new QDir(module_path);
}
void Script::setExecutablePaths(ExecutablePaths* executables)
{  
   m_executables = executables;
}
void Script::setSuffix(QString suffix)
{
   m_suffix = "_" + suffix;
}
void Script::setOverwriting(bool overwriting)
{
   m_overwriting = overwriting; 
}
void Script::setStoppingIfError(bool stoppingIfError)
{
   m_stoppingIfError = stoppingIfError; 
}
void Script::setNeo(Neo neo)
{
   m_neo = neo;
   m_prefix = neo.prefix + "-";
}

QString Script::variable(QString variable_name)
{
   return "\" + " + variable_name + " + \"";
}

QString Script::str(QString str)
{
   return "\"" + str + "\"";
}

QString Script::listToString(QStringList argumentsList)
{
   QString arguments = argumentsList[0];

   QStringList::const_iterator it;
   for (it = argumentsList.constBegin()+1; it != argumentsList.constEnd(); ++it)
   {
      arguments += "," + (*it) + "";
   }

   return "[" + arguments + "]"; 
}

void Script::definePython()
{
   m_script = ""; 
   m_script += "#!/usr/bin/env python\n\n";
}

void Script::importGeneralModules()
{
   m_script += "import os\n";
   m_script += "import sys\n";
   m_script += "import logging\n";
   m_script += "import signal\n";
   m_script += "import subprocess\n\n";
}

void Script::importFusionModules()
{
   m_script += "import fnmatch\n\n";
}

void Script::importXmlModules()
{
   m_script += "from xml.etree import ElementTree\n";
   m_script += "from xml.etree.ElementTree import Element\n";
   m_script += "from xml.etree.ElementTree import SubElement\n";
   m_script += "import xml.dom.minidom\n\n";
}

void Script::defineExecutable(QString executable)
{
   m_script += executable + " = '" + m_executables->getExecutablePath(executable) + "'\n";
}

void Script::implementStop()
{
   m_script += "def stop(signal, frame):\n";
   m_script += m_indent + "print '*************** Signal stop received! ******************'\n";
   m_script += m_indent + "runningProcess.terminate()\n";
   m_script += m_indent + "sys.exit(0)\n\n";
}

void Script::implementCheckFileExistence()
{
   m_script += "def checkFileExistence(fileName):\n";
   m_script += m_indent + "try:\n";
   m_script += m_indent + m_indent + "with open(fileName):\n";
   m_script += m_indent + m_indent + m_indent + "return True\n";
   m_script += m_indent + "except IOError:\n";
   m_script += m_indent + m_indent + "return False\n\n";
}


void Script::implementFind()
{
   m_script += "def find(pattern, path):\n";
   m_script += m_indent + "result = []\n";
   m_script += m_indent + "for root,dirs,files in os.walk(path):\n";
   m_script += m_indent + m_indent + "for name in files:\n";
   m_script += m_indent + m_indent + m_indent + "if fnmatch.fnmatch(name, pattern):\n";
   m_script += m_indent + m_indent + m_indent + m_indent + "result.append(os.path.join(root, name))\n";
   m_script += m_indent + "return result\n\n";
}

void Script::implementExecute()
{
   m_script += "def execute(args):\n";

   m_script += m_indent + "global runningProcess\n";

   m_script += m_indent + "logger.debug(subprocess.list2cmdline(args))\n";
   m_script += m_indent + "runningProcess = subprocess.Popen(args,stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";

   m_script += m_indent + "stdout = ''\n";

   m_script += m_indent + "while True:\n";
   m_script += m_indent + m_indent + "line = runningProcess.stdout.readline()\n";
   m_script += m_indent + m_indent + "if line:\n";
   m_script += m_indent + m_indent + m_indent + "logger.debug(line.rstrip())\n";
   m_script += m_indent + m_indent + m_indent + "stdout = stdout + line\n";
   m_script += m_indent + m_indent + "if not line: break\n";

   m_script += m_indent + "while True:\n";
   m_script += m_indent + m_indent + "line = runningProcess.stderr.readline()\n";
   m_script += m_indent + m_indent + "if line:\n";
   m_script += m_indent + m_indent + m_indent + "logger.error(line.rstrip())\n";
   m_script += m_indent + m_indent + "if not line: break\n";

   m_script += m_indent + "runningProcess.wait()\n";

   if (m_stoppingIfError)
   {
      m_script += m_indent + "if runningProcess.returncode != 0 :\n";
      m_script += m_indent + m_indent + "logger.error('Error(s) occured, pipeline stopped!')\n";
      m_script += m_indent + m_indent + "sys.exit(1)\n";
   }   
   m_script += m_indent + "logger.debug('')\n\n";
   m_script += m_indent + "return stdout\n\n";
}

void Script::implementExecutePipe()
{
   m_script += "def executePipe(args1, args2):\n";

   m_script += m_indent + "global runningProcess\n";

   m_script += m_indent + "logger.debug(subprocess.list2cmdline(args1) + ' | ' + subprocess.list2cmdline(args2))\n";

   m_script += m_indent + "preProcess = subprocess.Popen(args1,stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";
   m_script += m_indent + "runningProcess = subprocess.Popen(args2,stdin=preProcess.stdout,stdout=subprocess.PIPE,stderr=subprocess.PIPE)\n";

   m_script += m_indent + "while True:\n";
   m_script += m_indent + m_indent + "line = runningProcess.stdout.readline()\n";
   m_script += m_indent + m_indent + "if line:\n";
   m_script += m_indent + m_indent + m_indent + "logger.debug(line.rstrip())\n";
   m_script += m_indent + m_indent + "if not line: break\n";

   m_script += m_indent + "while True:\n";
   m_script += m_indent + m_indent + "line = runningProcess.stderr.readline()\n";
   m_script += m_indent + m_indent + "if line:\n";
   m_script += m_indent + m_indent + m_indent + "logger.error(line.rstrip())\n";
   m_script += m_indent + m_indent + "if not line: break\n";

   m_script += m_indent + "runningProcess.wait()\n";

   if (m_stoppingIfError)
   {
      m_script += m_indent + "if runningProcess.returncode != 0 :\n";
      m_script += m_indent + m_indent + "logger.error('Error(s) occured, pipeline stopped!')\n";
      m_script += m_indent + m_indent + "sys.exit(1)\n";
   }
   m_script += m_indent + "logger.debug('')\n\n";
   m_script += m_indent + "stdout, stderr = runningProcess.communicate()\n";
   m_script += m_indent + "return stdout\n\n";
}

void Script::checkFinalOutputs()
{
   QMap<QString, QString>::iterator it = m_outputs.begin();   

   if(!m_overwriting)
   {
      QString checking = m_indent + "if ";

      if(!m_outputs.isEmpty())
      {
         for(it = m_outputs.begin(); it != m_outputs.end(); ++it )
         {
            m_script += m_indent + "" + it.key() + " = '" + it.value() + "'\n";
            
            checking += "checkFileExistence(" + it.key() + ")==True";

            if(it+1 != m_outputs.end())
            {
               checking += " and ";
            }
         }

         m_script += checking + ":\n"; 

         m_script += m_indent + m_indent + "logger.info('" + m_log + " -> Skipped')\n";
         m_script += m_indent + m_indent + "logger.info('')\n";
         m_script += m_indent + m_indent + "return\n\n";
      }
   }
   m_outputs.clear();
}

int Script::checkIndent(QString indent)
{
    size_t p = indent.toStdString().find_first_not_of(" \t");
    if( p == std::string::npos )
    {
        return 1 ;
    }
    else
    {
        return 0 ;
    }
}

void Script::setIndent(QString indent)
{
    if( !checkIndent(indent) )
    {
        m_indent = indent ;
    }
}

QString Script::getIndent()
{
    return m_indent ;
}

void Script::execute()
{
   QMap<QString, QString>::iterator it;
   QString indentation;

   if(!m_test.isEmpty())
   {
      m_script += m_indent + "if " + m_test + " :\n";
      indentation = m_indent + m_indent + "";
   }
   else
   {
      indentation = m_indent + "";
   }

   QString checking = indentation + "if ";

   if(!m_outputs.isEmpty())
   {
      // Outputs 
      for(it = m_outputs.begin(); it != m_outputs.end(); ++it )
      {
         m_script += indentation + it.key() + " = '" + it.value() + "'\n";
         
         checking += "checkFileExistence(" + it.key() + ")==False";

         if(it+1 != m_outputs.end())
         {
            checking += " or ";
         }
      }

      if(!m_overwriting)
      {
         m_script += checking + ":\n"; 
      }
   }

   if(!m_outputs.isEmpty() && !m_overwriting)
   {
      m_script += indentation + m_indent + "logger.info('" + m_log + "...')\n";

      // Inputs 
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         if(m_inputsTests.contains(it.key()))
         {
            m_script += indentation + m_indent + "if " + m_inputsTests[it.key()] + " :\n";
            m_script += indentation + m_indent + m_indent + "" + it.key() + " = '" + it.value() + "'\n";
         }      
         else
         {
            m_script += indentation + m_indent + "" + it.key() + " = '" + it.value() + "'\n";
         }
      }

      if(m_argsTests.size() > 0)
      {
         QMap<QString, QStringList>::iterator it_args;
         for(it_args = m_argsTests.begin(); it_args != m_argsTests.end(); ++it_args )
         {
            if(it_args == m_argsTests.begin())
            {
               m_script += indentation + m_indent + "if not " + it_args.key() + " :\n";
            } 

            else
            {
               m_script += indentation + m_indent + "elif not " + it_args.key() + " :\n";
            }

            m_script += indentation + m_indent + m_indent + "args = " + listToString(it_args.value()) + "\n";
            m_script += indentation + m_indent + m_indent + "execute(args)\n";
         }

         m_script += indentation + m_indent + "else :\n";
         m_script += indentation + m_indent + m_indent + "args = " + listToString(m_argumentsList) + "\n";

         if(!m_returnValue.isEmpty())
         {
            m_script += indentation + m_indent + m_indent + "" + m_returnValue + " = execute(args)\n";
         }
         else
         {
            m_script += indentation + m_indent + m_indent + "execute(args)\n";
         }
      }

      else
      {
         m_script += indentation + m_indent + "args = " + listToString(m_argumentsList) + "\n";
         if(!m_returnValue.isEmpty())
         {
            m_script += indentation + m_indent + "" + m_returnValue + " = execute(args)\n";
         }
         else
         {
            m_script += indentation + m_indent + "execute(args)\n";
         }
      }

      m_script += indentation + "else:\n";
      m_script += indentation + m_indent + "logger.info('" + m_log + " -> Skipped')\n\n";
   }

   else
   {
      m_script += indentation + "logger.info('" + m_log + "...')\n";

      // Inputs 
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         m_script += m_indent + "" + it.key() + " = '" + it.value() + "'\n";
      }

      m_script += indentation + "args = " + listToString(m_argumentsList) + "\n";
      if(!m_returnValue.isEmpty())
      {
         m_script += indentation + "" + m_returnValue + " = execute(args)\n";
      }
      else
      {
         m_script += indentation + "execute(args)\n";
      }
   }

   m_test.clear(); 
   m_inputs.clear(); 
   m_inputsTests.clear(); 
   m_outputs.clear(); 
   m_returnValue.clear(); 
   m_argumentsList.clear();
   m_argsTests.clear(); 
}

void Script::executePipe()
{
   QMap<QString, QString>::iterator it = m_outputs.begin();

   QString checking = m_indent + "if ";

   if(!m_outputs.isEmpty())
   {
      // Outputs 
      for(it = m_outputs.begin(); it != m_outputs.end(); ++it )
      {
         m_script += m_indent + "" + it.key() + " = '" + it.value() + "'\n";
         checking += "checkFileExistence(" + it.key() + ")==False";

         if(it+1 != m_outputs.end())
         {
            checking += " and ";
         }
      }

      if(!m_overwriting)
      {
         m_script += checking + ":\n"; 
      }
   }

   if(!m_outputs.isEmpty() && !m_overwriting)
   {
      m_script += m_indent + m_indent + "logger.info('" + m_log + "...')\n";

      // Inputs 
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         m_script += m_indent + m_indent + "" + it.key() + " = '" + it.value() + "'\n";
      }

      m_script += m_indent + m_indent + "args1 = " + listToString(m_argumentsList_1) + "\n";
      m_script += m_indent + m_indent + "args2 = " + listToString(m_argumentsList_2) + "\n";
      m_script += m_indent + m_indent + "executePipe(args1, args2)\n";

      m_script += m_indent + "else:\n";
      m_script += m_indent + m_indent + "logger.info('" + m_log + " -> Skipped')\n\n";

   }

   else
   {
      m_script += m_indent + "logger.info('" + m_log + "...')\n";

      // Inputs 
      for(it = m_inputs.begin(); it != m_inputs.end(); ++it )
      {
         m_script += m_indent + "" + it.key() + " = '" + it.value() + "'\n";
      }

      m_script += m_indent + "args1 = " + listToString(m_argumentsList_1) + "\n";
      m_script += m_indent + "args2 = " + listToString(m_argumentsList_2) + "\n";
      m_script += m_indent + "executePipe(args1, args2)\n";
   }

   m_inputs.clear(); 
   m_outputs.clear(); 
   m_argumentsList.clear();
   m_argumentsList_1.clear();
   m_argumentsList_2.clear();
}

void Script::addSubElement(QString element, QString pythonName, QString XMLname, QString value)
{
   m_script += m_indent + m_indent + "" + pythonName + " = SubElement(" + element + ", '" + XMLname + "')\n";
   m_script += m_indent + m_indent + "" + pythonName + ".text = '" + value + "'\n";
}

void Script::writeScript()
{
   QString script_path = m_processing_dir->filePath(m_script_name);

   std::ofstream* script_stream = new std::ofstream((script_path.toStdString()).c_str(), std::ios::out | std::ios::trunc);
   *script_stream << m_script.toStdString() << std::endl;

   script_stream->close();
}

void Script::cleanUp()
{
   for (int i = 0; i < m_unnecessaryFiles.size(); ++i)
   {
      QFile(m_unnecessaryFiles.at(i)).remove();
   }
}
