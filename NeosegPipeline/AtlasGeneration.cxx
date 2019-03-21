#include "AtlasGeneration.h"

AtlasGeneration::AtlasGeneration(QString module) : Script(module)
{
   m_white.name="white";
   m_gray.name="gray";
   m_csf.name="csf";
   m_rest.name="rest";
   m_templateT1.name="templateT1";
   m_templateT2.name="templateT2";
   m_UseT1 = true;
   m_ABCPipelineModeOn = false;
   m_ABCWhiteLabelOutputIndexString = "";
}

void AtlasGeneration::setAtlasPopulation(std::vector<Atlas> atlasPopulation)
{
   m_atlasPopulation = atlasPopulation;
}
void AtlasGeneration::setSmoothing(QString smoothing)
{
   m_smoothing = smoothing;
}
void AtlasGeneration::setSmoothingSize(double smoothingSize)
{
   m_smoothingSize = smoothingSize;
}
void AtlasGeneration::setComputingWeights(bool computingWeights)
{
   m_computingWeights = computingWeights;
}
void AtlasGeneration::setNeightborhoodRadius(double neightborhoodRadius)
{
   m_neightborhoodRadius = neightborhoodRadius;
}
void AtlasGeneration::setNeightborhoodRadiusUnit(QString neightborhoodRadiusUnit)
{
   m_neightborhoodRadiusUnit = neightborhoodRadiusUnit;
}
void AtlasGeneration::setIncludingFA(bool includingFA)
{
   m_includingFA = includingFA;
}
void AtlasGeneration::setFAShift(double FAShift)
{
   m_FAShift = FAShift;
}
void AtlasGeneration::setFASigmaScale(double FASigmaScale)
{
   m_FASigmaScale = FASigmaScale;
}
void AtlasGeneration::setFAWeight(double FAWeight)
{
   m_FAWeight = FAWeight;
}
void AtlasGeneration::setFASmoothingSize(double FASmoothingSize)
{
   m_FASmoothingSize = FASmoothingSize; 
}

void AtlasGeneration::createDirectories()
{  
   
   m_module_dir->mkpath( "temp/templates/templateT1" );
   QString templateT1 = m_module_dir->filePath( "temp/templates/templateT1" );
   m_templateT1.dir = new QDir( templateT1 );  
 
   m_module_dir->mkpath( "temp/templates/templateT2" );
   QString templateT2 = m_module_dir->filePath( "temp/templates/templateT2" );
   m_templateT2.dir = new QDir( templateT2 );  

   m_module_dir->mkpath( "temp/priorProbabilities/white" );
   QString white_path = m_module_dir->filePath( "temp/priorProbabilities/white" );
   m_white.dir = new QDir( white_path );  
 
   m_module_dir->mkpath( "temp/priorProbabilities/gray" );
   QString gray_path = m_module_dir->filePath( "temp/priorProbabilities/gray" );
   m_gray.dir = new QDir( gray_path );  
 
   m_module_dir->mkpath( "temp/priorProbabilities/csf" );
   QString csf_path = m_module_dir->filePath( "temp/priorProbabilities/csf" );
   m_csf.dir = new QDir( csf_path );  
 
   m_module_dir->mkpath( "temp/priorProbabilities/rest" );
   QString rest_path = m_module_dir->filePath( "temp/priorProbabilities/rest" );
   m_rest.dir = new QDir( rest_path ); 

   m_module_dir -> mkpath("temp/priorProbabilities");
   QString priorProbabilities_path = m_module_dir->filePath("temp/priorProbabilities");
   m_priorProbabilities_dir = new QDir(priorProbabilities_path);  
}

void AtlasGeneration::initializeScript()
{
   definePython();
   importGeneralModules();
   importXmlModules();
  
   defineExecutable("ImageMath");
   defineExecutable("ResampleScalarVectorDWIVolume");
   defineExecutable("unu");
   defineExecutable("SpreadFA");
   defineExecutable("WeightedLabelsAverage");

   m_script += "logger = logging.getLogger('NeosegPipeline')\n\n";

   m_script += "runningProcess = None\n\n";  
} 

