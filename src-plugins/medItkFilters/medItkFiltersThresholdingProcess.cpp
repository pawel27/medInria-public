/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medItkFiltersThresholdingProcess.h>

#include <dtkCore/dtkAbstractProcessFactory.h>
#include <medAbstractDataFactory.h>

#include <medMetaDataKeys.h>
#include <medDoubleParameter.h>
#include <medBoolParameter.h>

#include <itkImage.h>
#include <itkCommand.h>
#include <itkThresholdImageFilter.h>
#include <itkExceptionObject.h>


class medItkFiltersThresholdingProcessPrivate
{
public:
    medItkFiltersThresholdingProcessPrivate(medItkFiltersThresholdingProcess *q){parent = q;}
    virtual ~medItkFiltersThresholdingProcessPrivate(void) {}

    medItkFiltersThresholdingProcess* parent;
    medDoubleParameter *sigmaParam;
	medDoubleParameter *threshold, *outsideValue;
	//medBoolParameter *comparisonOperator;
    QList<medAbstractParameter*> parameters;

    template <class PixelType> void update ( void )
    {
        typedef itk::Image< PixelType, 3 > ImageType;
		typedef itk::ThresholdImageFilter < ImageType>  ThresholdImageFilterType;
        typename ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();

        thresholdFilter->SetInput ( dynamic_cast<ImageType *> ( ( itk::Object* ) ( parent->input<medAbstractData>(0)->data() ) ) );
        thresholdFilter->SetLower( threshold->value() );

        itk::CStyleCommand::Pointer callback = itk::CStyleCommand::New();
        callback->SetClientData ( ( void * ) parent );
        parent->setCallBack(callback);
        thresholdFilter->AddObserver ( itk::ProgressEvent(), callback );

        try
        {
            thresholdFilter->Update();
        }
        catch (itk::ExceptionObject err)
        {
            QString res = err.GetDescription();
            qDebug()<<res;
        }

        parent->output<medAbstractData>(0)->setData ( thresholdFilter->GetOutput() );

        QString newSeriesDescription = parent->input<medAbstractData>(0)->metadata ( medMetaDataKeys::SeriesDescription.key() );
        newSeriesDescription += " thresholding filter (" + QString::number(threshold->value()) + ")";

        parent->output<medAbstractData>(0)->setMetaData ( medMetaDataKeys::SeriesDescription.key(), newSeriesDescription );
    }
};


medItkFiltersThresholdingProcess::medItkFiltersThresholdingProcess(medItkFiltersThresholdingProcess *parent)
    : medItkFiltersProcessBase(parent), d(new medItkFiltersThresholdingProcessPrivate(this))
{
    this->setFilter(this);

    this->setDescription(tr("ITK thresholding filter"));
	
	d->threshold = new medDoubleParameter("Lower Threshold value", this);
	d->threshold->setRange(-10000, 10000);
	d->threshold->setValue(200.0);
	
	d->outsideValue = new medDoubleParameter("Outside value", this);
	d->outsideValue->setRange(-10000, 10000);
	d->outsideValue->setValue(0.0);
	
    d->parameters << d->threshold << d->outsideValue;
}

medItkFiltersThresholdingProcess::~medItkFiltersThresholdingProcess( void )
{
}

//-------------------------------------------------------------------------------------------

bool medItkFiltersThresholdingProcess::registered( void )
{
    return dtkAbstractProcessFactory::instance()->registerProcessType("itkThresholdingProcess", createmedItkFiltersThresholdingProcess);
}

//-------------------------------------------------------------------------------------------

QList<medAbstractParameter*> medItkFiltersThresholdingProcess::parameters()
{
    return d->parameters;
}

//-------------------------------------------------------------------------------------------

int medItkFiltersThresholdingProcess::update ( void )
{
    if ( !this->input<medAbstractData *>(0) )
        return -1;

    QString id = this->input<medAbstractData>(0)->identifier();

    qDebug() << "itkFilters, update : " << id;

    try
    {
        if ( id == "medItkChar3ImageData" )
        {
            d->update<char>();
        }
        else if ( id == "medItkUChar3ImageData" )
        {
            d->update<unsigned char>();
        }
        else if ( id == "medItkShort3ImageData" )
        {
            d->update<short>();
        }
        else if ( id == "medItkUShort3ImageData" )
        {
            d->update<unsigned short>();
        }
        else if ( id == "medItkInt3ImageData" )
        {
            d->update<int>();
        }
        else if ( id == "medItkUInt3ImageData" )
        {
            d->update<unsigned int>();
        }
        else if ( id == "medItkLong3ImageData" )
        {
            d->update<long>();
        }
        else if ( id== "medItkULong3ImageData" )
        {
            d->update<unsigned long>();
        }
        else if ( id == "medItkFloat3ImageData" )
        {
            d->update<float>();
        }
        else if ( id == "medItkDouble3ImageData" )
        {
            d->update<double>();
        }
        else
        {
            qDebug() << "Error : pixel type not yet implemented ("
            << id
            << ")";
            return -1;
        }
    }
    catch (itk::ExceptionObject &e)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// /////////////////////////////////////////////////////////////////
// Type instanciation
// /////////////////////////////////////////////////////////////////

dtkAbstractProcess * createmedItkFiltersThresholdingProcess ( void )
{
    return new medItkFiltersThresholdingProcess;
}
