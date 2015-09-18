#include "RasterizeMeshConfig.h"
#include "Rasterizer.h"

/**
 * This program opens a polydata (closed surface, vtkPolyData) and converts it into volume
 * representation (vtkImageData) where the foreground voxels are 1 and the background voxels are
 * 0. Internally vtkPolyDataToImageStencil is utilized. The resultant image is saved to disk 
 * in metaimage file format (SphereVolume.mhd).
 */
int main(int argc, char *argv[])
{  

	if (argc < 2)
    {
		fprintf(stdout,"%s Version %d.%d\n",
				argv[0],
				RasterizeMesh_VERSION_MAJOR,
				RasterizeMesh_VERSION_MINOR);
		fprintf(stdout,"Usage: %s number\n",argv[0]);
		//return 1;
    }

	std::string inMesh="C:/DATA/80XXXX_CuttingAneurysmBlock.ply";
	std::string outImage="C:/DATA/80XXXX_CuttingAneurysmBlock.mhd";
	double spacing[3]; // desired volume spacing
	spacing[0] = 0.1367190033197403;
	spacing[1] = 0.1367190033197403;
	spacing[2] = 0.1367190033197403;

	int dim[3];
	dim[0]=100;
	dim[1]=140;
	dim[2]=100;

	double origin[3];
	origin[0] = 25.095754500000002 ;//bounds[0] + spacing[0] / 2;
	origin[1] = -378.9042455 ;//bounds[2] + spacing[1] / 2;
	origin[2] = 490.98947449999895;//bounds[4] + spacing[2] / 2;
	
	Rasterizer* rasterizer = new Rasterizer();
	rasterizer->SetSpacing(spacing[0], spacing[1], spacing[2]);
	rasterizer->SetDimension(dim[0], dim[1], dim[2]);
	rasterizer->SetOrigin(origin[0], origin[1], origin[2]);
	rasterizer->SetMeshFilename(inMesh);
	rasterizer->SetOutputImageFilename(outImage);
	
	try
	{
		rasterizer->Rasterize();
	}
	catch (const char* msg)
	{
		std::cerr << msg << std::endl;
	}
	
	delete rasterizer;

  
  return EXIT_SUCCESS;
}