QString AtlasGeneration::getImage(Atlas atlas, QString name)
{
   if( name == "templateT1" )
   {
      return atlas.T1;
   }
  
   if( name == "templateT2" )
   {
      return atlas.T2;
   }

   return NULL;
}

void AtlasGeneration::generateTemplate(TemplateImage& templateImage)
{
   if( m_atlasPopulation.size() < 2 )
   {
       throw "GenerateTemplate: Needs at least 2 atlases in the population atlas" ;
   }
   QString ref = getImage(m_atlasPopulation[0], templateImage.name);
   m_inputs.insert("ref", ref);

   m_log = "- Averaging all the " + templateImage.name;
   m_argumentsList << "ImageMath" << "ref" << "'-avg'";

   int i=1;
   std::vector<Atlas>::iterator it;
   for (it=m_atlasPopulation.begin()+1; it!=m_atlasPopulation.end(); ++it)
   {
      if(!(getImage(*it, templateImage.name)).isEmpty())
      {
         QString atlas = "atlas_" + QString::number(i);
         m_inputs.insert(atlas, getImage(*it, templateImage.name));
         m_argumentsList << atlas; 
         i++;
      } 
   }

   QString average = templateImage.dir->filePath(templateImage.name + ".nrrd"); 
   templateImage.output = templateImage.name;
   m_outputs.insert(templateImage.name + "Average", average);

   m_argumentsList << "'-type'" << "'float'" << "'-outfile'" << templateImage.name + "Average";
   execute();

   QString rescaledAverage = m_module_dir->filePath(templateImage.name + ".nrrd");  

   m_log = "Rescaling template " + templateImage.name;
   m_outputs.insert(templateImage.output,rescaledAverage);
   m_argumentsList << "ImageMath" << templateImage.name + "Average" << "'-rescale'" << "'0, 255'" << "'-outfile'" << templateImage.name;
   execute();
}

