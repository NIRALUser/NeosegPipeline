#include "AntsJointFusionParameters.h"

AntsJointFusionParameters::AntsJointFusionParameters()
{

   m_output_dir_default="";
   m_output_dir=m_output_dir_default;

   m_compute_rois_parc_fusion_default= false ;
   m_compute_rois_parc_fusion=m_compute_rois_parc_fusion_default;

}

void AntsJointFusionParameters::setOutputDir(QString output_dir)
{
   m_output_dir=output_dir;
}

QString AntsJointFusionParameters::getOutputDir()
{
   return m_output_dir;
}

void AntsJointFusionParameters::setRoicParcFusion(bool rois_parc_fusion)
{
   m_compute_rois_parc_fusion=rois_parc_fusion;
}
   
bool AntsJointFusionParameters::getRoicParcFusion()
{
   return m_compute_rois_parc_fusion;
}

