/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medMeshTools.h"
#include <medAbstractFilteringProcess.h>

#include <medAbstractData.h>
#include <medAbstractDataFactory.h>
#include <dtkCore/dtkAbstractProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkImageToIsosurface.h>
#include <vtkMetaDataSet.h>
#include <vtkMetaSurfaceMesh.h>

#include <vtkMarchingCubes.h>
#include <vtkTriangleFilter.h>
#include <vtkSmartPointer.h>
#include <vtkAlgorithmOutput.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <medDataManager.h>
#include <medMetaDataKeys.h>
#include <medDoubleParameter.h>


// /////////////////////////////////////////////////////////////////
// medMeshToolsPrivate
// /////////////////////////////////////////////////////////////////

class medMeshToolsPrivate
{
public:
    dtkSmartPointer <medAbstractData> input;
    dtkSmartPointer <medAbstractData> output;
    double isoValue;
    double targetReduction;
    bool decimate;
    bool smooth;
    int iterations;
    double relaxationFactor;
    int nb_triangles;
	
	medMeshToolsPrivate(medMeshTools *q) {parent=q;}
	virtual ~medMeshToolsPrivate(void) {}
	
	medMeshTools *parent;
	QList<medAbstractParameter*> parameters;
	medDoubleParameter* isoValueParam;
	QString res;

    template <class PixelType> int update();
};

template <class PixelType> int medMeshToolsPrivate::update()
{
    input = parent->input<medAbstractData>(0);

    typedef itk::Image<PixelType, 3> ImageType;
    typedef itk::ImageToVTKImageFilter<ImageType>  FilterType;
    typename FilterType::Pointer filter = FilterType::New();
    typename ImageType::Pointer img = dynamic_cast<ImageType *> ( ( itk::Object* ) ( input->data() ) );
    filter->SetInput(img);
    filter->Update();

    // ----- Hack to keep the itkImages info (origin and orientation)
    vtkMatrix4x4* matrix = vtkMatrix4x4::New();
    matrix->Identity();
    for (unsigned int x=0; x<3; x++) {
        for (unsigned int y=0; y<3; y++) 
        {
            matrix->SetElement(x,y,img->GetDirection()[x][y]);
        }
    }
    
    typename itk::ImageBase<3>::PointType origin = img->GetOrigin();
    double v_origin[4], v_origin2[4];
    for (int i=0; i<3; i++)
        v_origin[i] = origin[i];
    v_origin[3] = 1.0;
    matrix->MultiplyPoint (v_origin, v_origin2);
    for (int i=0; i<3; i++)
        matrix->SetElement (i, 3, v_origin[i]-v_origin2[i]);

//------------------------------------------------------

    vtkImageData * vtkImage = filter->GetOutput();

    vtkContourFilter* contour = vtkContourFilter::New();
    contour->SetInput( vtkImage );
    contour->SetValue(0, 1/*isoValue*/);
    contour->Update();

    vtkTriangleFilter* contourTrian = vtkTriangleFilter::New();
    contourTrian->SetInputConnection(contour->GetOutputPort());
    contourTrian->PassVertsOn();
    contourTrian->PassLinesOn();
    contourTrian->Update();

    vtkPolyDataAlgorithm * lastAlgo = contourTrian;

    vtkDecimatePro* contourDecimated = 0;
    // if (decimate) {
        // // Decimate the mesh if required
        // contourDecimated = vtkDecimatePro::New();
        // contourDecimated->SetInputConnection(lastAlgo->GetOutputPort());
        // contourDecimated->SetTargetReduction(targetReduction);
        // contourDecimated->SplittingOff();
        // contourDecimated->PreserveTopologyOn();
        // contourDecimated->Update();
        // lastAlgo = contourDecimated;
    // }

    // vtkSmoothPolyDataFilter* contourSmoothed = 0;
    // if(smooth) {
        // // Smooth the mesh if required
        // contourSmoothed = vtkSmoothPolyDataFilter::New();
        // contourSmoothed->SetInputConnection(lastAlgo->GetOutputPort());
        // contourSmoothed->SetNumberOfIterations(iterations);
        // contourSmoothed->SetRelaxationFactor(relaxationFactor);
        // contourSmoothed->Update();
        // lastAlgo = contourSmoothed;
    // }


    vtkPolyData * polydata = lastAlgo->GetOutput();
    nb_triangles = polydata->GetNumberOfPolys();
    //To get the itkImage infos back
    vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
    t->SetMatrix(matrix);

    vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetInput(polydata);
    transformFilter->SetTransform(t);
    transformFilter->Update();

    polydata->DeepCopy(transformFilter->GetOutput());

    vtkMetaSurfaceMesh * smesh = vtkMetaSurfaceMesh::New();
    smesh->SetDataSet(polydata);

    //contour->Delete();
    //contourTrian->Delete();
    //if (contourDecimated) contourDecimated->Delete();
    //if (contourSmoothed) contourSmoothed->Delete();

	parent->output<medAbstractData>(0)->copyMetaDataFrom(parent->input<medAbstractData>(0));
    parent->output<medAbstractData>(0)->setData(smesh);
    
    return EXIT_SUCCESS;
}

