/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposer.h>
#include <medComposerScene.h>
#include <medComposerStack.h>

#include <dtkComposer/dtkComposerCompass.h>
#include <dtkComposer/dtkComposerFactory.h>
#include <dtkComposer/dtkComposerGraph.h>
#include <dtkComposer/dtkComposerMachine.h>
#include <dtkComposer/dtkComposerPath.h>
#include <dtkComposer/dtkComposerScene.h>
#include <dtkComposer/dtkComposerStack.h>
#include <dtkComposer/dtkComposerView.h>

class medComposerPrivate
{
public:
    medComposerScene *scene;
    medComposerStack *stack;
};

medComposer::medComposer(QWidget *parent): dtkComposer(parent), d(new medComposerPrivate)
{
    d->scene = new medComposerScene;
    d->stack = new medComposerStack;

    this->view()->setScene(d->scene);

    d->scene->setFactory(this->factory());
    d->scene->setMachine(this->machine());
    d->scene->setStack(d->stack);
    d->scene->setGraph(this->graph());
    d->scene->setContext(this->view()->context());

    this->path()->setScene(d->scene);

    this->compass()->setScene(d->scene);
}

dtkComposerScene *medComposer::scene(void)
{
    return d->scene;
}

void medComposer::setFactory(dtkComposerFactory *factory)
{
    dtkComposer::setFactory(factory);

    d->scene->setFactory(factory);
}
