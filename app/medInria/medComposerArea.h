/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <QtCore>
#include <QtGui>

#include <dtkComposer/dtkComposerWriter.h>

class medComposerArea : public QFrame
{
    Q_OBJECT
public:
    medComposerArea(QWidget *parent = 0);
    ~medComposerArea(void);
    void setup(QStatusBar *status);
    void setdw(QStatusBar *status);
public slots:
    bool compositionOpen(void);
    bool compositionOpen(const QString& file);
    bool compositionSave(void);
    bool compositionSaveAs(void);
    bool compositionSaveAs(const QString& file, dtkComposerWriter::Type type = dtkComposerWriter::Ascii);
    bool compositionInsert(void);
    bool compositionInsert(const QString& file);
private:
    class medComposerAreaPrivate *d;
};
