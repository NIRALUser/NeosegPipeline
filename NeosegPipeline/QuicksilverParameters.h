#ifndef DEF_QuicksilverParameters
#define DEF_QuicksilverParameters

#include <iostream>
#include <RegistrationParameters.h>
#include <QString>
#include <QStringList>

class QuicksilverParameters : public RegistrationParameters
{
   public:

   // Constructor 
   QuicksilverParameters();

   // Path to the registration script
   void setRegistrationScriptPath(QString registrationscript_path);
   bool checkRegistrationScriptPath(QString registrationscript_path);
   QString getRegistrationScriptPath();

   void setContainerId(int container_id);
   bool checkContainerId(int container_id);
   int getContainerId();

   private:

   //Path to registration script   
   QString qs_registrationscript_path_default;
   QString qs_registrationscript_path;

   //Docker container id
   int qs_container_id_default;
   int qs_container_id;
};

#endif