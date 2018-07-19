#ifndef DEF_Script
#define DEF_Script

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <stdlib.h>

#include <QString>
#include <QDir>
#include <QMap>

#include "ExecutablePaths.h"
#include "Neo.h"

class Script
{
   public :
   
   // Constructor 
   Script(QString module); 

   // Set Functions
   void setProcessingDirectory(QString processing_path);
   void setExecutablePaths(ExecutablePaths* executables);
   void setModuleDirectory(QString module_path);
   void setSuffix(QString suffix);
   void setOverwriting(bool overwriting);
   void setStoppingIfError(bool stoppingIfError);
   void setNeo(Neo neo);
   void setIndent(QString indent);
   static int checkIndent(QString indent);
   QString getIndent();

   // Initialize Script 
   void definePython();
   void importGeneralModules();
   void importFusionModules();
   void importXmlModules();
   void defineExecutable(QString executable);

   // Implement Functions
   void implementStop();
   void implementCheckFileExistence();
   void implementExecute();
   void implementExecutePipe();
   void implementFind();

   // Execute Command Line
   void checkFinalOutputs(); 
   void execute();
   void executePipe();


   // Write XML Files 
   void addSubElement(QString element, QString pythonName, QString XMLname, QString value);

   // Write Script 
   QString str(QString str);
   QString variable(QString variable_name);
   QString listToString(QStringList argumentsList);
   void writeScript();

   // Clean-Up Module Directory 
   void cleanUp();

   protected :

   // Module & Script Names 
   QString m_module_name; 
   QString m_script_name; 

   // Processing Directory 
   QDir* m_processing_dir;
   
   // Module Directory
   QString m_module_path; 
   QDir* m_module_dir; 

   // Executable Paths 
   ExecutablePaths* m_executables; 

   // Neo
   Neo m_neo; 
   QString m_prefix; 
   QString m_suffix; 
   
   // Computation
   bool  m_overwriting; 
   bool  m_stoppingIfError; 


   // Execute parameters 
   QString m_test;
   QString m_indent ;

   QMap<QString, QString> m_inputs; 
   QMap<QString, QString> m_inputsTests; 
   QMap<QString, QStringList> m_argsTests; 

   QMap<QString, QString> m_outputs; 

   QString m_returnValue; 

   QStringList m_argumentsList; 
   QStringList m_argumentsList_1; 
   QStringList m_argumentsList_2; 
   QString m_log; 
   
   // Script 
   QString m_script;

   // Unnecessary Files
   QStringList m_unnecessaryFiles; 

};

#endif
