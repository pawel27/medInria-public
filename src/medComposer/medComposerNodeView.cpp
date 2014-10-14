/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medComposerNodeView.h"

#include <medAbstractImageData.h>
#include <medAbstractView.h>

#include <dtkComposer/dtkComposerTransmitterEmitter.h>
#include <dtkComposer/dtkComposerTransmitterReceiver.h>

#include <medViewFactory.h>

#include <medAbstractLayeredView.h>

// /////////////////////////////////////////////////////////////////
// medComposerNodeViewPrivate declaration
// /////////////////////////////////////////////////////////////////

class medComposerNodeViewPrivate
{
public:
    medAbstractLayeredView *view;

public:
    dtkComposerTransmitterReceiver<medAbstractImageData> receiver_image;
};

// /////////////////////////////////////////////////////////////////
// medComposerNodeView implementation
// /////////////////////////////////////////////////////////////////

medComposerNodeView::medComposerNodeView(void) : dtkComposerNodeLeafView(), d(new medComposerNodeViewPrivate)
{
    d->view = medViewFactory::instance()->createView<medAbstractLayeredView>("medVtkView");

    this->appendReceiver(&(d->receiver_image));
}

medComposerNodeView::~medComposerNodeView(void)
{
    delete d;

    d = NULL;
}

bool medComposerNodeView::isAbstractView(void) const
{
    return false;
}

QString medComposerNodeView::abstractViewType(void) const
{
    return "medAbstractView";
}

void medComposerNodeView::run(void)
{
    foreach(medAbstractImageData *image, d->receiver_image.allData())
    {
        d->view ->addLayer(image);
    }
}

QString medComposerNodeView::type(void)
{
    return "medAbstractView";
}

QString medComposerNodeView::titleHint(void)
{
    return "View";
}

QString medComposerNodeView::inputLabelHint(int port)
{
    switch(port) {
    case 0:
        return "image";
    default:
        return dtkComposerNodeLeaf::inputLabelHint(port);
    }
}

QString medComposerNodeView::outputLabelHint(int port)
{
    return dtkComposerNodeLeaf::outputLabelHint(port);
}

QGraphicsWidget *medComposerNodeView::widget(QGLContext *context)
{
    //TODO
    return NULL;
}
