/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <dtkConfig.h>

#include "dtkComposer/dtkComposerFactory.h"
#include "dtkComposer/dtkComposerGraph.h"
#include "dtkComposer/dtkComposerGraphNode.h"
#include "dtkComposer/dtkComposerNode.h"
#include "dtkComposer/dtkComposerNodeComposite.h"
#include "dtkComposer/dtkComposerNodeControlCase.h"
#include "dtkComposer/dtkComposerNodeLeaf.h"
#if defined(DTK_BUILD_DISTRIBUTED)
#include "dtkComposer/dtkComposerNodeRemote.h"
#endif
#include "dtkComposer/dtkComposerReader.h"
#include "dtkComposer/dtkComposerScene.h"
#include "dtkComposer/dtkComposerScene_p.h"
#include "dtkComposer/dtkComposerSceneEdge.h"
#include "dtkComposer/dtkComposerSceneNode.h"
#include "dtkComposer/dtkComposerSceneNodeControl.h"
#include "dtkComposer/dtkComposerSceneNodeComposite.h"
#include "dtkComposer/dtkComposerSceneNodeLeaf.h"
#include "dtkComposer/dtkComposerSceneNote.h"
#include "dtkComposer/dtkComposerScenePort.h"
#include "dtkComposer/dtkComposerStackCommand.h"
#include "dtkComposer/dtkComposerStackUtils.h"
#include "dtkComposer/dtkComposerTransmitter.h"
#include "dtkComposer/dtkComposerTransmitterProxy.h"
#include "dtkComposer/dtkComposerTransmitterVariant.h"
#include "dtkComposer/dtkComposerWriter.h"

#include <dtkLog/dtkLog.h>

#include <medComposerStackCommand.h>
#include <medComposerNodeImage.h>
#include <medComposerNodeSegmentation.h>

// /////////////////////////////////////////////////////////////////
// Base Command
// /////////////////////////////////////////////////////////////////

class medComposerStackCommandPrivate
{
public:
    dtkComposerFactory *factory;
    dtkComposerScene *scene;
    dtkComposerGraph *graph;
};

medComposerStackCommand::medComposerStackCommand(medComposerStackCommand *parent) : QUndoCommand(parent), d(new medComposerStackCommandPrivate)
{
    d->factory = NULL;
    d->scene = NULL;
    d->graph = NULL;
}

medComposerStackCommand::~medComposerStackCommand(void)
{
    delete d;

    d = NULL;
}

void medComposerStackCommand::setFactory(dtkComposerFactory *factory)
{
    d->factory = factory;
}

void medComposerStackCommand::setScene(dtkComposerScene *scene)
{
    d->scene = scene;
}

void medComposerStackCommand::setGraph(dtkComposerGraph *graph)
{
    d->graph = graph;
}

// /////////////////////////////////////////////////////////////////
// Create Node Command
// /////////////////////////////////////////////////////////////////

class medComposerStackCommandCreateNodePrivate
{
public:
    QString name;
    QString type;
    QPointF position;
    const QMimeData *mimeData;

public:
    dtkComposerSceneNode *node;

public:
    dtkComposerSceneNodeComposite *parent;
};

medComposerStackCommandCreateNode::medComposerStackCommandCreateNode(medComposerStackCommand *parent) : medComposerStackCommand(parent), e(new medComposerStackCommandCreateNodePrivate)
{
    e->node = NULL;
    e->parent = NULL;
}

medComposerStackCommandCreateNode::~medComposerStackCommandCreateNode(void)
{
    delete e->node;
    delete e;

    e = NULL;
}

void medComposerStackCommandCreateNode::setParent(dtkComposerSceneNodeComposite *parent)
{
    e->parent = parent;
}

void medComposerStackCommandCreateNode::setPosition(const QPointF& position)
{
    e->position = position;
}

void medComposerStackCommandCreateNode::setName(const QString& name)
{
    e->name = name;

    this->setText(QString("Create node %1").arg(e->name));
}

void medComposerStackCommandCreateNode::setType(const QString& type)
{
    e->type = type;
}

void medComposerStackCommandCreateNode::setMimeData(const QMimeData* data)
{
    e->mimeData = data;
}

void medComposerStackCommandCreateNode::redo(void)
{
    if(!d->factory)
        return;

    if(!d->scene)
        return;

    if(!d->graph)
        return;

    if(!e->parent)
        return;

    if(e->type.isEmpty())
        return;

    if(!e->node) {

        dtkComposerNode *node = d->factory->create(e->type);

        if(dynamic_cast<dtkComposerNodeLeaf *>(node))
            e->node = new dtkComposerSceneNodeLeaf;
        else if(dynamic_cast<dtkComposerNodeComposite *>(node) )
            e->node = new dtkComposerSceneNodeComposite;
        else
            e->node = new dtkComposerSceneNodeControl;

        if(dynamic_cast<medComposerNodeImage *>(node))
        {
            medComposerNodeImage *n = dynamic_cast<medComposerNodeImage *>(node);
            n->setMimeData(e->mimeData);
        }

        e->node->wrap(node);
        e->node->setParent(e->parent);    
    }

    e->node->setPos(e->position);

    e->parent->addNode(e->node);
    e->parent->layout();

    d->graph->addNode(e->node);
    d->graph->layout();

// -- ??
    if (e->parent->visible()) {
        d->scene->addItem(e->node);
        e->node->layout();
    }

    d->scene->modify(true);
// --
}

void medComposerStackCommandCreateNode::undo(void)
{
    if(!d->scene)
        return;

    if(!d->graph)
        return;

    if(!e->node)
        return;

    if(!e->parent)
        return;

    if(!e->node)
        return;

    e->position = e->node->scenePos();


    d->graph->removeNode(e->node);
    d->graph->layout();

    e->parent->removeNode(e->node);
    e->parent->layout();
// -- ??
    if (e->parent->visible())
        d->scene->removeItem(e->node);

    d->scene->modify(true);
// --
}
