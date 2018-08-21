# NeoSegPipeline

## What is it?

This tool allows segmenting neonate brain MRI using a subject-specific atlas. It generates an subject-specific atlas based on an atlas population and some diffusion images of the subject to segment. Then we run a single atlas method with this atlas and obtain some good results, especially for in subcortical midbrain structure. We also included a tool to average deformation fields.

## License

See License.txt


## Build instructions

It builds the software tool and all the dependencies. If you want to build ANTs, check the option `BUILD_ANTs' in the cmake menu. 

---
  git clone https://github.com/NIRALUser/NeosegPipeline.git
  mkdir NeosegPipeline-build
  cd NeosegPipeline-build
  cmake ../NeosegPipeline
  make
---

## More information

Find the tool on [NITRC](http://www.nitrc.org/projects/neosegpipeline)

