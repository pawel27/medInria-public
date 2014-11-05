/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkComposer/dtkComposerEvaluator.h>

class medComposerEvaluatorPrivate;
class dtkComposerGraph;

class medComposerEvaluator : public dtkComposerEvaluator
{
    Q_OBJECT

public:
    medComposerEvaluator(QObject *parent = 0);
    ~medComposerEvaluator();

public:
    void setGraph(dtkComposerGraph *graph);

public slots:
    void run(bool run_concurrent = false);
    void showInteractiveDialog();

signals:
    void breakpointReached();

private:
    medComposerEvaluatorPrivate *d;
};
