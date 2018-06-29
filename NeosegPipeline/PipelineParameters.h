#ifndef DEF_PipelineParameters
#define DEF_PipelineParameters

// General Librairies // 
#include <iostream>
#include <vector>
#include <iomanip> 
#include <string>

// Qt Librairies // 
#include <QString>
#include <QDir>
#include <QFile>

// My Specific Librairies //
#include "Neo.h"
#include "Atlas.h"
#include "RegistrationParameters.h" 
#include "AntsJointFusionParameters.h"
#include "NeosegParameters.h" 
#include "ExecutablePaths.h" 
#include "LibraryPaths.h" 
#include "MinMax.h"
#include "Origin.h"

const int TISSUE_SEG_TYPE_NEOSEG = 0;
const int TISSUE_SEG_TYPE_ABC = 1;
const int TISSUE_SEG_TYPE_ANTSJOINTFUSION = 2;

const int REGISTRATION_TYPE_ANTS = 0;
const int REGISTRATION_TYPE_QUICKSILVER = 1;


class PipelineParameters
{
   public:

   PipelineParameters();

   bool isBoolean(int value);
   bool isSuperior(int value, int min);
   bool isSuperior(double value, double min);
   bool isBetween(int value, int min, int max);
   bool isBetween(double value, double min, double max);
   bool isIn(QString item, QStringList list);
   QFileInfoList find(QDir &dir, QString name , QString pre , QString post ) ;

   // Current Directory
   void setProgramPath(QString programPath); 
   QString getProgramPath();

   bool checkPrefixSuffix(QString string); 
   QString getForbiddenCharacters(); 

   // Prefix
   void setPrefix(QString prefix);
   QString getPrefix();

   // Suffix
   void setSuffix(QString suffix);
   QString getSuffix();

   // Output
   bool checkOutput(QString output);
   void setOutput(QString output);
   QString getOutput();

   // T1
   bool checkT1(QString T1);
   void setT1(QString T1);
   QString getT1(); 

   // T2
   bool checkT2(QString T2);
   void setT2(QString T2);
   QString getT2(); 

   // Mask 
   bool checkMask(QString mask);
   void setMask(QString Mask);
   QString getMask(); 

   // DWI 
   bool checkDWI(QString DWI);
   void setDWI(QString DWI);
   QString getDWI(); 

   // Neo
   void initializeNeo();
   void setNeo(Neo neo); 
   Neo getNeo(); 

   // SkullStripping 
   void setSkullStripping(bool skullStripping);
   bool getSkullStripping(); 

   // Correcting 
   void setCorrecting(bool correcting);
   bool getCorrecting();  

   // New Atlas    
   void setNewAtlas(bool newAtlas);
   bool getNewAtlas();

   // Existing Atlas
   bool checkExistingAtlas(QString existingAtlas);
   int setExistingAtlas( QString existingAtlas , bool alreadyExists ) ;
   QString getExistingAtlas(); 

   // Existing Atlas
   void setAtlasFormat();
   QString getAtlasFormat(); 

   // Atlas Population Directory 
   bool checkAtlasPopulationDirectory(QString atlasPopulationDirectory);
   void setAtlasPopulationDirectory(QString atlasPopulationDirectory);
   QString getAtlasPopulationDirectory(); 

   // Selected Atlases
   void setSelectedAtlases(QStringList selectedAtlases);
   QStringList getSelectedAtlases(); 

   // Atlas Population 
   QMap<QString,QFileInfoList> findAtlasFiles(QString atlas);
   bool checkAtlasFiles(QString atlas);
   bool checkAtlasRange(QString atlas);
   bool checkAtlas(QString atlas);
   void initializeAtlasPopulation();
   void setAtlasPopulation(std::vector<Atlas> m_atlasPopulation);
   std::vector<Atlas> getAtlasPopulation();   

   // Smoothing 
   bool checkSmoothing(QString smoothing);
   QString getSmoothingFlag();
   void setSmoothing(QString smoothing);
   QString getSmoothing();
   int getSmoothingIndex();
   QStringList getSmoothingValues(); 

   // Smoothing Size
   bool checkSmoothingSize(double smoothingSize);
   void setSmoothingSize(double smoothingSize);
   double getSmoothingSize();

   // Computing Weights
   void setComputingWeights(int computingWeights);
   bool getComputingWeights();

   // Weights Modality 
   bool checkWeightsModality(QString weightsModality);
   void setWeightsModality(QString weightsModality);
   QString getWeightsModality();
   int getWeightsModalityIndex();
   QStringList getWeightsModalityValues(); 

   // Weights Radius 
   bool checkWeightsRadius(double weightsRadius);
   void setWeightsRadius(double weightsRadius);
   double getWeightsRadius();

   // Weights Radius Unit
   bool checkWeightsRadiusUnit(QString radiusUnit);
   void setWeightsRadiusUnit(QString radiusUnit);
   QString getWeightsRadiusUnit();
   int getWeightsRadiusUnitIndex();
   QStringList getWeightsRadiusUnitValues();

   // Including FA
   void setIncludingFA(bool includingFA);
   bool getIncludingFA();

