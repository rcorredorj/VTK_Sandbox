Extracts boundary surface between two meshes. 
Very simple: Take one surface, calculate distance from this surface to the second one and keep the cells under a distance threshold.

Tested with:
vtk 5.10
Cmake 2.8

By now, under Windows (VS 2010) add VTK libs path to Additional Library Directory in the executable VS Project properties, i.e. right-click on BoundarySurfaceByDistance > Properties > Linker > General > Additional Library Directories>Add path to VTK libs (where vtkCommon, vtkFiltering, etc. are located)
