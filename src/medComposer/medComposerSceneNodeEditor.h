/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkComposer/dtkComposerSceneNodeEditor>

class medAbstractDataSource;
class medComposerSceneNodeEditorPrivate;

class medComposerSceneNodeEditor : public dtkComposerSceneNodeEditor
{
public:
    medComposerSceneNodeEditor(QWidget *parent = 0);

    void setDataSourceWidget( QWidget* dataSourceWidget );

public slots:
    virtual void setNode(dtkComposerSceneNode *node);

private:
    medComposerSceneNodeEditorPrivate *d;

};
