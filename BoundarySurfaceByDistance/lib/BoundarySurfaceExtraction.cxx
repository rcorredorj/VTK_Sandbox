#include "BoundarySurfaceExtraction.h"

BoundarySurfaceExtraction::BoundarySurfaceExtraction()
{
	Init();
}

BoundarySurfaceExtraction::~BoundarySurfaceExtraction()
{

}


void BoundarySurfaceExtraction::Init()
{
	_distanceThresholdBetweenMeshes=1;
	_inputPolydata1=NULL;
	_inputPolydata2=NULL;
	_outputPolydata=NULL;
	_timerLog = vtkSmartPointer<vtkTimerLog>::New();

}

void BoundarySurfaceExtraction::SetInputPolyData1(vtkSmartPointer<vtkPolyData> poly)
{
	_inputPolydata1 = poly;
}

void BoundarySurfaceExtraction::SetInputPolyData1(std::string polydataFilename)
{
	_inputPolydata1= ReadPolyData(polydataFilename);
}

void BoundarySurfaceExtraction::SetInputPolyData2(vtkSmartPointer<vtkPolyData> poly)
{
	_inputPolydata2 = poly;
}

void BoundarySurfaceExtraction::SetInputPolyData2(std::string polydataFilename)
{
	_inputPolydata2 = ReadPolyData(polydataFilename);
}

vtkSmartPointer<vtkPolyData> BoundarySurfaceExtraction::GetOutputPolyData()
{
	return _outputPolydata;
}

void BoundarySurfaceExtraction::SetDistanceThreshold(double distThreshold)
{
	_distanceThresholdBetweenMeshes = distThreshold;
}

void BoundarySurfaceExtraction::PreprocessMeshes()
{
	//Subdivide triangles one time
	vtkSmartPointer<vtkLinearSubdivisionFilter>  subdivFilterMesh1 = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
	subdivFilterMesh1->SetInput(_inputPolydata1);
	subdivFilterMesh1->SetNumberOfSubdivisions(1);
	subdivFilterMesh1->Update();
	
	vtkSmartPointer<vtkLinearSubdivisionFilter>  subdivFilterMesh2 = vtkSmartPointer<vtkLinearSubdivisionFilter>::New();
	subdivFilterMesh2->SetInput(_inputPolydata2);
	subdivFilterMesh2->SetNumberOfSubdivisions(1);
	subdivFilterMesh2->Update();

	//Deep copy of result into original meshes
	_inputPolydata1->DeepCopy(subdivFilterMesh1->GetOutput());
	_inputPolydata2->DeepCopy(subdivFilterMesh2->GetOutput());
}

void BoundarySurfaceExtraction::EvaluateDistance()
{
	
	 std::cout<<"RaC BoundarySurfaceExtraction::EvaluateDistance " << _inputPolydata1 << std::endl;
	if(_inputPolydata1!=NULL && _inputPolydata2!=NULL)
	{
		// Preprocess steps
		PreprocessMeshes();
		
		// Apply distance filter
		vtkSmartPointer<vtkDistancePolyDataFilter> distanceFilter = vtkSmartPointer<vtkDistancePolyDataFilter>::New();
 
		distanceFilter->SetInputConnection( 0, _inputPolydata1->GetProducerPort() );
		distanceFilter->SetInputConnection( 1, _inputPolydata2->GetProducerPort() );
		// Only absolute distance
		distanceFilter->SignedDistanceOff();
		distanceFilter->Update();

		vtkSmartPointer<vtkPolyData> distancesPolydata = distanceFilter->GetOutput();

		// Timer 
		
		std::cout << "Current time: " << _timerLog->GetUniversalTime() << std::endl;
		_timerLog->MarkEvent("Before everything");
		
		//The distance is obtained from cell data, but it can be obtained from point data too
		vtkSmartPointer<vtkDoubleArray> distancesByCellsArray = vtkDoubleArray::SafeDownCast(distancesPolydata->GetCellData()->GetScalars());
		//double minDistance = distancesArray->GetRange()[0];// << "," << distancesArray->GetRange()[1] << std::endl;
		
		vtkSmartPointer<vtkIdList> listCellIds = vtkSmartPointer<vtkIdList>::New();
		for(vtkIdType k=0;k<distancesByCellsArray->GetNumberOfTuples();k++)
		{
			if(distancesByCellsArray->GetTuple1(k)<=_distanceThresholdBetweenMeshes)
			{
				//std::cout << "RaC  id:" << k <<" Distance:"<< distancesByCellsArray->GetTuple1(k)<< std::endl;
				listCellIds->InsertNextId(k);
			}
		}
		_timerLog->MarkEvent("After extracting id's according to distance");

		vtkSmartPointer<vtkExtractCells> cellsExtractor = vtkSmartPointer<vtkExtractCells>::New();
		cellsExtractor->SetInputConnection(distanceFilter->GetOutputPort());
		cellsExtractor->SetCellList(listCellIds);
		_timerLog->MarkEvent("After cells extractor");

		vtkSmartPointer<vtkGeometryFilter> geoFilter = vtkSmartPointer<vtkGeometryFilter>::New();
		geoFilter->SetInputConnection(cellsExtractor->GetOutputPort());
		geoFilter->Update();
		_timerLog->MarkEvent("After geometry filter");
		
		// If needed to write a Polydata

		/*
		vtkSmartPointer<vtkPolyDataWriter> polyWriter = vtkSmartPointer<vtkPolyDataWriter>::New();
		polyWriter->SetFileName(".\\polydata.vtk");
		polyWriter->SetInputConnection(geoFilter->GetOutputPort());
		polyWriter->Write();
		*/
		std::cout << "Timer log:" << *_timerLog << std::endl;
		_outputPolydata=geoFilter->GetOutput();

	}

}