   // FA Shift
   bool checkFAShift(double FAShift);
   void setFAShift(double FAShift);
   double getFAShift();

   // FA Sigma Scale
   bool checkFASigmaScale(double FASigmaScale);
   void setFASigmaScale(double FASigmaScale);
   double getFASigmaScale();

   // FA Weight
   bool checkFAWeight(double FAWeight);
   void setFAWeight(double FAWeight);
   double getFAWeight(); 

   // FA Smoothing Size
   bool checkFASmoothingSize(double FASmoothingSize);
   void setFASmoothingSize(double FASmoothingSize);
   double getFASmoothingSize(); 

   // Using FA
   void setUsingFA(bool usingFA);
   bool getUsingFA();

   // Using AD
   void setUsingAD(bool usingAD);
   bool getUsingAD();

   // Computing 3-Labels Segmentation
   void setComputing3LabelsSeg(bool computing3LabelsSeg);
   bool getComputing3LabelsSeg();   

   // Reassigning White Matter 
   void setReassigningWhiteMatter(bool reassigningWhiteMatter); 
   bool getReassigningWhiteMatter(); 

   // Size Threshold 
   bool checkSizeThreshold(int sizeThreshold);
   void setSizeThreshold(int sizeThreshold);
   int getSizeThreshold();

   // Overwriting
   void setOverwriting(bool overwriting);
   bool getOverwriting();

   // Cleaning-Up 
   void setCleaningUp(bool cleaningUp);
   bool getCleaningUp(); 

   // Stopping If Error
   void setStoppingIfError(bool stoppingIfError); 
   bool getStoppingIfError();  

   // Computing System 
   bool checkComputingSystem(QString computingSystem);
   void setComputingSystem(QString computingSystem);
   QString getComputingSystem(); 
   int getComputingSystemIndex(); 
   QStringList getComputingSystemValues(); 

   // Number Of Cores 
   bool checkNumberOfCores(int NumberOfCores);
   void setNumberOfCores(int NumberOfCores); 
   int getNumberOfCores();

   // ANTS DTI Parameters 
   RegistrationParameters* getAntsParametersDTI();  

   // Registration Parameters
   RegistrationParameters* getRegistrationParameters();

   // AntsJointFusion Parameters
   AntsJointFusionParameters* getAntsJointFusionParameters();
   
   // Neoseg Parameters
   NeosegParameters* getNeosegParameters(); 

   // Executable Paths 
   ExecutablePaths* getExecutablePaths(); 

   // Library Paths 
   LibraryPaths* getLibraryPaths();

   // Segmentation
   QString getSegmentation(); 


   QString checkImages();

   // Typer of registration 0 for Ants 1 for Quicksilver
   void setRegistrationType(int type){
       m_antsRegistrationType =type;
   }

   /*
    * @return Type of Registration 0 for Ants 1 for Quicksilver
    */
   int getRegistrationType(){
       return m_antsRegistrationType;
   }

   // Type of tissue segmentation 0 for Neoseg 1 for ABC 2 for AntsJointFusion
   void setTissueSegmentationType(int type){
       m_abcTissueSegmentationType = type;
   }

   /*
    * @return Type of tissue segmentation 0 for Neoseg 1 for ABC 2 for antsJointFusion
    */
   int getTissueSegmentationType(){
       return m_abcTissueSegmentationType;
   }


   //Parameters for ABC execution
   void setABCPriorsCoefficients(std::vector<double> priorsCoefficients){
       m_abcPriorsCoefficients = priorsCoefficients;
   }

   std::vector<double> getABCPriorsCoefficients(){
       return m_abcPriorsCoefficients;
   }

   void setABCInitialDistributorEstimatorType(QString estimator){
       m_abcInitialDistributorEstimator = estimator;
   }

   QString getABCInitialDistributorEstimatorType(){
       return m_abcInitialDistributorEstimator;
   }

   void setABCMaximumDegreeBiasField(double bias){
       m_abcMaximumDegreeBiasField = bias;
   }

   double getABCMaximumDegreeBiasField(){
       return m_abcMaximumDegreeBiasField;
   }

   void setABCOutputImageFormat(QString format){
       m_abcOutputImageFormat = format;
   }

   QString getABCOutputImageFormat(){
       return m_abcOutputImageFormat;
   }

   typedef std::map<double, int> InputImageLabelMapType;
   typedef InputImageLabelMapType::iterator InputImageLabelMapIteratorType;
   void setImageLabelMap(InputImageLabelMapType labelsvaluetype){
       m_ImageLabelMap = labelsvaluetype;
   }

   InputImageLabelMapType getImageLabelMap(){
       return m_ImageLabelMap;
   }

   void setABCWhiteImageIndex(QString index){
       m_ABCWhiteImageIndex = index;
   }

   QString getABCWhiteImageIndex(){
       return m_ABCWhiteImageIndex;
   }

   struct ABCReassignLabels{
       bool m_ReassignEnabled;
       double m_Label;
       int m_Index;
       int m_Threshold;
       bool m_VoxelByVoxel;
   };

   typedef ABCReassignLabels ABCReassignLabelsType;

