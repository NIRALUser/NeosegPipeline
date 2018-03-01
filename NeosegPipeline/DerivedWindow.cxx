#include "DerivedWindow.h"
#include "CountNumberOfLabels.h"

DerivedWindow::DerivedWindow() : Ui_WindowMultiseg()
{
   setupUi(this);
   setAcceptDrops(true);

   neosegParameters = new Ui::neosegParameters ;
   neosegParameters->setupUi( this->softwareFrame ) ;
   abcParameters = new Ui::ABCParameters ;
   abcParameters->setupUi(this->softwareFrame);
   this->radioNeoseg->setChecked( true ) ;
   this->radioABC->setChecked( false ) ;

   antsParameters = new Ui::antsParameters ;
   antsParameters->setupUi( this->registrationsoftwareFrame ) ; 
   quicksilverParameters = new Ui::quicksilverParameters ;
   quicksilverParameters->setupUi(this->registrationsoftwareFrame) ;
   this->radioAnts->setChecked( true ) ;
   this->radioQuicksilver->setChecked( false ) ;

   m_parametersSet = false; 
   m_executablesSet = false; 
   m_pipelineWriten = false; 

   // Select Image Signal Mapper
   QSignalMapper* selectXMLFile_signalMapper = new QSignalMapper(this);
   connect(selectXMLFile_signalMapper, SIGNAL(mapped(int)), this, SLOT(selectXMLFile(int)));
   selectXMLFile_signalMapper->setMapping(loadDataConfiguration_action, 0);
   connect(loadDataConfiguration_action, SIGNAL(triggered()), selectXMLFile_signalMapper, SLOT(map()));
   selectXMLFile_signalMapper->setMapping(loadParametersConfiguration_action, 1);
   connect(loadParametersConfiguration_action, SIGNAL(triggered()), selectXMLFile_signalMapper, SLOT(map()));
   selectXMLFile_signalMapper->setMapping(loadSoftwaresConfiguration_action, 2);
   connect(loadSoftwaresConfiguration_action, SIGNAL(triggered()), selectXMLFile_signalMapper, SLOT(map()));
   // Data Configuration
   connect(saveDataConfiguration_action, SIGNAL(triggered()), this, SLOT(saveData()));

   // Parameters Configuration 
   connect(saveParametersConfiguration_action, SIGNAL(triggered()), this, SLOT(saveParameters()));

   // Executables Configuration
   connect(saveSoftwaresConfiguration_action, SIGNAL(triggered()), this, SLOT(saveExecutables()));

   // Quit
   connect(quit_action, SIGNAL(triggered()), this, SLOT(close())); 

   // Help
   connect(about_action, SIGNAL(triggered()), this, SLOT(about())); 

   // Toolbar 
   QWidget* spacer = new QWidget();
   spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
   toolBar->addWidget(spacer);
   toolBar->addAction(about_action);

   // Select Image Signal Mapper
   QSignalMapper* selectImage_signalMapper = new QSignalMapper(this);
   connect(selectImage_signalMapper, SIGNAL(mapped(QString)), this, SLOT(selectImage(QString)));

   // Enter Image Signal Mapper
   QSignalMapper* enterImage_signalMapper = new QSignalMapper(this);
   connect(enterImage_signalMapper, SIGNAL(mapped(QString)), this, SLOT(enterImage(QString)));

   // Connect Images Signal/Slot
   initializeImagesMap();
   QMap<QString, Image>::iterator it_images; 
   for(it_images = m_images_map.begin(); it_images != m_images_map.end(); ++it_images)
   {
      QString name = it_images.key(); 
      Image image = it_images.value();  

      selectImage_signalMapper->setMapping(image.select_button, name);
      connect(image.select_button, SIGNAL(clicked()), selectImage_signalMapper, SLOT(map())); 

      enterImage_signalMapper->setMapping(image.enter_lineEdit, name);
      connect(image.enter_lineEdit, SIGNAL(editingFinished()), enterImage_signalMapper, SLOT(map())); 
   }

   // Select Executable Signal Mapper
   QSignalMapper* selectExecutable_signalMapper = new QSignalMapper(this);
   connect(selectExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(selectExecutable(QString)));

   // Enter Executable Signal Mapper
   QSignalMapper* enterExecutable_signalMapper = new QSignalMapper(this);
   connect(enterExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(enterExecutable(QString)));

   // Reset Executable Signal Mapper
   QSignalMapper* resetExecutable_signalMapper = new QSignalMapper(this);
   connect(resetExecutable_signalMapper, SIGNAL(mapped(QString)), this, SLOT(resetExecutable(QString)));

   // Connect Executables Signal/Slot
   initializeExecutablesMap(); 
   QMap<QString, Executable>::iterator it_exec; 
   for(it_exec = m_executables_map.begin(); it_exec != m_executables_map.end(); ++it_exec)
   {
      QString name = it_exec.key(); 
      Executable executable = it_exec.value();  

      selectExecutable_signalMapper->setMapping(executable.select_button, name);
      connect(executable.select_button, SIGNAL(clicked()), selectExecutable_signalMapper, SLOT(map())); 

      enterExecutable_signalMapper->setMapping(executable.enter_lineEdit, name);
      connect(executable.enter_lineEdit, SIGNAL(editingFinished()), enterExecutable_signalMapper, SLOT(map())); 

      resetExecutable_signalMapper->setMapping(executable.reset_button, name);
      connect(executable.reset_button, SIGNAL(clicked()), resetExecutable_signalMapper, SLOT(map()));      
   }

   // Select Library Signal Mapper
   QSignalMapper* selectLibrary_signalMapper = new QSignalMapper(this);
   connect(selectLibrary_signalMapper, SIGNAL(mapped(QString)), this, SLOT(selectLibrary(QString)));

   // Enter Library Signal Mapper
   QSignalMapper* enterLibrary_signalMapper = new QSignalMapper(this);
   connect(enterLibrary_signalMapper, SIGNAL(mapped(QString)), this, SLOT(enterLibrary(QString)));

   // Connect Library Signal/Slot
   initializeLibrariesMap(); 
   QMap<QString, Library>::iterator it_lib; 
   for(it_lib = m_libraries_map.begin(); it_lib != m_libraries_map.end(); ++it_lib)
   {
      QString name = it_lib.key(); 
      Library library = it_lib.value();  

      selectLibrary_signalMapper->setMapping(library.select_button, name);
      connect(library.select_button, SIGNAL(clicked()), selectLibrary_signalMapper, SLOT(map())); 

      enterLibrary_signalMapper->setMapping(library.enter_lineEdit, name);
      connect(library.enter_lineEdit, SIGNAL(editingFinished()), enterLibrary_signalMapper, SLOT(map())); 
   }

   // OutputDir //
   connect(output_button, SIGNAL(clicked()), this, SLOT(selectOuput()));
   connect(output_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterOutput()));

   // Prefix // 
   connect(prefix_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterPrefix()));

   // Suffix //
   connect(suffix_lineEdit, SIGNAL(returnPressed()), this, SLOT(enterSuffix()));   

   // New Or Existing Atlas //
   connect(newAtlas_radioButton, SIGNAL(clicked()), this, SLOT(selectNewOrExistingAtlas()));
   connect(existingAtlas_radioButton, SIGNAL(clicked()), this, SLOT(selectNewOrExistingAtlas()));

   // New Atlas //
   connect(atlasPopulationDirectory_button, SIGNAL(clicked()), this, SLOT(selectAtlasPopulationDirectory())); 
   connect(resetAtlasPopulationDirectory_button, SIGNAL(clicked()), this, SLOT(resetAtlasPopulationDirectory()));
   connect(atlasPopulationDirectory_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterAtlasPopulationDirectory()));
   connect(atlasPopulation_listWidget,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(selectAtlas(QListWidgetItem*)));

   // Old Atlas // 
   connect(existingAtlas_button, SIGNAL(clicked()), this, SLOT(selectExistingAtlas()));
   connect(existingAtlas_lineEdit, SIGNAL(editingFinished()), this, SLOT(enterExistingAtlas()));      
   
   // Preprocessing Data //
   connect(usingMask_DTI_checkBox, SIGNAL(clicked(bool)), this, SLOT( changeUsingMaskDTI(bool) ) ) ;

   // Atlas Registration //
   connect(this->antsParameters->usingMask_atlas_checkBox, SIGNAL(clicked(bool)), this, SLOT( changeUsingMaskAtlas(bool) ) ) ;

   // Computing System
   connect(computingSystem_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeComputingSystem(int))); 

   // Reset all executables 
   connect(resetAllExecutables_button, SIGNAL(clicked()), this, SLOT(resetAllExecutables()));

   // Run Pipeline
   connect(runPipeline_button, SIGNAL(clicked()), this, SLOT(runPipeline()));

   // Logging 
   connect(pipeline_radioButton, SIGNAL(clicked()), this, SLOT(selectLog())); 
   connect(registrations_radioButton, SIGNAL(clicked()), this, SLOT(selectLog())); 

   // Stop Pipeline
   connect(stopPipeline_button, SIGNAL(clicked()), this, SLOT(stopPipeline()));

   // Display Results  
   connect(displayResults_button, SIGNAL(clicked()), this, SLOT(displayResults())); 
   
   // Display ANTS options or Quicksilver options in "Atlas Registration" tab
   connect( radioAnts, SIGNAL(clicked()), this, SLOT(registrationSoftwareSelection()) );
   connect( radioQuicksilver, SIGNAL(clicked()), this, SLOT(registrationSoftwareSelection()) );

   // Display ABC options or neoseg options in "Tissue Segmentation" tab
   connect( radioNeoseg, SIGNAL(clicked()), this, SLOT(tissueSegmentationSoftwareSelection()) ) ;
   connect( radioABC, SIGNAL(clicked()), this, SLOT(tissueSegmentationSoftwareSelection()) ) ;

   connect(this->abcParameters->pushButtonRefreshPriors, SIGNAL(clicked()), this, SLOT(pushButtonRefreshPriors()));
   connect(this->abcParameters->comboBoxOutputImageFormat, SIGNAL(currentIndexChanged(QString)), this, SLOT(comboBoxOutputImageFormat_currentIndexChanged(QString)));

   numberOfRegistrations_spinBox->setEnabled(true); 
   numberOfGB_spinBox->setEnabled(false); 

   stopPipeline_action->setEnabled(false);

   newAtlas_radioButton->setChecked(true);
   existingAtlas_widget->hide();

   runPipeline_progressBar->hide(); 

   displayResults_button->setEnabled(false);
   displayResults_action->setEnabled(false);

   stopPipeline_button->setEnabled(false);

   pipeline_radioButton->setEnabled(false);  
   registrations_radioButton->setEnabled(false);
   registrations_comboBox->setEnabled(false); 

   this->adjustSize();
   // We run the function "tissueSegmentationSoftwareSelection" once to initialize the display.
   radioABC->setChecked(true);
   tissueSegmentationSoftwareSelection() ;

   radioAnts->setChecked(true);
   registrationSoftwareSelection() ;
}


