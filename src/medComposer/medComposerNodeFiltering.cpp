/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medComposerNodeFiltering.h"

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>
#include <dtkComposer/dtkComposerSceneNodeLeaf.h>
#include <dtkComposer/dtkComposerScenePort.h>

#include <medAbstractFilteringProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

#include <medAbstractImageData.h>
#include <medDataManager.h>
#include <medMetaDataKeys.h>
#include <medComposerScene.h>


// /////////////////////////////////////////////////////////////////
// medComposerNodeFilteringPrivate interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeFilteringPrivate
{
public:
    medAbstractFilteringProcess *filtering;

public:
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;

public:
    dtkComposerTransmitterEmitter<medAbstractImageData> emitter_image;

public:
    qlonglong index;
    qreal value;
    QGraphicsProxyWidget *graphicsWidget;
};

// /////////////////////////////////////////////////////////////////
// medComposerNodeFiltering implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeFiltering::medComposerNodeFiltering(void) : dtkComposerNodeLeafProcess(), d(new medComposerNodeFilteringPrivate)
{
    this->appendReceiver(&(d->receiver_image));

    this->appendEmitter(&(d->emitter_image));

    d->filtering = NULL;
}

medComposerNodeFiltering::~medComposerNodeFiltering(void)
{
    this->clearProcess();

    delete d;
    d = NULL;
}

QString medComposerNodeFiltering::type(void) 
{
    return "medComposerNodeFiltering";
}

QString medComposerNodeFiltering::titleHint(void)
{
    return "Filtering";
}

QString medComposerNodeFiltering::inputLabelHint(int port) 
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeFiltering::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}

bool medComposerNodeFiltering::isAbstractProcess(void) const
{
    return true;
}

QString medComposerNodeFiltering::abstractProcessType(void) const
{
    return "medAbstractFilteringProcess";
}

void medComposerNodeFiltering::setProcess(dtkAbstractProcess *process)
{
    d->filtering = dynamic_cast<medAbstractFilteringProcess*>(process);

    // Short test to add an additional port and the toolbox widget
    if(d->filtering->description() == "animaSymmetryPlane")
    {
        dtkComposerTransmitterReceiver<medAbstractImageData> *newReceiver =
                new dtkComposerTransmitterReceiver<medAbstractImageData>(this);

        this->appendReceiver(newReceiver);

        medComposerScene *scene = dynamic_cast<medComposerScene *>(d->graphicsWidget->scene());
        dtkComposerSceneNodeLeaf *sceneNode = dynamic_cast<dtkComposerSceneNodeLeaf *>(d->graphicsWidget->parentItem());
        if(sceneNode)
        {
            dtkComposerScenePort *port = new dtkComposerScenePort(dtkComposerScenePort::Input, sceneNode);
            sceneNode->addInputPort(port);
            port->setLabel("Additional image");
        }

        d->graphicsWidget->setWidget(new QLineEdit("TODO: add process toolbox here"));

        sceneNode->layout();
        d->graphicsWidget->adjustSize();
        scene->update();
    }
}

dtkAbstractProcess *medComposerNodeFiltering::process(void) const
{
    return d->filtering;
}

void medComposerNodeFiltering::run()
{
    qDebug() << "Starting " <<  d->filtering->description();

    if (!d->receiver_image.isEmpty()) {

        if (!d->filtering){
            dtkWarn() << Q_FUNC_INFO << "No process instantiated, abort:" << this->currentImplementation();
            d->emitter_image.clearData();
            return;
        }

        medAbstractImageData *image = qobject_cast<medAbstractImageData *>(d->receiver_image.data());

        if (!image ) {
            qDebug() << Q_FUNC_INFO << "Input image is not allocated for" << d->filtering->description();;
            return;
        }

        d->filtering->setInputImage(image);

        d->index = d->filtering->run();

        d->emitter_image.setData(qobject_cast<medAbstractImageData *>(d->filtering->output()));

        //copied from filtering workspace
        //TODO: temporary

        if (! d->filtering->output()->hasMetaData(medMetaDataKeys::SeriesDescription.key()))
          {
            QString newSeriesDescription = image->metadata ( medMetaDataKeys::SeriesDescription.key() );
            newSeriesDescription += " filtered";
            d->filtering->output()->addMetaData ( medMetaDataKeys::SeriesDescription.key(), newSeriesDescription );
          }

        foreach ( QString metaData, image->metaDataList() )
          if (!d->filtering->output()->hasMetaData(metaData))
            d->filtering->output()->addMetaData ( metaData, image->metaDataValues ( metaData ) );

        foreach ( QString property, image->propertyList() )
          d->filtering->output()->addProperty ( property,image->propertyValues ( property ) );

        QString generatedID = QUuid::createUuid().toString().replace("{","").replace("}","");
        d->filtering->output()->setMetaData ( medMetaDataKeys::SeriesID.key(), generatedID );

        medDataManager::instance()->importData(d->filtering->output());

    } else {

        dtkWarn() << Q_FUNC_INFO << "The input are not all set. Nothing is done.";
        d->emitter_image.clearData();
    }
}

QGraphicsWidget *medComposerNodeFiltering::widget(QGLContext *context)
{
    d->graphicsWidget = new QGraphicsProxyWidget;

    d->graphicsWidget->setWidget(new QLabel("Choose Implementation"));

    return d->graphicsWidget;
}
