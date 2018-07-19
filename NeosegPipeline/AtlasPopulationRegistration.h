#ifndef DEF_AtlasPopulationRegistration
#define DEF_AtlasPopulationRegistration

// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <map>
#include <utility>
#include <string>

// Qt Librairies
#include <QString>
#include <QDir>

// My Specific Librairies
#include "Script.h"
#include "Atlas.h"
#include "Neo.h"
#include "RegistrationParameters.h"
#include "ExecutablePaths.h"

class AtlasPopulationRegistration : public Script  
{
   
   public: 

   // Constructor // 
   AtlasPopulationRegistration(QString module);

   // Set // 
   void setAtlasPopulation(std::vector<Atlas> atlasPopulation);  
   void setComputingSystem(QString computingSystem);
   void setRegistrationParameters(RegistrationParameters* RegistrationParameters);

   // Define Outputs //
   Atlas defineRegisteredAtlas(Atlas atlas);
   std::vector<Atlas> defineRegisteredAtlasPopulation();

   // Creating directories //
   void createDirectory();
   void createAtlasDirectories();

   // Scripts //
   void initializeScript();
   void implementExecute();

   // Register Atlas Script // 
   /*void implementRegisterAtlas(QString &script, bool probabilistic);
   void writeRegisterAtlas();
   void writeRegisterProbabilisticAtlas();*/

   // Register Atlas Population Script // 
   void implementTestRegistrationDone();
   void defineRegisterAtlasParameters(Atlas atlas);
   void submitRegisterAtlasJob(Atlas atlas, int i);
   void executeRegisterAtlasProcess(Atlas atlas, int i);
   void implementRegisterAtlasPopulation();
   void writeRegisterAtlasPopulation();

   // Output //
   void update();
   std::vector<Atlas> getOutput();

   private:

   // Input // 
   std::vector<Atlas>      m_atlasPopulation;
   RegistrationParameters* m_parameters;
   QString                 m_computingSystem;
   int                     m_nbCores;

};

#endif 
