/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerNodeSegmentation.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>
#include <dtkCore/dtkSmartPointer.h>

#include <medAbstractFilteringProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

#include <medAbstractImageData.h>
#include <medDataManager.h>
#include <medMetaDataKeys.h>

#include <qgraphicswidget.h>
#include <medComposerScene.h>
#include <medComposer.h>

// /////////////////////////////////////////////////////////////////
// medComposerNodeFilteringPrivate interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeSegmentationPrivate
{
public:
    medAbstractProcess *segmentationProcess;

public:
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;

public:
    dtkComposerTransmitterEmitter<medAbstractImageData> emitter_image;

public:

};

// /////////////////////////////////////////////////////////////////
// medComposerNodeSegmentation implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeSegmentation::medComposerNodeSegmentation(void) : dtkComposerNodeLeafProcess(), d(new medComposerNodeSegmentationPrivate)
{
    this->appendReceiver(&(d->receiver_image));

    this->appendEmitter(&(d->emitter_image));

    d->segmentationProcess = NULL;
}

medComposerNodeSegmentation::~medComposerNodeSegmentation(void)
{
    this->clearProcess();

    delete d;
    d = NULL;
}

QString medComposerNodeSegmentation::type(void)
{
    return "medComposerNodeSegmentation";
}

QString medComposerNodeSegmentation::titleHint(void)
{
    return "Segmentation";
}

QString medComposerNodeSegmentation::inputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeSegmentation::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}

bool medComposerNodeSegmentation::isAbstractProcess(void) const
{
    return false;
}

QString medComposerNodeSegmentation::abstractProcessType(void) const
{
    return "medAbstractSegmentationProcess";
}

void medComposerNodeSegmentation::setProcess(dtkAbstractProcess *process)
{
    d->segmentationProcess = dynamic_cast<medAbstractProcess*>(process);
}

dtkAbstractProcess *medComposerNodeSegmentation::process(void) const
{
    return d->segmentationProcess;
}

bool medComposerNodeSegmentation::isInteractive() const
{
    return true;
}

void medComposerNodeSegmentation::run()
{
    qDebug() << "Starting Segmentation" ;//<<  d->segmentationProcess->description();

}
