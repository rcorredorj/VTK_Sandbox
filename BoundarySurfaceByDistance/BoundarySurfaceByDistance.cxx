
//////////////
// INCLUDES

// SYSTEM LIBS
#include <stdio.h>
#include <stdlib.h>

// LOCAL LIBS 
#include <BoundarySurfaceExtraction.h>

//VTK
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkSmartPointer.h>

int main(int argc, char *argv[])
{
	std::cout << "RaC 2013 - BoundarySurfaceByDistance main START" << std::endl;

	if (argc < 4)
    {
		std::cout << "First arg: path to first .vtk polydata,\nSecond arg: path to second .vtk polydata,\nThird arg: distance threshold,\nFourth arg: output path of .vtk polydata"<<std::endl;
		return EXIT_SUCCESS;
    }
	std::string inputMesh1 = argv[1];
	std::string inputMesh2 = argv[2];
	double distThreshold = atof(argv[3]);
	std::string outputMesh = argv[4];
 
	BoundarySurfaceExtraction* surfExtraction = new BoundarySurfaceExtraction();
	surfExtraction->SetInputPolyData1(inputMesh1);
	surfExtraction->SetInputPolyData2(inputMesh2);
	surfExtraction->SetDistanceThreshold(distThreshold);
	surfExtraction->EvaluateDistance();
	surfExtraction->WriteOutputPolyData(outputMesh);

	delete surfExtraction;
	std::cout << "RaC 2013 - BoundarySurfaceByDistance main END" << std::endl;
  return EXIT_SUCCESS;
}

  