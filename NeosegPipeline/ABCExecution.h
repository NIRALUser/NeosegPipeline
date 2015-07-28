#ifndef ABCEXECUTION_H
#define ABCEXECUTION_H

#include <iostream>

#include <QtXml>
#include <QXmlStreamWriter>

#include "Script.h"
#include "PipelineParameters.h"

class ABCExecution : public Script
{
public:
    ABCExecution(QString module);

    void writeXMLFile();
    void writeABCParameters(QXmlStreamWriter *stream);

    void SetPipelineParameters(PipelineParameters* params){
        m_parameters = params;
    }

    void update();

    void initialize();
    void implementRun();

private:
    PipelineParameters* m_parameters;
};

#endif // ABCEXECUTION_H