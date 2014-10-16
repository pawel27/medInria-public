/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerGraph.h>


#include <dtkComposer/dtkComposerNode.h>
#include <dtkComposer/dtkComposerSceneNode.h>
#include <dtkComposer/dtkComposerGraphNode.h>

#include <medComposerNodeSegmentation.h>

medComposerGraph::medComposerGraph(): dtkComposerGraph()
{

}

void medComposerGraph::addNode(dtkComposerSceneNode *node)
{
    dtkComposerGraph::addNode(node);

    if(dynamic_cast<medComposerNodeSegmentation *>(node->wrapee()))
    {
        medComposerNodeSegmentation *n = dynamic_cast<medComposerNodeSegmentation *>(node->wrapee());
        if(n->isInteractive())
            this->graphNode(node)->setBreakPoint();
    }
}