void AtlasGeneration::generateWeightedAveragedLabels()
{
//   QStringList argumentsList;

   QString whiteAverage = "";
   QString grayAverage = "";
   QString csfAverage = "";

   if(!m_ABCPipelineModeOn){
       m_white.output = m_white.name + "_average";
       m_gray.output = m_gray.name + "_average";
       m_csf.output = m_csf.name + "_average";

       whiteAverage = m_white.dir->filePath(m_white.name + "-average" + m_suffix + ".nrrd");
       grayAverage = m_gray.dir->filePath(m_gray.name + "-average" + m_suffix + ".nrrd");
       csfAverage = m_csf.dir->filePath(m_csf.name + "-average" + m_suffix + ".nrrd");

       m_script += m_indent + "" + m_white.output + " = '" + whiteAverage + "'\n";
       m_script += m_indent + "" + m_gray.output + " = '" + grayAverage + "'\n";
       m_script += m_indent + "" + m_csf.output + " = '" + csfAverage + "'\n";
   }

   QString m_XML_path = m_priorProbabilities_dir->filePath("parameters.xml");
   m_script += m_indent + "parameters_path = \"" + m_XML_path + "\"\n";

   m_script += m_indent + "if checkFileExistence(parameters_path)==False:\n";
   m_script += m_indent + m_indent + "logger.info('- Writing the XML file...')\n";
   m_script += m_indent + m_indent + "parameters = Element('WEIGHTED-AVERAGED-LABELS-PARAMETERS')\n";

   if(m_UseT1){
       addSubElement("parameters", "Input", "INPUT", m_neo.T1);
   }else{
       addSubElement("parameters", "Input", "INPUT", m_neo.T2);
   }

   //addSubElement("parameters", "InputMask", "MASK", m_neo.mask);

   m_script += m_indent + m_indent + "atlasPopulation = SubElement(parameters, 'ATLAS-POPULATION')\n";

   std::vector<Atlas>::iterator it;
   Atlas atlas;

   for (it=m_atlasPopulation.begin(); it!=m_atlasPopulation.end(); ++it)
   {
      atlas = *it;

      if(atlas.probabilistic)
      {
         m_script += m_indent + m_indent + "atlas = SubElement(atlasPopulation, 'PROBABILISTIC-ATLAS')\n";
         if(m_UseT1){
             addSubElement("atlas", "image", "IMAGE", atlas.T1);
         }else{
             addSubElement("atlas", "image", "IMAGE", atlas.T2);
         }

         addSubElement("atlas", "seg", "WHITE", atlas.white);
         addSubElement("atlas", "seg", "GRAY", atlas.gray);
         addSubElement("atlas", "seg", "CSF", atlas.csf);
      }

      else
      {
         m_script += m_indent + m_indent + "atlas = SubElement(atlasPopulation, 'ATLAS')\n";
         if(m_UseT1){
             addSubElement("atlas", "image", "IMAGE", atlas.T1);
         }else{
             addSubElement("atlas", "image", "IMAGE", atlas.T2);
         }
         addSubElement("atlas", "seg", "SEG", atlas.seg);
      }
   }
   
   m_script += m_indent + m_indent + "weights = SubElement(parameters, 'WEIGHTS')\n";
   addSubElement("weights", "computingWeights", "COMPUTING-WEIGHTS", QString::number(m_computingWeights));
   addSubElement("weights", "neightborhoodRadius", "NEIGHTBORHOOD-RADIUS", QString::number(m_neightborhoodRadius));
   addSubElement("weights", "neightborhoodRadiusUnit", "NEIGHTBORHOOD-RADIUS-UNIT", m_neightborhoodRadiusUnit);

   if(m_ABCPipelineModeOn){
      addSubElement("parameters", "outputs", "OUTPUT", this->getOutput());
   }else{
      m_script += m_indent + m_indent + "outputs = SubElement(parameters, 'OUTPUTS')\n";
      addSubElement("outputs", "white", "WHITE-AVERAGE", whiteAverage);
      addSubElement("outputs", "gray", "GRAY-AVERAGE", grayAverage);
      addSubElement("outputs", "csf", "CSF-AVERAGE", csfAverage);
   }

   m_script += m_indent + m_indent + "XML = xml.dom.minidom.parseString(ElementTree.tostring(parameters))\n";
   m_script += m_indent + m_indent + "pretty_xml_as_string = XML.toprettyxml()\n";

   m_script += m_indent + m_indent + "parameters = open(parameters_path, 'w')\n";
   m_script += m_indent + m_indent + "parameters.write(pretty_xml_as_string)\n";
   m_script += m_indent + m_indent + "parameters.close()\n";
   
   m_script += m_indent + "else:\n";
   m_script += m_indent + m_indent + "logger.info('- Writing the XML file : Done ')\n";


   m_log = "- Computing weighted labels averages";
   if(!m_ABCPipelineModeOn){
      m_outputs.insert(m_white.output, whiteAverage);
      m_outputs.insert(m_gray.output, grayAverage);
      m_outputs.insert(m_csf.output, csfAverage);
   }
   m_argumentsList << "WeightedLabelsAverage" << "'--parameters'" << "parameters_path";
   execute(); 
   m_unnecessaryFiles << m_XML_path;
   if(!m_ABCPipelineModeOn){
      m_unnecessaryFiles << whiteAverage; 
      m_unnecessaryFiles << grayAverage; 
      m_unnecessaryFiles << csfAverage; 
   }
}

