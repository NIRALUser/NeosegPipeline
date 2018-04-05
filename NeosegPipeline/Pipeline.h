#ifndef DEF_Pipeline
#define DEF_Pipeline

// General Librairies
#include <iostream>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <vector>
#include <utility>
#include <sys/stat.h>
#include <unistd.h>

// Qt Librairies
#include <QDir>
#include <QStringList>
#include <QProcess>
#include <QTime>
#include <QPlainTextEdit>

// My Specific Librairies
#include "PipelineParameters.h"
#include "PreProcessingData.h"
#include "DtiRegistration.h"
#include "AtlasPopulationRegistration.h"
#include "AtlasRegistration.h"
#include "AtlasGeneration.h"
#include "ExistingAtlasRegistration.h"
#include "NeosegExecution.h"
#include "XmlWriter.h"
#include "ABCExecution.h"


class Pipeline
{

   public:

   // Constructor
   Pipeline();

   // Destructor
   ~Pipeline(); 

   // Parameters
   void setPipelineParameters(PipelineParameters* parameters);
   void setPlainTextEdit(QPlainTextEdit* plainTextEdit); 
   void setIndent( QString indent );
   // Process
   QProcess* getMainScriptProcess();

   // Write XML Files
   void writeXMLFiles(); 

   // Write Pipeline 
   void writePipeline(); 
   void runPipeline();
   void stopPipeline(); 
   
   private: 

   // Create Directories
   void createProcessingDirectory();
   QString createModuleDirectory(QString directory_name);

   // Write Neoseg Pipeline Steps
   void writePreProcessingData();
   void writeAtlasRegistration();
   void writeAtlasPopulationRegistration();
   void writeDTIRegistration();   
   void writeAtlasGeneration();
   void writeExistingAtlasRegistration();
   void writeNeosegExecution();
   void writeABCExecution();

   // Write Main Script 
   void initializeMainScript();
   void initializeLogging();
   void defineSignalHandler();
   void writeMainScript();
   void executeMainScript();
   void copySegmentations();
   
   // Clean-Up
   void cleanUp(); 


   // Neoseg Pipeline Steps
   PreProcessingData* m_preProcessingData;
   DtiRegistration* m_dtiRegistration; 
   AtlasRegistration* m_atlasRegistration; 
   AtlasPopulationRegistration* m_atlasPopulationRegistration; 
   AtlasGeneration* m_atlasGeneration; 
   ExistingAtlasRegistration* m_existingAtlasRegistration; 
   NeosegExecution* m_neosegExecution; 
   ABCExecution* m_ABCExecution;

   PipelineParameters* m_parameters;  

   QString m_processing_name;
   QString m_processing_path;

   QString m_script;
   QString m_importingModules;
   QString m_runningModules;

   QString m_main_path;

   QString m_log_path; 

   QString m_jobID;
   QString m_indent;
   // QProcess
   QProcess* m_mainScriptProcess;

   // PlainTextEdit
   QPlainTextEdit* m_plainTextEdit; 
 
   // Timer
   QTime m_timer; 
};

#endif