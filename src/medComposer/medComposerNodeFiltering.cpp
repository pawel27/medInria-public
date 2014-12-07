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
#include <medToolBox.h>


// /////////////////////////////////////////////////////////////////
// medComposerNodeFilteringPrivate interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeFilteringPrivate
{
public:
    medAbstractFilteringProcess *filtering;

public:
    dtkComposerTransmitterReceiver<medAbstractData> receiver_image;

public:
    dtkComposerTransmitterEmitter<medAbstractData> emitter_image;

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

bool medComposerNodeFiltering::isInteractive() const
{
    //TODO: temporary, for test only
    return false;
}

void medComposerNodeFiltering::setProcess(dtkAbstractProcess *process)
{
    qDebug()<<"node filtering : set Process";
    d->filtering = dynamic_cast<medAbstractFilteringProcess*>(process);

    if(d->filtering)
    {
        medComposerScene *scene = dynamic_cast<medComposerScene *>(d->graphicsWidget->scene());
        dtkComposerSceneNodeLeaf *sceneNode = dynamic_cast<dtkComposerSceneNodeLeaf *>(d->graphicsWidget->parentItem());

        foreach(dtkComposerTransmitter* receiver, this->receivers())
          this->removeReceiver(receiver);

        foreach(dtkComposerScenePort *port, sceneNode->inputPorts())
        {
          sceneNode->removeInputPort(port);
          delete port;
        }

        foreach(medProcessIOPort *port, d->filtering->inputs())
        {
            this->appendReceiver(port->toTransmitter());

            dtkComposerScenePort *scenePort = new dtkComposerScenePort(dtkComposerScenePort::Input, sceneNode);
            sceneNode->addInputPort(scenePort);
            scenePort->setLabel(port->name());
        }

        d->graphicsWidget->setWidget(d->filtering->toolbox());

        sceneNode->layout();
        d->graphicsWidget->adjustSize();
        scene->update();

        sceneNode->show();
    }
}

dtkAbstractProcess *medComposerNodeFiltering::process(void) const
{
    return d->filtering;
}

void medComposerNodeFiltering::run()
{
    qDebug() << "Starting " <<  d->filtering->description();


    for(int i=0; i<this->receivers().count(); i++)
    {
        //TODO: find a way to link the port and the receiver
        dtkComposerTransmitter* receiver = this->receivers()[i];
        medProcessIOPort *port = d->filtering->inputs()[i];

        port->updateFromTransmitter(receiver);
    }

//    if (!d->receiver_image.isEmpty()) {

//        if (!d->filtering){
//            dtkWarn() << Q_FUNC_INFO << "No process instantiated, abort:" << this->currentImplementation();
//            d->emitter_image.clearData();
//            return;
//        }

//        medAbstractImageData *image = qobject_cast<medAbstractImageData *>(d->receiver_image.data());

//        if (!image ) {
//            qDebug() << Q_FUNC_INFO << "Input image is not allocated for" << d->filtering->description();;
//            return;
//        }

//        d->filtering->setInput<medAbstractImageData>(image,0);

        d->index = d->filtering->run();

//        d->emitter_image.setData(qobject_cast<medAbstractImageData *>(d->filtering->output<medAbstractImageData>(0)));

//    } else {

//        dtkWarn() << Q_FUNC_INFO << "The input are not all set. Nothing is done.";
//        d->emitter_image.clearData();
//    }
}

QGraphicsWidget *medComposerNodeFiltering::widget(QGLContext *context)
{
    d->graphicsWidget = new QGraphicsProxyWidget;

    d->graphicsWidget->setWidget(new QLabel("Choose Implementation"));

    return d->graphicsWidget;
}