void DerivedWindow::addNumbersOfPriorsForABC(std::vector< double > labelsvalue)
{


    for(int i = 0; i < (int)labelsvalue.size(); i++ ){

        PriorSpinBox* psb = new PriorSpinBox(m_VectorABCPriorCheckBoxes.size() + 1, labelsvalue[i]);
        m_VectorABCPriorCheckBoxes.push_back( psb ) ;

//            QHBoxLayout *postphlayout = new QHBoxLayout ;
//            QCheckBox *reassignSmallIsland = new QCheckBox ;
//            postphlayout->addWidget(reassignSmallIsland);
//            postphlayout->addSpacerItem(new QSpacerItem(10,0, QSizePolicy::Expanding, QSizePolicy::Ignored));
//            QSpinBox *spin = new QSpinBox ;
//            postphlayout->addWidget(spin);
//            postphlayout->setParent(abcParameters->ppFrame) ;


        abcParameters->nbPriorLayout->addLayout(psb);
    }
    this->repaint();
}

void DerivedWindow::removeNumbersOfPriorsForABC(int nbPriors){
    int start = m_VectorABCPriorCheckBoxes.size() - 1;
    int end = m_VectorABCPriorCheckBoxes.size() - nbPriors;
    for(int i = start; i >= end && i >= 0; i--){
        delete m_VectorABCPriorCheckBoxes[i];
        m_VectorABCPriorCheckBoxes.pop_back();
    }
}

void DerivedWindow::updateNumbersOfPriorsForABC()
{
    std::vector<int> numberOfLabels;

    m_parameters->setSelectedAtlases(m_selectedAtlases);
    m_parameters->initializeAtlasPopulation();
    std::vector<Atlas> atlas = m_parameters->getAtlasPopulation();

    CountNumberOfLabels* count = new CountNumberOfLabels();
    try{
        for(int i = 0; i < (int)atlas.size(); i++){
            Atlas a = atlas[i];
            std::string fname = a.seg.toStdString();
            if(fname.compare("")!=0){
                count->SetFilename(fname);
                count->Update();
                numberOfLabels.push_back(count->GetNumberOfLabels());
                if(i == 0){//In this step we assume all the labels in the images have the same value for the same structure. That's why only the first map is ok in this step.
                    //The type of labels for each image will be checked during the pipeline execution -> Check WeightedLabelsAverage implementation.
                    m_parameters->setImageLabelMap(count->GetImageLabelMap());
                }
            }else{
                numberOfLabels.push_back(4);//white, gray, csf, rest
            }
        }
    }catch(itk::ExceptionObject exp){
        std::cout<<exp.GetDescription()<<endl;
    }

    int first = 0;
    if(numberOfLabels.size() > 0){
        first = numberOfLabels[0];
    }
    for(int i = 1; i < (int)numberOfLabels.size(); i++){
        if(numberOfLabels[i] != first){
            QMessageBox msgBox;
            msgBox.setText("A different number of labels has been detected in the Atlas population.");
            msgBox.exec();
            cerr<<"Number of labels :";
            for(int j = 0; j < (int)numberOfLabels.size(); j++){
              cerr<< numberOfLabels[j] <<",";
            }
            cerr<<endl;
            break;
        }
    }
    delete count;

    PipelineParameters::InputImageLabelMapType labelmap = m_parameters->getImageLabelMap();
    PipelineParameters::InputImageLabelMapIteratorType itmap;

    int newlabels = first - m_VectorABCPriorCheckBoxes.size();//New labels to add or remove.
    if(newlabels > 0){
        std::vector< double > labelsvalue;

        for(itmap = labelmap.begin(); itmap != labelmap.end(); ++itmap){
            if(itmap->first != 0){
                labelsvalue.push_back(itmap->first);
            }
        }
        labelsvalue = std::vector< double >(labelsvalue.begin() + m_VectorABCPriorCheckBoxes.size(), labelsvalue.end());

        if(m_VectorABCPriorCheckBoxes.size() == 0){
            labelsvalue.push_back(-1);
        }

        addNumbersOfPriorsForABC(labelsvalue);
    }else if(newlabels < 0){
        removeNumbersOfPriorsForABC(newlabels);
    }

    this->comboBoxLabelValueFA->clear();

    if(labelmap.size() > 0){
        for(itmap  = labelmap.begin(); itmap != labelmap.end(); ++itmap){
            if(itmap->first != 0){
                this->comboBoxLabelValueFA->addItem(QString::number(itmap->first));
            }
        }
    }else{
        this->comboBoxLabelValueFA->addItem(QString("white"));
    }


    this->abcParameters->lineEditNbPriors->setText(QString::number(first));
}

void DerivedWindow::tissueSegmentationSoftwareSelection()
{
    if( this->radioNeoseg->isChecked() )
    {
        neosegParameters->frame->show() ;
        abcParameters->frame->hide() ;
    }
    else
    {
        neosegParameters->frame->hide() ;
        abcParameters->frame->show() ;
    }
}

void DerivedWindow::registrationSoftwareSelection()
{
    if(this->radioAnts->isChecked() )
    {
        antsParameters->registration_frame->show() ;
        quicksilverParameters->registration_frame->hide() ;
        m_registrationParameters_atlas->setUsingAnts();
    }
    else
    {
        antsParameters->registration_frame->hide() ;
        quicksilverParameters->registration_frame->show() ;
        m_registrationParameters_atlas->setUsingQuicksilver();
    }
}

void DerivedWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void DerivedWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();
    // check for needed mime type, a file or a list of files
    if( mimeData->hasUrls() )
    {
        QList<QUrl> urlList = mimeData->urls();
        // extract the local paths of the files
        for (int i = 0; i < urlList.size(); ++i)
        {
            QFileInfo currentFile ( urlList.at(i).toLocalFile() );
            if ( !currentFile.completeSuffix().compare( "xml" , Qt::CaseInsensitive ) ) // load xml file
            {
                QString errors ;
                QString filename = currentFile.absoluteFilePath() ;
                XmlReader xmlReader ;
                PipelineParameters parameters ;//local variable to find which kind of XML file was given (Data/Parameters/Executables)
                xmlReader.setPipelineParameters( &parameters ) ;
                errors = xmlReader.readDataConfigurationFile( filename ) ;
                if( errors.isEmpty() )
                {
                    loadXMLFile( filename , 0 ) ;
                    continue ;
                }
                errors = xmlReader.readParametersConfigurationFile( filename ) ;
                if( errors.isEmpty() )
                {
                    loadXMLFile( filename , 1 ) ;
                    continue ;
                }
                errors = xmlReader.readExecutablesConfigurationFile( filename ) ;
                if( errors.isEmpty() )
                {
                    loadXMLFile( filename , 2 ) ;
                    continue ;
                }
                printErrors( errors ) ;
            }
        }
    } // if mimedata hasurls
} // dropEvent

void DerivedWindow::setPipelineParameters(PipelineParameters* parameters)
{  
   m_parameters = parameters;
   //m_quicksilverParameters = m_parameters->getQuicksilverParameters();
   m_antsParameters_DTI = m_parameters->getAntsParametersDTI(); 
   //m_antsParameters_atlas = m_parameters->getAntsParametersAtlas(); 
   m_registrationParameters_atlas = m_parameters->getRegistrationParameters();
   m_neosegParameters = m_parameters->getNeosegParameters(); 
   m_executables = m_parameters->getExecutablePaths();
   m_libraries = m_parameters->getLibraryPaths();
   initializeXMLParameters();
   initializeDataParameters();
   initializeExecutables();
   updateNumbersOfPriorsForABC();
}

void DerivedWindow::setPipeline(Pipeline* pipeline) 
{
   m_pipeline = pipeline;
}

void DerivedWindow::setMainScriptThread(MainScriptThread* thread) 
{
   m_thread = thread;
   connect(m_thread, SIGNAL(finished()), this, SLOT(enableDisplayButton()));
}

void DerivedWindow::printErrors(QString errors)
{
   QMessageBox::critical(this, tr("Errors in XML file(s)"), errors);   
}

void DerivedWindow::initializeImagesMap()
{
   Image T1 = {T1_button, T1_lineEdit}; 
   m_images_map.insert("T1", T1);

   Image T2 = {T2_button, T2_lineEdit}; 
   m_images_map.insert("T2", T2);

   Image mask = {mask_button, mask_lineEdit}; 
   m_images_map.insert("mask", mask);

   Image DWI = {DWI_button, DWI_lineEdit}; 
   m_images_map.insert("DWI", DWI);
}

