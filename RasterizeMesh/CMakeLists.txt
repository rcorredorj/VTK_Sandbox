cmake_minimum_required(VERSION 2.8)

PROJECT(RasterizeMesh)

# The version number.
set(RasterizeMesh_VERSION_MAJOR 0)
set(RasterizeMesh_VERSION_MINOR 1)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/RasterizeMeshConfig.h.in"
  "${PROJECT_BINARY_DIR}/RasterizeMeshConfig.h"
  )
  
# add the binary tree to the search path for include files
# so that we will find RasterizeMeshConfig.h
include_directories("${PROJECT_BINARY_DIR}")

FIND_PACKAGE(VTK REQUIRED)
IF(VTK_FOUND)
    INCLUDE(${VTK_USE_FILE})
ENDIF(VTK_FOUND)


include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory(lib) 

if(VTK_LIBRARIES)
  # add the executable
	add_executable(RasterizeMesh RasterizeMesh.cxx)
	target_link_libraries(RasterizeMesh ${VTK_LIBRARIES} MeshFunctions)
endif()