void AtlasGeneration::extractWMFromFA()
{
   m_white.dir->mkpath("FA");
   QString FA_path = m_white.dir->filePath("FA");
   QDir* FA_dir = new QDir(FA_path);  

   m_script += m_indent + "FA = '" + m_neo.FA + "'\n\n";

   // Rescaling FA
   QString rescaledFA = FA_dir->filePath(m_prefix + "FA" + "-rescaled" + m_suffix + ".nrrd");
   m_outputs.insert("rescaledFA", rescaledFA);
 
   m_argumentsList << "ImageMath" << "FA" << "'-rescale'"<< "'0,255'" << "'-outfile'" << "rescaledFA";
   m_log = "- Rescaling the FA between 0 and 255";
   execute();
   m_unnecessaryFiles << rescaledFA; 

   // Spreading FA
   QString spreadFA = FA_dir->filePath(m_prefix + "FA" + "-spread" + m_suffix + ".nrrd");
   m_outputs.insert("spreadFA", spreadFA);
 
   m_argumentsList << "SpreadFA" << "'--input'" << "rescaledFA" << "'--shift'" << "'" + QString::number(m_FAShift) + "'" << "'--sigma'" << "'" + QString::number(m_FASigmaScale) + "'" << "'--output'" << "spreadFA";
   m_log = "- Spreading the FA";
   execute();
   m_unnecessaryFiles << spreadFA; 

   // Smoothing FA
   QString smoothedFA = FA_dir->filePath(m_prefix + "FA" + "-smoothed" + m_suffix + ".nrrd");
   m_outputs.insert("smoothedFA", smoothedFA);
 
   m_argumentsList << "ImageMath" << "spreadFA" << "'-smooth'"<< "'-gauss'" << "'-size'" << "'" + QString::number(m_FASmoothingSize) + "'" << "'-type'" << "'float'" << "'-outfile'" << "smoothedFA";
   m_log = "- Smoothing the FA (type=gauss, size=" + QString::number(m_FASmoothingSize) + ")";
   execute();
   m_unnecessaryFiles << smoothedFA; 

   // Eroding brain mask
   m_script += m_indent + "mask = '" + m_neo.mask + "'\n";

   QString erodedMask = FA_dir->filePath(m_prefix + "mask" + "-eroded" + m_suffix + ".nrrd");
   m_outputs.insert("erodedMask", erodedMask);
   m_argumentsList << "ImageMath" << "mask" << "'-erode'"<< "'2,1'" << "'-outfile'" << "erodedMask";
   m_log = "- Eroding the brain mask (radius=2)";
   execute();
   m_unnecessaryFiles << erodedMask; 

   // Masking FA
   QString maskedFA = FA_dir->filePath(m_prefix + "FA" + "-masked" + m_suffix + ".nrrd");
   m_outputs.insert("maskedFA", maskedFA);
   m_argumentsList << "ImageMath" << "smoothedFA" << "'-mask'"<< "erodedMask" << "'-type'" << "'float'" << "'-outfile'" << "maskedFA";
   m_log = "- Masking the FA with the eroded brain mask"; 
   execute();
   m_unnecessaryFiles << maskedFA; 
  
   // Multiplying FA 
   QString weightedFA = FA_dir->filePath(m_prefix + "FA" + "-weighted" + m_suffix + ".nrrd");
   m_outputs.insert("weightedFA", weightedFA);
   m_argumentsList << "ImageMath" << "maskedFA" << "'-constOper'" << "'2," + QString::number(m_FAWeight) + "'" << "'-type'" << "'float'" << "'-outfile'" << "weightedFA";
   m_log = "- Multiplying the FA by " + QString::number(m_FAWeight);  
   execute(); 
   m_unnecessaryFiles << weightedFA; 

   // Adding FA
   m_white.input = m_white.output; 
   m_white.output = "whitePlusFA";

   QString whitePlusFA = m_white.dir->filePath(m_prefix + "white" + "-plusFA" + m_suffix + ".nrrd");
   m_outputs.insert(m_white.output, whitePlusFA);
   if(m_ABCPipelineModeOn){
       QString filename = "'" + this->getOutput() + "/temp/priorProbabilities/" + m_ABCWhiteLabelOutputIndexString + ".nrrd'";
       m_argumentsList << "ImageMath" << filename << "'-add'" << "weightedFA" << "'-type'" << "'float'" << "'-outfile'" << filename;
   }else{
       m_argumentsList << "ImageMath" << m_white.input << "'-add'" << "weightedFA" << "'-type'" << "'float'" << "'-outfile'" << m_white.output;
   }

   m_log = "- Adding the FA to the white average...";
   execute();
   m_unnecessaryFiles << whitePlusFA;
}

