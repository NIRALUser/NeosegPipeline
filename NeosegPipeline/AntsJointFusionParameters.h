#ifndef DEF_AntsJointFusionParameters
#define DEF_AntsJointFusionParameters

// Genaral Librairies
#include <stdlib.h>
#include <iostream>

// Qt Librairies
#include <QString>
#include <QStringList>


class AntsJointFusionParameters
{
   public:

      // Constructor 
      AntsJointFusionParameters();

      //Output Directory
      void setOutputDir(QString output_dir);
      QString getOutputDir();

      void setRoicParcFusion(bool rois_parc_fusion);
      bool getRoicParcFusion();

   private:

      QString m_output_dir;
      QString m_output_dir_default;

      bool m_compute_rois_parc_fusion; 
      bool m_compute_rois_parc_fusion_default;
};

#endif

