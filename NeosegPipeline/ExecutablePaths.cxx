#include "ExecutablePaths.h"

ExecutablePaths::ExecutablePaths()
{
   // Path
   QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
   QString path = env.value("PATH", QString::null);
   #ifdef Q_OS_WIN
   const char separator = ';';
   #else
   const char separator = ':';
   #endif
   m_splitPath = path.split(separator);
   // Executables with version
   m_executables_withVersionLongFlag << "SegPostProcessCLP" << "python" << "N4ITKBiasFieldCorrection" << "dtiestim" << "dtiprocess" << "ResampleScalarVectorDWIVolume" << "WeightedLabelsAverage" << "ReassignWhiteMatter" << "neoseg" << "SpreadFA" << "ABC";
   m_executables_withVersionShortFlag << "ImageMath";
   m_executables_withVersionArgument << "ITKTransformTools";
   m_executables_withHelpVersionFlag << "bet2" << "ANTS" << "SNAP" << "unu" << "antsJointFusion";
   m_executables_versions.insert("SegPostProcessCLP","1.0");
   m_executables_versions.insert("N4ITKBiasFieldCorrection","9");
   m_executables_versions.insert("dtiestim","1.1.2");
   m_executables_versions.insert("dtiprocess","1.0.1");
   m_executables_versions.insert("ResampleScalarVectorDWIVolume","0.1");
   m_executables_versions.insert("WeightedLabelsAverage","1.2");
   m_executables_versions.insert("ReassignWhiteMatter","1.2");
   m_executables_versions.insert("SpreadFA","1.1");
   m_executables_versions.insert("neoseg","2.0.0");
   m_executables_versions.insert("ImageMath","1.1");
   m_executables_versions.insert("ITKTransformTools","1.1.0");
   m_executables_versions.insert("python","2.7.3");
   m_executables_versions.insert("ABC","1.5.1");
   //m_executables_versions.insert("antsJointFusion","2.2.0");
   m_skipExecutables.append("InsightSNAP");
   //m_skipExecutables.append("antsJointFusion");
}

void ExecutablePaths::setProgramPath(QString programPath)
{
   m_currentDirectory = ((QFileInfo(programPath)).dir()).absolutePath();
   m_splitPath.prepend(m_currentDirectory); 
   m_commandDirectory = itksys::SystemTools::GetFilenamePath(itksys::SystemTools::GetRealPath( programPath.toStdString().c_str() ));
}

