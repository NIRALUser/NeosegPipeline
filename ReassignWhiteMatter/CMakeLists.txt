# This is the root ITK CMakeList file: 
cmake_minimum_required(VERSION 2.8.3)

# This project is designed to be built outside the Insight source tree.
project(ReassignWhiteMatter)

# Find SlicerExecutionModel
FIND_PACKAGE(SlicerExecutionModel REQUIRED)
INCLUDE(${SlicerExecutionModel_USE_FILE})
INCLUDE(${GenerateCLP_USE_FILE})

# Find ITK
find_package(ITK REQUIRED)
INCLUDE (${ITK_USE_FILE})

# GENERATECLP(ReassignWhiteMatter_CLP ReassignWhiteMatter.xml)

# include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

SET(ConnectedComponents_SOURCE Parameters.cxx ParametersXMLFile.cxx)
SET(ConnectedComponents_HEADERS Parameters.h ParametersXMLFile.h)

# add_executable(ReassignWhiteMatter ${ConnectedComponents_SOURCE} ${ReassignWhiteMatter_CLP})

# TARGET_LINK_LIBRARIES(ReassignWhiteMatter ${ITK_LIBRARIES})

# install(TARGETS ReassignWhiteMatter RUNTIME DESTINATION bin)

SEMMacroBuildCLI(
   NAME ReassignWhiteMatter
   EXECUTABLE_ONLY
   INCLUDE_DIRECTORIES ${ITK_INCLUDE_DIR}
   LINK_DIRECTORIES ${ITK_LIBRARY_DIRS}
   TARGET_LIBRARIES ${ITK_LIBRARIES}
   ADDITIONAL_SRCS ${ConnectedComponents_SOURCE}
   RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
   LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
   INSTALL_RUNTIME_DESTINATION ${INSTALL_RUNTIME_DESTINATION}
   INSTALL_LIBRARY_DESTINATION ${INSTALL_LIBRARY_DESTINATION}
   INSTALL_ARCHIVE_DESTINATION ${INSTALL_ARCHIVE_DESTINATION}
)
