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


FILE(READ MultiSegPipeline.xml var)

STRING(REGEX MATCH "<version>.*</version>" ext "${var}")
STRING(REPLACE "<version>" "" version_number ${ext} )
STRING(REPLACE "</version>" "" version_number ${version_number})

ADD_DEFINITIONS(-DNEOSEGPIPELINE_VERSION="${version_number}")

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





