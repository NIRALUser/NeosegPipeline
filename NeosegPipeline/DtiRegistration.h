#ifndef DEF_DtiRegistration
#define DEF_DtiRegistration


// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>

// Qt Librairies
#include <QString>
#include <QDir>

// My Specific Librairies
#include "Script.h"
#include "Neo.h"
#include "RegistrationParameters.h"
#include "ExecutablePaths.h"


class DtiRegistration : public Script
{
   public:

   // Constructor
   DtiRegistration(QString module); 

   // Set 
   void setRegistrationParameters(RegistrationParameters* registrationParameters);

   // Implementing Scripts 
   void initializeScript();
   void upsample(QString image); 
   void generateDTIAndb0();
   void skullStripb0();
   void skullStripDTI();
   void generateFA();
   void generateAD();
   void calculateTransformations();
   void applyTransformations(QString image);
   void implementRun(); 

   // Output //
   void update();
   Neo getOutput();

   private:
  
   // Suffix
   QString m_grid_suffix;
   QString m_upsampling_suffix; 
   QString m_stripping_suffix;       
   QString m_registering_suffix;

   // Ants Parameters 
   RegistrationParameters* m_registrationParameters; 

   // Output 
   Neo      m_registeredNeo;
}; 

#endif
