/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerNodeImage.h>

#include <medAbstractImageData.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <medAbstractDataFactory.h>
#include <medDataIndex.h>
#include <medDataManager.h>
#include <medMetaDataKeys.h>



// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlenderPrivate declaration
// /////////////////////////////////////////////////////////////////

class medComposerNodeImagePrivate
{
public:
    const QMimeData *intitialData;
    medDataIndex dataIndex;
    dtkSmartPointer<medAbstractImageData> imageData;

public:
    dtkComposerTransmitterEmitter<medAbstractImageData> emitter_image;
};

// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlender implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeImage::medComposerNodeImage(void) : dtkComposerNodeLeaf(), d(new medComposerNodeImagePrivate)
{
    d->imageData = 0;
    this->appendEmitter(&(d->emitter_image));
}

medComposerNodeImage::~medComposerNodeImage(void)
{
    delete d;

    d = NULL;
}

bool medComposerNodeImage::isAbstractData(void) const
{
    return false;
}

QString medComposerNodeImage::abstractDataType(void) const
{
    return "medAbstractImageData";
}

void medComposerNodeImage::setMimeData(const QMimeData* initialData)
{
    d->intitialData = initialData;

    if(d->intitialData->hasFormat("med/index") )
    {
        d->dataIndex = medDataIndex::readMimeData(d->intitialData);

        if(d->dataIndex.isValid())
        {
            d->imageData = dynamic_cast<medAbstractImageData *>(medDataManager::instance()->retrieveData(d->dataIndex));
        }
    }
}

void medComposerNodeImage::run(void)
{
    if(d->imageData)
    {
        d->imageData = dynamic_cast<medAbstractImageData *>(medDataManager::instance()->retrieveData(d->dataIndex));
        d->emitter_image.setData(d->imageData);
    }
}

QString medComposerNodeImage::type(void)
{
    return "medComposerNodeImage";
}

QString medComposerNodeImage::titleHint(void)
{
    if(d->imageData && !d->imageData->metadata ( medMetaDataKeys::SeriesDescription.key()).isEmpty() )
    {
        QString title = d->imageData->metadata ( medMetaDataKeys::PatientName.key() ) + " / " +
                                                d->imageData->metadata ( medMetaDataKeys::SeriesDescription.key() );
        return title;
    }
    else return "Image";
}

QString medComposerNodeImage::inputLabelHint(int port)
{
    switch (port) {
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeImage::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}
