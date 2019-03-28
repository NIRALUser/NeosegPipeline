# This is the root ITK CMakeList file: 
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# This project is designed to be built outside the Insight source tree.
PROJECT(NeosegPipeline)


# find Qt5 headers
if(UNIX AND NOT APPLE)
  find_package(Qt5 COMPONENTS Widgets Core Xml Gui X11Extras REQUIRED)
else()
  find_package(Qt5 COMPONENTS Widgets Core Xml Gui REQUIRED)
endif()

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Xml_INCLUDE_DIRS})
add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(QT_LIBRARIES ${Qt5Widgets_LIBRARIES})

#Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})
INCLUDE(${GenerateCLP_USE_FILE})

#Find ITK
FIND_PACKAGE(ITK REQUIRED)
INCLUDE(${ITK_USE_FILE})

QT5_ADD_RESOURCES(RESOURCES resources.qrc)
#QT4_ADD_RESOURCES(quicksilver_generated_rcc ${quicksilver_rcc})
QT5_WRAP_UI(UI_SOURCES WindowMultiseg.ui antsParameters.ui quicksilverParameters.ui neosegParameters.ui ABCParameters.ui antsJointFusionParameters.ui About.ui)
QT5_WRAP_CPP(MOC_SOURCES DerivedWindow.h MainScriptThread.h)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR})

# NeosegPipeline #
SET(MultiSegPipeline_SOURCE  NeosegPipelineTool.cxx 
                           DerivedWindow.cxx
                           XmlReader.cxx 
                           XmlWriter.cxx
                           Pipeline.cxx
                           PipelineParameters.cxx
                           RegistrationParameters.cxx
                           NeosegParameters.cxx
                           ExecutablePaths.cxx
                           LibraryPaths.cxx
                           Script.cxx
                           PreProcessingData.cxx
                           AtlasPopulationRegistration.cxx
                           AtlasRegistration.cxx
                           AtlasGeneration.cxx
                           ExistingAtlasRegistration.cxx
                           NeosegExecution.cxx
                           DtiRegistration.cxx
                           MainScriptThread.cxx
                           ABCExecution.cxx
                           AntsJointFusionExecution.cxx
                           AntsJointFusionParameters.cxx
                           CountNumberOfLabels.cxx
)

SET(MultiSegPipeline_HEADERS NeosegPipelineTool.h
                           DerivedWindow.h
                           About.h
                           XmlReader.h
                           XmlWriter.h
                           Pipeline.h
                           PipelineParameters.h
                           Atlas.h
                           Neo.h
                           RegistrationParameters.h
                           NeosegParameters.h
                           ExecutablePaths.h
                           LibraryPaths.h
                           Script.h
                           PreProcessingData.h
                           AtlasPopulationRegistration.h
                           AtlasRegistration.h
                           AtlasGeneration.h
                           ExistingAtlasRegistration.h
                           NeosegExecution.h
                           DtiRegistration.h
                           PriorProbability.h
                           TemplateImage.h
                           MainScriptThread.h
                           Origin.h
                           MinMax.h
                           ABCExecution.h
                           AntsJointFusionExecution.h
                           AntsJointFusionParameters.h
                           CountNumberOfLabels.h
)


SEMMacroBuildCLI(
   NAME MultiSegPipeline
   EXECUTABLE_ONLY
   INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR} ${ITK_INCLUDE_DIR}
   LINK_DIRECTORIES ${QT_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS}
   TARGET_LIBRARIES ${QT_LIBRARIES} ${ITK_LIBRARIES}
   ADDITIONAL_SRCS ${MultiSegPipeline_SOURCE} ${MultiSegPipeline_HEADERS} ${MOC_SOURCES} ${UI_SOURCES} ${NeosegPipeline_CLP} ${RESOURCES}
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
   LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   INSTALL_RUNTIME_DESTINATION ${INSTALL_RUNTIME_DESTINATION}
   INSTALL_LIBRARY_DESTINATION ${INSTALL_LIBRARY_DESTINATION}
   INSTALL_ARCHIVE_DESTINATION ${INSTALL_ARCHIVE_DESTINATION}
)