void DerivedWindow::initializeExecutablesMap()
{
   Executable python = {python_button, python_lineEdit, resetPython_button};
   m_executables_map.insert("python", python);

   Executable SegPostProcessCLP = {SegPostProcessCLP_button, SegPostProcessCLP_lineEdit, resetSegPostProcessCLP_button};
   m_executables_map.insert("SegPostProcessCLP", SegPostProcessCLP);

   Executable N4ITKBiasFieldCorrection = {N4ITKBiasFieldCorrection_button, N4ITKBiasFieldCorrection_lineEdit, resetN4ITKBiasFieldCorrection_button};
   m_executables_map.insert("N4ITKBiasFieldCorrection", N4ITKBiasFieldCorrection);

   Executable ITKTransformTools = {ITKTransformTools_button, ITKTransformTools_lineEdit, resetITKTransformTools_button};
   m_executables_map.insert("ITKTransformTools", ITKTransformTools);

   Executable bet2 = {bet2_button, bet2_lineEdit, resetBet2_button};
   m_executables_map.insert("bet2", bet2);

   Executable unu = {unu_button, unu_lineEdit, resetUnu_button};
   m_executables_map.insert("unu", unu);

   Executable dtiestim = {dtiestim_button, dtiestim_lineEdit, resetDtiestim_button};
   m_executables_map.insert("dtiestim", dtiestim);

   Executable dtiprocess = {dtiprocess_button, dtiprocess_lineEdit, resetDtiprocess_button};
   m_executables_map.insert("dtiprocess", dtiprocess);

   Executable ANTS = {ANTS_button, ANTS_lineEdit, resetANTS_button};
   m_executables_map.insert("ANTS", ANTS);

   Executable ResampleScalarVectorDWIVolume = {ResampleScalarVectorDWIVolume_button, ResampleScalarVectorDWIVolume_lineEdit, resetResampleScalarVectorDWIVolume_button};
   m_executables_map.insert("ResampleScalarVectorDWIVolume", ResampleScalarVectorDWIVolume);

   Executable ImageMath = {ImageMath_button, ImageMath_lineEdit, resetImageMath_button};
   m_executables_map.insert("ImageMath", ImageMath);

   Executable WeightedLabelsAverage = {WeightedLabelsAverage_button, WeightedLabelsAverage_lineEdit, resetWeightedLabelsAverage_button};
   m_executables_map.insert("WeightedLabelsAverage", WeightedLabelsAverage);

   Executable SpreadFA = {SpreadFA_button, SpreadFA_lineEdit, resetSpreadFA_button};
   m_executables_map.insert("SpreadFA", SpreadFA);

   Executable ReassignWhiteMatter = {ReassignWhiteMatter_button, ReassignWhiteMatter_lineEdit, resetReassignWhiteMatter_button};
   m_executables_map.insert("ReassignWhiteMatter", ReassignWhiteMatter);

   Executable Neoseg = {Neoseg_button, Neoseg_lineEdit, resetNeoseg_button};
   m_executables_map.insert("neoseg", Neoseg);

   Executable InsightSNAP = {InsightSNAP_button, InsightSNAP_lineEdit, resetInsightSNAP_button};
   m_executables_map.insert("InsightSNAP", InsightSNAP);

   Executable ABC = {ABC_button, ABC_lineEdit, resetABC_button};
   m_executables_map.insert("ABC", ABC);
}

void DerivedWindow::initializeLibrariesMap()
{
   Library FSL = {FSL_button, FSL_lineEdit};
   m_libraries_map.insert("FSL", FSL);
}


void DerivedWindow::about()
{
   m_about = new::About();   
   m_about->setVersion(NEOSEGPIPELINE_VERSION); 
   m_about->show();  
}


void DerivedWindow::selectImage(QString image_name)
{
   Image image = m_images_map[image_name]; 

   QString image_path = (image.enter_lineEdit)->text();

   QString dir_path = ""; 
   if(!(image_path.isEmpty()))
   {
      dir_path = (QFileInfo(image_path).dir()).absolutePath(); 
   }

   image_path = QFileDialog::getOpenFileName(this, "Open file", dir_path,"Images (*.gipl *.gipl.gz *.nhdr *.nrrd *.mha *.mhd *.hdr *.nii *.nii.gz);; All files (*.*)");

   if(!image_path.isEmpty())
   {
      (image.enter_lineEdit)->setText(image_path);
   }
}
void DerivedWindow::enterImage(QString image_name)
{
   Image image = m_images_map[image_name];

   QString image_path = (image.enter_lineEdit)->text();

   if(!image_path.isEmpty()) 
   {    
      if(QFileInfo(image_path).exists())
      {
         if(!QFileInfo(image_path).isFile())
         {
            (image.enter_lineEdit)->clear(); 
            QMessageBox::critical(this, image_name, image_path + tr(" is not a file,\nPlease enter a new file path"));
         }
      }
      else
      {
         (image.enter_lineEdit)->clear(); 
         QMessageBox::critical(this, image_name, image_path + tr(" does not exist,\nPlease enter a new file path"));

      }
   }
}

//***** Ouput *****//
void DerivedWindow::selectOuput()
{
   QString output = QFileDialog::getExistingDirectory (this, "Open Directory",m_tests_path , QFileDialog::ShowDirsOnly);
   if(!(output.isEmpty()))
   {
      output_lineEdit->setText(output);
   }
}
void DerivedWindow::enterOutput()
{   
   QString output = output_lineEdit->text();

   if(!output.isEmpty()) 
   {
      if(!m_parameters->checkOutput(output))
      {
         createOutput(output); 
      }
   }
}

void DerivedWindow::createOutput(QString output)
{
   QMessageBox messageBox(this);
   messageBox.setText(output + tr(" does not exist,"));
   messageBox.setInformativeText(tr("Do you want to create it?"));
   messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
   messageBox.setDefaultButton(QMessageBox::Yes);
   int answer = messageBox.exec();
 
   QString output_path = QFileInfo(output).absoluteFilePath();
   bool result;

   switch (answer)
   {
      case QMessageBox::Yes:
         result = (QDir::root()).mkpath(output_path);
         if(!result)
         {
            QMessageBox::critical(this, tr("Output directory"), output_path + tr("\n can not be created,\n Please enter a new directory path"));             
         }
         break;
      case QMessageBox::No:
         //output_lineEdit->clear();
         break;
      case QMessageBox::Cancel:
         //output_lineEdit->clear();
         break;
   }
}

// Prefix 
void DerivedWindow::enterPrefix()
{
   QString prefix = prefix_lineEdit->text();

   if(!prefix.isEmpty()) 
   {
      if(!m_parameters->checkPrefixSuffix(prefix))
      {
         QMessageBox::critical(this, tr("Prefix"), prefix + tr(" is not a valid prefix,\nIt can not contains ") + m_parameters->getForbiddenCharacters() + tr(",\nPlease enter a new prefix"));  
         prefix_lineEdit->clear();           
      }
   }
}

// Suffix
void DerivedWindow::enterSuffix()
{
   QString suffix = suffix_lineEdit->text();

   if(!suffix.isEmpty()) 
   {
      if(!m_parameters->checkPrefixSuffix(suffix))
      {
         QMessageBox::critical(this, tr("Suffix"), suffix + tr(" is not a valid suffix,\nIt can not contains ") + m_parameters->getForbiddenCharacters() + tr(",\nPlease enter a new suffix"));  
         suffix_lineEdit->clear();               
      }
   }
}

//***** New Atlas/Existing Atlas *****//
void DerivedWindow::selectNewOrExistingAtlas()
{
   if(newAtlas_radioButton->isChecked())
   {
      newAtlas_widget->show();
      existingAtlas_widget->hide();

      atlasPopulationDirectory_lineEdit->setText(m_parameters->getAtlasPopulationDirectory()); 
      checkAtlases();
      displayAtlases();
      m_selectedAtlases = m_goodAtlases;
      checkSelectedAtlases();
      atlasGeneration_tab->setDisabled( false ) ;
   } 

   if(existingAtlas_radioButton->isChecked())
   {
      newAtlas_widget->hide();
      existingAtlas_widget->show();
      atlasGeneration_tab->setDisabled( true ) ;
   } 

   tabs->adjustSize();
}


//***** Atlas Population Directory *****//

void DerivedWindow::UpdateAtlasPopulationDirectoryDisplay()
{
    checkAtlases() ;
    displayAtlases() ;
    m_selectedAtlases = m_goodAtlases ;
    checkSelectedAtlases() ;
}

void DerivedWindow::selectAtlasPopulationDirectory()
{
   QString atlasPopulationDirectory = QFileDialog::getExistingDirectory (this, tr("Open Directory"), atlasPopulationDirectory_lineEdit->text(), QFileDialog::ShowDirsOnly);
   atlasPopulationDirectory_lineEdit->setText(atlasPopulationDirectory);
   m_parameters->setAtlasPopulationDirectory( atlasPopulationDirectory_lineEdit->text() ) ;
   UpdateAtlasPopulationDirectoryDisplay() ;
    updateNumbersOfPriorsForABC();
}

void DerivedWindow::enterAtlasPopulationDirectory()
{
   QString atlasPopulationDirectory = atlasPopulationDirectory_lineEdit->text();

   if(!atlasPopulationDirectory.isEmpty()) 
   {
      if(!m_parameters->checkAtlasPopulationDirectory(atlasPopulationDirectory))
      {
         atlasPopulationDirectory_lineEdit->clear();
         QMessageBox::critical(this, tr("Population atlas directory"), atlasPopulationDirectory + tr("\ndoes not exist, enter a new directory path"));
      }
      else
      {
          m_parameters->setAtlasPopulationDirectory( atlasPopulationDirectory_lineEdit->text() ) ;
      }
   }
   UpdateAtlasPopulationDirectoryDisplay() ;
}

void DerivedWindow::resetAtlasPopulationDirectory()
{
   atlasPopulationDirectory_lineEdit->setText(m_parameters->getAtlasPopulationDirectory()); 
   UpdateAtlasPopulationDirectoryDisplay() ;
}


//***** Checking Atlases *****//
void DerivedWindow::checkAtlases()
{
   QDir* m_atlasPopulation_dir = new QDir(atlasPopulationDirectory_lineEdit->text());
   QStringList atlasPopulation_list = m_atlasPopulation_dir->entryList(QDir::AllDirs | QDir::NoDotAndDotDot); 

   m_goodAtlases.clear();
   m_wrongAtlases.clear();
 
   QStringList::const_iterator it;
   for (it = atlasPopulation_list.constBegin(); it != atlasPopulation_list.constEnd(); ++it) 
   {
      QString atlas_dir = m_atlasPopulation_dir->filePath(*it);
      if(m_parameters->checkAtlas(atlas_dir))
      {
         m_goodAtlases << *it;
      }
      else
      {
         m_wrongAtlases << *it;
      }
   }
} 

//***** Display Atlases *****//
void DerivedWindow::displayAtlases()
{
   atlasPopulation_listWidget->clear();

   QStringList::const_iterator it;
   for (it = m_goodAtlases.constBegin(); it != m_goodAtlases.constEnd(); ++it) 
   {
        QListWidgetItem* item = new QListWidgetItem(*it, atlasPopulation_listWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable);
        item->setCheckState(Qt::Unchecked);
   } 
   for (it = m_wrongAtlases.constBegin(); it != m_wrongAtlases.constEnd(); ++it) 
   {
        QListWidgetItem* item = new QListWidgetItem(*it, atlasPopulation_listWidget);
        item->setFlags(item->flags() & ~Qt::ItemIsEnabled);
        item->setCheckState(Qt::Unchecked);
   }
}


void DerivedWindow::checkSelectedAtlases()
{
   QStringList::const_iterator it;
   for (it = m_selectedAtlases.constBegin(); it != m_selectedAtlases.constEnd(); ++it) 
   {
      QList<QListWidgetItem *> items = atlasPopulation_listWidget->findItems(*it, Qt::MatchExactly);

      QList<QListWidgetItem *>::iterator item; 
      for(item=items.begin(); item!=items.end(); ++item)
      {
         (*item)->setCheckState(Qt::Checked);
      }
   }    
}


