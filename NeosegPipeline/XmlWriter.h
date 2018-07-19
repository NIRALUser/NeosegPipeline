#ifndef DEF_XmlWriter
#define DEF_XmlWriter

#include <iostream>

#include <QtXml>
#include <QXmlStreamWriter>

#include "PipelineParameters.h"
#include "RegistrationParameters.h"


class XmlWriter
{

   public:

      XmlWriter();

      void setPipelineParameters(PipelineParameters* parameters);

      void writeElement(QXmlStreamWriter* stream, QString tag, QString name, QString value);
      void writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2);
      void writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2, QString name3, QString value3);
      void writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2, QString name3, QString value3, QString name4, QString value4);
      void writeElement(QXmlStreamWriter* stream, QString tag, QString name1, QString value1, QString name2, QString value2, QString name3, QString value3, QString name4, QString value4, QString name5, QString value5);

      void writeData(QXmlStreamWriter* stream);
      void writeDataConfiguration(QString file_path); 

      void writeGeneralParameters(QXmlStreamWriter* stream);
      void writeRegistrationParameters(QXmlStreamWriter* stream, RegistrationParameters* registrationParameters);
      void writeNeosegParameters(QXmlStreamWriter* stream);
      void writeAntsJointFusionParameters(QXmlStreamWriter* stream);
      void writeParametersConfiguration(QString file_path); 

      void writeExecutables(QXmlStreamWriter* stream);
      void writeExecutablesConfiguration(QString file_path); 

   private:
      
      PipelineParameters* m_parameters; 
};

#endif