# Generate a bundle
if(CREATE_BUNDLE)

  set(bundle_name MultiSegPipeline${version_number})

  if(APPLE)
    set(OS_BUNDLE MACOSX_BUNDLE)
  elseif(WIN32)
    set(OS_BUNDLE WIN32)
  endif()

  #--------------------------------------------------------------------------------
  SET(qtconf_dest_dir bin)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${bundle_name}")

  
  set(myApp_ICON ${CMAKE_SOURCE_DIR}/Resources/Icons/MultiSegPipeline_Icon.png)
  set(CPACK_PACKAGE_ICON "${myApp_ICON}")

  IF(WIN32)
    SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/${bundle_name}.exe")
  ENDIF(WIN32)

  IF(APPLE)
    SET(qtconf_dest_dir ${bundle_name}.app/Contents/Resources)
    SET(APPS "\${CMAKE_INSTALL_PREFIX}/${bundle_name}.app")

    set(MACOSX_BUNDLE_BUNDLE_NAME "${bundle_name}")
    set(MACOSX_BUNDLE_INFO_STRING "MultiSegPipeline: Atlas based neonate brain MRI segmentation")
    set(MACOSX_BUNDLE_GUI_IDENTIFIER "niral.unc.edu.${bundle_name}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "MultiSegPipeline version - ${version_number}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${version_number}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${version_number}")
    set(MACOSX_BUNDLE_COPYRIGHT "Copyright 2019 University of North Carolina , Chapel Hill.")
    
    set_source_files_properties(
      ${myApp_ICON}
      PROPERTIES
      MACOSX_PACKAGE_LOCATION Resources
      )

    get_filename_component(apple_icon_filename ${myApp_ICON} NAME)
    set(MACOSX_BUNDLE_ICON_FILE ${apple_icon_filename})

    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    add_executable(${bundle_name} ${OS_BUNDLE}
      ${myApp_ICON} ${MultiSegPipeline_SOURCE} MultiSegPipeline.cxx
    )
    target_link_libraries(${bundle_name} ${QT_LIBRARIES} ${ITK_LIBRARIES})

    #--------------------------------------------------------------------------------
    # Install the QtTest application, on Apple, the bundle is at the root of the
    # install tree, and on other platforms it'll go into the bin directory.
    INSTALL(TARGETS ${bundle_name}
      DESTINATION . COMPONENT Runtime
      RUNTIME DESTINATION bin COMPONENT Runtime
    )

    macro(install_qt5_plugin _qt_plugin_name _qt_plugins_var)
      get_target_property(_qt_plugin_path "${_qt_plugin_name}" LOCATION)
      if(EXISTS "${_qt_plugin_path}")
        get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
        get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
        get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)
        set(_qt_plugin_dest "${bundle_name}.app/Contents/PlugIns/${_qt_plugin_type}")
        install(FILES "${_qt_plugin_path}"
          DESTINATION "${_qt_plugin_dest}"
          COMPONENT Runtime)
        set(${_qt_plugins_var}
          "${${_qt_plugins_var}};\${CMAKE_INSTALL_PREFIX}/${_qt_plugin_dest}/${_qt_plugin_file}")
      else()
        message(FATAL_ERROR "QT plugin ${_qt_plugin_name} not found")
      endif()
    endmacro()
    
    install_qt5_plugin("Qt5::QCocoaIntegrationPlugin" QT_PLUGINS)

    #--------------------------------------------------------------------------------
    # install a qt.conf file
    # this inserts some cmake code into the install script to write the file
    INSTALL(CODE "
        file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"[Paths]\nPlugins = PlugIns\n\")
        " COMPONENT Runtime)

    # Install the license
    INSTALL(FILES 
      ${CMAKE_SOURCE_DIR}/LICENSE
      DESTINATION "${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}"
      COMPONENT Runtime)


    find_package(Qt5Widgets)
    get_target_property(Qt5_location Qt5::Widgets LOCATION)

    string(FIND ${Qt5_location} "/QtWidgets" length)
    string(SUBSTRING ${Qt5_location} 0 ${length} Qt5_location)
    #Fix the bundle
    install(CODE "
      include(BundleUtilities)
      fixup_bundle(\"${APPS}\" \"${QT_PLUGINS};\" \"${Qt5_location}\")
     "
      COMPONENT Runtime)

  ENDIF(APPLE)

  if(UNIX AND NOT APPLE)
    
    set(QT_PLUGINS "")
    set(qt_plugins Qt5::QXcbIntegrationPlugin Qt5::QXcbGlxIntegrationPlugin)
    foreach(qt_plugin ${qt_plugins})

      get_target_property(_qt_plugin_path "${qt_plugin}" LOCATION)
      get_filename_component(_qt_plugin_file "${_qt_plugin_path}" NAME)
      get_filename_component(_qt_plugin_type "${_qt_plugin_path}" PATH)
      get_filename_component(_qt_plugin_type "${_qt_plugin_type}" NAME)

      install(PROGRAMS "${_qt_plugin_path}"
            DESTINATION "lib/QtPlugins/${_qt_plugin_type}"
            COMPONENT RuntimePlugins)
      
      list(APPEND
        QT_PLUGINS 
        \${CMAKE_INSTALL_PREFIX}/lib/QtPlugins/${_qt_plugin_type}/${_qt_plugin_file}
        )
    endforeach()

    INSTALL(CODE "
        file(WRITE \"\${CMAKE_INSTALL_PREFIX}/bin/qt.conf\" \"[Paths]\nPrefix=..\nPlugins=lib/QtPlugins\n\")
        " COMPONENT Runtime)

    get_target_property(Qt5_location Qt5::Widgets LOCATION)  
    string(FIND ${Qt5_location} "libQt5Widgets" length)
    string(SUBSTRING ${Qt5_location} 0 ${length} Qt5_location)

    install(CODE "
      include(BundleUtilities)
      fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/bin/MultiSegPipeline\" \"${QT_PLUGINS};\" \"${Qt5_location};\")
      file(GLOB qt_libs \${CMAKE_INSTALL_PREFIX}/bin/*.so*)
      foreach(qt_lib \${qt_libs})
        get_filename_component(qt_file \"\${qt_lib}\" NAME)
        file(RENAME \${qt_lib} \${CMAKE_INSTALL_PREFIX}/lib/\${qt_file})
      endforeach()
     "
      COMPONENT Runtime)

    
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/MultiSegPipeline "\#!/bin/bash\nSCRIPT=$(realpath -s $0)\nSCRIPTPATH=$(dirname $SCRIPT)\nexport LD_LIBRARY_PATH=$SCRIPTPATH/lib\n$SCRIPTPATH/bin/MultiSegPipeline\n")

    install(CODE "
      file(
        COPY ${CMAKE_CURRENT_BINARY_DIR}/MultiSegPipeline
        DESTINATION \${CMAKE_INSTALL_PREFIX}
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      )
      " COMPONENT Runtime)
    
  endif()

endif()