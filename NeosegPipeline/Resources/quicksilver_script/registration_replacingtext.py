#!/tools/bin_linux64 python
#Python script that perfoms the prediction 

import sys
sys.path.append('../vectormomentum/Code/Python');
sys.path.append('../library')
import subprocess
import argparse
import os




def execute(args):

  subprocess.Popen(['docker','cp',args.movingImage,'"@CONTAINER-ID@":/scratch/quicksilver/data'])
  subprocess.Popen(['docker','cp',args.targetImage,'"@CONTAINER-ID@":/scratch/quicksilver/data'])

  data_moving=os.path.basename(os.path.normpath(args.movingImage))
  data_target=os.path.basename(os.path.normpath(args.targetImage))
  data_output=os.path.basename(os.path.normpath(@OUTPUT-PREFIX@))
  global_deformation_field=os.path.basename(os.path.normpath(@OUTPUT-DEFFIELD@))

  subprocess.call('nvidia-docker exec @CONTAINER-ID@ /scratch/docker_prediction.sh --moving-images /scratch/quicksilver/data/'+data_moving+' --target-images /scratch/quicksilver/data/'+data_target+' --output-prefix /scratch/result/'+data_output+' --output-deffield '+@OUTPUT-DEFFIELD@,shell=True)
 
  subprocess.call('nvidia-docker cp @CONTAINER-ID@:/scratch/result/'+data_output+' '+@OUTPUT-PREFIX@,shell=True) 
  subprocess.call('nvidia-docker cp @CONTAINER-ID@:/scratch/result/'+global_deformation_field+' '+@OUTPUT-DEFFIELD@,shell=True) 

def main():
  parser = argparse.ArgumentParser(description='Deformation prediction given set of moving and target images.')

  requiredNamed = parser.add_argument_group('required named arguments')

  # requiredNamed.add_argument('--moving-images', nargs='+',required=True, metavar=('m1', 'm2,m3...'),
  #                            help='moving images.')
  requiredNamed.add_argument('--moving-images', nargs='+',default="@MOVING-IMAGES@", type="str", metavar=('m1', 'm2,m3...'),
                             help='moving images.')
  requiredNamed.add_argument('--target-images', nargs='+',default="@TARGET-IMAGES@", type="str", metavar=('t1', 't2,t3...'),
                             help='target images.')
  requiredNamed.add_argument('--output-prefix', nargs='+',default="@OUTPUT-PREFIX@", type="str", metavar=('o1', 'o2,o3...'),
                             help='registration output prefix for moving/target. Preferred to be a directory (e.g. /some_path/output_dir/)')
  requiredNamed.add_argument('--output-deffield',nargs='+',default="@OUTPUT-DEFFIELD@", type="str", metavar=('d1', 'd2,d3...'),
                             help='output prefix for deformation field.')
  requiredNamed.add_argument('--container-id', default="@CONTAINER-ID@", type="str", metavar=('i1'),
                             help='docker container id.')

  args = parser.parse_args()

  execute(args)


if __name__ == '__main__':
  main()