///////////////////////////////////////
// BEGIN TEST CODE 01
// Piece of test code that could be used to extract the cells by hand using a map structure that saves
// the relations among the vertices.
/*
		_timerLog->MarkEvent("Before handcraft RaC method");
		vtkSmartPointer<vtkDoubleArray> distancesArray = vtkDoubleArray::SafeDownCast(distancesPolydata->GetPointData()->GetScalars());
  
		vtkSmartPointer<vtkCellArray> cellArray = distancesPolydata->GetPolys();
		vtkSmartPointer<vtkIdList> listIds = vtkSmartPointer<vtkIdList>::New();
		vtkSmartPointer<vtkCellArray> newCellArray = vtkSmartPointer<vtkCellArray>::New();
		vtkSmartPointer<vtkPoints> newlstPoints = vtkSmartPointer<vtkPoints>::New();

		std::map<vtkIdType,vtkIdType> mapIds;
		vtkIdType newId=0;

		timerLog->MarkEvent("Before looking all the cells");
		cellArray->InitTraversal();
		while(cellArray->GetNextCell(listIds)!=0)
		{
			vtkIdType idP1 = listIds->GetId(0);
			vtkIdType idP2 = listIds->GetId(1);
			vtkIdType idP3 = listIds->GetId(2);
			double distP1 = distancesPolydata->GetPointData()->GetScalars()->GetTuple1(idP1);
			double distP2 = distancesPolydata->GetPointData()->GetScalars()->GetTuple1(idP2);
			double distP3 = distancesPolydata->GetPointData()->GetScalars()->GetTuple1(idP3);

			double dist = (distP1+distP2+distP3)/3;
			if(dist<=_distanceBetweenMeshes)
			{				
				double posPoint1[3], posPoint2[3], posPoint3[3];
				distanceFilter->GetOutput()->GetPoint(idP1,posPoint1);
				distanceFilter->GetOutput()->GetPoint(idP2,posPoint2);
				distanceFilter->GetOutput()->GetPoint(idP3,posPoint3);
				
				if(mapIds.count(idP1)==0)
				{
					mapIds[idP1]=newId;
					newlstPoints->InsertPoint(newId,posPoint1);
					newId++;
				}			
				if(mapIds.count(idP2)==0)
				{
					mapIds[idP2]=newId;
					newlstPoints->InsertPoint(newId,posPoint2);
					newId++;
				}
				if(mapIds.count(idP3)==0)
				{
					mapIds[idP3]=newId;
					newlstPoints->InsertPoint(newId,posPoint3);
					newId++;
				}

				vtkSmartPointer<vtkIdList> listNew = vtkSmartPointer<vtkIdList>::New();
				listNew->InsertId(0,mapIds[idP1]);
				listNew->InsertId(1,mapIds[idP2]);
				listNew->InsertId(2,mapIds[idP3]);
				newCellArray->InsertNextCell(listNew);
			}
		}
	
	  
	  timerLog->MarkEvent("Before constructing new polydata");
	  // Add the grid points to a polydata object
	  vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
	  polydata->SetPoints(newlstPoints);
	  polydata->SetPolys(newCellArray);

	  timerLog->MarkEvent("After constructing new polydata");

	  _outputPolydata->DeepCopy(polydata);
*/

// END TEST CODE 01
///////////////////////////////////////

vtkSmartPointer<vtkPolyData> BoundarySurfaceExtraction::ReadPolyData( std::string polydataFilename)
 {
	 vtkSmartPointer<vtkPolyDataReader> polyReader = vtkSmartPointer<vtkPolyDataReader>::New();
	 polyReader->SetFileName(polydataFilename.c_str());
	 polyReader->Update();
	 return polyReader->GetOutput();
 }

 void BoundarySurfaceExtraction::WriteOutputPolyData(std::string polydataFilename)
 {
	 WritePolyData(_outputPolydata,polydataFilename);
 }
 
 void BoundarySurfaceExtraction::WritePolyData(vtkSmartPointer<vtkPolyData> poly, std::string polydataFilename)
 {
	 vtkSmartPointer<vtkPolyDataWriter> polyWriter = vtkSmartPointer<vtkPolyDataWriter>::New();
	 polyWriter->SetFileName(polydataFilename.c_str());
	 polyWriter->SetInputConnection(poly->GetProducerPort());
	 polyWriter->Write();
 }