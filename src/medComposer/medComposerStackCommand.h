/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore>
#include <QtGui>

class dtkComposerFactory;
class dtkComposerGraph;
class dtkComposerScene;
class dtkComposerSceneEdge;
class dtkComposerSceneEdgeList;
class dtkComposerSceneNode;
class dtkComposerSceneNodeComposite;
class dtkComposerSceneNodeControl;
class dtkComposerSceneNodeList;
class dtkComposerSceneNote;
class dtkComposerSceneNoteList;
class dtkComposerScenePort;
class medComposerStackCommandPrivate;

// /////////////////////////////////////////////////////////////////
// Command Interface
// /////////////////////////////////////////////////////////////////

class medComposerStackCommand : public QUndoCommand
{
public:
             medComposerStackCommand(medComposerStackCommand *parent = 0);
    virtual ~medComposerStackCommand(void);

public:
    void setFactory(dtkComposerFactory *factory);
    void setScene(dtkComposerScene *scene);
    void setGraph(dtkComposerGraph *graph);

protected:
    medComposerStackCommandPrivate *d;
};

// /////////////////////////////////////////////////////////////////
// Create Node Command
// /////////////////////////////////////////////////////////////////

class medComposerStackCommandCreateNodePrivate;

class medComposerStackCommandCreateNode : public medComposerStackCommand
{
public:
     medComposerStackCommandCreateNode(medComposerStackCommand *parent = 0);
    ~medComposerStackCommandCreateNode(void);

public:
    void setParent(dtkComposerSceneNodeComposite *parent);
    void setPosition(const QPointF& position);
    void setName(const QString& name);
    void setType(const QString& type);
    void setMimeData(const QMimeData *data);

public:
    void redo(void);
    void undo(void);

protected:
    medComposerStackCommandCreateNodePrivate *e;
};
