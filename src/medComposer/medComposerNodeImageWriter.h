/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <dtkComposer/dtkComposerNodeLeafData.h>

#include <QtCore>

class medComposerNodeImageWriterPrivate;

class medComposerNodeImageWriter : public dtkComposerNodeLeafData
{
public:
     medComposerNodeImageWriter(void);
    ~medComposerNodeImageWriter(void);

public:
    bool isAbstractData(void) const;

    QString abstractDataType(void) const;

public:
    void run(void);

public:
    QString type(void);
    QString titleHint(void) ;

public:
    QString  inputLabelHint(int port);
    QString outputLabelHint(int port);

private:
    medComposerNodeImageWriterPrivate *d;
};
