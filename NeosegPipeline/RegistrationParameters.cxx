#include "RegistrationParameters.h"

RegistrationParameters::RegistrationParameters(QString type)
{
	m_numberOfGB_min = 1;
   m_numberOfRegistrations_default = 1;
   m_numberOfCores_default = 1;
   m_numberOfGB_default = 4;

   m_imageMetric_values << "CC" << "MI" << "PR" << "MSQ";    
   m_weight_min = 0;
   m_radius_min = 0;

   m_iterations_min = 0; 

   m_transformationType_values << "SyN" << "Diff" << "Elast" << "Exp"; 

   m_gradientStepLength_min = 0; 
   m_numberOfTimeSteps_min = 0;
   m_deltaTime_min = 0;

   m_regularizationType_values << "Gauss" << "DMFFD"; 
   m_gradientFieldSigma_min = 0;
   m_deformationFieldSigma_min = 0;

   m_registrationSoftware_default=true ;

   if(type == "DTI")
   { 
      m_name = "DTI";

      m_imageMetric_default = m_imageMetric_values[0];
      m_weight_default = 1; 
      m_radius_default = 2;

      m_iterationsJ_default = 30;
      m_iterationsK_default = 20; 
      m_iterationsL_default = 10;

      m_tranformation_default = m_transformationType_values[0];
      m_gradientStepLength_default = 0.125;    
      m_numberOfTimeSteps_default = 2;
      m_deltaTime_default = 0.01;

      m_regularizationType_default = m_regularizationType_values[0];
      m_gradientFieldSigma_default = 5;
      m_deformationFieldSigma_default = 5;

      m_usingMask_default = false;
      m_usingSmoothedMask_default = false; 
   }

   if(type == "atlas")
   {
      m_name = "atlas";

      m_numberOfRegistrations_default = 1;
      m_numberOfCores_default = 1;
      m_numberOfGB_default = 4;

      m_imageMetric_default = m_imageMetric_values[0];
      m_weight_default = 1; 
      m_radius_default = 4;

      m_iterationsJ_default = 200;
      m_iterationsK_default = 75; 
      m_iterationsL_default = 50;

      m_tranformation_default = m_transformationType_values[0];
      m_gradientStepLength_default = 0.125;   
      m_numberOfTimeSteps_default = 2;
      m_deltaTime_default = 0.01;

      m_regularizationType_default = m_regularizationType_values[0];
      m_gradientFieldSigma_default = 3;
      m_deformationFieldSigma_default = 0;

      m_usingMask_default = true;
      m_usingSmoothedMask_default = false; 

      m_container_id_default="";
      m_output_dir_default="";
   }

   m_numberOfRegistrations = m_numberOfRegistrations_default;
   m_numberOfCores = m_numberOfCores_default;
   m_numberOfGB = m_numberOfGB_default;

   m_imageMetric1 = m_imageMetric_default;
   m_imageMetric2 = m_imageMetric_default;

   m_weight1 = m_weight_default;
   m_weight2 = m_weight_default;

   m_radius1 = m_radius_default;
   m_radius2 = m_radius_default;

   m_iterationsJ = m_iterationsJ_default;
   m_iterationsK = m_iterationsK_default;
   m_iterationsL = m_iterationsL_default;

   m_transformationType = m_tranformation_default;
   m_gradientStepLength = m_gradientStepLength_default;
   m_numberOfTimeSteps = m_numberOfTimeSteps_default;
   m_deltaTime = m_deltaTime_default;

   m_regularizationType = m_regularizationType_default;
   m_gradientFieldSigma = m_gradientFieldSigma_default;
   m_deformationFieldSigma = m_deformationFieldSigma_default;

   m_usingMask = m_usingMask_default;
   m_usingSmoothedMask = m_usingSmoothedMask_default;

   m_container_id=m_container_id_default;
   m_output_dir=m_output_dir_default;

   m_registrationSoftware=m_registrationSoftware_default;


}

QString RegistrationParameters::getName()
{
   return m_name; 
}

