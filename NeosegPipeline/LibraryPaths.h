#ifndef DEF_LibraryPaths
#define DEF_LibraryPaths

#include <iostream>
#include <vector>

#include <QString>
#include <QDir>
#include <QFile>
#include <QFileInfo>
// #include <QFileInfoListIterator>
#include <QProcessEnvironment>
#include <QMap>


class LibraryPaths
{
   public:

   LibraryPaths();

   bool checkLibraryPath(QString name, QString path);
   void setLibraryPath(QString name, QString path); 
   QString getLibraryPath(QString name); 
  
   QString checkLibraries();

   private: 

   QString m_currentDirectory; 

   QMap<QString, QString> m_libraries; 

   QStringList m_splitPath;
};

#endif
