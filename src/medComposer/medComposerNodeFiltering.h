/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/


#pragma once

#include <dtkComposer/dtkComposerNodeLeafProcess.h>

class medComposerNodeFilteringPrivate;

// /////////////////////////////////////////////////////////////////
// numComposerNodeMeshBuilder interface
// /////////////////////////////////////////////////////////////////

class medComposerNodeFiltering : public dtkComposerNodeLeafProcess
{
public:
     medComposerNodeFiltering(void);
    ~medComposerNodeFiltering(void);

public:
    QString type(void);
    QString titleHint(void);

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

public:
    bool isAbstractProcess(void) const;

    QString abstractProcessType(void) const;

public:
    void setProcess(dtkAbstractProcess *process);

    dtkAbstractProcess *process(void) const;

public:
    void run(void);

public:
    bool isInteractive() const;

public:
    QGraphicsWidget *widget(QGLContext *context);

private:
    medComposerNodeFilteringPrivate *d;
};
