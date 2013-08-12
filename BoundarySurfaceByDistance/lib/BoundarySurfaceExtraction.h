#ifndef _BoundarySurfaceExtraction_H_
#define _BoundarySurfaceExtraction_H_


//////////////
// INCLUDES

// SYSTEM LIBS
#include <stdio.h>
#include <stdlib.h>

// LOCAL LIBS 


//VTK
#include "vtkPolyData.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "vtkDoubleArray.h"
#include "vtkIdTypeArray.h"
#include "vtkCellArray.h"
#include "vtkSmartPointer.h"
#include "vtkDistancePolyDataFilter.h"
#include "vtkIdList.h"
#include "vtkLinearSubdivisionFilter.h"
#include "vtkPolyDataWriter.h"
#include "vtkPolyDataReader.h"
#include "vtkExtractCells.h"
#include "vtkGeometryFilter.h"
#include "vtkTimerLog.h"

//---------------------------------------------
//
//---------------------------------------------

class BoundarySurfaceExtraction
{
   public :
      BoundarySurfaceExtraction();
      ~BoundarySurfaceExtraction();

	  void SetInputPolyData1(vtkSmartPointer<vtkPolyData> poly);
	  void SetInputPolyData1(std::string polydataFilename);
	  void SetInputPolyData2(vtkSmartPointer<vtkPolyData> poly);
	  void SetInputPolyData2(std::string polydataFilename);	  
	  void SetDistanceThreshold(double distThreshold);	
	  vtkSmartPointer<vtkPolyData> GetOutputPolyData();
	  void WriteOutputPolyData(std::string polydataFilename);
	  void EvaluateDistance();
	  
      
   private:
	   vtkSmartPointer<vtkPolyData> _inputPolydata1;
	   vtkSmartPointer<vtkPolyData> _inputPolydata2;
	   vtkSmartPointer<vtkPolyData> _outputPolydata;
	   vtkSmartPointer<vtkTimerLog> _timerLog;
	   double _distanceThresholdBetweenMeshes;

	  void Init();
	  void PreprocessMeshes();
	  vtkSmartPointer<vtkPolyData> ReadPolyData(std::string polydataFilename);
	  void WritePolyData(vtkSmartPointer<vtkPolyData> poly, std::string polydataFilename);

};

//-----------------------------------------------------------------------------
#endif
