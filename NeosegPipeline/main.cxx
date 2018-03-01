#include <QApplication>

#include "NeosegPipelineCLP.h"
#include "NeosegPipelineTool.h"


int main(int argc, char *argv[])
{
   PARSE_ARGS; 

   NeosegPipelineTool tool(QString::fromStdString(argv[0]));
   if( !data.empty()
       && ( !DWI.empty()
            || !mask.empty()
            || !T1.empty()
            || !T2.empty()
            || !output.empty()
            )
     )
   {
       std::cerr << "Do not specify a data file and input or output options at the same time" << std::endl ;
       return -1 ;
   }
   tool.setOutput(QString::fromStdString(output));
   tool.setT1(QString::fromStdString(T1));
   tool.setT2(QString::fromStdString(T2));
   tool.setMask(QString::fromStdString(mask));
   tool.setDWI(QString::fromStdString(DWI));
   tool.setPrefix(QString::fromStdString(prefix));
   tool.setSuffix(QString::fromStdString(suffix));
   tool.setForce(force);
   
   tool.setDataFile(QString::fromStdString(data));
   tool.setParametersFile(QString::fromStdString(parameters));
   tool.setExecutablesFile(QString::fromStdString(executables));

   return tool.launch(argc, argv, !noGUI);

}
