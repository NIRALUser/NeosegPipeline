# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8.3)

# This project is designed to be built outside the Insight source tree.
project(WeightedLabelsAverage)

# Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})
INCLUDE(${GenerateCLP_USE_FILE})

# Find ITK
find_package(ITK REQUIRED)
INCLUDE (${ITK_USE_FILE})

# GENERATECLP(WeightedLabelsAverage_CLP WeightedLabelsAverage.xml) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(WeightedLabelsAverage_SOURCE itkLocalNormalizedCorrelationMetricFilter.hxx itkWeightedLabelsAverageFilter.hxx Parameters.cxx ParametersXMLFile.cxx)
SET(WeightedLabelsAverage_HEADERS itkLocalNormalizedCorrelationMetricFilter.h itkWeightedLabelsAverageFilter.h itkSpreadWeights.h Atlas.h Parameters.h ParametersXMLFile.h)

# add_executable(WeightedLabelsAverage ${WeightedLabelsAverage_SOURCE} ${WeightedLabelsAverage_CLP})

# TARGET_LINK_LIBRARIES(WeightedLabelsAverage ${ITK_LIBRARIES})

# install(TARGETS WeightedLabelsAverage RUNTIME DESTINATION bin)


SEMMacroBuildCLI(
   NAME WeightedLabelsAverage
   EXECUTABLE_ONLY
   INCLUDE_DIRECTORIES ${QT_INCLUDE_DIR} ${ITK_INCLUDE_DIR}
   LINK_DIRECTORIES ${QT_LIBRARY_DIRS} ${ITK_LIBRARY_DIRS}
   TARGET_LIBRARIES ${ITK_LIBRARIES}
   ADDITIONAL_SRCS ${WeightedLabelsAverage_SOURCE}
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
   LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   INSTALL_RUNTIME_DESTINATION ${INSTALL_RUNTIME_DESTINATION}
   INSTALL_LIBRARY_DESTINATION ${INSTALL_LIBRARY_DESTINATION}
   INSTALL_ARCHIVE_DESTINATION ${INSTALL_ARCHIVE_DESTINATION}
)

