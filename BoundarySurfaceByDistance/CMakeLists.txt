CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(CPack)

PROJECT(BoundarySurfaceByDistance)

# VTK
FIND_PACKAGE(VTK REQUIRED)
IF(VTK_FOUND)
    INCLUDE(${VTK_USE_FILE})
ENDIF(VTK_FOUND)

INCLUDE_DIRECTORIES(
	${BoundarySurfaceByDistance_SOURCE_DIR}/lib
)

ADD_EXECUTABLE(BoundarySurfaceByDistance MACOSX_BUNDLE BoundarySurfaceByDistance)

# Make sure the linker can find the library once it is built. Look into VStudio if OS=Windows 
TARGET_LINK_LIBRARIES(BoundarySurfaceByDistance ${VTK_LIBRARIES} MeshTools)

INSTALL(TARGETS BoundarySurfaceByDistance DESTINATION exec)

ADD_SUBDIRECTORY (lib) 