QString ExecutablePaths::findExecutablePath(QString executableName)
{
   // QStringList::iterator it;
   // for(it = m_splitPath.begin(); it != m_splitPath.end(); ++it)
   // {
   //    QString executablePath = QDir(*it).filePath( executableName ) ;
   //    if( checkExecutablePath( executableName , executablePath ) )
   //    {
   //       return executablePath ;
   //    }
   // }   
   // return QString::null ;
  std::vector<std::string> hints;
  // std::cout<<"Command directory: "<<m_commandDirectory<<std::endl;
  hints.push_back(m_commandDirectory);

  #ifdef Slicer_CLIMODULES_BIN_DIR

  hints.push_back(m_commandDirectory + "/../../../../ResampleDTIlogEuclidean/" + std::string(Slicer_CLIMODULES_BIN_DIR));
  hints.push_back(m_commandDirectory + "/../../../../DTI-Reg/" + std::string(Slicer_CLIMODULES_BIN_DIR));
  hints.push_back(m_commandDirectory + "/../../../../DTI-Reg/" + std::string(Slicer_CLIMODULES_BIN_DIR) + "/../ExternalBin");
  hints.push_back(m_commandDirectory + "/../../../../DTIProcess/" + std::string(Slicer_CLIMODULES_BIN_DIR));
  hints.push_back(m_commandDirectory + "/../../../../DTIProcess/" + std::string(Slicer_CLIMODULES_BIN_DIR) + "/../ExternalBin");
  hints.push_back(m_commandDirectory + "/../../../../SPHARM-PDM/" + std::string(Slicer_CLIMODULES_BIN_DIR));
  hints.push_back(m_commandDirectory + "/../ExternalBin/");
  hints.push_back(m_commandDirectory + "/../../../../../" + std::string(Slicer_CLIMODULES_BIN_DIR));

  // This path is used when is built as Slicer extension and in debug/build directory
  hints.push_back(m_commandDirectory + "/../../../ResampleDTIlogEuclidean-build/" + std::string(Slicer_CLIMODULES_BIN_DIR));
  hints.push_back(m_commandDirectory + "/../../../DTIProcess-build/niral_utilities-install/bin");
  hints.push_back(m_commandDirectory + "/../../../DTIProcess-build/DTIProcess-inner-build/bin");
  hints.push_back(m_commandDirectory + "/../../../DTI-Reg-build/ANTs-install/bin");
  hints.push_back(m_commandDirectory + "/../../../DTI-Reg-build/DTI-Reg-inner-build/bin");
  hints.push_back(m_commandDirectory + "/../../../DTI-Reg-build/ITKTransformTools-build");
  hints.push_back(m_commandDirectory + "/../../../DTIProcess-build/niral_utilities-install/Slicer.app/Contents/" + std::string(Slicer_CLIMODULES_BIN_DIR));

  //For Slicer executable
  hints.push_back(m_commandDirectory + "/../../../../../");//LINUX
  hints.push_back(m_commandDirectory + "/../../../../../MacOS");//MAC

  #endif

  // This paths are used when is built as standalone and testing
  hints.push_back(m_commandDirectory + "/../../niral_utilities-install/bin");
  hints.push_back(m_commandDirectory + "/../../ResampleDTIlogEuclidean-install/bin");
  hints.push_back(m_commandDirectory + "/../../DTI-Reg-install/bin/");
  hints.push_back(m_commandDirectory + "/../../ITKTransformTools-install/bin/");
  hints.push_back(m_commandDirectory + "/../../DTIProcess-install/bin/");
  hints.push_back(m_commandDirectory + "/../../Teem-install/bin");
  hints.push_back(m_commandDirectory + "/../../");

  return QString::fromStdString( itksys::SystemTools::FindProgram( executableName.toStdString(), hints, true ) );
}

//Converts software number from string to vector<int>
//Splits string at '.'
//eg: "1.1.3" to vector<int>={1,1,3}
std::vector<int> ExecutablePaths::ConvertStringVersionToVector( std::string version )
{
  std::vector<int> vecVersion ;
  std::size_t posOrig = 0 ;
  std::size_t posDot ;
  do
  {
    posDot = version.find( "." , posOrig ) ;
    if( posDot == std::string::npos )
    {
      posDot = version.size() ;
    }
    std::istringstream extractString( version.substr( posOrig , posDot - posOrig ) ) ;
    int convert ;
    extractString >> convert ;
    if( !extractString )
    {
      convert = -1 ;
    }
    vecVersion.push_back( convert ) ;
    posOrig = posDot + 1 ;
  } while( posDot != version.size() ) ;
  return vecVersion ;
}

//Compares 2 softare version number and returns 'true' if
//givenVersion is larger than comparedVersion
//eg: givenVersion = 1.3
//    comparedVersion = 1.0.1
//    returns true
//returns true if versions are equal
//eg: givenVersion = 2.0
//    comparedVersion = 2.0
//    returns true
//returns false if comparedVersion is larger than givenVersion
//eg: givenVersion = 3.2
//    comparedVersion = 4.5.2
//    returns false
bool ExecutablePaths::testVersion( std::string givenVersion , std::string comparedVersion )
{
  std::vector<int> givenVersionVec = ConvertStringVersionToVector( givenVersion ) ;
  std::vector<int> comparedVersionVec = ConvertStringVersionToVector( comparedVersion ) ;
  std::size_t max = std::max( givenVersionVec.size() , comparedVersionVec.size() ) ;
  givenVersionVec.resize( max , 0 ) ;
  comparedVersionVec.resize( max , 0 ) ;
  for( std::size_t i = 0 ; i < max ; i++ )
  {
    if( givenVersionVec[ i ] > comparedVersionVec[ i ] )
    {
      return true ;
    }
    else if( givenVersionVec[ i ] < comparedVersionVec[ i ] )
    {
      return false ;
    }
  }
  return true ; //If we are here, the 2 versions are equal
}



