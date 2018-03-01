cmake_minimum_required(VERSION 3.4)

project(AverageDeformationField)

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

find_package(SlicerExecutionModel REQUIRED)
include(${SlicerExecutionModel_USE_FILE})

include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add_executable(AverageDeformationField AverageDeformationField.cxx)
#target_link_libraries(AverageDeformationField ${ITK_LIBRARIES})


SEMMacroBuildCLI(
	NAME AverageDeformationField
	EXECUTABLE_ONLY
	TARGET_LIBRARIES ${ITK_LIBRARIES} 
)

if(BUILD_TESTING)
	add_subdirectory(Testing)
endif()