// /////////////////////////////////////////////////////////////////
// medMeshTools
// /////////////////////////////////////////////////////////////////

medMeshTools::medMeshTools(medItkFiltersProcessBase *parent) : medAbstractFilteringProcess(parent), d(new medMeshToolsPrivate(this))
{
	d->isoValueParam = new medDoubleParameter("Iso value", this);
    d->isoValueParam->setRange(0,2000);
    d->isoValueParam->setValue(1.0);

    d->parameters << d->isoValueParam;
}

medMeshTools::~medMeshTools()
{
    delete d;
}

bool medMeshTools::registered()
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("medMeshTools", createMedMeshTools, "medAbstractFilteringProcess");
}


QList<medAbstractParameter*> medMeshTools::parameters()
{
    return d->parameters;
}

void medMeshTools::setInput ( medAbstractData *data, int channel )
{
    if ( !data )
        return;
    d->output = medAbstractDataFactory::instance()->createSmartPointer ( "vtkDataMesh" );
    
    d->input = data;
	qDebug()<<"d->input->identifier() : "<<d->input->identifier();
}    

void medMeshTools::setParameter ( double data, int channel )
{
    switch (channel) {
        case 0:
            d->isoValue = data;
            break;
        case 1:
            d->decimate = (data > 0) ? true : false;
            break;
        case 2:
            d->targetReduction = data;
            break;
        case 3:
            d->smooth = (data > 0) ? true : false;
            break;
        case 4:
            d->iterations = data;
            break;
        case 5:
            d->relaxationFactor = data;
            break;
    }
}

int medMeshTools::update()
{
    if ( !this->input<medAbstractData*>(0) )
        return -1;
        
    const QString& id = this->input<medAbstractData>(0)->identifier();

	dtkSmartPointer<medAbstractData>output = medAbstractDataFactory::instance()->createSmartPointer("medVtkMeshData");
	this->setOutput<medAbstractData>(output, 0);
	
	int res = EXIT_SUCCESS;
	
    if (id == "medItkChar3ImageData") {
        res = d->update<char>();
     }
    else if (id == "medItkUChar3ImageData") {
        res = d->update<unsigned char>();
     }
    else if (id == "medItkShort3ImageData") {
        res = d->update<short>();
     }
    else if (id == "medItkUShort3ImageData") {
        res = d->update<unsigned short>();
     }
    else if (id == "medItkInt3ImageData") {
        res = d->update<int>();
     }
    else if (id == "medItkUInt3ImageData") {
        res = d->update<unsigned int>();
     }
    else if (id == "medItkLong3ImageData") {
        res = d->update<long>();
     }
    else if (id== "medItkULong3ImageData") {
        res = d->update<unsigned long>();
     }
    else if (id == "medItkFloat3ImageData") {
        res = d->update<float>();
     }
    else if (id == "medItkDouble3ImageData") {
        res = d->update<double>();
     }
    else
    {
        d->res = "Error : pixel type not yet implemented (" + id + ")";
        res = EXIT_FAILURE;
    }
    if(!d->res.isEmpty())
        emit showError(d->res);
		
    return res;
}


//medAbstractData * medMeshTools::output()
//{
//    return ( d->output );
//}

int medMeshTools::getNumberOfTriangles()
{
    return d->nb_triangles;
}

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess *createMedMeshTools()
{
    return new medMeshTools;
}
