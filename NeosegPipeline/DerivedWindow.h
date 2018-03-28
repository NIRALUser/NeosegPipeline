#ifndef DEF_DerivedWindow
#define DEF_DerivedWindow

// General Librairies
#include <iostream>
#include <string>
#include <cstring>
#include <map>

// Qt Librairies
#include <QWidget>
#include <QtGui>  //MessageBox
#include <QMap>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QList>

// My Specific Librairies
#include "ui_WindowMultiseg.h"
#include "ui_About.h"
#include "ui_neosegParameters.h"
#include "ui_ABCParameters.h"
#include "ui_antsParameters.h"
#include "ui_quicksilverParameters.h"
#include "Pipeline.h"
#include "XmlReader.h"
#include "XmlWriter.h"
#include "MainScriptThread.h"
#include "ExecutablePaths.h" 
#include "LibraryPaths.h"
#include "About.h" 
#include "RegistrationParameters.h"

#ifndef NEOSEGPIPELINE_VERSION
#define NEOSEGPIPELINE_VERSION "unknown"
#endif

class DerivedWindow : public QMainWindow , public Ui_WindowMultiseg
{
   Q_OBJECT // Enable slots and signals

   struct Executable
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
      QPushButton* reset_button; 
   };

   struct Library
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
   };

   struct Image
   {
      QPushButton* select_button;
      QLineEdit* enter_lineEdit; 
   }; 

   public:

   // Constructor
   DerivedWindow();

   // Drag and drop
   void dragEnterEvent(QDragEnterEvent *event);
   void dropEvent(QDropEvent* event);

   // Set
   void setPipeline(Pipeline* pipeline);
   void setPipelineParameters(PipelineParameters* parameters);
   void setMainScriptThread(MainScriptThread* thread);

   // Print XML Errors
   void printErrors(QString errors);

   // Initialization
   void initializeImagesMap();
   void initializeExecutablesMap();
   void initializeLibrariesMap();
   void initializeXMLParameters();
   void initializeExecutables();
   void initializeDataParameters();

   // Connection 

   // Atlas Population
   void checkAtlases();
   void displayAtlases();
   void checkSelectedAtlases();

   // Parameter
   void setData();
   void setParameters(); 
   void setExecutables();


   public slots :

   // About 
   void about(); 

   // Output
   void selectOuput();
   void enterOutput();
   void createOutput(QString output);

   // Prefix 
   void enterPrefix(); 

   // Suffix
   void enterSuffix();

   // Images
   void selectImage(QString image); 
   void enterImage(QString image);

   // New or Existing Atlas
   void selectNewOrExistingAtlas();

   // New Atlas
   void selectAtlasPopulationDirectory();
   void enterAtlasPopulationDirectory();
   void resetAtlasPopulationDirectory();
   void UpdateAtlasPopulationDirectoryDisplay();
   void selectAtlas(QListWidgetItem* item);

   // Existing Atlas
   void selectExistingAtlas();
   void enterExistingAtlas();

   // Preprocessing Data //
   void changeUsingMaskDTI(bool) ;

   // Atlas Registration //
   void changeUsingMaskAtlas(bool) ;

   //Atlas Registration
   void registrationSoftwareSelection() ;

   //Tissue Segmentation
   void tissueSegmentationSoftwareSelection() ;

   // Computing System
   void changeComputingSystem(int index);

   // Executables/Data/Parameters
   void selectXMLFile(int XMLFileType ) ;
   void loadXMLFile(QString xmlFileName , int XMLFileType );

   // Parameters
   void saveParameters();  
   void saveExecutables();
   void saveData();

   // Executables 
   void selectExecutable(QString executable_name);
   void enterExecutable(QString executable_name);
   void resetExecutable(QString executable_name);
   void resetAllExecutables(); 

   // Executables 
   void selectLibrary(QString library_name);
   void enterLibrary(QString library_name);

   // Run Pipeline
   void runPipeline();

   // Logging
   void initializePipelineLogging();
   void initializeRegistrationsLogging();

   void printPipelineLog();    
   void printRegistrationLog(QString file_name); 

   void selectLog();
   void changeExecutionPlainTextEdit();

   // Enable widgets 
   void setParametersWidgetEnabled(bool enabled);
   
   // Stop Pipeline
   void stopPipeline(); 

   // Display Results
   void enableDisplayButton();
   void displayResults(); 

   // Exit Application 
   void closeEvent(QCloseEvent *event);

   //ABC dynamic UI
   void addNumbersOfPriorsForABC(std::vector<double> labelsvalue) ;
   //ABC dynamic UI
   void removeNumbersOfPriorsForABC(int nbPriors) ;
   //ABC dynamic UI
   void updateNumbersOfPriorsForABC() ;
   void comboBoxOutputImageFormat_currentIndexChanged(const QString &arg1);

   void pushButtonRefreshPriors();

