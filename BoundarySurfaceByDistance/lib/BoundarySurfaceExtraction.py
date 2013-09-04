# Same filtering task using a Programmable Python Filter in Paraview.

aneurysm = self.GetInputDataObject(0,0)
vessel = self.GetInputDataObject(0,1)
output = self.GetOutputDataObject(0)

distanceFilter = vtk.vtkDistancePolyDataFilter()
distanceFilter.SetInputConnection( 0, vessel.GetProducerPort())
distanceFilter.SetInputConnection( 1, aneurysm.GetProducerPort())

# Only absolute distance
distanceFilter.SignedDistanceOff()
distanceFilter.Update()

# Cell Threshold
threshold = vtk.vtkThreshold()
threshold.SetInputConnection(distanceFilter.GetOutputPort())
threshold.ThresholdBetween(0,2)
threshold.SetInputArrayToProcess(0, 0, 0, vtk.vtkDataObject.FIELD_ASSOCIATION_CELLS, "Distance") 
threshold.Update()

# Convert UnstructuredGrid into PolyData
geometryFilter = vtk.vtkGeometryFilter()
geometryFilter.SetInputConnection(threshold.GetOutputPort())
geometryFilter.Update()
 
output.ShallowCopy(geometryFilter.GetOutput())
