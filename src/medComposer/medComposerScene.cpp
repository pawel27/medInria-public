/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include "medComposerScene.h"

#include <medDataIndex.h>
#include <medComposerStack.h>
#include <medComposerStackCommand.h>

#include <dtkComposer/dtkComposerStackCommand.h>
#include <dtkComposer/dtkComposerFactory.h>
#include <dtkComposer/dtkComposerGraph.h>

class medComposerScenePrivate
{
public:
    medComposerStack *stack;
    dtkComposerFactory *factory;
    dtkComposerGraph *graph;
};

medComposerScene::medComposerScene(QObject *parent): dtkComposerScene(parent), d(new medComposerScenePrivate)
{
    d->stack = NULL;
    d->factory = NULL;
    d->graph = NULL;
}

void medComposerScene::setStack(medComposerStack *stack)
{
    dtkComposerScene::setStack(stack);
    d->stack = stack;
}

void medComposerScene::setFactory(dtkComposerFactory *factory)
{
    dtkComposerScene::setFactory(factory);
    d->factory = factory;
}

void medComposerScene::setGraph(dtkComposerGraph *graph)
{
    dtkComposerScene::setGraph(graph);
    d->graph = graph;
}

void medComposerScene::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    medDataIndex dataIndex;

    if(event->mimeData()->hasFormat("med/index") )
    {
        dataIndex = medDataIndex::readMimeData(event->mimeData());
    }

    if (event->mimeData()->hasUrls()) {

        QString name = event->mimeData()->text();
        QUrl url = event->mimeData()->urls().first();

        if (url.scheme() == "note") {

           /* dtkComposerStackCommandCreateNote *command = new dtkComposerStackCommandCreateNote;
            command->setScene(this);
            command->setParent(this->parentAt(event->scenePos()));
            command->setPosition(event->scenePos());

            d->stack->push(command);

            event->acceptProposedAction();*/
            dtkComposerScene::dropEvent(event);

            return;
        }

        if (url.scheme() == "node") {

            medComposerStackCommandCreateNode *command = new medComposerStackCommandCreateNode;
            command->setFactory(d->factory);
            command->setScene(this);
            command->setGraph(d->graph);
            command->setParent(this->parentAt(event->scenePos()));
            command->setPosition(event->scenePos());
            command->setType(url.path());
            command->setName(name);
            command->setMimeData(event->mimeData());

            d->stack->push(command);

            event->acceptProposedAction();

            return;
        }

    }

    QGraphicsScene::dropEvent(event);
}
