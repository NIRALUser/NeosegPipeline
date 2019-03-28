# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8.3)

# This project is designed to be built outside the Insight source tree.
project(NeosegPipeline-AllExecutables)

include(${CMAKE_CURRENT_SOURCE_DIR}/Common.cmake)

option(CREATE_BUNDLE "Create MACOSX_BUNDLE" OFF)

if(CREATE_BUNDLE)

  if(APPLE)
    set(INSTALL_RUNTIME_DESTINATION ${bundle_name}.app/Contents/ExternalBin)    
    set(CPACK_BINARY_DRAGNDROP ON)
  endif()

  if(UNIX AND NOT APPLE AND CREATE_BUNDLE)
    # the RPATH to be used when installing
    SET(CMAKE_INSTALL_RPATH "$ORIGIN/../lib;../lib")
  endif()

endif()

FILE(READ NeosegPipeline/MultiSegPipeline.xml var)

STRING(REGEX MATCH "<version>.*</version>" ext "${var}")
STRING(REPLACE "<version>" "" version_number ${ext} )
STRING(REPLACE "</version>" "" version_number ${version_number})

ADD_DEFINITIONS(-DNEOSEGPIPELINE_VERSION="${version_number}")


string(REPLACE "." ";" VERSION_LIST ${version_number})
list(GET VERSION_LIST 0 CPACK_PACKAGE_VERSION_MAJOR)
list(GET VERSION_LIST 1 CPACK_PACKAGE_VERSION_MINOR)
list(GET VERSION_LIST 2 CPACK_PACKAGE_VERSION_PATCH)

if( BUILD_NeosegPipeline )
  add_subdirectory( NeosegPipeline )
endif()

if( BUILD_WeightedLabelsAverage )
  add_subdirectory( WeightedLabelsAverage )
endif()

if( BUILD_SpreadFA )
  add_subdirectory( SpreadFA )
endif()

if( BUILD_ReassignWhiteMatter )
  add_subdirectory( ReassignWhiteMatter )
endif()

if(neoseg_DIR)

  find_program(neoseg_PATH 
    neoseg
    HINTS ${neoseg_DIR}/bin ${neoseg_DIR}
    NO_SYSTEM_ENVIRONMENT_PATH)

  if(neoseg_PATH)
    if (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
      install(PROGRAMS ${neoseg_PATH} 
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}/../ExternalBin
        COMPONENT RUNTIME)
    else()
      install(PROGRAMS ${neoseg_PATH}
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)
    endif()
  endif()
endif()

if(ABC_DIR)

  find_program(ABC_PATH 
    ABC
    HINTS ${ABC_DIR}/bin ${ABC_DIR} ${ABC_DIR}/StandAloneCLI
    NO_SYSTEM_ENVIRONMENT_PATH)

  if(ABC_PATH)
    if (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
      install(PROGRAMS ${ABC_PATH}
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}/../ExternalBin
        COMPONENT RUNTIME)
    else()
      install(PROGRAMS ${ABC_PATH}
        DESTINATION ${INSTALL_RUNTIME_DESTINATION}
        COMPONENT RUNTIME)
    endif()
  endif()
endif()

if(teem_DIR)

  find_program(UNU_PATH 
    unu
    HINTS ${teem_DIR}/bin ${teem_DIR} ${teem_DIR}/../bin
    NO_SYSTEM_ENVIRONMENT_PATH)

  if (${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION)
    install(PROGRAMS ${UNU_PATH}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}/../ExternalBin
      COMPONENT RUNTIME)
  else()
    install(PROGRAMS ${UNU_PATH}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)
  endif()
endif()

if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_CPACK})
endif()


if(CREATE_BUNDLE)

  find_package(DTI-Reg)

  get_target_property(DTI-Reg_location DTI-Reg LOCATION_RELEASE)
  get_filename_component(DTI-Reg_BIN_DIR ${DTI-Reg_location} DIRECTORY)
  
  install(PROGRAMS ${DTI-Reg_BIN_DIR}/ANTS
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)

  install(PROGRAMS ${DTI-Reg_BIN_DIR}/antsJointFusion
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)

  install(PROGRAMS ${DTI-Reg_BIN_DIR}/ITKTransformTools
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)


  find_package(DTIProcess)

  # get_target_property(DTIProcess_LOCATION dtiprocess LOCATION_RELEASE)
  get_filename_component(DTIProcess_BIN_DIR ${DTIProcess_dtiprocess_EXECUTABLE} DIRECTORY)

  install(PROGRAMS ${DTIProcess_dtiprocess_EXECUTABLE}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)

  install(PROGRAMS ${DTIProcess_BIN_DIR}/dtiestim
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)


  find_package(niral_utilities)

  get_target_property(ImageMath_location ImageMath LOCATION_RELEASE)
  
  install(PROGRAMS ${ImageMath_location}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)


  set(Slicer_CLI "" CACHE FILEPATH "File path for built directory of Slicer_CLI")


  find_program(N4ITKBiasFieldCorrection_PATH 
    N4ITKBiasFieldCorrection
    HINTS ${SlicerCLI_DIR}/Modules/CLI/N4ITKBiasFieldCorrection/bin/
    NO_SYSTEM_ENVIRONMENT_PATH)  

  install(PROGRAMS ${N4ITKBiasFieldCorrection_PATH}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)  

  find_program(ResampleScalarVectorDWIVolume_PATH 
    ResampleScalarVectorDWIVolume
    HINTS ${SlicerCLI_DIR}/Modules/CLI/ResampleScalarVectorDWIVolume/bin/
    NO_SYSTEM_ENVIRONMENT_PATH)  

  install(PROGRAMS ${ResampleScalarVectorDWIVolume_PATH}
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)  


  set(SPHARM_PDM_DIR "" CACHE FILEPATH "File path for built directory of SPHARM_PDM")
  install(PROGRAMS ${SPHARM_PDM_DIR}/SPHARM-PDM-build/Modules/CLI/SegPostProcessCLP/bin/SegPostProcessCLP
      DESTINATION ${INSTALL_RUNTIME_DESTINATION}
      COMPONENT RUNTIME)

  # To Create a package, one can run "cpack -G DragNDrop CPackConfig.cmake" on Mac OS X
  # where CPackConfig.cmake is created by including CPack
  # And then there's ways to customize this as well  
  include(InstallRequiredSystemLibraries)
  include(CPack) 
endif()