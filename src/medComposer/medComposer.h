/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkComposer/dtkComposer.h>

class medComposerPrivate;
class dtkComposerFactory;

class medComposer : public dtkComposer
{
public:
    medComposer(QWidget* parent = 0);

    dtkComposerScene *scene(void);

    void setFactory(dtkComposerFactory *factory);

private:
    medComposerPrivate *d;
};


