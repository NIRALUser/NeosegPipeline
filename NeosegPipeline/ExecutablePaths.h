#ifndef DEF_ExecutablePaths
#define DEF_ExecutablePaths

#include <iostream>
#include <vector>
#include <sstream>
#include <ctype.h>

#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
// #include <QFileInfoListIterator>
#include <QProcessEnvironment>
#include <QMap>
#include <QtGlobal>
#include <itksys/SystemTools.hxx>

class ExecutablePaths
{
   public:

   ExecutablePaths();
   
   void setProgramPath(QString programPath);

   bool checkExecutablePath(QString name, QString path);

   void setExecutablePath(QString name, QString path); 
   QString getExecutablePath(QString name); 

   void setDefaultExecutablePath(QString name);
   QString getDefaultExecutablePath(QString name);

   QString findExecutablePath(QString executableName);
  
   QString checkExecutables();
   bool skipCheck( QString executableName ) ;

   private: 
   std::vector<int> ConvertStringVersionToVector( std::string version ) ;
   bool testVersion( std::string givenVersion , std::string comparedVersion ) ;
   void ltrim(std::string& s) ;
   QString m_currentDirectory;
   std::string m_commandDirectory; 

   QMap<QString, QString> m_executables; 

   QStringList m_splitPath;
   QStringList m_skipExecutables ;
   QStringList m_executables_withVersionLongFlag; 
   QStringList m_executables_withVersionShortFlag; 
   QStringList m_executables_withVersionArgument; 
   QStringList m_executables_withHelpVersionFlag;
   QStringList m_executables_withoutFlag;
   QMap<QString, std::string> m_executables_versions ;
};

#endif
