#ifndef DEF_RegistrationParameters
#define DEF_RegistrationParameters

#include <iostream>

#include <QString>
#include <QStringList>

class RegistrationParameters
{
   public:

   // Constructor 
   RegistrationParameters(QString type);

   bool checkImageMetric(QString imageMetric);
   QStringList getImageMetricValues(); 

   // Name 
   QString getName(); 

   // Tests
   bool isSuperior(int value, int min);
   bool isSuperior(double value, double min);

   bool isSuperiorOrEqual(int value, int min);
   bool isSuperiorOrEqual(double value, double min);

   bool isBetween(int value, int min, int max);
   bool isBetween(double value, double min, double max);
   
   bool isIn(QString item, QStringList list);

   void setNumberOfRegistrations(int numberOfRegistrations);
   int getNumberOfRegistrations(); 

   // Number of Cores 
   void setNumberOfCores(int numberOfCores);
   int getNumberOfCores(); 

   // Number of GB
   bool checkNumberOfGB(int numberOfGB);
   void setNumberOfGB(int numberOfGB);
   int getNumberOfGB(); 

   // Image Metric 1 
   void setImageMetric1(QString imageMetric1);
   QString getImageMetric1();
   int getImageMetric1Index();

   // Image Metric 2
   void setImageMetric2(QString imageMetric2);
   QString getImageMetric2();
   int getImageMetric2Index();

   double getWeightMin();

   // Weight1
   void setWeight1(double weight1);
   bool checkWeight1(double weight1);
   double getWeight1();

   // Weight 2 
   void setWeight2(double weight2);
   bool checkWeight2(double weight2);
   double getWeight2();

   int getRadiusMin();

   // Radius 1 
   void setRadius1(int radius1);
   bool checkRadius1(int radius1);
   int getRadius1();

   // Radius 2 
   void setRadius2(int radius2);
   bool checkRadius2(int radius2);
   int getRadius2();

   int getIterationsMin();    
  
   // Iterations J 
   void setIterationsJ(int iterationJ);
   bool checkIterationsJ(int iterationsJ);
   int getIterationsJ();

   // Iterations K 
   void setIterationsK(int iterationK);
   bool checkIterationsK(int iterationsK);
   int getIterationsK();

   // Iterations L
   void setIterationsL(int iterationL);
   bool checkIterationsL(int iterationsL);
   int getIterationsL();

   // Transformation type 
   void setTransformationType(QString transformationType);
   bool checkTransformationType(QString transformationType);
   QString getTransformationType();
   int getTransformationTypeIndex(); 
   QStringList getTransformationTypeValues(); 

   // Gradient step Length
   double getGradientStepLengthMin();
   void setGradientStepLength(double gradientStepLength);
   bool checkGradientStepLength(double gradientStepLength);
   double getGradientStepLength();

   // Number of time steps
   double getNumberOfTimeStepsMin();
   void setNumberOfTimeSteps(double numberOfTimeSteps);
   bool checkNumberOfTimeSteps(double numberOfTimeSteps);
   double getNumberOfTimeSteps();

   // Delta time 
   double getDeltaTimeMin();
   void setDeltaTime(double deltaTime);
   bool checkDeltaTime(double deltaTime);
   double getDeltaTime();

   // Regularization type 
   void setRegularizationType(QString regularizationType);
   bool checkRegularizationType(QString regularizationType);
   QString getRegularizationType();
   int getRegularizationTypeIndex(); 
   QStringList getRegularizationTypeValues(); 
   
   // Gradient field sigma
   double getGradientFieldSigmaMin();
   void setGradientFieldSigma(double gradientFieldSigma);
   bool checkGradientFieldSigma(double gradientFieldSigma);
   double getGradientFieldSigma();

   // Deformation field sigma 
   double getDeformationFieldSigmaMin();
   void setDeformationFieldSigma(double deformationFieldSigma);
   bool checkDeformationFieldSigma(double deformationFieldSigma);
   double getDeformationFieldSigma();

