#include "RegistrationParameters.h"

RegistrationParameters::RegistrationParameters()
{
	m_numberOfGB_min = 1;
   m_numberOfRegistrations_default = 1;
   m_numberOfCores_default = 1;
   m_numberOfGB_default = 4;

   m_numberOfRegistrations = m_numberOfRegistrations_default;
   m_numberOfCores = m_numberOfCores_default;
   m_numberOfGB = m_numberOfGB_default;

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