bool RegistrationParameters::isSuperior(int value, int min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool RegistrationParameters::isSuperiorOrEqual(int value, int min)
{
   if(value>=min) 
   {
      return true; 
   }
   return false; 
}

bool RegistrationParameters::isBetween(int value, int min, int max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
}

bool RegistrationParameters::isSuperior(double value, double min)
{
   if(value>min)
   {
      return true; 
   }
   return false; 
}

bool RegistrationParameters::isSuperiorOrEqual(double value, double min)
{
   if(value>=min)
   {
      return true; 
   }
   return false; 
}


bool RegistrationParameters::isBetween(double value, double min, double max)
{
   if(value>min && value<max)
   {
      return true; 
   }
   return false; 
} 


bool RegistrationParameters::isIn(QString item, QStringList list)
{
   QStringList::iterator it; 
   for(it=list.begin(); it!=list.end(); ++it)
   {
      if(item.compare(*it, Qt::CaseInsensitive))
      {
         return true;
      }
   }
   return false; 
}
 
void RegistrationParameters::setNumberOfRegistrations(int numberOfRegistrations)
{
   m_numberOfRegistrations = numberOfRegistrations; 
}
int RegistrationParameters::getNumberOfRegistrations()
{
   return m_numberOfRegistrations; 
}

// Number Of Cores//  
void RegistrationParameters::setNumberOfCores(int numberOfCores)
{
   m_numberOfCores = numberOfCores; 
}
int RegistrationParameters::getNumberOfCores()
{
   return m_numberOfCores; 
}

// Number of GB//  
bool RegistrationParameters::checkNumberOfGB(int numberOfGB)
{
   return isSuperior(numberOfGB, m_numberOfGB_min); 
}
void RegistrationParameters::setNumberOfGB(int numberOfGB)
{
   m_numberOfGB = numberOfGB; 
}
int RegistrationParameters::getNumberOfGB()
{
   return m_numberOfGB;  
}

bool RegistrationParameters::checkImageMetric(QString imageMetric)
{
   return isIn(imageMetric, m_imageMetric_values);
}
QStringList RegistrationParameters::getImageMetricValues()
{
   return m_imageMetric_values; 
}


void RegistrationParameters::setImageMetric1(QString imageMetric1)
{
   m_imageMetric1=imageMetric1;
}
QString RegistrationParameters::getImageMetric1()
{
   return m_imageMetric1;
}
int RegistrationParameters::getImageMetric1Index()
{
   return m_imageMetric_values.indexOf(m_imageMetric1);
}

void RegistrationParameters::setImageMetric2(QString imageMetric2)
{
   m_imageMetric2 = imageMetric2;
}
QString RegistrationParameters::getImageMetric2()
{
   return m_imageMetric2;
}
int RegistrationParameters::getImageMetric2Index()
{
   return m_imageMetric_values.indexOf(m_imageMetric2);
}


double RegistrationParameters::getWeightMin()
{
   return m_weight_min;
}

void RegistrationParameters::setWeight1(double weight1)
{
   m_weight1=weight1;
}
bool RegistrationParameters::checkWeight1(double weight1)
{
   return isSuperior(weight1, m_weight_min);
}
double RegistrationParameters::getWeight1()
{
   return m_weight1;
}

void RegistrationParameters::setWeight2(double weight2)
{
   m_weight2=weight2;
}
bool RegistrationParameters::checkWeight2(double weight2)
{
   return isSuperior(weight2, m_weight_min);
}
double RegistrationParameters::getWeight2()
{
   return m_weight2;
}


int RegistrationParameters::getRadiusMin()
{
   return m_radius_min;
}

void RegistrationParameters::setRadius1(int radius1)
{
   m_radius1=radius1;
}
bool RegistrationParameters::checkRadius1(int radius1)
{
   return isSuperior(radius1, m_radius_min);
}
int RegistrationParameters::getRadius1()
{
   return m_radius1;
}

void RegistrationParameters::setRadius2(int radius2)
{
   m_radius2=radius2;
}
bool RegistrationParameters::checkRadius2(int radius2)
{
   return isSuperior(radius2, m_radius_min);
}
int RegistrationParameters::getRadius2()
{
   return m_radius2;
}

int RegistrationParameters::getIterationsMin()
{
   return m_iterations_min;
}
void RegistrationParameters::setIterationsJ(int iterationsJ)
{
   m_iterationsJ=iterationsJ;
}
bool RegistrationParameters::checkIterationsJ(int iterationsJ)
{
   return isSuperior(iterationsJ, m_iterations_min);
}
int RegistrationParameters::getIterationsJ()
{
   return m_iterationsJ;
}

void RegistrationParameters::setIterationsK(int iterationsK)
{
   m_iterationsK=iterationsK;
}
bool RegistrationParameters::checkIterationsK(int iterationsK)
{
   return isSuperior(iterationsK, m_iterations_min);
}
int RegistrationParameters::getIterationsK()
{
   return m_iterationsK;
}

void RegistrationParameters::setIterationsL(int iterationsL)
{
   m_iterationsL=iterationsL;
}
bool RegistrationParameters::checkIterationsL(int iterationsL)
{
   return isSuperior(iterationsL, m_iterations_min);
}
int RegistrationParameters::getIterationsL()
{
   return m_iterationsL;
}

void RegistrationParameters::setTransformationType(QString transformationType)
{
   m_transformationType = transformationType;
}
bool RegistrationParameters::checkTransformationType(QString transformationType)
{
   return isIn(transformationType, m_transformationType_values);
}
QString RegistrationParameters::getTransformationType()
{
   return m_transformationType;
}
int RegistrationParameters::getTransformationTypeIndex()
{
   return m_transformationType_values.indexOf(m_transformationType);
}
QStringList RegistrationParameters::getTransformationTypeValues()
{
   return m_transformationType_values; 
}

void RegistrationParameters::setGradientStepLength(double gradientStepLength)
{
   m_gradientStepLength=gradientStepLength;
}
bool RegistrationParameters::checkGradientStepLength(double gradientStepLength)
{
   return isSuperior(gradientStepLength, m_gradientStepLength_min);
}
double RegistrationParameters::getGradientStepLength()
{
   return m_gradientStepLength;
}
double RegistrationParameters::getGradientStepLengthMin()
{
   return m_gradientStepLength_min;
}

void RegistrationParameters::setNumberOfTimeSteps(double numberOfTimeSteps)
{
   m_numberOfTimeSteps=numberOfTimeSteps;
}
bool RegistrationParameters::checkNumberOfTimeSteps(double numberOfTimeSteps)
{
   return isSuperior(numberOfTimeSteps, m_numberOfTimeSteps_min);
}
double RegistrationParameters::getNumberOfTimeSteps()
{
   return m_numberOfTimeSteps;
}
double RegistrationParameters::getNumberOfTimeStepsMin()
{
   return m_numberOfTimeSteps_min;
}

void RegistrationParameters::setDeltaTime(double deltaTime)
{
   m_deltaTime=deltaTime;
}
bool RegistrationParameters::checkDeltaTime(double deltaTime)
{
   return isSuperior(deltaTime, m_deltaTime_min);
}
double RegistrationParameters::getDeltaTime()
{
   return m_deltaTime;
}
double RegistrationParameters::getDeltaTimeMin()
{
   return m_deltaTime_min;
}

void RegistrationParameters::setRegularizationType(QString regularizationType)
{
   m_regularizationType = regularizationType;
}
bool RegistrationParameters::checkRegularizationType(QString regularizationType)
{
   return isIn(regularizationType, m_regularizationType_values);
}
QString RegistrationParameters::getRegularizationType()
{
   return m_regularizationType;
}
int RegistrationParameters::getRegularizationTypeIndex()
{
   return m_regularizationType_values.indexOf(m_regularizationType);
}
QStringList RegistrationParameters::getRegularizationTypeValues()
{
   return m_regularizationType_values; 
}

void RegistrationParameters::setGradientFieldSigma(double gradientFieldSigma)
{
   m_gradientFieldSigma=gradientFieldSigma;
}
bool RegistrationParameters::checkGradientFieldSigma(double gradientFieldSigma)
{
   return isSuperior(gradientFieldSigma, m_gradientFieldSigma_min);
}
double RegistrationParameters::getGradientFieldSigma() 
{
   return m_gradientFieldSigma;
}
double RegistrationParameters::getGradientFieldSigmaMin()
{
   return m_gradientFieldSigma_min;
}

void RegistrationParameters::setDeformationFieldSigma(double deformationFieldSigma)
{
   m_deformationFieldSigma=deformationFieldSigma;
}
bool RegistrationParameters::checkDeformationFieldSigma(double deformationFieldSigma)
{
   return isSuperiorOrEqual(deformationFieldSigma, m_deformationFieldSigma_min);
}
double RegistrationParameters::getDeformationFieldSigma()
{
   return m_deformationFieldSigma;
}
double RegistrationParameters::getDeformationFieldSigmaMin()
{
   return m_deformationFieldSigma_min;
}

void RegistrationParameters::setUsingMask(bool usingMask)
{
   m_usingMask = usingMask; 
}
bool RegistrationParameters::getUsingMask()
{
   return m_usingMask; 
}

void RegistrationParameters::setUsingSmoothedMask(bool usingSmoothedMask)
{
   m_usingSmoothedMask = usingSmoothedMask; 
}
bool RegistrationParameters::getUsingSmoothedMask()
{
   return m_usingSmoothedMask; 
}

// Quicksilver container id
void RegistrationParameters::setContainerId(QString container_id)
{
   m_container_id=container_id;
}

bool RegistrationParameters::checkContainerId(QString container_id)
{
   if(container_id!="")
   {
      return true;
   }
   else
   {
      return false;
   }
}

QString RegistrationParameters::getContainerId()
{
   return m_container_id;
}

void RegistrationParameters::setOutputDir(QString output_dir)
{
   m_output_dir=output_dir;
}

QString RegistrationParameters::getOutputDir()
{
   return m_output_dir;
}

void RegistrationParameters::setUsingAnts()
{
 m_registrationSoftware=true;
}

void RegistrationParameters::setUsingQuicksilver()
{
 m_registrationSoftware=false ;
}

bool RegistrationParameters::getRegistrationSoftware()
{
   return m_registrationSoftware;
}
