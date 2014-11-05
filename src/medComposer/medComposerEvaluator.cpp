/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerEvaluator.h>

#include <dtkComposer/dtkComposerGraph.h>
#include <dtkComposer/dtkComposerGraphNode.h>
#include <dtkComposer/dtkComposerNodeLeafProcess.h>
#include <dtkComposer/dtkComposerTransmitter.h>

#include <medComposerInteractiveDialog.h>
#include <medAbstractProcess.h>
#include <medAbstractImageData.h>

class medComposerEvaluatorPrivate
{
public:
    dtkComposerGraph *graph;
    dtkComposerNode *breakPointNode;

};

medComposerEvaluator::medComposerEvaluator(QObject *parent) : dtkComposerEvaluator(parent), d(new medComposerEvaluatorPrivate)
{
    d->graph = NULL;

    connect(this, SIGNAL(breakpointReached()), this, SLOT(showInteractiveDialog()));
}

medComposerEvaluator::~medComposerEvaluator()
{

}

void medComposerEvaluator::setGraph(dtkComposerGraph *graph)
{
    dtkComposerEvaluator::setGraph(graph);
    d->graph = graph;
}

void  medComposerEvaluator::run(bool run_concurrent)
{
    dtkComposerEvaluator::run(run_concurrent);

    foreach(dtkComposerGraphNode* node, d->graph->nodes())
    {
        //TODO: experimental....need to be re-thought
        if(node->status() == dtkComposerGraphNode::Break)
        {
            d->breakPointNode = node->wrapee();
            emit breakpointReached();
        }
    }
}

void medComposerEvaluator::showInteractiveDialog()
{
    medComposerInteractiveDialog msgBox;

    dtkComposerNodeLeafProcess *processNode = dynamic_cast<dtkComposerNodeLeafProcess *>(d->breakPointNode);
    if(processNode)
    {
        medAbstractProcess *process = dynamic_cast<medAbstractProcess*>(processNode->process());
        msgBox.setContainerSplitter(process->viewContainerSplitter());

        int i = 0;
        foreach(dtkComposerTransmitter *receiver, processNode->receivers())
        {
            // TODO GPR-RDE: find a better way to link transmitters and process ports
            medAbstractData *image = qobject_cast<medAbstractData *>(receiver->object());
            if (image )
                process->setInput<medAbstractData>(image,i);

            i++;
        }
    }
    msgBox.exec();
}
