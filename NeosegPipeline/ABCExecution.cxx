#include "ABCExecution.h"

ABCExecution::ABCExecution(QString module)
    : Script(module)
{

}

void ABCExecution::update(){

    initialize();
    writeXMLFile();
    implementStop();
    implementCheckFileExistence();
    implementExecute();
    implementRun();
    implementReassignImageIslands();
    writeScript();
}

void ABCExecution::initialize(){
    definePython();
    importGeneralModules();
    importXmlModules();

    defineExecutable("ABC");


    m_script += "logger = logging.getLogger('NeosegPipeline')\n\n";

    m_script += "runningProcess = None\n\n";
}

void ABCExecution::implementRun(){

    m_script += "def run():\n";

    m_script += m_indent + "signal.signal(signal.SIGINT, stop)\n";
    m_script += m_indent + "signal.signal(signal.SIGTERM, stop)\n\n";

    m_script += m_indent + "logger.info('=== ABC Execution ===')\n";

    m_script += m_indent + "logger.info('Rename generated atlas')\n";


    QString atlasdir = m_parameters->getExistingAtlas() + QString("/");
    QString atlasExt = QString(m_parameters->getAtlasFormat());
    //T1 or T2
    QString imagetype = m_parameters->getNeosegParameters()->getReferenceImage();


    QStringList inputStringList;
    QStringList outputStringList;

    inputStringList.push_back(atlasdir + QString("template") + imagetype + QString(".") + atlasExt );
    outputStringList.push_back(atlasdir + QString("template.") + atlasExt);

    inputStringList.push_back(atlasdir + QString("white.") + atlasExt);
    inputStringList.push_back(atlasdir + QString("csf.") + atlasExt);
    inputStringList.push_back(atlasdir + QString("gray.") + atlasExt);
    inputStringList.push_back(atlasdir + QString("rest.") + atlasExt);

    outputStringList.push_back(atlasdir + QString("1.") + atlasExt);
    outputStringList.push_back(atlasdir + QString("2.") + atlasExt);
    outputStringList.push_back(atlasdir + QString("3.") + atlasExt);

    if(m_parameters->getTissueSegmentationType() == TISSUE_SEG_TYPE_NEOSEG){
        outputStringList.push_back(atlasdir + QString("4.") + atlasExt);
    }else if(m_parameters->getTissueSegmentationType() == TISSUE_SEG_TYPE_ABC){
        int numberofpriors = m_parameters->getABCPriorsCoefficients().size();
        outputStringList.push_back(atlasdir + QString::number(numberofpriors) + QString(".") + atlasExt);
    }

    addRename(inputStringList, outputStringList);

    m_script+= m_indent + "args = [ABC, parameters_path]\n";
    m_script+= m_indent + "execute(args)\n";
}

void ABCExecution::addRename(QString s1, QString s2){
    m_script += m_indent + "if checkFileExistence('"+ s1 + "')==True:\n";
    m_script += m_indent + m_indent + "os.rename('" + s1 + "', '" + s2 + "')\n";
}

void ABCExecution::addRename(QStringList l1, QStringList l2){
    for(QStringList::iterator s1 = l1.begin(), s2 = l2.begin(); s1 != l1.end() && s2 != l2.end(); ++s1, ++s2){
        addRename(*s1, *s2);
    }
}

void ABCExecution::writeXMLFile(){

    QString XML_path = m_module_dir->filePath("parameters.xml");
    m_script += "parameters_path = \"" + XML_path + "\"\n";

    QFile* file = new::QFile(XML_path);
    file->open(QIODevice::WriteOnly);
    QXmlStreamWriter* stream = new::QXmlStreamWriter(file);
    stream->setAutoFormatting(true);

    stream->writeStartDocument();
    stream->writeDTD("<!DOCTYPE SEGMENTATION-PARAMETERS>");

    this->writeXML(stream);

    stream->writeEndDocument();

    file->close();
}

void ABCExecution::writeXMLFile(QXmlStreamWriter* stream){
    this->writeXML(stream);
}

void ABCExecution::writeXML(QXmlStreamWriter *stream){
    stream->writeStartElement("SEGMENTATION-PARAMETERS");
    writeABCParameters(stream);
    stream->writeEndElement();
}

