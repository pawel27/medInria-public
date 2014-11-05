/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerInteractiveDialog.h>

#include <QtGui>

#include <medAbstractWorkspace.h>
#include <medWorkspaceFactory.h>
#include <medToolBoxContainer.h>
#include <medTabbedViewContainers.h>
#include <medToolBox.h>


class medComposerInteractiveDialogPrivate
{
public:
    QSplitter* splitter;
    medToolBoxContainer* toolBoxContainer;
    medAbstractWorkspace *workspace;
};

medComposerInteractiveDialog::medComposerInteractiveDialog(QWidget *parent):
    QDialog(parent), d(new medComposerInteractiveDialogPrivate)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    d->workspace = medWorkspaceFactory::instance()->createWorkspace("medVisualizationWorkspace", this);
    d->workspace->setToolBoxesVisibility(true);
    if(d->workspace)
    {
        d->splitter = new QSplitter(this);
        d->splitter->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
        d->splitter->setHandleWidth(2);

        d->toolBoxContainer = new medToolBoxContainer(this);
        d->toolBoxContainer->setVisible(true);

        d->splitter->addWidget(d->workspace->tabbedViewContainers());
        d->splitter->addWidget(d->toolBoxContainer);

        foreach(medToolBox *toolbox, d->workspace->toolBoxes())
        {
            d->toolBoxContainer->addToolBox(toolbox);
            toolbox->show();
        }

        d->toolBoxContainer->addToolBox(d->workspace->selectionToolBox());
        d->workspace->selectionToolBox()->show();

        layout->addWidget(d->splitter);
    }
}

medComposerInteractiveDialog::~medComposerInteractiveDialog()
{

}

void medComposerInteractiveDialog::setContainerSplitter(medViewContainerSplitter* splitter)
{
    if(d->workspace)
      d->workspace->tabbedViewContainers()->setSplitter(0, splitter);
}