void ExecutablePaths::ltrim(std::string& s)
{
    size_t p = s.find_first_not_of(" \t\n\v\f\r");
    s.erase(0, p);
}

bool ExecutablePaths::checkExecutablePath(QString executable_name, QString executable_path)
{
   if(executable_path.isEmpty() || !QFileInfo(executable_path).isExecutable())
   {
      return false; 
   }
   
   QString command;
   if(m_executables_withVersionLongFlag.contains(executable_name))
   {
      command =  executable_path + " --version "; 
   }
   else if(m_executables_withVersionShortFlag.contains(executable_name))
   {
      command =  executable_path + " -version "; 
   }
   else if(m_executables_withVersionArgument.contains(executable_name))
   {
      command =  executable_path + " version "; 
   }
   else if(m_executables_withHelpVersionFlag.contains(executable_name))
   {
      command =  executable_path + " --help";
   }
   else if( m_executables_withoutFlag.contains(executable_name))
   {
      command =  executable_path ;
   }
   else//Executable cannot be tested because it is not defined how to test it. It belongs to no QStringList that are used above to check that it runs
   {
      return false ;
   }
   std::string expected_version = m_executables_versions.value( executable_name , "-" ) ;
   QProcess test_process;   
   test_process.start(command);
   test_process.waitForFinished() ;
   if( expected_version != "-" )
   {
       test_process.waitForReadyRead();
       QByteArray BA =  test_process.readAllStandardOutput();
       QString text = QString( BA.data() );
       if( BA.size() == 0 ) //If no version number, we call it version 0
       {
           BA =  test_process.readAllStandardError() ;
           text = QString( BA.data() ) ;
           if( BA.size() == 0 ) //If no version number, we call it version 0
           {
             text = "0" ;
           }
       }
       std::string currentVersion = text.toLower().toStdString() ;
       std::string constStringVersion("version:") ;
       std::size_t pos = currentVersion.find( constStringVersion ) ;//it should keep just the version number
       bool errorReadingVersion = false ;
       if( pos != std::string::npos )
       {
           pos += constStringVersion.size() ;
       }
       else
       {
          pos = currentVersion.find( executable_name.toLower().toStdString() ) ;//it should keep just the version number
          if( pos != std::string::npos )
          {
              pos += executable_name.size() ;
          }
          else
          {
              errorReadingVersion = true ;
          }
       }
       if( currentVersion.size() < pos )
       {
           errorReadingVersion = true ;
       }
       else
       {
           currentVersion = currentVersion.substr( pos , currentVersion.size() - pos ) ;
           currentVersion.erase( std::remove( currentVersion.begin() , currentVersion.end() , '\n' ) , currentVersion.end() ) ;//remove newlines
           ltrim( currentVersion ) ;
       }
       if( errorReadingVersion || !testVersion( currentVersion , expected_version ) )
       {
           return false ;
       }
   }
   if( !test_process.exitStatus() )
   {
     return true ;
   }
   else
   {
     return false ;
   }
}

void ExecutablePaths::setDefaultExecutablePath(QString name)
{
   m_executables[name]=findExecutablePath(name);
}
QString ExecutablePaths::getDefaultExecutablePath(QString name)
{
   return findExecutablePath(name);
} 

void ExecutablePaths::setExecutablePath(QString name, QString path)
{
   m_executables[name]=path;
}
QString ExecutablePaths::getExecutablePath(QString name)
{
   return m_executables[name];
}

QString ExecutablePaths::checkExecutables()
{
   QString errors;
   QMap<QString, QString>::iterator it; 
   for(it = m_executables.begin(); it != m_executables.end(); ++it)
   {
      QString name = it.key();
      QString path = it.value(); 
      if( skipCheck( name ) )//We do not check for Insight SNAP as it is not necessary to process the images
      {
          continue ;
      }
      if( path.isEmpty() )
      {
         errors += name + " path is empty\n";
      }
      else if(!checkExecutablePath(name, path))
      {
         errors += name + " path does not seem to be the good one\n";
      }
   }

   return errors;
}

bool ExecutablePaths::skipCheck( QString executableName )
{
    return m_skipExecutables.contains( executableName , Qt::CaseInsensitive ) ;
}
