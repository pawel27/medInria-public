/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "meshMapping.h"

#include <dtkCore/dtkAbstractProcessFactory.h>
#include <dtkCore/dtkSmartPointer.h>

#include <dtkCore/dtkAbstractProcess.h>

#include <medMetaDataKeys.h>
#include <medAbstractDataFactory.h>
#include <medAbstractData.h>

#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkMetaDataSet.h>
#include <vtkMetaSurfaceMesh.h>
#include <vtkMatrix4x4.h>
#include <vtkImageGradientMagnitude.h>
#include <vtkProbeFilter.h>
#include <vtkTransformFilter.h>
#include <vtkSmartPointer.h>
#include <vtkTransform.h>
#include <vtkImageCast.h>

#include <itkImageToVTKImageFilter.h>

// /////////////////////////////////////////////////////////////////
// meshMappingPrivate
// /////////////////////////////////////////////////////////////////

class meshMappingPrivate
{
public:
		
	meshMappingPrivate(meshMapping *q) {parent=q;}
	virtual ~meshMappingPrivate(void) {}
	
	meshMapping *parent;
	QList<medAbstractParameter*> parameters;
	QString res;

    template <class PixelType> int mapImageOnMesh()
    {
        medAbstractData* structure = parent->input<medAbstractData>(0);
        medAbstractData* data = parent->input<medAbstractData>(1);

		if ( !structure ||!structure->data() 
            || !data ||!data->data())
			return -1;
			
        typedef itk::Image<PixelType, 3> ImageType;

        //Converting the mesh
        if(!structure->identifier().contains("vtkDataMesh"))
            return -1;
        vtkMetaDataSet * structureDataset = static_cast<vtkMetaDataSet*>(structure->data());
        vtkPolyData * structurePolydata = static_cast<vtkPolyData*>(structureDataset->GetDataSet());

        // Converting the image
        typedef itk::ImageToVTKImageFilter<ImageType> FilterType;
        typename FilterType::Pointer filter = FilterType::New();
        typename ImageType::Pointer img = static_cast<ImageType *>(data->data());
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

        vtkImageCast * cast = vtkImageCast::New();
        cast->SetInput(vtkImage);
        cast->SetOutputScalarTypeToFloat();

        //To get the itkImage infos back
        vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
        t->SetMatrix(matrix);

        vtkSmartPointer<vtkTransformFilter> transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
        transformFilter->SetInputConnection(cast->GetOutputPort());
        transformFilter->SetTransform(t);

        // Probe magnitude with iso-surface.
        vtkProbeFilter* probe = vtkProbeFilter::New();
        probe->SetInput(structurePolydata);
        probe->SetSourceConnection(transformFilter->GetOutputPort());
        probe->SpatialMatchOn();
        probe->Update();
        vtkPolyData * polydata = probe->GetPolyDataOutput();


        vtkMetaSurfaceMesh * smesh = vtkMetaSurfaceMesh::New();
        smesh->SetDataSet(polydata);

		parent->output<medAbstractData>(0)->copyMetaDataFrom(structure);
        parent->output<medAbstractData>(0)->setData(smesh);

        return EXIT_SUCCESS;
    }

    //int mapMeshOnMesh()
    //{
    //    medAbstractData* structure = parent->input<medAbstractData>(0);
    //    medAbstractData* data = parent->input<medAbstractData>(1);

    //    if ( !structure ||!structure->data() || !data ||!data->data())
    //        return -1;

    //    //Converting the meshes
    //     if(!structure->identifier().contains("vtkDataMesh"))
    //        return -1;
    //    vtkMetaDataSet * structureDataset = static_cast<vtkMetaDataSet*>(structure->data());
    //    vtkPolyData * structurePolydata = static_cast<vtkPolyData*>(structureDataset->GetDataSet());

    //    vtkMetaDataSet * dataDataset = static_cast<vtkMetaDataSet*>(data->data());
    //    vtkPolyData * dataPolydata = static_cast<vtkPolyData*>(dataDataset->GetDataSet());

    //    // Probe magnitude with iso-surface.
    //    vtkProbeFilter* probe = vtkProbeFilter::New();
    //    probe->SetInput(structurePolydata);
    //    probe->SetSource(dataPolydata);
    //    probe->SpatialMatchOn();
    //    probe->Update();
    //    vtkPolyData * polydata = probe->GetPolyDataOutput();

    //    vtkMetaSurfaceMesh * smesh = vtkMetaSurfaceMesh::New();
    //    smesh->SetDataSet(polydata);

    //    parent->output<medAbstractData>(0)->copyMetaDataFrom(structure);
    //    parent->output<medAbstractData>(0)->setData(smesh);

    //    return EXIT_SUCCESS;
    //}

};

// /////////////////////////////////////////////////////////////////
// meshMapping
// /////////////////////////////////////////////////////////////////

meshMapping::meshMapping(medItkFiltersProcessBase *parent) : medAbstractFilteringProcess(parent), d(new meshMappingPrivate(this))
{
    medInputDataPort *input = new medInputDataPort("Data to map", false);
    input->setInput(NULL);

    this->appendInput(input);
}

meshMapping::~meshMapping()
{
    delete d;
}

bool meshMapping::registered()
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("meshMapping", createMeshMapping, "medAbstractFilteringProcess");
}


QList<medAbstractParameter*> meshMapping::parameters()
{
    return QList<medAbstractParameter*>();
}

int meshMapping::update()
{
    if ( !this->input<medAbstractData*>(0) )
        return -1;

    QString id = this->input<medAbstractData>(0)->identifier();
    dtkSmartPointer <medAbstractData> output = medAbstractDataFactory::instance()->createSmartPointer("medVtkMeshData");

    int res = EXIT_SUCCESS;
	
    qDebug() << "itkFilters, update : " << id;

    if ( id == "medItkChar3ImageData" ) //TODO: create new function for meshes
    {
        res = d->mapImageOnMesh<char>();
    }
    else if ( id == "medItkUChar3ImageData" )
    {
        res = d->mapImageOnMesh<unsigned char>();
    }
    else if ( id == "medItkShort3ImageData" )
    {
        res = d->mapImageOnMesh<short>();
    }
    else if ( id == "medItkUShort3ImageData" )
    {
        res = d->mapImageOnMesh<unsigned short>();
    }
    else if ( id == "medItkInt3ImageData" )
    {
        res = d->mapImageOnMesh<int>();
    }
    else if ( id == "medItkUInt3ImageData" )
    {
        res = d->mapImageOnMesh<unsigned int>();
    }
    else if ( id == "medItkLong3ImageData" )
    {
        res = d->mapImageOnMesh<long>();
    }
    else if ( id== "medItkULong3ImageData" )
    {
        res = d->mapImageOnMesh<unsigned long>();
    }
    else if ( id == "medItkFloat3ImageData" )
    {
        res = d->mapImageOnMesh<float>();
    }
    else if ( id == "medItkDouble3ImageData" )
    {
        res = d->mapImageOnMesh<double>();
    }
    //else if ( id == "medVtkMeshData" )
    //{
    //    res = d->mapMeshOnMesh();
    //}
    else
    {
        d->res = "Error : pixel type not yet implemented (" + id + ")";
        res = EXIT_FAILURE;
    }
    if(!d->res.isEmpty())
        emit showError(d->res);

    return res;
}        

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess *createMeshMapping()
{
    return new meshMapping;
}
