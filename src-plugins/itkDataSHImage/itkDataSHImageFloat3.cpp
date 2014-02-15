/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <itkDataSHImageFloat3.h>

#include <medAbstractDataFactory.h>

#include <itkImage.h>
#include <itkImageRegionConstIterator.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkImageFileWriter.h>
#include <itkVectorImage.h>
#include <itkVector.h>

// /////////////////////////////////////////////////////////////////
// itkDataSHImagePrivate
// /////////////////////////////////////////////////////////////////

class itkDataSHImageFloat3Private
{
public:
    typedef itk::VectorImage<float, 3> SHImageType;
    typedef SHImageType::PixelType    SHType;
  
    SHImageType::Pointer shs;
};

// /////////////////////////////////////////////////////////////////
// itkDataSHImage
// /////////////////////////////////////////////////////////////////

itkDataSHImageFloat3::itkDataSHImageFloat3(): medAbstractTypedImageData<3,float>(), d(new itkDataSHImageFloat3Private)
{
    d->shs = 0;
}

itkDataSHImageFloat3::~itkDataSHImageFloat3()
{
    delete d;
    d = 0;
}

bool itkDataSHImageFloat3::registered()
{
    return medAbstractDataFactory::instance()->registerDataType("itkDataSHImageFloat3", createItkDataSHImageFloat3);
}

QString itkDataSHImageFloat3::description() const
{
    return tr("itk spherical harmonic 3d image data (float)");
}
QString itkDataSHImageFloat3::identifier() const
{
    return "itkDataSHImageFloat3";
}

void *itkDataSHImageFloat3::data()
{
    return d->shs.GetPointer();
}

void *itkDataSHImageFloat3::output()
{
    return d->shs.GetPointer();
}

void itkDataSHImageFloat3::setData(void *data)
{
    typedef itkDataSHImageFloat3Private::SHImageType SHImageType;

    if( SHImageType* shs = dynamic_cast<SHImageType*>( (itk::Object*)(data) ) ){
        d->shs = shs;
    }
    else
      qDebug() << "Cannot cast pointer to correct SH float type";
}

int itkDataSHImageFloat3::xDimension()
{
    if (!d->shs.IsNull())
      return d->shs->GetLargestPossibleRegion().GetSize()[0];
    return -1;
}

int itkDataSHImageFloat3::yDimension()
{
    if (!d->shs.IsNull())
        return d->shs->GetLargestPossibleRegion().GetSize()[1];
    return -1;
}

int itkDataSHImageFloat3::zDimension()
{
    if (!d->shs.IsNull())
        return d->shs->GetLargestPossibleRegion().GetSize()[2];
    return -1;
}

// /////////////////////////////////////////////////////////////////
// Type instantiation
// /////////////////////////////////////////////////////////////////

medAbstractData *createItkDataSHImageFloat3()
{
    return new itkDataSHImageFloat3;
}
