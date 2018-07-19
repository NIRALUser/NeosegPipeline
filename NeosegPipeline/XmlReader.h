#ifndef DEF_XmlReader
#define DEF_XmlReader

#include <QXmlStreamReader>

#include "PipelineParameters.h"
#include "RegistrationParameters.h"
#include "NeosegParameters.h"
#include "ExecutablePaths.h"
#include "LibraryPaths.h"

class XmlReader
{

   public:

   void setPipelineParameters(PipelineParameters* parameters);

   bool isBoolean(int value);

   QString readParametersConfigurationFile(QString file_path); 
   void readGeneralParameters(QXmlStreamReader* stream, QString errors);
   void readRegistrationParameters(QXmlStreamReader* stream, QString errors, RegistrationParameters* parameters);
   void readNeosegParameters(QXmlStreamReader* stream, QString errors);
   void readABCParameters(QXmlStreamReader* stream, QString errors);
   void readAntsJointFusionParameters(QXmlStreamReader* stream, QString errors);

   QString readExecutablesConfigurationFile(QString file_path);
   QString readDataConfigurationFile( QString file_path ) ;

   private:

   PipelineParameters* m_parameters;  
   RegistrationParameters* m_antsParameters_DTI;  
   RegistrationParameters* m_registrationParameters_atlas; 
   NeosegParameters* m_neosegParameters; 
   AntsJointFusionParameters* m_antsJointFusionParameters;
   ExecutablePaths* m_executablePaths;
   LibraryPaths* m_libraryPaths;
   
   QStringList m_selectedAtlases; 

}; 



#endif 