private :
   
   // Window
   Ui_WindowMultiseg ui;
   Ui::ABCParameters *abcParameters ;
   Ui::neosegParameters *neosegParameters ;
   Ui::antsParameters *antsParameters ;
   Ui::quicksilverParameters *quicksilverParameters ;
   // Pipeline 
   Pipeline* m_pipeline;

   // Parameters
   PipelineParameters* m_parameters;
   RegistrationParameters* m_antsParameters_DTI; 
   /*AntsParameters* m_antsParameters_atlas; 
   QuicksilverParameters* m_quicksilverParameters;*/
   RegistrationParameters* m_registrationParameters_atlas;
   NeosegParameters* m_neosegParameters; 

   // Executables 
   ExecutablePaths* m_executables; 

   // Libraries
   LibraryPaths* m_libraries;

   QString m_tests_path;
   QString m_data_path;
   QString m_existingAtlases_path;
   QStringList m_goodAtlases;
   QStringList m_wrongAtlases;  
   QStringList m_selectedAtlases;

   //ABC dynamic UI
   class PriorSpinBox : public QHBoxLayout{
   public:
       PriorSpinBox(int n, double labelvalue){

           labelsp = new QLabel();
           if(labelvalue != -1){
               labelsp->setText( QString("Prior %1 coefficient for label = " + QString::number(labelvalue) + ":").arg(n) ) ;
           }else{
               labelsp->setText( QString("Prior %1 coefficient for label = rest:").arg(n) ) ;
           }
           m_LabelValue = labelvalue;
           m_Index = n - 1;

           this->addWidget(labelsp );

           dspin = new QDoubleSpinBox();
           dspin->setSingleStep(0.1);
           dspin->setMinimum(0);
           dspin->setValue(1);
           this->addWidget(dspin);

           if(labelvalue != -1){
               checkboxIslands = new QCheckBox();
               checkboxIslands->setText("Reassign islands.");
               checkboxIslands->setToolTip(QString("Reassign isolated regions in the volume using a threshold for the number of voxels in the region."));
               checkboxIslands->setChecked(false);
               this->addWidget(checkboxIslands);


               spinBoxIslands = new QDoubleSpinBox();
               spinBoxIslands->setSingleStep(1.0);
               spinBoxIslands->setMinimum(0.0);
               spinBoxIslands->setMaximum(10000.0);
               spinBoxIslands->setValue(100.0);
               spinBoxIslands->setToolTip(QString("Set the threshold for the number of voxels in the region (island). If the number of voxels in the island is inferior, all voxels are reassigned to the second highest probability."));
               spinBoxIslands->setEnabled(false);
               this->addWidget(spinBoxIslands);

               connect(checkboxIslands, SIGNAL(clicked(bool)), spinBoxIslands, SLOT(setEnabled(bool)));

               checkBoxVoxelByVoxel = new QCheckBox();
               checkBoxVoxelByVoxel->setText("Voxel by voxel.");
               checkBoxVoxelByVoxel->setToolTip(QString("Replace the labels in the island voxel by voxel or compute the average probability in the region. The next highest probability is assigned to the whole region."));
               checkBoxVoxelByVoxel->setChecked(true);
               checkBoxVoxelByVoxel->setEnabled(false);
               this->addWidget(checkBoxVoxelByVoxel);

               connect(checkboxIslands, SIGNAL(clicked(bool)), checkBoxVoxelByVoxel, SLOT(setEnabled(bool)));
           }else{
               checkboxIslands = 0;
               spinBoxIslands = 0;
               checkBoxVoxelByVoxel = 0;
           }
       }

       ~PriorSpinBox(){
           delete labelsp;
           delete dspin;
           delete checkboxIslands;
           delete spinBoxIslands;
       }
       QLabel *labelsp;
       QDoubleSpinBox *dspin;
       QCheckBox* checkboxIslands;
       QDoubleSpinBox* spinBoxIslands;
       QCheckBox* checkBoxVoxelByVoxel;
       double m_LabelValue;
       int m_Index;
   };
   std::vector<PriorSpinBox*> m_VectorABCPriorCheckBoxes ;

   bool m_parametersSet;
   bool m_executablesSet;
   bool m_pipelineWriten; 
   //bool m_softwareRegistration;
   QString m_abcOutputImageFormat;

   MainScriptThread* m_thread; 

   QMap<QString, Image> m_images_map; 
   QMap<QString, Executable> m_executables_map; 
   QMap<QString, Library> m_libraries_map; 

   struct Logging
   {
      QTextStream* textStream;
      QPlainTextEdit* plainTextEdit; 
   };

   QTextStream* m_log_textStream; 

   QMap<QString, Logging> m_registrationLoggings; 

   About* m_about; 
   

};

#endif
