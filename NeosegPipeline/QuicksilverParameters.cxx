#include "RegistrationParameters.h"
#include "QuicksilverParameters.h"

QuicksilverParameters::QuicksilverParameters()
{
   qs_registrationscript_path_default=" ";
   qs_container_id_default=0;

   qs_registrationscript_path=qs_registrationscript_path_default;
   qs_container_id=qs_container_id_default;
}

// Path to the regisration script quicksilver
void QuicksilverParameters::setRegistrationScriptPath(QString registrationscript_path)
{
   qs_registrationscript_path=registrationscript_path;
}

bool QuicksilverParameters::checkRegistrationScriptPath(QString registrationscript_path)
{
   if(registrationscript_path!=NULL)
   {
      return true;
   }
   else
   {
      return false;
   }
}

QString QuicksilverParameters::getRegistrationScriptPath()
{
   return qs_registrationscript_path;
}

// Quicksilver container id
void QuicksilverParameters::setContainerId(int container_id)
{
   qs_container_id=container_id;
}

bool QuicksilverParameters::checkContainerId(int container_id)
{
   if(container_id!=0)
   {
      return true;
   }
   else
   {
      return false;
   }
}

int QuicksilverParameters::getContainerId()
{
   return qs_container_id;
}