   // Using mask 
   void setUsingMask(bool usingMask); 
   bool getUsingMask(); 

   // Using smoothed mask
   void setUsingSmoothedMask(bool usingSmoothedMask); 
   bool getUsingSmoothedMask();

   void setContainerId(QString container_id);
   bool checkContainerId(QString container_id);
   QString getContainerId();

   void setOutputDir(QString output_dir);
   QString getOutputDir();

   void setUsingAnts() ;
   void setUsingQuicksilver() ;
   bool getRegistrationSoftware();

   void setBoolParc(bool bool_parc);
   bool getBoolParc();
   
   void setBoolRoi(bool bool_roi);
   bool getBoolRoi();
   
   void setRoiNbr(int nbr_roi);
   int getRoiNbr();

   bool m_registrationSoftware_default;
   bool m_registrationSoftware;

	private:

   // Number of Registrations  
   int m_numberOfRegistrations_default;   
   int m_numberOfRegistrations;   

   // Number of Cores Requested 
   int m_numberOfCores_min; 
   int m_numberOfCores_default; 
   int m_numberOfCores; 

   // Number Of GB
   int m_numberOfGB_min; 
   int m_numberOfGB_default;
   int m_numberOfGB;

    // Name 
   QString m_name; 

   // Image Metrics
   QStringList m_imageMetric_values; 
   QString m_imageMetric_default;
   QString m_imageMetric1;
   QString m_imageMetric2;

   // Weights
   double m_weight_min;
   double m_weight_max;
   double m_weight_default; 
   double m_weight1;
   double m_weight2;

   // Radius
   int m_radius_min;
   int m_radius_max;
   int m_radius_default;
   int m_radius1;
   int m_radius2;

   // Iterations J
   int m_iterations_min; 
   int m_iterationsJ_max; 
   int m_iterationsJ_default;
   int m_iterationsJ;

   // Iterations K
   int m_iterationsK_min; 
   int m_iterationsK_max; 
   int m_iterationsK_default; 
   int m_iterationsK;

   // Iterations L
   int m_iterationsL_min; 
   int m_iterationsL_max; 
   int m_iterationsL_default;
   int m_iterationsL;

   // Transformation Type
   QStringList m_transformationType_values;
   QString m_tranformation_default;
   QString m_transformationType;

   // Gradient Step Lenght
   double m_gradientStepLength_min;   
   double m_gradientStepLength_max;   
   double m_gradientStepLength_default;   
   double m_gradientStepLength;

   // Number Of Time Step
   double m_numberOfTimeSteps_min;
   double m_numberOfTimeSteps_max;
   double m_numberOfTimeSteps_default;
   double m_numberOfTimeSteps;

   // Delta Time
   double m_deltaTime_min;
   double m_deltaTime_max;
   double m_deltaTime_default;
   double m_deltaTime;

   // Regularization Type
   QStringList m_regularizationType_values;
   QString m_regularizationType_default;
   QString m_regularizationType;

   // Gradient Field Sigma
   double m_gradientFieldSigma_min;
   double m_gradientFieldSigma_max;
   double m_gradientFieldSigma_default;
   double m_gradientFieldSigma;

   // Deformation field
   double m_deformationFieldSigma_min;
   double m_deformationFieldSigma_max;
   double m_deformationFieldSigma_default;
   double m_deformationFieldSigma;

   // Using Mask
   bool m_usingMask_default;
   bool m_usingMask; 

   // Using Smoothed Mask
   bool m_usingSmoothedMask_default;
   bool m_usingSmoothedMask; 

   //Docker container id
   QString m_container_id_default;
   QString m_container_id;

   QString m_output_dir_default;
   QString m_output_dir;

   bool m_parc_ok;
   bool m_roi_ok;
   int m_roi_nbr;
};

#endif