void AtlasGeneration::generatePriorProbability(PriorProbability& priorProbability)
{
   // Smoothing probability
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_probability";
   QString probability = priorProbability.dir->filePath(priorProbability.name + "-probability" + m_suffix + ".nrrd");

   QString smoothing = "'-" + m_smoothing + "'";
   QString smoothingSize = "'" + QString::number(m_smoothingSize) + "'";

   m_log = "- Smoothing the " + priorProbability.name + " average (type=" + m_smoothing + ", size=" + QString::number(m_smoothingSize);
   m_argumentsList << "ImageMath" << priorProbability.input << "'-smooth'" << smoothing << "'-size'" << smoothingSize << "'-type'" << "'float'" << "'-outfile'" << priorProbability.output;
   m_outputs.insert(priorProbability.output, probability); 
   execute();
   m_unnecessaryFiles << probability;

   // Masking probability
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_maskedProbability"; 
   QString maskedProbability = priorProbability.dir->filePath(priorProbability.name + "-maskedProbability" + m_suffix + ".nrrd");

   m_log = "- Masking the " + priorProbability.name + " probability with the brain mask";
   m_outputs.insert(priorProbability.output, maskedProbability);
   m_argumentsList << "ImageMath" << priorProbability.input << "'-mask'"<< "mask" << "'-type'" << "'float'" << "'-outfile'" << priorProbability.output;
   execute();
   m_unnecessaryFiles << maskedProbability;

   // Rescaling Probability
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_rescaledProbability";
   QString rescaledProbability = priorProbability.dir->filePath(priorProbability.name + "-rescaledProbability" + m_suffix + ".nrrd");

   m_log = "- Multiplying the probability by 255";
   m_outputs.insert(priorProbability.output, rescaledProbability);
   m_argumentsList << "ImageMath" << priorProbability.input << "'-constOper'"<< "'2,255'" << "'-outfile'" << priorProbability.output;
   execute(); 
   m_unnecessaryFiles << rescaledProbability;
}

void AtlasGeneration::computeRest()
{
   QString outbase = m_rest.dir->filePath("label"); 
   m_inputs.insert("outbase", outbase);

   m_white.input = m_white.output;
   m_white.output = "white_normalizedProbability";
   QString label1 = m_rest.dir->filePath("label1_normalized.nrrd");
   m_outputs.insert(m_white.output, label1); 

   m_gray.input = m_gray.output;
   m_gray.output = "gray_normalizedProbability";
   QString label2 = m_rest.dir->filePath("label2_normalized.nrrd");
   m_outputs.insert(m_gray.output, label2); 

   m_csf.input = m_csf.output;
   m_csf.output = "csf_normalizedProbability";
   QString label3 = m_rest.dir->filePath("label3_normalized.nrrd");
   m_outputs.insert(m_csf.output, label3); 

   m_rest.output = "rest_normalizedProbability";
   QString label4 = m_rest.dir->filePath("label4_normalized.nrrd");
   m_outputs.insert(m_rest.output, label4); 

   QString priorProbabilities = "' + " + m_white.input + " + ',' + " + m_gray.input + " + ',' + " + m_csf.input + " + '";
   m_inputs.insert("priorProbabilities", priorProbabilities);   

   m_argumentsList << "ImageMath" << m_templateT1.output << "'-normalizeEMS'"<< "'3'" << "'-EMSfile'" << "priorProbabilities" << "'-type'" << "'float'" << "'-extension'" << "'.nrrd'" << "'-outbase'" << "outbase";
   m_log = "- Computing the rest probability";
   execute();
   m_unnecessaryFiles << label1;
   m_unnecessaryFiles << label2;
   m_unnecessaryFiles << label3;
   m_unnecessaryFiles << label4;

   // Dilating brain mask 
   QString dilatedMask = m_priorProbabilities_dir->filePath(m_prefix + "mask" + "-dilated" + m_suffix + ".nrrd");

   m_inputs.insert("brainMask", m_neo.mask);
   m_outputs.insert("dilatedMask", dilatedMask);
   m_log = "- Dilating the brain mask (radius=5)";
   m_argumentsList << "ImageMath" << "brainMask" << "'-dilate'"<< "'5,1'" << "'-outfile'" << "dilatedMask";
   execute(); 
   m_unnecessaryFiles << dilatedMask;

   // Masking rest
   m_rest.input = m_rest.output; 
   m_rest.output = "rest_maskedProbability"; 

   QString restMasked = m_rest.dir->filePath("rest-masked" + m_suffix + ".nrrd");

   m_outputs.insert(m_rest.output, restMasked);
   m_argumentsList << "ImageMath" << m_rest.input << "'-mask'"<< "dilatedMask" << "'-outfile'" << m_rest.output;
   m_log = "- Masking the rest with the dilated mask";
   execute();
   m_unnecessaryFiles << restMasked;
}