//***** Select/Unselect Atlas *****//
void DerivedWindow::selectAtlas(QListWidgetItem* item)
{
   if(item->checkState())
   {
      m_selectedAtlases << item->text();           
   } 
  else
  {
      m_selectedAtlases.removeAt(m_selectedAtlases.indexOf(item->text())); 
  }
}


//***** Existing Atlas *****//
void DerivedWindow::selectExistingAtlas()
{
   QString existingAtlas = QFileDialog::getExistingDirectory (0, tr("Open Directory"), m_existingAtlases_path, QFileDialog::ShowDirsOnly);
   std::string errorMessage = std::string( "\nis not a valid atlas. It should contain all the following files:\n" )
                          + std::string( "templateT1.*, templateT2.*, white.*, gray.*, csf.*, rest.*" )
                          + std::string( "\nAll files must have the same extension." )
                          + std::string( "\nNone of these files should be in the folder multiple times with different extensions." ) ;
   if(m_parameters->checkExistingAtlas(existingAtlas)==false)
   {
      QMessageBox::critical( this , tr( "Atlas Directory" ) , existingAtlas + tr( errorMessage.c_str() ) ) ;
   }
   else
   {
      existingAtlas_lineEdit->setText(existingAtlas);
   }
}
void DerivedWindow::enterExistingAtlas()
{
   QString existingAtlas = existingAtlas_lineEdit->text();
   std::string errorMessage = std::string( "\nis not a valid atlas. It should contain all the following files:\n" )
                          + std::string( "templateT1.*, templateT2.*, white.*, gray.*, csf.*, rest.*" )
                          + std::string( "\nAll files must have the same extension." )
                          + std::string( "\nNone of these files should be in the folder multiple times with different extensions." ) ;
   if(!existingAtlas.isEmpty()) 
   {
      if (!m_parameters->checkExistingAtlas(existingAtlas))
      {
         existingAtlas_lineEdit->clear();
         QMessageBox::critical( this , tr( "Atlas Directory" ) , existingAtlas + tr( errorMessage.c_str() ) ) ;
      } 
   }
}

//***** Preprocessing Data *****//
void DerivedWindow::changeUsingMaskDTI(bool checked)
{
  if( !checked )
  {
    usingSmoothedMask_DTI_checkBox->setChecked( Qt::Unchecked ) ;
    usingSmoothedMask_DTI_checkBox->setDisabled( true ) ;
  }
  else
  {
    usingSmoothedMask_DTI_checkBox->setDisabled( false ) ;
  }
}

//***** Atlas Registration *****//
void DerivedWindow::changeUsingMaskAtlas(bool checked)
{
  if( !checked )
  {
    this->antsParameters->usingSmoothedMask_atlas_checkBox->setChecked( Qt::Unchecked ) ;
    this->antsParameters->usingSmoothedMask_atlas_checkBox->setDisabled( true ) ;
  }
  else
  {
    this->antsParameters->usingSmoothedMask_atlas_checkBox->setDisabled( false ) ;
  }
}

//***** Computing System *****//
void DerivedWindow::changeComputingSystem(int index)
{
   if(index == 0)
   {
      numberOfRegistrations_spinBox->setEnabled(true);
      numberOfGB_spinBox->setEnabled(false);
   }
   else
   {
      numberOfRegistrations_spinBox->setEnabled(false);
      numberOfRegistrations_spinBox->setValue(0);
      numberOfGB_spinBox->setEnabled(true);
   }
}

void DerivedWindow::selectXMLFile( int XMLFileType )
{
  QString xmlFileName = QFileDialog::getOpenFileName( this , tr( "Open file" ) , m_data_path , "XML (*.xml)" ) ;
  loadXMLFile( xmlFileName , XMLFileType ) ;
}

void DerivedWindow::loadXMLFile( QString xmlFileName , int XMLFileType )
{
  if(!xmlFileName.isEmpty())
  {
    XmlReader xmlReader ;
    xmlReader.setPipelineParameters( m_parameters ) ;
    QString errors ;
    switch( XMLFileType )
    {
    case 0:
        errors = xmlReader.readDataConfigurationFile( xmlFileName ) ;
        tabs->setCurrentWidget( data_tab ) ;
        initializeDataParameters() ;
        break ;
    case 1:
        errors = xmlReader.readParametersConfigurationFile( xmlFileName ) ;
        initializeXMLParameters() ;
        break ;
    case 2:
    default:
        errors = xmlReader.readExecutablesConfigurationFile( xmlFileName ) ;
        tabs->setCurrentWidget( software_tab ) ;
        initializeExecutables() ;
        break ;
    }
    if( !errors.isEmpty() )
    {
      errors = tr( "Errors in the XML file:\n" ) + errors ;
      errors += tr( "\n" ) ;
      errors += tr( "All the parameters that are nor valid, are left to their default value\n") ;
      QMessageBox::critical( this , tr("Errors in XML file") , errors ) ;
    }
  }
}


// Executables
void DerivedWindow::selectExecutable(QString executable_name)
{
   Executable executable = m_executables_map[executable_name];

   QString executable_path = (executable.enter_lineEdit)->text();
   QString dir_path = "";

   if(!(executable_path.isEmpty()))
   {
      dir_path = (QFileInfo(executable_path).dir()).absolutePath();
   }

     executable_path = QFileDialog::getOpenFileName(this, tr("Select executable"), dir_path);
   if(!executable_path.isEmpty())
   {
       if( m_executables->checkExecutablePath( executable_name , executable_path) )
       {
           (executable.enter_lineEdit)->setText(executable_path) ;
           m_executables->setExecutablePath( executable_name , executable_path ) ;
       }
      else
      {
         QMessageBox::critical(this, executable_name, executable_path + tr("\nis not executable or is the incorrect version"));
      }
   }
}

void DerivedWindow::enterExecutable(QString executable_name)
{
    Executable executable = m_executables_map[executable_name];

    QString executable_path = (executable.enter_lineEdit)->text();

    std::cout<<executable_name.toStdString()<<std::endl;
    std::cout<<executable_path.toStdString()<<std::endl;

    if(executable_path.toStdString().compare("") != 0){
        if( m_executables->checkExecutablePath( executable_name , executable_path) )
        {
            (executable.enter_lineEdit)->setText(executable_path) ;
            m_executables->setExecutablePath( executable_name , executable_path ) ;
        }
        else
        {
            QMessageBox::critical(this, executable_name, executable_path + tr("\nis not executable or is the incorrect version"));
        }
    }
}

void DerivedWindow::resetExecutable(QString executable_name)
{
   Executable executable = m_executables_map[executable_name]; 
   (executable.enter_lineEdit)->setText(m_executables->getExecutablePath(executable_name));
}

void DerivedWindow::resetAllExecutables()
{
   resetExecutable("python"); 
   resetExecutable("SegPostProcessCLP"); 
   resetExecutable("N4ITKBiasFieldCorrection"); 
   resetExecutable("ITKTransformTools"); 
   resetExecutable("bet2"); 
   resetExecutable("unu"); 
   resetExecutable("dtiestim"); 
   resetExecutable("dtiprocess"); 
   resetExecutable("ANTS"); 
   resetExecutable("ResampleScalarVectorDWIVolume"); 
   resetExecutable("ImageMath"); 
   resetExecutable("WeightedLabelsAverage"); 
   resetExecutable("SpreadFA");
   resetExecutable("neoseg"); 
   resetExecutable("ReassignWhiteMatter"); 
   resetExecutable("InsightSNAP");
   resetExecutable("ABC");
}


// Libraries
void DerivedWindow::selectLibrary(QString library_name)
{   
   Library library = m_libraries_map[library_name];

   QString library_path = (library.enter_lineEdit)->text(); 
   QString dir_path = ""; 

   if(!(library_path.isEmpty()))
   {
      dir_path = (QFileInfo(library_path).dir()).absolutePath(); 
   }

   library_path =  QFileDialog::getExistingDirectory (this, tr("Select library"), dir_path);
   if(!library_path.isEmpty())
   {
      if(QFileInfo(library_path).isDir())
      {
         (library.enter_lineEdit)->setText(library_path);
      }
      else
      {
         QMessageBox::critical(this, library_name, library_path + tr("\nis not a directory"));      
      }
   }
}
void DerivedWindow::enterLibrary(QString library_name)
{
   Library library = m_libraries_map[library_name];   
   QString library_path = (library.enter_lineEdit)->text();
   if(!library_name.isEmpty()) 
   {  
      if( !QFileInfo(library_path).exists() || !QFileInfo(library_path).isDir() )
      {
         (library.enter_lineEdit)->clear();
         QMessageBox::critical(this, library_name, library_path + tr("\nis not a directory, enter a new path"));            
      }
      //else everything is good
   }
   else
   {
      QMessageBox::critical(this, library_name, tr("Please enter a directory name"));
   }
}


void DerivedWindow::initializeDataParameters()
{
  prefix_lineEdit->setText(m_parameters->getPrefix());
  suffix_lineEdit->setText(m_parameters->getSuffix());

//  output_lineEdit->setText(QString("/Users/prieto/NetBeansProjects/UNC/data/testNeoSeg/abcout"));
//  T1_lineEdit->setText(QString("/Users/prieto/NetBeansProjects/UNC/data/testNeoSeg/BUSS_2002_T1_Bias_regT2_regAtlas_corrected_EMS-stripped.nrrd"));
//  T2_lineEdit->setText(QString("/Users/prieto/NetBeansProjects/UNC/data/testNeoSeg/BUSS_2002_T2_Bias_regAtlas_corrected_EMS-stripped.nrrd"));
//  mask_lineEdit->setText(QString("/Users/prieto/NetBeansProjects/UNC/data/testNeoSeg/BUSS_2002_mask.nrrd"));

  output_lineEdit->setText(m_parameters->getOutput());
  T1_lineEdit->setText(m_parameters->getT1());
  T2_lineEdit->setText(m_parameters->getT2());
  mask_lineEdit->setText(m_parameters->getMask());
  DWI_lineEdit->setText(m_parameters->getDWI());
}

