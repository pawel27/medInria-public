/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QDialog>

class medViewContainerSplitter;
class medComposerInteractiveDialogPrivate;

class medComposerInteractiveDialog : public QDialog
{
public:
    medComposerInteractiveDialog(QWidget *parent = 0);
    ~medComposerInteractiveDialog();

public:
    void setContainerSplitter(medViewContainerSplitter*);

private:
    medComposerInteractiveDialogPrivate *d;
};
