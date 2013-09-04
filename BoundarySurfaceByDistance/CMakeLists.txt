CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

INCLUDE(CPack)

PROJECT(VTK_Tools)

# VTK
FIND_PACKAGE(VTK REQUIRED)
IF(VTK_FOUND)
    INCLUDE(${VTK_USE_FILE})
ENDIF(VTK_FOUND)

INCLUDE_DIRECTORIES(
	${VTK_Tools_SOURCE_DIR}/lib
)
MESSAGE(${VTK_Tools_SOURCE_DIR}/lib)
ADD_EXECUTABLE(BoundarySurfaceByDistanceApp MACOSX_BUNDLE BoundarySurfaceByDistanceApp)

# Make sure the linker can find the library once it is built. Look into VStudio if OS=Windows 
TARGET_LINK_LIBRARIES(BoundarySurfaceByDistanceApp ${VTK_LIBRARIES} MeshTools)

INSTALL(TARGETS BoundarySurfaceByDistanceApp DESTINATION exec)

ADD_SUBDIRECTORY (lib) 