void AtlasGeneration::copyFinalPriorProbability(PriorProbability& priorProbability)
{
   priorProbability.input = priorProbability.output;
   priorProbability.output = priorProbability.name + "_finalProbability"; 

   QString finalProbability = m_module_dir->filePath(priorProbability.name + ".nrrd");  
   m_outputs.insert(priorProbability.output, finalProbability); 

   m_argumentsList << "ResampleScalarVectorDWIVolume" << priorProbability.input << priorProbability.output;
   execute(); 
} 

void AtlasGeneration::implementRun()
{
   m_script += "def run():\n";

   m_script += m_indent + "signal.signal(signal.SIGINT, stop)\n";
   m_script += m_indent + "signal.signal(signal.SIGTERM, stop)\n\n";

   m_script += m_indent + "logger.info('=== Atlas Generation ===')\n";

   QString finalTemplateT1_path = m_module_dir->filePath(m_templateT1.name + ".nrrd");  
   QString finalTemplateT2_path = m_module_dir->filePath(m_templateT2.name + ".nrrd");  
   QString finalWhite_path = m_module_dir->filePath(m_white.name + ".nrrd");  
   QString finalGray_path = m_module_dir->filePath(m_gray.name + ".nrrd");  
   QString finalCsf_path = m_module_dir->filePath(m_csf.name + ".nrrd");  
   QString finalRest_path = m_module_dir->filePath(m_rest.name + ".nrrd");  

   m_outputs.insert("finalTemplateT1", finalTemplateT1_path); 
   m_outputs.insert("finalTemplateT2", finalTemplateT2_path); 
   m_outputs.insert("finalWhite", finalWhite_path); 
   m_outputs.insert("finalGray", finalGray_path); 
   m_outputs.insert("finalCsf", finalCsf_path); 
   m_outputs.insert("finalrest", finalRest_path); 
   
   checkFinalOutputs(); 

   m_script += m_indent + "logger.debug('')\n";

   m_script += m_indent + "mask = '" + m_neo.mask + "'\n";

   m_script += m_indent + "### TEMPLATES ###\n";

   m_script += m_indent + "# Compute Template T1 #\n";
   m_script += m_indent + "logger.info('Computing template T1')\n";
   generateTemplate(m_templateT1);

   m_script += m_indent + "# Compute Template T2 #\n";
   m_script += m_indent + "logger.info('Computing template T2')\n";
   generateTemplate(m_templateT2);

   m_script += m_indent + "### PRIOR PROBABILITIES ###\n";

   m_script += m_indent + "# Compute Tissue Weighted Averages #\n";
   m_script += m_indent + "logger.info('Computing weighted labels averages...')\n";
   generateWeightedAveragedLabels();

   if(m_includingFA)
   {
      m_script += m_indent + "# Extract WM From FA #\n";
      m_script += m_indent + "logger.info('Extracting and adding the FA to the white average :')\n";
      extractWMFromFA();
   }

   if(m_ABCPipelineModeOn){
       m_script += m_indent + "# Compute Priors Probability #\n";
       m_script += m_indent + "logger.info('Computing priors probability :')\n";
       generatePriorsProbability();

       QString arrayname = "allpriors";
       QString execfunct = arrayname + " = generatePriorsProbability('" + this->getOutput() + "/temp/priorProbabilities/', '.nrrd')\n";
       m_script += m_indent + execfunct;


       computeRestABC();//This implements the function to compute the rest map

       execfunct = "computeRestABC(" + arrayname + ")\n"; //This executes the function in the python script with the previous arrayname as parameter
       m_script += m_indent + execfunct;
   }else{

       m_script += m_indent + "# Compute White Probability #\n";
       m_script += m_indent + "logger.info('Computing white probability :')\n";
       generatePriorProbability(m_white);

       m_script += m_indent + "# Compute Gray Probability #\n";
       m_script += m_indent + "logger.info('Computing gray probability :')\n";
       generatePriorProbability(m_gray);

       m_script += m_indent + "# Compute CSF Probability #\n";
       m_script += m_indent + "logger.info('Computing csf probability :')\n";
       generatePriorProbability(m_csf);

       m_script += m_indent + "# Compute Rest Probability #\n";
       m_script += m_indent + "logger.info('Computing rest probability :')\n";
       computeRest();

       m_script += m_indent + "# Copy White Probability #\n";
       m_script += m_indent + "logger.info('Copying final white probability...')\n";
       copyFinalPriorProbability(m_white);

       m_script += m_indent + "# Copy Gray Probability #\n";
       m_script += m_indent + "logger.info('Copying final gray probability...')\n";
       copyFinalPriorProbability(m_gray);

       m_script += m_indent + "# Copy CSF Probability #\n";
       m_script += m_indent + "logger.info('Copying final csf probability...')\n";
       copyFinalPriorProbability(m_csf);

       m_script += m_indent + "# Copy Rest Probability #\n";
       m_script += m_indent + "logger.info('Copying final rest probability...')\n";
       copyFinalPriorProbability(m_rest);
   }

}