void DerivedWindow::initializeXMLParameters()
{
   skullStripping_checkBox->setChecked(m_parameters->getSkullStripping());   
   correcting_checkBox->setChecked(m_parameters->getCorrecting());

   if(m_parameters->getNewAtlas())
   {
      newAtlas_radioButton->setChecked(true);
      existingAtlas_radioButton->setChecked(false);
      newAtlas_widget->show();
      existingAtlas_widget->hide();

      atlasPopulationDirectory_lineEdit->setText(m_parameters->getAtlasPopulationDirectory()); 
      checkAtlases();  
      displayAtlases();
     
      if(!(m_parameters->getSelectedAtlases()).empty())
      {
         m_selectedAtlases = m_parameters->getSelectedAtlases();
      }
      else
      {
         m_selectedAtlases = m_goodAtlases;
      }
      checkSelectedAtlases(); 
      smoothing_comboBox->clear() ;
      smoothing_comboBox->insertItems(0, m_parameters->getSmoothingValues()); 
      smoothing_comboBox->setCurrentIndex(m_parameters->getSmoothingIndex()); 
      smoothingSize_spinBox->setValue(m_parameters->getSmoothingSize());

      computingWeights_checkBox->setChecked(m_parameters->getComputingWeights()); 
      weightsModality_comboBox->clear() ;
      weightsModality_comboBox->insertItems(0,m_parameters->getWeightsModalityValues());
      weightsModality_comboBox->setCurrentIndex(m_parameters->getWeightsModalityIndex());
      weightsRadius_spinBox->setValue(m_parameters->getWeightsRadius());
      weightsRadiusUnit_comboBox->clear() ;
      weightsRadiusUnit_comboBox->insertItems(0, m_parameters->getWeightsRadiusUnitValues()); 
      weightsRadiusUnit_comboBox->setCurrentIndex(m_parameters->getWeightsRadiusUnitIndex());
     
      includingFA_checkBox->setChecked(m_parameters->getIncludingFA()); 
      FAShift_spinBox->setValue(m_parameters->getFAShift());
      FASigmaScale_spinBox->setValue(m_parameters->getFASigmaScale());
      FAWeight_spinBox->setValue(m_parameters->getFAWeight());
      FASmoothingSize_spinBox->setValue(m_parameters->getFASmoothingSize());
   } 
   else
   {
      newAtlas_radioButton->setChecked(false);
      existingAtlas_radioButton->setChecked(true);
      newAtlas_widget->hide();
      existingAtlas_widget->show(); 
      existingAtlas_lineEdit->setText(m_parameters->getExistingAtlas()); 
   } 

   usingFA_checkBox->setChecked(m_parameters->getUsingFA()); 
   usingAD_checkBox->setChecked(m_parameters->getUsingAD()); 

   neosegParameters->computing3LabelsSeg_checkBox->setChecked(m_parameters->getComputing3LabelsSeg() ) ;
   neosegParameters->reassigningWhite_checkBox->setChecked(m_parameters->getReassigningWhiteMatter());
   neosegParameters->whiteThreshold_spinBox->setValue(m_parameters->getSizeThreshold());

   overwriting_checkBox->setChecked(m_parameters->getOverwriting()); 
   cleaningUp_checkBox->setChecked(m_parameters->getCleaningUp()); 
   stoppingIfError_checkBox->setChecked(m_parameters->getStoppingIfError());

   computingSystem_comboBox->clear() ;
   computingSystem_comboBox->insertItems(0, m_parameters->getComputingSystemValues()); 
   computingSystem_comboBox->setCurrentIndex(m_parameters->getComputingSystemIndex());

   // ANTS
   imageMetric1_DTI_comboBox->clear() ;
   imageMetric1_DTI_comboBox->insertItems(0, m_antsParameters_DTI->getImageMetricValues()); 
   imageMetric1_DTI_comboBox->setCurrentIndex(m_antsParameters_DTI->getImageMetric1Index()); 
   weight1_DTI_spinBox->setValue(m_antsParameters_DTI->getWeight1());
   radius1_DTI_spinBox->setValue(m_antsParameters_DTI->getRadius1());

   imageMetric2_DTI_comboBox->clear() ;
   imageMetric2_DTI_comboBox->insertItems(0, m_antsParameters_DTI->getImageMetricValues()); 
   imageMetric2_DTI_comboBox->setCurrentIndex(m_antsParameters_DTI->getImageMetric2Index()); 
   weight2_DTI_spinBox->setValue(m_antsParameters_DTI->getWeight2());
   radius2_DTI_spinBox->setValue(m_antsParameters_DTI->getRadius2());
   
   iterationsJ_DTI_spinBox->setValue(m_antsParameters_DTI->getIterationsJ());   
   iterationsK_DTI_spinBox->setValue(m_antsParameters_DTI->getIterationsK());
   iterationsL_DTI_spinBox->setValue(m_antsParameters_DTI->getIterationsL());

   transformationType_DTI_comboBox->clear() ;
   transformationType_DTI_comboBox->insertItems(0, m_antsParameters_DTI->getTransformationTypeValues());
   transformationType_DTI_comboBox->setCurrentIndex(m_antsParameters_DTI->getTransformationTypeIndex());
   gradientStepLength_DTI_spinBox->setValue(m_antsParameters_DTI->getGradientStepLength());
   numberOfTimeSteps_DTI_spinBox->setValue(m_antsParameters_DTI->getNumberOfTimeSteps());
   deltaTime_DTI_spinBox->setValue(m_antsParameters_DTI->getDeltaTime());

   regularizationType_DTI_comboBox->clear() ;
   regularizationType_DTI_comboBox->insertItems(0, m_antsParameters_DTI->getRegularizationTypeValues());
   regularizationType_DTI_comboBox->setCurrentIndex(m_antsParameters_DTI->getRegularizationTypeIndex());
   gradientFieldSigma_DTI_spinBox->setValue(m_antsParameters_DTI->getGradientFieldSigma());
   deformationFieldSigma_DTI_spinBox->setValue(m_antsParameters_DTI->getDeformationFieldSigma());

   usingMask_DTI_checkBox->setChecked(m_antsParameters_DTI->getUsingMask());
   if( m_antsParameters_DTI->getUsingMask() )
   {
     usingSmoothedMask_DTI_checkBox->setDisabled( false ) ;
     usingSmoothedMask_DTI_checkBox->setChecked( m_antsParameters_DTI->getUsingSmoothedMask() ) ;
   }
   else
   {
     usingSmoothedMask_DTI_checkBox->setChecked( false ) ;
     usingSmoothedMask_DTI_checkBox->setDisabled( true ) ;
   }

   // Quicksilver

   //QuicksilverParameters* m_quicksilverParameters=(QuicksilverParameters*) m_registrationParameters_atlas;

   quicksilverParameters->registrationscript_lineEdit->setText(m_registrationParameters_atlas->getRegistrationScriptPath());
   quicksilverParameters->containerId_lineEdit->setText(m_registrationParameters_atlas->getContainerId());

   // ANTS
   //AntsParameters* m_antsParameters_atlas=(AntsParameters*) m_registrationParameters_atlas;

   numberOfRegistrations_spinBox->setValue(m_registrationParameters_atlas->getNumberOfRegistrations());
   numberOfCores_spinBox->setValue(m_registrationParameters_atlas->getNumberOfCores());
   numberOfGB_spinBox->setValue(m_registrationParameters_atlas->getNumberOfGB());

   antsParameters->imageMetric1_atlas_comboBox->clear() ;
   antsParameters->imageMetric1_atlas_comboBox->insertItems(0, m_registrationParameters_atlas->getImageMetricValues()); 
   antsParameters->imageMetric1_atlas_comboBox->setCurrentIndex(m_registrationParameters_atlas->getImageMetric1Index()); 
   antsParameters->weight1_atlas_spinBox->setValue(m_registrationParameters_atlas->getWeight1());
   antsParameters->radius1_atlas_spinBox->setValue(m_registrationParameters_atlas->getRadius1());

   antsParameters->imageMetric2_atlas_comboBox->clear() ;
   antsParameters->imageMetric2_atlas_comboBox->insertItems(0, m_registrationParameters_atlas->getImageMetricValues()); 
   antsParameters->imageMetric2_atlas_comboBox->setCurrentIndex(m_registrationParameters_atlas->getImageMetric2Index()); 
   antsParameters->weight2_atlas_spinBox->setValue(m_registrationParameters_atlas->getWeight2());
   antsParameters->radius2_atlas_spinBox->setValue(m_registrationParameters_atlas->getRadius2());
   
   antsParameters->iterationsJ_atlas_spinBox->setValue(m_registrationParameters_atlas->getIterationsJ());   
   antsParameters->iterationsK_atlas_spinBox->setValue(m_registrationParameters_atlas->getIterationsK());
   antsParameters->iterationsL_atlas_spinBox->setValue(m_registrationParameters_atlas->getIterationsL());

   antsParameters->transformationType_atlas_comboBox->clear() ;
   antsParameters->transformationType_atlas_comboBox->insertItems(0, m_registrationParameters_atlas->getTransformationTypeValues());
   antsParameters->transformationType_atlas_comboBox->setCurrentIndex(m_registrationParameters_atlas->getTransformationTypeIndex());
   antsParameters->gradientStepLength_atlas_spinBox->setValue(m_registrationParameters_atlas->getGradientStepLength());
   antsParameters->numberOfTimeSteps_atlas_spinBox->setValue(m_registrationParameters_atlas->getNumberOfTimeSteps());
   antsParameters->deltaTime_atlas_spinBox->setValue(m_registrationParameters_atlas->getDeltaTime());

   antsParameters->regularizationType_atlas_comboBox->clear() ;
   antsParameters->regularizationType_atlas_comboBox->insertItems(0, m_registrationParameters_atlas->getRegularizationTypeValues());
   antsParameters->regularizationType_atlas_comboBox->setCurrentIndex(m_registrationParameters_atlas->getRegularizationTypeIndex());
   antsParameters->gradientFieldSigma_atlas_spinBox->setValue(m_registrationParameters_atlas->getGradientFieldSigma());
   antsParameters->deformationFieldSigma_atlas_spinBox->setValue(m_registrationParameters_atlas->getDeformationFieldSigma());

   antsParameters->usingMask_atlas_checkBox->setChecked(m_registrationParameters_atlas->getUsingMask());
   if( m_registrationParameters_atlas->getUsingMask() )
   {
     antsParameters->usingSmoothedMask_atlas_checkBox->setDisabled( false ) ;
     antsParameters->usingSmoothedMask_atlas_checkBox->setChecked( m_registrationParameters_atlas->getUsingSmoothedMask() ) ;
   }
   else
   {
     antsParameters->usingSmoothedMask_atlas_checkBox->setChecked( false ) ;
     antsParameters->usingSmoothedMask_atlas_checkBox->setDisabled( true ) ;
   }

   //Neoseg
   referenceModality_comboBox->clear() ;
   referenceModality_comboBox->insertItems(0, m_neosegParameters->getReferenceImageValues());
   referenceModality_comboBox->setCurrentIndex(m_neosegParameters->getReferenceImageIndex());

   filterMethod_comboBox->clear() ;
   filterMethod_comboBox->insertItems(0, m_neosegParameters->getFilterMethodValues()); 
   filterMethod_comboBox->setCurrentIndex(m_neosegParameters->getFilterMethodIndex()); 
   numberOfIterations_spinBox->setValue(m_neosegParameters->getNumberOfIterations()); 
   timeStep_spinBox->setValue(m_neosegParameters->getTimeStep()); 

   neosegParameters->priorThreshold_spinBox->setValue(m_neosegParameters->getPriorThreshold());
   neosegParameters->maxDegree_spinBox->setValue(m_neosegParameters->getMaxBiasDegree());

   neosegParameters->prior1_spinBox->setValue(m_neosegParameters->getPrior1());
   neosegParameters->prior2_spinBox->setValue(m_neosegParameters->getPrior2());
   neosegParameters->prior3_spinBox->setValue(m_neosegParameters->getPrior3());
   neosegParameters->prior4_spinBox->setValue(m_neosegParameters->getPrior4());
   neosegParameters->prior5_spinBox->setValue(m_neosegParameters->getPrior5());

   neosegParameters->refinement_checkBox->setChecked(m_neosegParameters->getRefinement());
   neosegParameters->initialParzenKernelWidth_spinBox->setValue(m_neosegParameters->getInitialParzenKernelWidth());
   if( newAtlas_radioButton->isChecked() )
   {
      atlasGeneration_tab->setDisabled( false ) ;
   }
   else
   {
      atlasGeneration_tab->setDisabled( true ) ;
   }

   abcParameters->maxDegree_spinBox->setValue(m_parameters->getABCMaximumDegreeBiasField());
   QString estim = m_parameters->getABCInitialDistributorEstimatorType();
   int index = abcParameters->initialDistributionEstimatorCombo->findText(estim);
   if(index >= 0){
       abcParameters->initialDistributionEstimatorCombo->setCurrentIndex(index);
   }
   updateNumbersOfPriorsForABC();

   std::vector<double> coeffs = m_parameters->getABCPriorsCoefficients();
   PipelineParameters::ABCVectorReassignLabelsType reassign =  m_parameters->getABCReassignLabels();

   for(unsigned i = 0; i < m_VectorABCPriorCheckBoxes.size() && i < coeffs.size() && i < reassign.size(); i++){
       PriorSpinBox* psb = m_VectorABCPriorCheckBoxes[i];
       psb->dspin->setValue(coeffs[i]);


       PipelineParameters::ABCReassignLabelsType rea = reassign[i];
       if(psb->checkboxIslands){
           psb->checkboxIslands->setChecked(rea.m_ReassignEnabled);
           psb->spinBoxIslands->setEnabled(rea.m_ReassignEnabled);
           psb->checkBoxVoxelByVoxel->setEnabled(rea.m_ReassignEnabled);
           psb->spinBoxIslands->setValue(rea.m_Threshold);
           psb->checkBoxVoxelByVoxel->setChecked(rea.m_VoxelByVoxel);
       }

   }

}

