#ifndef DEF_RegistrationParameters
#define DEF_RegistrationParameters

#include <iostream>

#include <QString>
#include <QStringList>

class RegistrationParameters
{
   public:

   RegistrationParameters();

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

	protected:

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
};

#endif
