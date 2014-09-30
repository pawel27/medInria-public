/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#pragma once

#include <dtkComposer/dtkComposerNodeLeafView.h>

class medComposerNodeViewPrivate;

class medComposerNodeView : public dtkComposerNodeLeafView
{
public:
     medComposerNodeView(void);
    ~medComposerNodeView(void);

public:
    bool isAbstractView(void) const;

    QString abstractViewType(void) const;

public:
    void run(void);

public:
    QString type(void);
    QString titleHint(void);

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

public:
    QGraphicsWidget *widget(QGLContext *context);

 private:
    medComposerNodeViewPrivate *d;
};
