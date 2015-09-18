#ifndef _Rasterizer_H_
#define _Rasterizer_H_


//////////////
// INCLUDES

// CMAKE
#include "RasterizeMeshConfig.h"

// SYSTEM LIBS
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <exception>

// LOCAL LIBS 


// VTK
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkImageData.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkPointData.h>
#include <vtkMetaImageWriter.h>
#include <vtkMetaImageReader.h>

//---------------------------------------------
//
//---------------------------------------------

class Rasterizer
{
	public :
		Rasterizer();
		~Rasterizer();
		
		void SetMeshFilename(std::string mesh);
		void SetImageTemplateFilename(std::string image);
		void SetOutputImageFilename(std::string image);
		void SetSpacing(double spcx, double spcy, double spcz);
		void SetOrigin(double originx, double originy, double originz);
		void SetDimension(int dimx, int dimy, int dimz);

		void SetFillInVale(int value);
		void SetFillOutVale(int value);
		void SetTolerance(int value);
		
		void Rasterize() throw();
	    
	private:
		double _spacing[3];
		double _origin[3];
		int _dims[3];
		int _inValue;
		int _outValue;
		int _tolerance;

		std::string _inputMeshFilename;
		std::string _inputImageTemplateFilename;
		std::string _outputImageFilename;
		vtkSmartPointer<vtkPolyData> _mesh;
		vtkSmartPointer<vtkImageData> _templateImage;
		vtkSmartPointer<vtkImageData> _outputImage;
		
		void Init();
		void ReadMesh(std::string mesh) throw();

};

//-----------------------------------------------------------------------------
#endif
