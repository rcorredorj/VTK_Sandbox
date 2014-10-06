#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPolyDataReader.h>
#include <vtkImageData.h>
#include <vtkSphereSource.h>
#include <vtkMetaImageWriter.h>
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkPointData.h>

/**
 * This program opens a polydata (closed surface, vtkPolyData) and converts it into volume
 * representation (vtkImageData) where the foreground voxels are 1 and the background voxels are
 * 0. Internally vtkPolyDataToImageStencil is utilized. The resultant image is saved to disk 
 * in metaimage file format (SphereVolume.mhd).
 */
int main(int, char *[])
{  

vtkSmartPointer<vtkPolyDataReader> polydataReader = 
    vtkSmartPointer<vtkPolyDataReader>::New();  
    polydataReader->SetFileName("contourPTV.vtk");
    polydataReader->Update();
  vtkSmartPointer<vtkPolyData> pd = polydataReader->GetOutput();

  vtkSmartPointer<vtkImageData> whiteImage = 
    vtkSmartPointer<vtkImageData>::New();    
  double bounds[6];
  pd->GetBounds(bounds);
  double spacing[3]; // desired volume spacing
  spacing[0] = 0.9765625;
  spacing[1] = 0.9765625;
  spacing[2] = 3;
  whiteImage->SetSpacing(spacing);

  // compute dimensions
  int dim[3];
  for (int i = 0; i < 3; i++)
    {
    dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i]));
    }
  dim[0] = dim[0]+5;
  dim[1] = dim[1]+5;
  dim[2] = dim[2]+5;
  whiteImage->SetDimensions(dim);
  whiteImage->SetExtent(0, dim[0]+5 - 1, 0, dim[1]+5 - 1, 0, dim[2]+5 - 1);

  double origin[3];
  origin[0] = bounds[0] + spacing[0] / 2 - spacing[0]*3;
  origin[1] = bounds[2] + spacing[1] / 2 - spacing[1]*3;
  origin[2] = bounds[4] + spacing[2] / 2 - spacing[2]*3;;
  whiteImage->SetOrigin(origin);

#if VTK_MAJOR_VERSION <= 5
  whiteImage->SetScalarTypeToUnsignedChar();
  whiteImage->AllocateScalars();
#else
  whiteImage->AllocateScalars(VTK_UNSIGNED_CHAR,1);
#endif
  // fill the image with foreground voxels:
  unsigned char inval = 255;
  unsigned char outval = 0;
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
  pol2stenc->SetInputData(pd);
#endif
  pol2stenc->SetOutputOrigin(origin);
  pol2stenc->SetOutputSpacing(spacing);
  pol2stenc->SetOutputWholeExtent(whiteImage->GetExtent());
  pol2stenc->SetTolerance(0.00001);
  pol2stenc->Update();

  // cut the corresponding white image and set the background:
  vtkSmartPointer<vtkImageStencil> imgstenc = 
    vtkSmartPointer<vtkImageStencil>::New();
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
  writer->SetFileName("PTVContourVolume.mhd");
#if VTK_MAJOR_VERSION <= 5
  writer->SetInput(imgstenc->GetOutput());
#else
  writer->SetInputData(imgstenc->GetOutput());
#endif
  writer->Write();  
  
  return EXIT_SUCCESS;
}
