#!/tools/bin_linux64 python 
#Python script that calls the docker container to perform the prediction

import sys
sys.path.append('../vectormomentum/Code/Python');
sys.path.append('../library')
import subprocess
import argparse
#import commands
import os
#parse command line input
parser = argparse.ArgumentParser(description='Deformation prediction given set of moving and target images.')

requiredNamed = parser.add_argument_group('required named arguments')

requiredNamed.add_argument('--moving-images', nargs='+',required=True, metavar=('m1', 'm2,m3...'),
                           help='moving images.')
requiredNamed.add_argument('--target-images', nargs='+',required=True, metavar=('t1', 't2,t3...'),
                           help='target images.')
requiredNamed.add_argument('--output-prefix', nargs='+',required=True, metavar=('o1', 'o2,o3...'),
                           help='registration output prefixe for moving/target. Preferred to be a directory (e.g. /some_path/output_dir/)')
requiredNamed.add_argument('--output-deffield',nargs='+',required=True, metavar=('d1', 'd2,d3...'),
                           help='output prefix for deformation field.')
requiredNamed.add_argument('--container-id', required=True, metavar=('i1'),
                           help='docker container id.')

args = parser.parse_args()


if __name__ == '__main__':
 for i in range(0, len(args.moving_images)):
  subprocess.Popen(['docker','cp',args.moving_images[i],args.container_id+':/scratch/quicksilver/data'])
  subprocess.Popen(['docker','cp',args.target_images[i],args.container_id+':/scratch/quicksilver/data'])

  data_moving=os.path.basename(os.path.normpath(args.moving_images[i]))
  data_target=os.path.basename(os.path.normpath(args.target_images[i]))
  data_output=os.path.basename(os.path.normpath(args.output_prefix[i]))
  #global_deformation_field=os.path.splitext(data_moving)[0]+"_to_"+os.path.splitext(data_target)[0]+"_global_deformation_field.nrrd"
  global_deformation_field=os.path.basename(os.path.normpath(args.output_deffield[i]))

  subprocess.call('nvidia-docker exec '+args.container_id+' /scratch/docker_prediction.sh --moving-images /scratch/quicksilver/data/'+data_moving+' --target-images /scratch/quicksilver/data/'+data_target+' --output-prefix /scratch/result/'+data_output+' --output-deffield '+args.output_deffield[i],shell=True)
 
  subprocess.call('nvidia-docker cp '+args.container_id+':/scratch/result/'+data_output+' '+args.output_prefix[i],shell=True) 
  subprocess.call('nvidia-docker cp '+args.container_id+':/scratch/result/'+global_deformation_field+' '+args.output_deffield[i],shell=True) 