void AtlasGeneration::generatePriorsProbability(){

   QString localf = "";


   localf += m_indent + "def generatePriorsProbability(indir, ext):\n";
   localf += m_indent + m_indent + "outindex = 1\n";
   localf += m_indent + m_indent + "generatePriorsProbabilityReturn = list()\n";
   localf += m_indent + m_indent + "average = os.path.join(indir, str(outindex) + ext)\n";
   localf += m_indent + m_indent + "while os.path.isfile(average):\n";
   localf += m_indent + m_indent + m_indent + "probability = os.path.join(indir, str(outindex) + '.probability' + ext)\n";
   localf += m_indent + m_indent + m_indent + "maskedProbability = os.path.join(indir, str(outindex) + '.maskedProbability' + ext)\n";
   localf += m_indent + m_indent + m_indent + "rescaledProbability = os.path.join(indir, str(outindex) + '.rescaledProbability' + ext)\n";
   localf += m_indent + m_indent + m_indent + "args = [ImageMath, average,'-smooth','-" + m_smoothing  + "','-size','" + QString::number(m_smoothingSize) + "','-type','float','-outfile', probability]\n";
   localf += m_indent + m_indent + m_indent + "execute(args)\n";
   localf += m_indent + m_indent + m_indent + "args = [ImageMath, probability,'-mask',mask,'-type','float','-outfile', maskedProbability]\n";
   localf += m_indent + m_indent + m_indent + "execute(args)\n";
   localf += m_indent + m_indent + m_indent + "args = [ImageMath, maskedProbability,'-constOper','2,255','-outfile', rescaledProbability]\n";
   localf += m_indent + m_indent + m_indent + "execute(args)\n";
   localf += m_indent + m_indent + m_indent + "generatePriorsProbabilityReturn.append(rescaledProbability)\n";
   localf += m_indent + m_indent + m_indent + "outindex += 1\n";
   localf += m_indent + m_indent + m_indent + "average = os.path.join(indir, str(outindex) + ext)\n";
   localf += m_indent + m_indent + "return generatePriorsProbabilityReturn\n";

   m_script+= localf;
   
}