void ABCExecution::writeABCParameters(QXmlStreamWriter *stream){

    stream->writeTextElement(QString("SUFFIX"), m_parameters->getSuffix());

    stream->writeTextElement(QString("ATLAS-DIRECTORY"), m_parameters->getExistingAtlas());

    stream->writeTextElement(QString("ATLAS-ORIENTATION"), QString("file"));
    stream->writeTextElement(QString("ATLAS-FORMAT"), m_parameters->getAtlasFormat());
    stream->writeTextElement(QString("OUTPUT-DIRECTORY"), m_module_path);
    stream->writeTextElement(QString("OUTPUT-FORMAT"), m_parameters->getAtlasFormat());

    stream->writeStartElement("IMAGE");
    stream->writeTextElement(QString("FILE"), m_parameters->getT1());
    stream->writeTextElement(QString("ORIENTATION"), QString("file"));
    stream->writeEndElement();

    stream->writeStartElement("IMAGE");
    stream->writeTextElement(QString("FILE"), m_parameters->getT2());
    stream->writeTextElement(QString("ORIENTATION"), QString("file"));
    stream->writeEndElement();

    if(m_parameters->getUsingAD() || m_parameters->getUsingFA()){
        stream->writeStartElement("IMAGE");
        stream->writeTextElement(QString("FILE"), QString(m_parameters->getDWI()));
        stream->writeTextElement(QString("ORIENTATION"), QString("file"));
        stream->writeEndElement();
    }

    stream->writeTextElement(QString("FILTER-ITERATIONS"), QString::number(m_parameters->getNeosegParameters()->getNumberOfIterations()));
    stream->writeTextElement(QString("FILTER-TIME-STEP"), QString::number(m_parameters->getNeosegParameters()->getTimeStep()));
    stream->writeTextElement(QString("FILTER-METHOD"), m_parameters->getNeosegParameters()->getFilterMethod());

    stream->writeTextElement(QString("MAX-BIAS-DEGREE"), QString::number(m_parameters->getABCMaximumDegreeBiasField()));

    std::vector<double> coeffs = m_parameters->getABCPriorsCoefficients();
    for(unsigned i = 0; i < coeffs.size(); i++){
        stream->writeTextElement(QString("PRIOR"), QString::number(coeffs[i]));
    }

    stream->writeTextElement(QString("INITIAL-DISTRIBUTION-ESTIMATOR"), m_parameters->getABCInitialDistributorEstimatorType());

    stream->writeTextElement("DO-ATLAS-WARP", QString("0"));
    stream->writeTextElement("ATLAS-WARP-FLUID-ITERATIONS", QString("0"));
    stream->writeTextElement("ATLAS-WARP-FLUID-MAX-STEP", QString("0"));
    stream->writeTextElement("ATLAS-LINEAR-MAP-TYPE", QString("id"));
    stream->writeTextElement("IMAGE-LINEAR-MAP-TYPE", QString("id"));

}

void ABCExecution::implementReassignImageIslands(){

    PipelineParameters::ABCVectorReassignLabelsType abcreassign =  m_parameters->getABCReassignLabels();
    QString inputfile = m_parameters->getT1();
    
    QString basefilename = inputfile.mid(0, inputfile.lastIndexOf(m_parameters->getAtlasFormat()) - 1) + QString("_labels");
    inputfile = basefilename;
    inputfile += QString(".") + m_parameters->getAtlasFormat();
    
    QString outputfile = "";
    QStringList list;
    for(int i = 0; i < (int)abcreassign.size(); i++){

        PipelineParameters::ABCReassignLabels abcr = abcreassign[i];
        if(abcr.m_ReassignEnabled){

            QString XML_path = m_module_dir->filePath("parametersReassign" + QString::number(abcr.m_Label) + ".xml");
            list.append(XML_path);

            QFile* file = new::QFile(XML_path);
            file->open(QIODevice::WriteOnly);
            QXmlStreamWriter* stream = new::QXmlStreamWriter(file);
            stream->setAutoFormatting(true);

            stream->writeStartDocument();

            stream->writeDTD("<!DOCTYPE REASSIGN-WHITE-MATTER-PARAMETERS>");
            stream->writeStartElement("REASSIGN-WHITE-MATTER-PARAMETERS");

            stream->writeTextElement(QString("INPUT"), inputfile);

            stream->writeTextElement(QString("LABEL-VALUE"), QString::number(abcr.m_Label));

            stream->writeTextElement(QString("THRESHOLD"), QString::number(abcr.m_Threshold));

            stream->writeTextElement(QString("VOXEL-BY-VOXEL"), QString::number(abcr.m_VoxelByVoxel));

            stream->writeStartElement(QString("PROBABILITY-MAPS-DIR"));

            PipelineParameters::InputImageLabelMapType labelsmap = m_parameters->getImageLabelMap();
            int numlabels = labelsmap.size();
            if(labelsmap.find(0) != labelsmap.end()){
                numlabels--;
            }
            QString atlasdir = m_parameters->getExistingAtlas() + QString("/");
            QString atlasformat = QString(".") + m_parameters->getAtlasFormat();
            for(int ii = 0; ii < numlabels; ii++){
                stream->writeTextElement(QString("PROBABILITY-MAP"), atlasdir + QString::number(ii + 1) + atlasformat);
            }

            stream->writeEndElement();


            outputfile = basefilename;
            outputfile += "_reassign" + QString::number(i) + atlasformat;
            stream->writeTextElement(QString("OUTPUT"), outputfile);


            stream->writeEndElement();
            stream->writeEndDocument();

            file->close();

            inputfile = outputfile;
        }
    }
    QStringList::Iterator it;
    for(it = list.begin(); it != list.end(); ++it){
        m_script+= m_indent + "args = [ReassignWhiteMatter, '" + *it + "']\n";
        m_script+= m_indent + "execute(args)\n";
    }
}