   typedef std::vector<ABCReassignLabelsType> ABCVectorReassignLabelsType;

   void setABCReassignLabels(ABCVectorReassignLabelsType reassign){
       m_ABCReassignLabels = reassign;
   }

   ABCVectorReassignLabelsType getABCReassignLabels(){
       return m_ABCReassignLabels;
   }

   void setNumberOfLabels(int labels){
       m_NumberOfLabels = labels;
   }

   /*int m_abcTissueSegmentationType;
   int m_antsRegistrationType;*/
   
   private:

   int m_abcTissueSegmentationType;
   int m_antsRegistrationType;
   std::vector<double> m_abcPriorsCoefficients;
   QString m_abcInitialDistributorEstimator;
   double m_abcMaximumDegreeBiasField;
   QString m_abcOutputImageFormat;
   InputImageLabelMapType m_ImageLabelMap;
   QString m_ABCWhiteImageIndex;
   ABCVectorReassignLabelsType m_ABCReassignLabels;
   int m_NumberOfLabels;

   QString m_programPath; 
   
   QStringList m_forbiddenCharacters; 

   // Data
   QString m_prefix;
   QString m_suffix;
   QString m_output;
   QString m_T1; 
   QString m_T2;
   QString m_mask; 
   QString m_DWI; 

   Neo m_neo; 

   // SkullStripping 
   bool m_skullStripping_default; 
   bool m_skullStripping;

   // Correcting
   bool m_correcting_default; 
   bool m_correcting;    

   // New Atlas 
   bool m_newAtlas_default;
   bool m_newAtlas;

   // Existing Atlas
   QString m_existingAtlas; 

   // Atlas Format
   QString m_atlasFormat_default; 
   QString m_atlasFormat; 

   // Atlas Population Directory
   QString m_atlasPopulationDirectory_default; 
   QString m_atlasPopulationDirectory; 

   // Atlas Population
   int m_precision;
   QStringList m_selectedAtlases; 
   std::vector<Atlas> m_atlasPopulation; 

   // Smoothing 
   QStringList m_smoothing_values;
   QString m_smoothing_default;
   QString m_smoothing;

   // Smoothing Size
   double m_smoothingSize_min;
   double m_smoothingSize_max;
   double m_smoothingSize_default;
   double m_smoothingSize;

   // Computing Weights
   bool m_computingWeights_default;
   bool m_computingWeights;

   // Weights Modality
   QStringList m_weightsModality_values;
   QString m_weightsModality_default;
   QString m_weightsModality;

   // Weights Radius
   double m_weightsRadius_min;
   double m_weightsRadius_max;
   double m_weightsRadius_default;
   double m_weightsRadius;

   // Weights Radius Unit
   QStringList m_weightsRadiusUnit_values; 
   QString m_weightsRadiusUnit_default; 
   QString m_weightsRadiusUnit; 

   // Including FA
   bool m_includingFA_default; 
   bool m_includingFA;    

   // FA Shift 
   double m_FAShift_min; 
   double m_FAShift_max; 
   double m_FAShift_default; 
   double m_FAShift; 

   // FA Sigma Scale
   double m_FASigmaScale_min;
   double m_FASigmaScale_default; 
   double m_FASigmaScale; 

   // FA Weight
   double m_FAWeight_min;
   double m_FAWeight_default; 
   double m_FAWeight; 

   // FA Smoothing Size 
   double m_FASmoothingSize_min;
   double m_FASmoothingSize_default;
   double m_FASmoothingSize;
    
   // Using FA
   bool m_usingFA_default; 
   bool m_usingFA; 

   // Using AD
   bool m_usingAD_default; 
   bool m_usingAD; 

   // Computing 3-labels segmentation
   bool m_computing3LabelsSeg_default; 
   bool m_computing3LabelsSeg;

   // Reassigning White Matter
   bool m_reassigningWhiteMatter_default; 
   bool m_reassigningWhiteMatter; 

   // Size Threshold 
   int m_sizeThreshold_min;
   int m_sizeThreshold_default;
   int m_sizeThreshold; 

   // Overwriting 
   bool m_overwriting_default;
   bool m_overwriting;

   // Cleaning-Up
   bool m_cleaningUp_default;
   bool m_cleaningUp;

   // Stopping if error 
   bool m_stoppingIfError_default; 
   bool m_stoppingIfError; 

   // Computing System
   QStringList m_computingSystem_values;
   QString m_computingSystem_default;
   QString m_computingSystem;

   // Number Of Cores
   int m_numberOfCores_min;
   int m_numberOfCores_max;
   int m_numberOfCores_default;
   int m_numberOfCores;

 
   // Registration Parameters
   RegistrationParameters* m_registrationParameters_atlas;
   RegistrationParameters* m_antsParameters_DTI;

   // AntsJointFusion parameters
   AntsJointFusionParameters* m_antsJointFusionParameters;

   // Neoseg Parameters
   NeosegParameters* m_neosegParameters;

   // Executable Paths 
   ExecutablePaths* m_executablePaths;

   // Library Paths 
   LibraryPaths* m_libraryPaths;
};


#endif
