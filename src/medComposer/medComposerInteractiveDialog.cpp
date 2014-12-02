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

    QPropertyAnimation *animation;

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

    QPushButton *nextButton = new QPushButton("Next");
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(nextButton, 0, Qt::AlignRight);

    layout->addLayout(buttonLayout);

    connect(nextButton, SIGNAL(clicked()), this, SLOT(accept()));

    QSize mainWindowSize = QApplication::activeWindow()->size();
    QSize dialogSize(mainWindowSize.width()-100, mainWindowSize.height()-100);

    d->animation = new QPropertyAnimation(this, "geometry");
    d->animation->setDuration(400);
    d->animation->setStartValue(QRect(25, 25, 0, 0));
    d->animation->setEndValue(QRect(25, 25, dialogSize.width() , dialogSize.height()));

    connect(d->animation, SIGNAL(finished()), this, SLOT(showWorkspace()));

}

medComposerInteractiveDialog::~medComposerInteractiveDialog()
{

}

void medComposerInteractiveDialog::setContainerSplitter(medViewContainerSplitter* splitter)
{
    if(d->workspace)
      d->workspace->tabbedViewContainers()->setSplitter(0, splitter);
}

void medComposerInteractiveDialog::showEvent ( QShowEvent * event )
{
    d->splitter->hide();

    d->animation->start();
}

void medComposerInteractiveDialog::showWorkspace()
{
    d->splitter->show();

}

