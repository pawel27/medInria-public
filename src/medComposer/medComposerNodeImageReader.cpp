/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerNodeImageReader.h>

#include <medAbstractImageData.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <medAbstractDataFactory.h>



// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlenderPrivate declaration
// /////////////////////////////////////////////////////////////////

class medComposerNodeImageReaderPrivate
{
public:
    dtkSmartPointer<dtkAbstractDataReader> dataReader;

public:
    dtkComposerTransmitterReceiver<QString> receiver_filename;
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;

public:
    dtkComposerTransmitterEmitter<medAbstractData> emitter_image;
};

// /////////////////////////////////////////////////////////////////
// sislComposerNodeSplineBlender implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeImageReader::medComposerNodeImageReader(void) : dtkComposerNodeLeafData(), d(new medComposerNodeImageReaderPrivate)
{
    d->dataReader = NULL;

    this->appendReceiver(&(d->receiver_filename));
    this->appendReceiver(&(d->receiver_image));

    this->appendEmitter(&(d->emitter_image));
}

medComposerNodeImageReader::~medComposerNodeImageReader(void)
{
    delete d;

    d = NULL;
}

bool medComposerNodeImageReader::isAbstractData(void) const
{
    return false;
}

QString medComposerNodeImageReader::abstractDataType(void) const
{
    return "medAbstractImageData";
}

void medComposerNodeImageReader::run(void)
{
    QString filename;

    if(!d->receiver_filename.isEmpty()) {
        filename = *(d->receiver_filename.data());
    }

    if (!d->receiver_filename.isEmpty() && !(filename.isEmpty())) {

        bool read = false;

        QList<QString> readers = medAbstractDataFactory::instance()->readers();

        if ( readers.size() == 0 ) {
            qDebug() <<  "No image readers found";
            return;
        }

        // cycle through readers to see if the last used reader can handle the file
        dtkSmartPointer<dtkAbstractDataReader> tempdataReader = NULL;

        for (int i=0; i<readers.size(); i++) {
            tempdataReader = medAbstractDataFactory::instance()->readerSmartPointer(readers[i]);
            if (tempdataReader->canRead(filename)) {
                /*d->lastSuccessfulReaderDescription = dataReader->identifier();*/
                tempdataReader->enableDeferredDeletion(false);
                d->dataReader = tempdataReader;
                break;
            }
        }

        if(d->dataReader)
        {
            read = d->dataReader->read(filename);
            d->emitter_image.setData(dynamic_cast<medAbstractData *>(d->dataReader->data()));

            if(read)
                qDebug() << "Read success";
            else
                qDebug() << "Read failure";
        } else {
            qDebug() << "no reader !";
        }

    } else  if (!d->receiver_image.isEmpty()) {
        medAbstractData *image = d->receiver_image.data();
        d->emitter_image.setData(image);
    } else {
        qDebug() << Q_FUNC_INFO << " No port connected";
    }

}

QString medComposerNodeImageReader::type(void)
{
    return "medComposerNodeImageReader";
}

QString medComposerNodeImageReader::titleHint(void)
{
    return "ImageReader";
}

QString medComposerNodeImageReader::inputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "file";
    case 1:
        return "image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeImageReader::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}
