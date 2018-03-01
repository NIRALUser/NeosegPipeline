#ifndef DEF_AtlasRegistration
#define DEF_AtlasRegistration

// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <utility>

// Qt Librairies
#include <QString>
#include <QDir>

// My Specific Librairies
#include "Script.h"
#include "Atlas.h"
#include "Neo.h"
#include "RegistrationParameters.h"
/*#include "AntsParameters.h"
#include "QuicksilverParameters.h"*/
#include "ExecutablePaths.h"
#include "MinMax.h"

class AtlasRegistration : public Script  
{
   
   public: 

   // Constructor // 
   AtlasRegistration(QString module);

   // Set // 
   void setLog(QString log_path); 
   /*void setAntsParameters(AntsParameters* AntsParameters);
   void setQuicksilverParameters(QuicksilverParameters* QuicksilverParameters);*/
   void setRegistrationParameters(RegistrationParameters* RegistrationParameters);

   // Scripts //
   void initializeScript();
   void initializeLogging();

   // Register Atlas Script // 
   void implementRegisterAtlas(bool probabilistic);
   void writeRegisterAtlas();
   void writeRegisterProbabilisticAtlas();

   // Output //
   void update();
   std::vector<Atlas> getOutput();


   private:
   // Log Path 
   QString m_log_path; 

   // Input // 
   RegistrationParameters* m_parameters;


};

#endif 