void DerivedWindow::initializeExecutables()
{
   // Executables 
   python_lineEdit->setText(m_executables->getExecutablePath("python"));
   SegPostProcessCLP_lineEdit->setText(m_executables->getExecutablePath("SegPostProcessCLP"));
   N4ITKBiasFieldCorrection_lineEdit->setText(m_executables->getExecutablePath("N4ITKBiasFieldCorrection"));
   ITKTransformTools_lineEdit->setText(m_executables->getExecutablePath("ITKTransformTools"));
   bet2_lineEdit->setText(m_executables->getExecutablePath("bet2"));
   unu_lineEdit->setText(m_executables->getExecutablePath("unu"));
   dtiestim_lineEdit->setText(m_executables->getExecutablePath("dtiestim"));
   dtiprocess_lineEdit->setText(m_executables->getExecutablePath("dtiprocess"));
   ANTS_lineEdit->setText(m_executables->getExecutablePath("ANTS"));
   ResampleScalarVectorDWIVolume_lineEdit->setText(m_executables->getExecutablePath("ResampleScalarVectorDWIVolume"));
   ImageMath_lineEdit->setText(m_executables->getExecutablePath("ImageMath"));
   WeightedLabelsAverage_lineEdit->setText(m_executables->getExecutablePath("WeightedLabelsAverage"));
   SpreadFA_lineEdit->setText(m_executables->getExecutablePath("SpreadFA"));
   ReassignWhiteMatter_lineEdit->setText(m_executables->getExecutablePath("ReassignWhiteMatter"));
   Neoseg_lineEdit->setText(m_executables->getExecutablePath("neoseg"));
   InsightSNAP_lineEdit->setText(m_executables->getExecutablePath("InsightSNAP"));
   ABC_lineEdit->setText(m_executables->getExecutablePath("ABC"));
   // Libraries
   FSL_lineEdit->setText(m_libraries->getLibraryPath("FSL"));
}


void DerivedWindow::setData()
{
   // Input
   m_parameters->setT1(T1_lineEdit->text());
   m_parameters->setT2(T2_lineEdit->text());
   m_parameters->setMask(mask_lineEdit->text());
   m_parameters->setDWI(DWI_lineEdit->text());

   // Output
   m_parameters->setPrefix(prefix_lineEdit->text());
   m_parameters->setSuffix(suffix_lineEdit->text());
   if(!m_parameters->checkOutput(output_lineEdit->text()))
   {
      createOutput(output_lineEdit->text()); 
   }
   if(this->radioNeoseg->isChecked()){
       m_parameters->setTissueSegmentationType(TISSUE_SEG_TYPE_NEOSEG);
   }else{
       m_parameters->setTissueSegmentationType(TISSUE_SEG_TYPE_ABC);
       std::vector<double> coeffs;
       PipelineParameters::ABCVectorReassignLabelsType  vectorReassign;
       for(unsigned i = 0; i < m_VectorABCPriorCheckBoxes.size(); i++){
           PriorSpinBox* priorspin = m_VectorABCPriorCheckBoxes[i];
           coeffs.push_back(priorspin->dspin->value());

           PipelineParameters::ABCReassignLabelsType reassign;
           if(priorspin->checkboxIslands){
               reassign.m_ReassignEnabled = priorspin->checkboxIslands->isChecked();
               reassign.m_Threshold = (int)priorspin->spinBoxIslands->value();
               reassign.m_VoxelByVoxel = priorspin->checkBoxVoxelByVoxel->isChecked();
               reassign.m_Label = priorspin->m_LabelValue;
               reassign.m_Index = priorspin->m_Index;
           }else{
               reassign.m_ReassignEnabled = false;
               reassign.m_Threshold = 0;
               reassign.m_VoxelByVoxel = false;
               reassign.m_Label = priorspin->m_LabelValue;
               reassign.m_Index = priorspin->m_Index;
           }
           vectorReassign.push_back(reassign);

       }
       m_parameters->setNumberOfLabels(m_VectorABCPriorCheckBoxes.size() - 1);//Remove the rest label
       m_parameters->setABCPriorsCoefficients(coeffs);
       m_parameters->setABCReassignLabels(vectorReassign);
       m_parameters->setABCInitialDistributorEstimatorType(this->abcParameters->initialDistributionEstimatorCombo->currentText());
       m_parameters->setABCMaximumDegreeBiasField(this->abcParameters->maxDegree_spinBox->value());
       m_parameters->setABCOutputImageFormat(this->abcParameters->comboBoxOutputImageFormat->currentText());

   }



   m_parameters->setOutput(output_lineEdit->text()); 
}