void AtlasGeneration::computeRestABC(){

   QString localf = "";

   localf += m_indent + "def computeRestABC(allpriors):\n";
   localf += m_indent + m_indent + "priorProbabilities = ''\n";
   localf += m_indent + m_indent + "for i in range(0, len(allpriors)):\n";
   localf += m_indent + m_indent + m_indent + "priorProbabilities += allpriors[i]\n";
   localf += m_indent + m_indent + m_indent + "if(i < len(allpriors) - 1):\n";
   localf += m_indent + m_indent + m_indent + m_indent + "priorProbabilities += ','\n";
   localf += m_indent + m_indent + "#The 'rest' label prior map is generated using all the other labels. \n";
   localf += m_indent + m_indent + "outbase = '" + this->getOutput() + "/temp/priorProbabilities/'\n";
   localf += m_indent + m_indent + "numEMS = str(len(allpriors))\n";
   localf += m_indent + m_indent + "args = [ImageMath,templateT1,'-normalizeEMS',numEMS,'-EMSfile',priorProbabilities,'-type','float','-extension','.nrrd','-outbase',outbase]\n";
   localf += m_indent + m_indent + "execute(args)\n";
   localf += m_indent + m_indent + "dilatedMask = '" + this->getOutput() + "/temp/priorProbabilities/neo-mask-dilated_NP.nrrd" + "'\n";
   localf += m_indent + m_indent + "args = [ImageMath,mask,'-dilate','5,1','-outfile',dilatedMask]\n";
   localf += m_indent + m_indent + "execute(args)\n";
   localf += m_indent + m_indent + "#Generate the masked probability\n";
   localf += m_indent + m_indent + "for i in range(1, len(allpriors) + 1):\n";
   localf += m_indent + m_indent + m_indent + "infile = outbase + str(i) + '_normalized.nrrd'\n";
   localf += m_indent + m_indent + m_indent + "outfile = '" + this->getOutput() + "/' + str(i) + '.nrrd'\n";
   localf += m_indent + m_indent + m_indent + "args = [ResampleScalarVectorDWIVolume, infile , outfile]\n";
   localf += m_indent + m_indent + m_indent + "execute(args)\n";
   localf += m_indent + m_indent + "restindex = str( len(allpriors) + 1 )\n";
   localf += m_indent + m_indent + "restfinal = '" + this->getOutput() + "/' + restindex + '.nrrd'\n";
   localf += m_indent + m_indent + "args = [ImageMath, outbase + restindex + '_normalized.nrrd','-mask', dilatedMask,'-outfile', restfinal]\n";
   localf += m_indent + m_indent + "execute(args)\n";

    m_script+= localf;
}

void AtlasGeneration::update()
{
   createDirectories();  
   initializeScript();
   implementStop(); 
   implementCheckFileExistence();
   implementExecute();
   implementExecutePipe();

   implementRun();
   writeScript();
}

QString AtlasGeneration::getOutput()
{
   return m_module_path;
}

void AtlasGeneration::cleanUp()
{
   for (int i = 0; i < m_unnecessaryFiles.size(); ++i)
   {
      QFile(m_unnecessaryFiles.at(i)).remove();
   }

   m_module_dir->rmpath("temp/templates/templateT1");
   m_module_dir->rmpath("temp/templates/templateT2");
   m_module_dir->rmpath("temp/priorProbabilities/white/FA");
   m_module_dir->rmpath("temp/priorProbabilities/gray");
   m_module_dir->rmpath("temp/priorProbabilities/csf");
   m_module_dir->rmpath("temp/priorProbabilities/rest");
}

