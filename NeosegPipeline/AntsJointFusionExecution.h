#ifndef DEF_AntsJointFusionExecution
#define DEF_AntsJointFusionExecution

// General Librairies
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <vector>

// Qt Librairies
#include <QString>
#include <QStringList>
#include <QDebug>
#include <QDir>
#include "Script.h"
#include "Atlas.h"
#include "ExecutablePaths.h"
#include "AntsJointFusionParameters.h"

class AntsJointFusionExecution : public Script 
{
   public:

	   // Constructor
	   AntsJointFusionExecution(QString module);
	   void setAntsJointFusionParameters(AntsJointFusionParameters* parameters);


	   /*QFileInfoList find( QDir &dir, QString name , QString pre , QString post );
	   QMap<QString,QFileInfoList> findAtlasFiles(QString atlas_name);*/

	   // Implementing Script 
	   void initializeScript();
	   void update();
	   void implementRun();


   private:

   		// Atlas Population Directory
	   QString m_RegisteredAtlasesDirectory; 

	   // Input // 
	   AntsJointFusionParameters* m_parameters;
	   
};    


#endif

