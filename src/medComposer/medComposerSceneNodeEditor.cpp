/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerSceneNodeEditor.h>

#include <medComposerNodeImageReader.h>
#include <medAbstractDataSource.h>
#include <dtkComposer/dtkComposerSceneNode.h>

class medComposerSceneNodeEditorPrivate
{
public:
    QWidget *dataSource;
};

medComposerSceneNodeEditor::medComposerSceneNodeEditor(QWidget *parent) : dtkComposerSceneNodeEditor(parent),
    d(new medComposerSceneNodeEditorPrivate)
{

}

void medComposerSceneNodeEditor::setDataSourceWidget( QWidget* dataSourceWidget )
{
    d->dataSource = dataSourceWidget;

    this->layout()->addWidget(d->dataSource);
    this->layout()->addWidget(new QLabel("TEST"));
}

void medComposerSceneNodeEditor::setNode(dtkComposerSceneNode *node)
{
    dtkComposerSceneNodeEditor::setNode(node);


    this->layout()->addWidget(d->dataSource);
    this->layout()->addWidget(new QLabel("TEST"));

    if (dynamic_cast<medComposerNodeImageReader *>(node->wrapee()))
    {
        qDebug() << "medComposerNodeImage";
        d->dataSource->setVisible(true);

    }
}
