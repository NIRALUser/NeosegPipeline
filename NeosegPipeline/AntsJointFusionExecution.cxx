#include "AntsJointFusionExecution.h"

AntsJointFusionExecution::AntsJointFusionExecution(QString module) : Script(module)
{
}

void AntsJointFusionExecution::setAntsJointFusionParameters(AntsJointFusionParameters* parameters)
{
   m_parameters = parameters;
}

void AntsJointFusionExecution::initializeScript()
{
   definePython();
   importGeneralModules();
   importFusionModules();
   importXmlModules();
  
   defineExecutable("antsJointFusion");

   m_script += "logger = logging.getLogger('NeosegPipeline')\n\n";

   m_script += "runningProcess = None\n\n";   
}

void AntsJointFusionExecution::implementRun()
{
  m_script += "def run():\n\n";

  m_script += m_indent + "logger.info('=== Ants Joint Fusion ===')\n";

  m_script += m_indent + "signal.signal(signal.SIGINT, stop)\n";
  m_script += m_indent + "signal.signal(signal.SIGTERM, stop)\n\n";

  //Get output directory
  QString output_dir = m_parameters->getOutputDir();
  QString fusion_dir = output_dir + "/6.AntsJointFusionExecution/";
  m_script += m_indent + "ants_path='/tools/ANTS/ANTSv2.2.0/bin/antsJointFusion'\n";
  // I pray to the coding gods that the 'T1-stripped-corrected' hard coded strings in PreProcessingData will never change, amen. 
  m_script += m_indent + "target_path = '" + output_dir + "'+'/1.PreProcessingData/" + m_prefix + "T1-stripped-corrected" + m_suffix + ".nrrd'\n";

  m_script += m_indent + "output_dir = '" + output_dir + "'\n";
  m_script += m_indent + "fusion_dir = '" + fusion_dir + "'\n";
  m_script += m_indent + "RegisteredAtlases_dir = output_dir+'/3.AtlasRegistration/'\n";
  m_script += m_indent + "RegisteredAtlases_list = os.listdir(RegisteredAtlases_dir)\n\n";

  //tissue segmentation fusion
  m_script += m_indent + "Seg_fusion_output = fusion_dir + 'Seg_fusion_output.nrrd'\n\n";
  //test if SegOutput already exists
  m_script += m_indent + "if checkFileExistence(Seg_fusion_output)==False:\n";
  m_script += m_indent + m_indent + "segStringList=[]\n";
  m_script += m_indent + m_indent + "movingsegStringList=[]\n\n";
  //Look for segAtlases in the registered subjects and create list of registered segAtlases
  m_script += m_indent + m_indent + "for i in range (0,len(RegisteredAtlases_list)):\n";
  m_script += m_indent + m_indent + m_indent + "propagated_T1=find('*T1*',RegisteredAtlases_dir+RegisteredAtlases_list[i])\n";
  m_script += m_indent + m_indent + m_indent + "seg=find('*seg*',RegisteredAtlases_dir+RegisteredAtlases_list[i])\n";
  m_script += m_indent + m_indent + m_indent + "segStringList = segStringList + seg\n";  
  m_script += m_indent + m_indent + m_indent + "if seg:\n";
  m_script += m_indent + m_indent + m_indent + m_indent + "movingsegStringList=movingsegStringList+propagated_T1\n\n";

  //Compute de fusion if there are more than 2 registered segAtlases found
  m_script += m_indent + m_indent + "if len(segStringList)>=2:\n";
  m_script += m_indent + m_indent + m_indent + "movingsegList = ' '.join(movingsegStringList)\n";
  m_script += m_indent + m_indent + m_indent + "segString = ' '.join(segStringList)\n";
  m_script += m_indent + m_indent + m_indent + "subprocess.call(antsJointFusion+' -d 3 -t '+target_path+' -o '+Seg_fusion_output+' -g '+movingsegList+' -l '+segString,shell=True)\n\n";
  
  m_script += m_indent + "else:\n";
  m_script += m_indent + m_indent + "logger.info('Applying Fusion to the Seg -> Skipped')\n\n";

  if(m_parameters->getRoicParcFusion())
  {
  //brain parcellations fusion  
  m_script += m_indent + "Parc_fusion_output = fusion_dir + 'Parc_fusion_output.nrrd'\n";
  //test if ParcOutput already exists
  m_script += m_indent + "if checkFileExistence(Parc_fusion_output)==False:\n";
  m_script += m_indent + m_indent + "parcStringList=[]\n";
  m_script += m_indent + m_indent + "movingparcStringList=[]\n\n";
  //Look for parcAtlases in the registered subjects and create list of registered parcAtlases
  m_script += m_indent + m_indent + "for i in range (0,len(RegisteredAtlases_list)):\n";
  m_script += m_indent + m_indent + m_indent + "propagated_T1=find('*T1*',RegisteredAtlases_dir+RegisteredAtlases_list[i])\n";
  m_script += m_indent + m_indent + m_indent + "parc=find('*parc*',RegisteredAtlases_dir+RegisteredAtlases_list[i])\n";
  m_script += m_indent + m_indent + m_indent + "parcStringList = parcStringList + parc\n";  
  m_script += m_indent + m_indent + m_indent + "if parc:\n";
  m_script += m_indent + m_indent + m_indent + m_indent + "movingparcStringList=movingparcStringList+propagated_T1\n\n";

  //Compute de fusion if there are more than 2 registered parcAtlases found
  m_script += m_indent + m_indent + "if len(parcStringList)>=2:\n";
  m_script += m_indent + m_indent + m_indent + "Parc_fusion_output = fusion_dir + 'Parc_fusion_output.nrrd'\n";
  m_script += m_indent + m_indent + m_indent + "movingparcList = ' '.join(movingparcStringList)\n";
  m_script += m_indent + m_indent + m_indent + "parcString = ' '.join(parcStringList)\n";
  m_script += m_indent + m_indent + m_indent + "subprocess.call(antsJointFusion+' -d 3 -t '+target_path+' -o '+Parc_fusion_output+' -g '+movingparcList+' -l '+parcString,shell=True)\n\n";

  m_script += m_indent + "else:\n";
  m_script += m_indent + m_indent + "logger.info('Applying Fusion to the Parc -> Skipped')\n\n";

  //brain ROIs fusion
  m_script += m_indent + "roi0StringList=[]\n";
  m_script += m_indent + "roi1StringList=[]\n";
  m_script += m_indent + "roi2StringList=[]\n";
  m_script += m_indent + "roi3StringList=[]\n";
  m_script += m_indent + "roi4StringList=[]\n";
  m_script += m_indent + "roi5StringList=[]\n";
  m_script += m_indent + "roi6StringList=[]\n";
  m_script += m_indent + "roi7StringList=[]\n";
  m_script += m_indent + "roi8StringList=[]\n";
  m_script += m_indent + "roi9StringList=[]\n\n";

  m_script += m_indent + "roiStringList=[roi0StringList,roi1StringList,roi2StringList,roi3StringList,roi4StringList,roi5StringList,roi6StringList,roi7StringList,roi8StringList,roi9StringList]\n\n";

  m_script += m_indent + "movingroi0StringList=[]\n";
  m_script += m_indent + "movingroi1StringList=[]\n";
  m_script += m_indent + "movingroi2StringList=[]\n";
  m_script += m_indent + "movingroi3StringList=[]\n";
  m_script += m_indent + "movingroi4StringList=[]\n";
  m_script += m_indent + "movingroi5StringList=[]\n";
  m_script += m_indent + "movingroi6StringList=[]\n";
  m_script += m_indent + "movingroi7StringList=[]\n"; 
  m_script += m_indent + "movingroi8StringList=[]\n";
  m_script += m_indent + "movingroi9StringList=[]\n\n";

  m_script += m_indent + "movingroiStringList=[movingroi0StringList,movingroi1StringList,movingroi2StringList,movingroi3StringList,movingroi4StringList,movingroi5StringList,movingroi6StringList,movingroi7StringList,movingroi8StringList,movingroi9StringList]\n\n";

  m_script += m_indent + "findroi=['*roi0*','*roi1*','*roi2*','*roi3*','*roi4*','*roi5*','*roi6*','*roi7*','*roi8*','*roi9*']\n\n"; 
  //Look for roiXAtlases in the registered subjects and create list of registered roiXAtlases
  m_script += m_indent + "for i in range (0,len(RegisteredAtlases_list)):\n";  
  m_script += m_indent + m_indent + "propagated_T1=find('*T1*',RegisteredAtlases_dir+RegisteredAtlases_list[i])\n"; 
  m_script += m_indent + m_indent + "print(propagated_T1)\n"; 
  m_script += m_indent + m_indent + "for j in range (0,10):\n"; 
  m_script += m_indent + m_indent + m_indent + "roi=find(findroi[j],RegisteredAtlases_dir+RegisteredAtlases_list[i])\n"; 
  m_script += m_indent + m_indent + m_indent + "roiStringList[j] = roiStringList[j] + roi\n"; 
  m_script += m_indent + m_indent + m_indent + "if roi:\n"; 
  m_script += m_indent + m_indent + m_indent + m_indent + "movingroiStringList[j] = movingroiStringList[j] + propagated_T1\n\n";//Create list of moving images for the fusion
  //Compute de fusion if there are more than 2 registered roiXAtlases found
  m_script += m_indent + "for m in range (0, 10):\n";
  m_script += m_indent + m_indent + "ROI_fusion_output = fusion_dir + 'ROI'+str(m)+'_fusion_output.nrrd'\n"; 
  //test if RoiXOutput already exists, if not compute the fusion
  m_script += m_indent + m_indent + "if checkFileExistence(ROI_fusion_output)==False:\n";
  m_script += m_indent + m_indent + m_indent + "if len(roiStringList[m])>=2:\n";  
  m_script += m_indent + m_indent + m_indent + m_indent + "movingroiString = ' '.join(movingroiStringList[m])\n"; 
  m_script += m_indent + m_indent + m_indent + m_indent + "roiString = ' '.join(roiStringList[m])\n"; 
  m_script += m_indent + m_indent + m_indent + m_indent + "ROI_fusion_output = fusion_dir + 'ROI'+str(m)+'_fusion_output.nrrd'\n"; 
  m_script += m_indent + m_indent + m_indent + m_indent + "subprocess.call(antsJointFusion+' -d 3 -t '+target_path+' -o '+ROI_fusion_output+' -g '+movingroiString+' -l '+roiString,shell=True)\n"; 
  
  m_script += m_indent + m_indent + "else:\n";
  m_script += m_indent + m_indent + m_indent + "logger.info('Applying Fusion to the ROI'+str(m)+' -> Skipped')\n\n";

  m_script += m_indent + "logger.info('')\n";
  }
} 

void AntsJointFusionExecution::update()
{
   initializeScript();

   implementStop();
   implementCheckFileExistence();
   implementFind();   
   implementRun();

   writeScript();
}

