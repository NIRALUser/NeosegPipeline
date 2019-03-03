# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8.3)

# This project is designed to be built outside the Insight source tree.
project(NeosegPipeline-AllExecutables)

include(${CMAKE_CURRENT_SOURCE_DIR}/Common.cmake)

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

if( ${LOCAL_PROJECT_NAME}_BUILD_SLICER_EXTENSION )
  set(CPACK_INSTALL_CMAKE_PROJECTS "${CPACK_INSTALL_CMAKE_PROJECTS};${CMAKE_BINARY_DIR};${EXTENSION_NAME};ALL;/")
  include(${Slicer_EXTENSION_CPACK})
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

if(Teem_DIR)

  find_program(UNU_PATH 
    unu
    HINTS ${Teem_DIR}/bin ${Teem_DIR} ${Teem_DIR}/../bin
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