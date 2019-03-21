# Make sure this file is included only once
get_filename_component(CMAKE_CURRENT_LIST_FILENAME ${CMAKE_CURRENT_LIST_FILE} NAME_WE)
if(${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED)
  return()
endif()
set(${CMAKE_CURRENT_LIST_FILENAME}_FILE_INCLUDED 1)

# Sanity checks
if(DEFINED ABC_SOURCE_DIR AND NOT EXISTS ${ABC_SOURCE_DIR})
  message(FATAL_ERROR "ABC_SOURCE_DIR variable is defined but corresponds to non-existing directory")
endif()

# Include dependent projects if any
set(proj ABC)

# Set CMake OSX variable to pass down the external project
set(CMAKE_OSX_EXTERNAL_PROJECT_ARGS)
if(APPLE)
  list(APPEND CMAKE_OSX_EXTERNAL_PROJECT_ARGS
    -DCMAKE_OSX_ARCHITECTURES=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_SYSROOT=${CMAKE_OSX_SYSROOT}
    -DCMAKE_OSX_DEPLOYMENT_TARGET=${CMAKE_OSX_DEPLOYMENT_TARGET})
endif()

if(NOT DEFINED ABC_SOURCE_DIR)
  #message(STATUS "${__indent}Adding project ${proj}")

  if(NOT DEFINED git_protocol)
    set(git_protocol "git")
  endif()

  ExternalProject_Add(${proj}
    GIT_REPOSITORY "${git_protocol}://github.com/NIRALUser/ABC.git"
    GIT_TAG master
    SOURCE_DIR ${proj}
    BINARY_DIR ${proj}-build
    CMAKE_GENERATOR ${gen}
    "${cmakeversion_external_update}"
    CMAKE_ARGS
      -DUSE_SYSTEM_ITK:BOOL=OFF
      # -DITK_DIR:PATH=/tools/ITK/ITKv4.8.2/ITKv4.8.2_THL64_stat_Release/lib/cmake/ITK-4.8/
      # -DABC_SUPERBUILD:BOOL=OFF
      -DABC_SUPERBUILD:BOOL=ON
      -DCMAKE_C_FLAGS:STRING=-fPIC
      -DCMAKE_CXX_FLAGS:STRING=-fPIC
      ${CMAKE_OSX_EXTERNAL_PROJECT_ARGS}
      #${COMMON_EXTERNAL_PROJECT_ARGS}
      -DCOMPILE_COMMANDLINE:BOOL=ON
      -DCOMPILE_SLICER4COMMANDLINE:BOOL=OFF
      # -DCMAKE_INSTALL_PREFIX:PATH=${EXTERNAL_BINARY_DIRECTORY}/${proj}-install
      -DINSTALL_RUNTIME_DESTINATION=bin
      -DABC_CLI_NO_SUFFIX:BOOL=ON
      -DABC-inner_INSTALL_PATH:PATH=${EXTERNAL_BINARY_DIRECTORY}/${proj}-install
      ${${proj}_CMAKE_OPTIONS}
    INSTALL_COMMAND ""
    )
  
  set(${proj}_DIR ${EXTERNAL_BINARY_DIRECTORY}/${proj}-install)
endif()