void DerivedWindow::setParameters()
{
   // PreProcessing Data
   m_parameters->setSkullStripping(skullStripping_checkBox->isChecked()); 
   m_parameters->setCorrecting(correcting_checkBox->isChecked());    

   //Atlas Population 
   if (newAtlas_radioButton->isChecked()) 
   {
      m_parameters->setNewAtlas(true);

      // Atlas Population
      m_parameters->setAtlasPopulationDirectory(atlasPopulationDirectory_lineEdit->text());
      m_parameters->setSelectedAtlases(m_selectedAtlases);

      // Smoothing
      m_parameters->setSmoothing(smoothing_comboBox->currentText());
      m_parameters->setSmoothingSize(smoothingSize_spinBox->value());

      // Computing Weights
      m_parameters->setComputingWeights(computingWeights_checkBox->isChecked());
      m_parameters->setWeightsRadius(weightsRadius_spinBox->value());
      m_parameters->setWeightsRadiusUnit(weightsRadiusUnit_comboBox->currentText()); 
      m_parameters->setWeightsModality(weightsModality_comboBox->currentText());
      // Including FA
      m_parameters->setIncludingFA(includingFA_checkBox->isChecked()); 
      m_parameters->setFAShift(FAShift_spinBox->value());
      m_parameters->setFASigmaScale(FASigmaScale_spinBox->value());
      m_parameters->setFAWeight(FAWeight_spinBox->value()); 
      m_parameters->setFASmoothingSize(FASmoothingSize_spinBox->value()); 
   }

   else 
   {
      m_parameters->setNewAtlas(false);  
      m_parameters->setExistingAtlas(existingAtlas_lineEdit->text() , true );
   }

   m_parameters->setUsingFA(usingFA_checkBox->isChecked());
   m_parameters->setUsingAD(usingAD_checkBox->isChecked());

   // Reassigning White Matter
   m_parameters->setReassigningWhiteMatter(neosegParameters->reassigningWhite_checkBox->isChecked());
   m_parameters->setSizeThreshold(neosegParameters->whiteThreshold_spinBox->value());


   // Computation
   m_parameters->setOverwriting(overwriting_checkBox->isChecked());
   m_parameters->setCleaningUp(cleaningUp_checkBox->isChecked());
   m_parameters->setStoppingIfError(stoppingIfError_checkBox->isChecked()); 
   m_parameters->setComputingSystem(computingSystem_comboBox->currentText());

   // ANTS parameters for DTI Registration
   //AntsParameters* m_antsParameters_atlas=(AntsParameters*) m_registrationParameters_atlas;

   m_antsParameters_DTI->setImageMetric1(imageMetric1_DTI_comboBox->currentText());
   m_antsParameters_DTI->setWeight1(weight1_DTI_spinBox->value());
   m_antsParameters_DTI->setRadius1(radius1_DTI_spinBox->value());

   m_antsParameters_DTI->setImageMetric2(imageMetric2_DTI_comboBox->currentText());
   m_antsParameters_DTI->setWeight2(weight2_DTI_spinBox->value());
   m_antsParameters_DTI->setRadius2(radius2_DTI_spinBox->value());

   m_antsParameters_DTI->setIterationsJ(iterationsJ_DTI_spinBox->value());
   m_antsParameters_DTI->setIterationsK(iterationsK_DTI_spinBox->value());
   m_antsParameters_DTI->setIterationsL(iterationsL_DTI_spinBox->value());

   m_antsParameters_DTI->setTransformationType(transformationType_DTI_comboBox->currentText());
   m_antsParameters_DTI->setGradientStepLength(gradientStepLength_DTI_spinBox->value());
   m_antsParameters_DTI->setNumberOfTimeSteps(numberOfTimeSteps_DTI_spinBox->value());
   m_antsParameters_DTI->setDeltaTime(deltaTime_DTI_spinBox->value());

   m_antsParameters_DTI->setRegularizationType(regularizationType_DTI_comboBox->currentText());  
   m_antsParameters_DTI->setGradientFieldSigma(gradientFieldSigma_DTI_spinBox->value());
   m_antsParameters_DTI->setDeformationFieldSigma(deformationFieldSigma_DTI_spinBox->value());

   m_antsParameters_DTI->setUsingMask(usingMask_DTI_checkBox->isChecked());
   m_antsParameters_DTI->setUsingSmoothedMask( usingSmoothedMask_DTI_checkBox->isChecked() ) ;

   //Registration parameters for Atlas Population Registration 
   m_registrationParameters_atlas->setRegistrationScriptPath(this->quicksilverParameters->registrationscript_lineEdit->text());
   m_registrationParameters_atlas->setContainerId(this->quicksilverParameters->containerId_lineEdit->text());

   m_registrationParameters_atlas->setNumberOfRegistrations(numberOfRegistrations_spinBox->value());
   m_registrationParameters_atlas->setNumberOfCores(numberOfCores_spinBox->value());
   m_registrationParameters_atlas->setNumberOfGB(numberOfGB_spinBox->value());

   m_registrationParameters_atlas->setImageMetric1(this->antsParameters->imageMetric1_atlas_comboBox->currentText());
   m_registrationParameters_atlas->setWeight1(this->antsParameters->weight1_atlas_spinBox->value());
   m_registrationParameters_atlas->setRadius1(this->antsParameters->radius1_atlas_spinBox->value());

   m_registrationParameters_atlas->setImageMetric2(this->antsParameters->imageMetric2_atlas_comboBox->currentText());
   m_registrationParameters_atlas->setWeight2(this->antsParameters->weight2_atlas_spinBox->value());
   m_registrationParameters_atlas->setRadius2(this->antsParameters->radius2_atlas_spinBox->value());

   m_registrationParameters_atlas->setIterationsJ(this->antsParameters->iterationsJ_atlas_spinBox->value());
   m_registrationParameters_atlas->setIterationsK(this->antsParameters->iterationsK_atlas_spinBox->value());
   m_registrationParameters_atlas->setIterationsL(this->antsParameters->iterationsL_atlas_spinBox->value());

   m_registrationParameters_atlas->setTransformationType(this->antsParameters->transformationType_atlas_comboBox->currentText());
   m_registrationParameters_atlas->setGradientStepLength(this->antsParameters->gradientStepLength_atlas_spinBox->value());
   m_registrationParameters_atlas->setNumberOfTimeSteps(this->antsParameters->numberOfTimeSteps_atlas_spinBox->value());
   m_registrationParameters_atlas->setDeltaTime(this->antsParameters->deltaTime_atlas_spinBox->value());

   m_registrationParameters_atlas->setRegularizationType(this->antsParameters->regularizationType_atlas_comboBox->currentText());  
   m_registrationParameters_atlas->setGradientFieldSigma(this->antsParameters->gradientFieldSigma_atlas_spinBox->value());
   m_registrationParameters_atlas->setDeformationFieldSigma(this->antsParameters->deformationFieldSigma_atlas_spinBox->value());

   m_registrationParameters_atlas->setUsingMask(this->antsParameters->usingMask_atlas_checkBox->isChecked());
   m_registrationParameters_atlas->setUsingSmoothedMask(this->antsParameters->usingSmoothedMask_atlas_checkBox->isChecked() ) ;

   // Neoseg parameters 
   m_neosegParameters->setReferenceImage(referenceModality_comboBox->currentText()); 

   m_neosegParameters->setFilterMethod(filterMethod_comboBox->currentText()); 
   m_neosegParameters->setNumberOfIterations(numberOfIterations_spinBox->value());
   m_neosegParameters->setTimeStep(timeStep_spinBox->value()); 

   m_neosegParameters->setPriorThreshold(neosegParameters->priorThreshold_spinBox->value());
   m_neosegParameters->setMaxBiasDegree(neosegParameters->maxDegree_spinBox->value());
   m_neosegParameters->setPrior1(neosegParameters->prior1_spinBox->value());
   m_neosegParameters->setPrior2(neosegParameters->prior2_spinBox->value());
   m_neosegParameters->setPrior3(neosegParameters->prior3_spinBox->value());
   m_neosegParameters->setPrior4(neosegParameters->prior4_spinBox->value());
   m_neosegParameters->setPrior5(neosegParameters->prior5_spinBox->value());

   m_neosegParameters->setRefinement(neosegParameters->refinement_checkBox->isChecked());
   m_neosegParameters->setInitialParzenKernelWidth(neosegParameters->initialParzenKernelWidth_spinBox->value());

   m_parameters->setComputing3LabelsSeg(neosegParameters->computing3LabelsSeg_checkBox->isChecked());
   m_parametersSet = true;

   if(this->comboBoxLabelValueFA->currentText().compare(QString("white")) == 0){
       m_parameters->setABCWhiteImageIndex(QString("white"));
   }else{
       double combovalue = this->comboBoxLabelValueFA->currentText().toDouble();
       int index = m_parameters->getImageLabelMap()[combovalue];
       m_parameters->setABCWhiteImageIndex(QString::number(index));
   }

}

void DerivedWindow::setExecutables()
{
   // Executables
   m_executables->setExecutablePath("python", python_lineEdit->text()); 
   m_executables->setExecutablePath("SegPostProcessCLP", SegPostProcessCLP_lineEdit->text()); 
   m_executables->setExecutablePath("N4ITKBiasFieldCorrection", N4ITKBiasFieldCorrection_lineEdit->text()); 
   m_executables->setExecutablePath("N4ITKBiasFieldCorrection", N4ITKBiasFieldCorrection_lineEdit->text()); 
   m_executables->setExecutablePath("bet2", bet2_lineEdit->text()); 
   m_executables->setExecutablePath("unu", unu_lineEdit->text()); 
   m_executables->setExecutablePath("dtiestim", dtiestim_lineEdit->text()); 
   m_executables->setExecutablePath("dtiprocess", dtiprocess_lineEdit->text()); 
   m_executables->setExecutablePath("ANTS", ANTS_lineEdit->text()); 
   m_executables->setExecutablePath("ResampleScalarVectorDWIVolume", ResampleScalarVectorDWIVolume_lineEdit->text()); 
   m_executables->setExecutablePath("ImageMath", ImageMath_lineEdit->text()); 
   m_executables->setExecutablePath("WeightedLabelsAverage", WeightedLabelsAverage_lineEdit->text()); 
   m_executables->setExecutablePath("SpreadFA", SpreadFA_lineEdit->text()); 
   m_executables->setExecutablePath("ReassignWhiteMatter", ReassignWhiteMatter_lineEdit->text()); 
   m_executables->setExecutablePath("neoseg", Neoseg_lineEdit->text()); 
   m_executables->setExecutablePath("InsightSNAP", InsightSNAP_lineEdit->text());
   m_executables->setExecutablePath("ABC", ABC_lineEdit->text());

   m_libraries->setLibraryPath("FSL", FSL_lineEdit->text());    

   m_executablesSet = true;
}

void DerivedWindow::saveParameters()
{
   setParameters() ;
   setData() ;
   QString parameters_path = QFileDialog::getSaveFileName(this, tr("Save file"), tr("parameters.xml"), "XML files (*.xml)") ;
   XmlWriter parameters ;
   parameters.setPipelineParameters( m_parameters ) ;
   parameters.writeParametersConfiguration( parameters_path ) ;
}

void DerivedWindow::saveExecutables()
{
   setExecutables() ;
   QString executables_path = QFileDialog::getSaveFileName(this, tr("Save file"), tr("executables.xml"), "XML files (*.xml)") ;
   XmlWriter executables ;
   executables.setPipelineParameters( m_parameters ) ;
   executables.writeExecutablesConfiguration( executables_path ) ;
}

void DerivedWindow::saveData()
{
   setData() ;
   QString data_path = QFileDialog::getSaveFileName(this, tr("Save file"), tr("data.xml"), "XML files (*.xml)") ;
   XmlWriter dataWriter ;
   dataWriter.setPipelineParameters( m_parameters ) ;
   dataWriter.writeDataConfiguration( data_path ) ;
}

void DerivedWindow::setParametersWidgetEnabled(bool enabled)
{
   // Actions 
   loadParametersConfiguration_action->setEnabled(enabled); 
   saveParametersConfiguration_action->setEnabled(enabled); 
   loadSoftwaresConfiguration_action->setEnabled(enabled); 
   saveSoftwaresConfiguration_action->setEnabled(enabled); 
   runPipeline_action->setEnabled(enabled); 
   
   // Data 
   inputs_groupBox->setEnabled(enabled);
   outputs_groupBox->setEnabled(enabled);

   // Atlas
   newAtlas_radioButton->setEnabled(enabled);
   existingAtlas_radioButton->setEnabled(enabled);
   existingAtlas_groupBox->setEnabled(enabled);
   atlasPopulation_groupBox->setEnabled(enabled); 

   //Preprocessing Data
   DiffusionImagesRegistration_groupBox->setEnabled(enabled);
   StructuralImages_groupBox->setEnabled(enabled);

   // Atlas Registration
   numberOfRegistrations_groupBox->setEnabled(enabled);
   AntsParameters_groupBox->setEnabled(enabled);

   // Atlas Generation
   computingCCWeights_groupBox->setEnabled(enabled);
   includingFA_groupBox->setEnabled(enabled);
   smoothingProbabilities_groupBox->setEnabled(enabled);

   // Neoseg 
   neosegModalities_groupBox->setEnabled(enabled);
   neosegParameters_groupBox->setEnabled(enabled);
   neosegParameters->mergedSegmentation_groupBox->setEnabled(enabled);

   // Software
   Executables_groupBox->setEnabled(enabled);
   Libraries_groupBox->setEnabled(enabled);

   resetAllExecutables_button->setEnabled(enabled);

   // Computing 
   overwriting_checkBox->setEnabled(enabled);
   stoppingIfError_checkBox->setEnabled(enabled);
   cleaningUp_checkBox->setEnabled(enabled);
   computingSystem_label->setEnabled(enabled);
   computingSystem_comboBox->setEnabled(enabled);   
} 

