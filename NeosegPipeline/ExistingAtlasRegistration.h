#ifndef DEF_ExistingAtlasRegistration
#define DEF_ExistingAtlasRegistration

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
#include "Neo.h"
#include "RegistrationParameters.h"
#include "ExecutablePaths.h"

class ExistingAtlasRegistration : public Script  
{
   public: 

   // Constructor // 
   ExistingAtlasRegistration(QString module);

   // Set // 
   void setRegistrationParameters(RegistrationParameters* REGISTRATIONParamaters);
   void setExistingAtlas(QString atlas_path);

   // Scripts //
   void initializeScript();

   // Register Atlas Script // 
   QString getFilePath(QDir* dir, QString name);
   void implementRun();

   // Output //
   void update();
   QString getOutput();


   private:
 
   // Input // 
   RegistrationParameters*         m_parameters;
   
   QString m_atlas_path; 
   QDir* m_atlas_dir; 

};

#endif 
