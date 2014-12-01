/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medComposerNodeRegistration.h"

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>
#include <dtkComposer/dtkComposerSceneNodeLeaf.h>

#include <medComposerScene.h>

#include <medAbstractEstimateTransformationProcess.h>
#include <dtkCore/dtkAbstractProcessFactory.h>

#include <medAbstractImageData.h>
#include <medToolBox.h>

// /////////////////////////////////////////////////////////////////
// medComposerNodeRegistrationPrivate interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeRegistrationPrivate
{
public:
    medAbstractEstimateTransformationProcess *registrationProcess;

public:
    dtkComposerTransmitterReceiver<medAbstractImageData> fixedImage;
    dtkComposerTransmitterReceiver<medAbstractImageData> movingImage;

public:
    dtkComposerTransmitterEmitter<medAbstractImageData> outputImage;

public:
    qlonglong index;
    qreal value;

    QGraphicsProxyWidget *graphicsWidget;
};

// /////////////////////////////////////////////////////////////////
// medComposerNodeRegistration implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeRegistration::medComposerNodeRegistration(void) : dtkComposerNodeLeafProcess(), d(new medComposerNodeRegistrationPrivate)
{
    this->appendReceiver(&(d->fixedImage));
    this->appendReceiver(&(d->movingImage));

    this->appendEmitter(&(d->outputImage));

    d->registrationProcess = NULL;
}

medComposerNodeRegistration::~medComposerNodeRegistration(void)
{
    this->clearProcess();

    delete d;
    d = NULL;
}

QString medComposerNodeRegistration::type(void)
{
    return "medComposerNodeRegistration";
}

QString medComposerNodeRegistration::titleHint(void)
{
    return "Registration";
}

QString medComposerNodeRegistration::inputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "fixed image";
    case 1:
        return "moving image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeRegistration::outputLabelHint(int port)
{
    switch (port) {
    case 0:
        return "output image";
    default:
        return dtkComposerNodeLeaf::outputLabelHint(port);
    }
}

bool medComposerNodeRegistration::isAbstractProcess(void) const
{
    return true;
}

QString medComposerNodeRegistration::abstractProcessType(void) const
{
    return "medAbstractEstimateTransformationProcess";
}

void medComposerNodeRegistration::setProcess(dtkAbstractProcess *process)
{
    d->registrationProcess = dynamic_cast<medAbstractEstimateTransformationProcess*>(process);

    if(d->registrationProcess)
    {
        medComposerScene *scene = dynamic_cast<medComposerScene *>(d->graphicsWidget->scene());
        dtkComposerSceneNodeLeaf *sceneNode = dynamic_cast<dtkComposerSceneNodeLeaf *>(d->graphicsWidget->parentItem());

        d->graphicsWidget->setWidget(d->registrationProcess->toolbox());

        sceneNode->layout();
        d->graphicsWidget->adjustSize();
        scene->update();
    }
}

dtkAbstractProcess *medComposerNodeRegistration::process(void) const
{
    return d->registrationProcess;
}

void medComposerNodeRegistration::run()
{
    qDebug() << "Starting " <<  d->registrationProcess->description();

    if (!d->fixedImage.isEmpty()) {

        if (!d->registrationProcess){
            dtkWarn() << Q_FUNC_INFO << "No process instantiated, abort:" << this->currentImplementation();
            d->outputImage.clearData();
            return;
        }

        medAbstractImageData *fixedImage = qobject_cast<medAbstractImageData *>(d->fixedImage.data());

        if (!fixedImage) {
            dtkError() << Q_FUNC_INFO << "Fixed image is not allocated";
            return;
        }

        medAbstractImageData *movingImage = qobject_cast<medAbstractImageData *>(d->movingImage.data());

        if (!movingImage) {
            dtkError() << Q_FUNC_INFO << "Fixed image is not allocated";
            return;
        }

        d->registrationProcess->setFixedInput(fixedImage);
        d->registrationProcess->setMovingInput(movingImage);

        d->index = d->registrationProcess->run();

        d->outputImage.setData(qobject_cast<medAbstractImageData*>(d->registrationProcess->output<medAbstractImageData>(0)));

    } else {

        dtkWarn() << Q_FUNC_INFO << "The input are not all set. Nothing is done.";
        d->outputImage.clearData();
    }
}


QGraphicsWidget *medComposerNodeRegistration::widget(QGLContext *context)
{
    d->graphicsWidget = new QGraphicsProxyWidget;

    d->graphicsWidget->setWidget(new QLabel("Choose Implementation"));

    return d->graphicsWidget;
}