void DerivedWindow::runPipeline()
{
   setData();
   setParameters();
   setExecutables();

   QString imagesErrors = m_parameters->checkImages(); 
   QString executablesErrors = m_executables->checkExecutables(); 
   QString librariesErrors = m_libraries->checkLibraries(); 
   /*  Check output directory specified  */
   QString outputDirError ;
   if( m_parameters->getOutput().isEmpty() )
   {
       outputDirError = "Please specify an output directory" ;
   }
   /*/////////////////////////*/
   if( imagesErrors.isEmpty()
    && executablesErrors.isEmpty()
    && librariesErrors.isEmpty()
    && outputDirError.isEmpty()
     )
   {
      m_pipeline->writeXMLFiles(); 
      m_pipeline->writePipeline();

      initializePipelineLogging();
      if(m_parameters->getNewAtlas())
      {
         initializeRegistrationsLogging();
      }

      m_pipeline->setPlainTextEdit(m_log_plainTextEdit); 
      m_thread->setPipeline(m_pipeline);

      m_thread->start();

      setParametersWidgetEnabled(false);
      runPipeline_button->setEnabled(false);
      stopPipeline_button->setEnabled(true);
      displayResults_button->setEnabled(false);
      displayResults_action->setEnabled(false);
      tabs->setCurrentWidget(execution_tab);

      runPipeline_progressBar->setTextVisible(false);
      runPipeline_progressBar->show();
      runPipeline_progressBar->setMinimum(0);
      runPipeline_progressBar->setMaximum(0);
   }
   else
   {
      QMessageBox::critical( this , tr("Errors") ,
                             imagesErrors + "\n" + executablesErrors + "\n"
                             + librariesErrors + "\n" + outputDirError
                           ) ;
   }
}

void DerivedWindow::initializePipelineLogging()
{
   // Radio Button
   pipeline_radioButton->setEnabled(true); 
   pipeline_radioButton->setChecked(true); 

   // Plain Text Edit
   m_log_plainTextEdit->show();
   m_log_plainTextEdit->clear();
   m_log_plainTextEdit->setMaximumBlockCount(500);

   // Log path
   QDir* output_dir = new QDir(m_parameters->getOutput());
   QString log_path = output_dir->filePath(m_parameters->getPrefix() + ".log");

   // Log File
   QFile* log_file = new::QFile(log_path); 
   log_file->open(QIODevice::ReadWrite);    
   m_log_textStream = new::QTextStream(log_file); 

   // QFileSystemWatcher
   QFileSystemWatcher* log_watcher = new::QFileSystemWatcher(this); 
   log_watcher->addPath(log_path); 
   connect(log_watcher, SIGNAL(fileChanged(QString)), this, SLOT(printPipelineLog()));
}

void DerivedWindow::initializeRegistrationsLogging()
{
   QStringList selectedAtlases = m_parameters->getSelectedAtlases(); 

   QDir output_dir(m_parameters->getOutput());
   QString atlasRegistration_path = output_dir.filePath("3.AtlasRegistration");
   QDir atlasRegistration_dir(atlasRegistration_path);

   Logging logging; 

   QStringList::iterator it; 
   for(it = selectedAtlases.begin(); it != selectedAtlases.end(); ++it)
   {
      // Existing QPlainTextEdit
      if(m_registrationLoggings.contains(*it))
      {
         delete (m_registrationLoggings[*it]).plainTextEdit;
      }

      // QPlainTextEdit   
      QPlainTextEdit* log_plainTextEdit = new::QPlainTextEdit(execution_widget); 

      QFont font("Courier 10 Pitch", 9);
      log_plainTextEdit->setFont(font);  

      QLayout* execution_layout = execution_widget->layout(); 
      execution_layout->addWidget(log_plainTextEdit);

      log_plainTextEdit->setMaximumBlockCount(500);
      log_plainTextEdit->hide(); 

      // Atlas Directory 
      QString atlas_path = atlasRegistration_dir.filePath(*it);
      QDir atlas_dir(atlas_path); 

      // Log File
      QString log_name = *it + "_to_neo.log";
      QString log_path = atlas_dir.filePath(log_name); 
      QFile* log_file = new::QFile(log_path); 
      log_file->open(QIODevice::ReadWrite);
      QTextStream* log_textStream = new::QTextStream(log_file); 
      
      // QFileSystemWatcher
      QFileSystemWatcher* log_watcher = new::QFileSystemWatcher(this); 
      log_watcher->addPath(log_path); 
      connect(log_watcher, SIGNAL(fileChanged(QString)), this, SLOT(printRegistrationLog(QString)));

      logging.textStream = log_textStream;
      logging.plainTextEdit = log_plainTextEdit; 

      m_registrationLoggings.insert(*it, logging); 
   }

   // Radio Button
   registrations_radioButton->setEnabled(true);

   // Combo Box 
   registrations_comboBox->clear();
   registrations_comboBox->setEnabled(true); 
   registrations_comboBox->insertItems(0, selectedAtlases); 
   connect(registrations_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(changeExecutionPlainTextEdit()));
}

void DerivedWindow::printPipelineLog()
{  
   QScrollBar *scrollBar = m_log_plainTextEdit->verticalScrollBar();

   QString line = m_log_textStream->readAll();

   if(scrollBar->value() == scrollBar->maximum())
   {
      m_log_plainTextEdit->insertPlainText(line);
      scrollBar->setValue(scrollBar->maximum());
   }
   else
   {
      m_log_plainTextEdit->insertPlainText(line);
   }
}

void DerivedWindow::printRegistrationLog(QString log_path)
{  
   QString atlas_name = (QFileInfo(log_path).absoluteDir()).dirName(); 
   Logging logging = m_registrationLoggings[atlas_name];
   QTextStream* log_textStream = logging.textStream;
   QPlainTextEdit* log_plainTextEdit = logging.plainTextEdit; 

   QScrollBar *scrollBar = log_plainTextEdit->verticalScrollBar();

   if(scrollBar->value() == scrollBar->maximum())
   {
      log_plainTextEdit->insertPlainText((log_textStream->readAll()));
      scrollBar->setValue(scrollBar->maximum());
   }
   else
   {
      log_plainTextEdit->insertPlainText((log_textStream->readAll()));
   }
}

void DerivedWindow::selectLog()
{
   QString registration = registrations_comboBox->currentText();   
   Logging logging = m_registrationLoggings[registration];

   if(pipeline_radioButton->isChecked())
   {
      m_log_plainTextEdit->show();
      logging.plainTextEdit->hide(); 
   }

   if(registrations_radioButton->isChecked())
   {
      m_log_plainTextEdit->hide();
      logging.plainTextEdit->show();     
   }
}

void DerivedWindow::changeExecutionPlainTextEdit()
{
   QString atlas_name = registrations_comboBox->currentText(); 
   
   QMap <QString, Logging>::iterator it; 
   QString name; 
   Logging logging; 
   for(it = m_registrationLoggings.begin(); it != m_registrationLoggings.end(); ++it)
   {
      name = it.key(); 
      logging = it.value(); 

      if(name == atlas_name && registrations_radioButton->isChecked())
      {
         logging.plainTextEdit->show();
      }
      else
      {
         logging.plainTextEdit->hide();
      }   
   }   
}

void DerivedWindow::stopPipeline()
{
   m_thread->terminate();

   setParametersWidgetEnabled(true);

   runPipeline_button->setEnabled(true);
   runPipeline_action->setEnabled(true);

   stopPipeline_button->setEnabled(false);
   stopPipeline_action->setEnabled(false);

   runPipeline_progressBar->hide();
}

void DerivedWindow::enableDisplayButton()
{
   if( m_executables->checkExecutablePath( "InsightSNAP" ,  m_executables->getExecutablePath( "InsightSNAP" ) ) )
   {
       if(QFile(m_parameters->getSegmentation()).exists())
       {
          displayResults_button->setEnabled(true);
          displayResults_action->setEnabled(true);
       }
   }
   setParametersWidgetEnabled(true); 

   runPipeline_button->setEnabled(true);
   runPipeline_action->setEnabled(true);

   stopPipeline_button->setEnabled(false); 
   stopPipeline_action->setEnabled(false); 

   runPipeline_progressBar->hide();
   QMessageBox::information( this , tr("Information"), tr("Done") ) ;
}

void DerivedWindow::displayResults()
{
   QProcess insightSNAP_process;

   QString command = m_executables->getExecutablePath("InsightSNAP") + " -g " + m_parameters->getT1() + " -s " + m_parameters->getSegmentation(); 
   insightSNAP_process.start( command ) ;
   insightSNAP_process.waitForFinished() ;
   insightSNAP_process.waitForReadyRead() ;
   QByteArray BA =  insightSNAP_process.readAllStandardOutput() ;
   QString outputMessage = QString( BA.data() ) ;
   BA =  insightSNAP_process.readAllStandardError() ;
   outputMessage += QString( BA.data() ) ;
   if( !outputMessage.isEmpty() )
   {
       outputMessage = "Check that the selected version is 2.2 or 2.4\n\n" + outputMessage ;
       QMessageBox::information( this , tr("Error running InsightSNAP"), outputMessage ) ;
   }
}

void DerivedWindow::closeEvent(QCloseEvent *event)
{
   if(m_thread->isRunning())
   {
      QMessageBox* messageBox = new::QMessageBox(this);

      messageBox->setText(tr("The pipeline is still running,"));
      messageBox->setInformativeText(tr("Do you want to terminate it?"));
      messageBox->setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
      messageBox->setDefaultButton(QMessageBox::No);

      int answer = messageBox->exec();
      switch (answer)
      {
         case QMessageBox::Yes:
            m_thread->terminate(); 
            m_thread->wait();
            QMainWindow::closeEvent(event);
            break;
         case QMessageBox::No:
            this->hide();
            break;
         case QMessageBox::Cancel:
            event->ignore();
            break;
      }
   }
}

void DerivedWindow::comboBoxOutputImageFormat_currentIndexChanged(const QString &arg1)
{
    m_parameters->setABCOutputImageFormat(arg1);
}

void DerivedWindow::pushButtonRefreshPriors()
{
    updateNumbersOfPriorsForABC();
}
