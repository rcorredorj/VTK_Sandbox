#include "Rasterizer.h"

Rasterizer::Rasterizer()
{
	Init();
}

Rasterizer::~Rasterizer()
{

}


void Rasterizer::Init()
{
	_spacing[0] = 1;
	_spacing[1] = 1;
	_spacing[2] = 1;

	_inputMeshFilename = "";
	_inputImageTemplateFilename = "";
	_outputImageFilename = "";

	_inValue = 255;
	_outValue = 0;
	_tolerance = 0;

	_mesh = vtkSmartPointer<vtkPolyData>::New();
}

void Rasterizer::ReadMesh(std::string mesh) throw()
{

	if( mesh.substr(mesh.find_last_of(".") + 1) == "ply" || mesh.substr(mesh.find_last_of(".") + 1) == "PLY" ) 
	{
		std::cout << "Mesh File Extension: ply/PLY" << std::endl;
		vtkSmartPointer<vtkPLYReader> reader = vtkSmartPointer<vtkPLYReader>::New();  
		reader->SetFileName ( mesh.c_str() );
		reader->Update();
		_mesh = reader->GetOutput();
	} 
	else if ( mesh.substr(mesh.find_last_of(".") + 1) == "vtk" || mesh.substr(mesh.find_last_of(".") + 1) == "VTK" ) 
	{
		std::cout << "Mesh File Extension: vtk/VTK" << std::endl;
		vtkSmartPointer<vtkPolyDataReader> reader = vtkSmartPointer<vtkPolyDataReader>::New();  
		reader->SetFileName ( mesh.c_str() );
		reader->Update();
		_mesh = reader->GetOutput();
	}
	else
	{
		std::cout << "Mesh File Extension: ... " << std::endl;
		throw "The file format is not recognized or the reader has not been implemented yet.";
	}


}

void Rasterizer::SetMeshFilename(std::string mesh)
{
	_inputMeshFilename = mesh;
}

void Rasterizer::SetImageTemplateFilename(std::string image)
{
	_inputImageTemplateFilename = image;
}

void Rasterizer::SetOutputImageFilename(std::string image)
{
	_outputImageFilename = image;
}

void Rasterizer::SetSpacing(double spcx, double spcy, double spcz)
{
	_spacing[0] = spcx;
	_spacing[1] = spcy;
	_spacing[2] = spcz;
}

void Rasterizer::SetOrigin(double originx, double originy, double originz)
{
	_origin[0] = originx;
	_origin[1] = originy;
	_origin[2] = originz;
}

void Rasterizer::SetDimension(int dimx, int dimy, int dimz)
{
	_dims[0] = dimx;
	_dims[1] = dimy;
	_dims[2] = dimz;
}

void Rasterizer::SetFillInVale(int value)
{
	_inValue = value;
}

void Rasterizer::SetFillOutVale(int value)
{
	_outValue = value;
}

void Rasterizer::SetTolerance(int value)
{
	_tolerance = value;
}

void Rasterizer::Rasterize() throw()
{

	if ( _inputMeshFilename.empty() )
	{
		throw "Input mesh not available";
	}

	ReadMesh(_inputMeshFilename);

	vtkSmartPointer<vtkImageData> whiteImage = vtkSmartPointer<vtkImageData>::New();    

	//double spacing[3]; // desired volume spacing
	//spacing[0] = 0.136719;
	//spacing[1] = 0.136719;
	//spacing[2] = 0.136719;
	whiteImage->SetSpacing(_spacing);

	// compute dimensions
	//int dim[3];

	//dim[0]=100;
	//dim[1]=140;
	//dim[2]=100;
	whiteImage->SetDimensions(_dims);
	whiteImage->SetExtent(0, _dims[0] - 1, 0, _dims[1] - 1, 0, _dims[2] - 1);

	//double origin[3];
	//origin[0] = -6.90425 ;//bounds[0] + spacing[0] / 2;
	//origin[1] = -375.904 ;//bounds[2] + spacing[1] / 2;
	//origin[2] = 720.989;//bounds[4] + spacing[2] / 2;

	whiteImage->SetOrigin(_origin);

	#if VTK_MAJOR_VERSION <= 5
		whiteImage->SetScalarTypeToUnsignedChar();
		whiteImage->AllocateScalars();
	#else
		whiteImage->AllocateScalars(VTK_UNSIGNED_CHAR,1);
	#endif
	// fill the image with foreground voxels:
	// unsigned char inval = 255;
	// unsigned char outval = 0;

	unsigned char inval = (unsigned char) _inValue;
	unsigned char outval = (unsigned char) _outValue;


	vtkIdType count = whiteImage->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		whiteImage->GetPointData()->GetScalars()->SetTuple1(i, inval);
	}

	// polygonal data --> image stencil:
	vtkSmartPointer<vtkPolyDataToImageStencil> pol2stenc = 
		vtkSmartPointer<vtkPolyDataToImageStencil>::New();
	#if VTK_MAJOR_VERSION <= 5
		pol2stenc->SetInput(pd);
	#else
		pol2stenc->SetInputData(_mesh);
	#endif
	pol2stenc->SetOutputOrigin(_origin);
	pol2stenc->SetOutputSpacing(_spacing);
	pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
	pol2stenc->SetTolerance(_tolerance);
	pol2stenc->Update();

		// cut the corresponding white image and set the background:
		vtkSmartPointer<vtkImageStencil> imgstenc = vtkSmartPointer<vtkImageStencil>::New();
	#if VTK_MAJOR_VERSION <= 5
		imgstenc->SetInput(whiteImage);
		imgstenc->SetStencil(pol2stenc->GetOutput());
	#else
		imgstenc->SetInputData(whiteImage);
		imgstenc->SetStencilConnection(pol2stenc->GetOutputPort());
	#endif
	imgstenc->ReverseStencilOff();
	imgstenc->SetBackgroundValue(outval);
	imgstenc->Update();

	vtkSmartPointer<vtkMetaImageWriter> writer = 
			vtkSmartPointer<vtkMetaImageWriter>::New();
	writer->SetFileName(_outputImageFilename.c_str());
	#if VTK_MAJOR_VERSION <= 5
		writer->SetInput(imgstenc->GetOutput());
	#else
		writer->SetInputData(imgstenc->GetOutput());
	#endif
		writer->Write();  

}

