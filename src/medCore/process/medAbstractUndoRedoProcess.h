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
#include <medCoreExport.h>

class medAbstractUndoRedoProcess;


class MEDCORE_EXPORT medAbstractUndoRedoProcessWrapper : public QObject
{
    Q_OBJECT

public:
    medAbstractUndoRedoProcessWrapper(medAbstractUndoRedoProcess* parent);
    virtual ~medAbstractUndoRedoProcessWrapper();

public slots:
    virtual void undo();
    virtual void redo();
    virtual void reset();

signals:
    void processUndone();
    void processRedone();
    void undoRedoStackReset();

private:
    medAbstractUndoRedoProcess *wrappee;

};

class medUndoRedoToolBox;
class medAbstractUndoRedoProcessPrivate;

class MEDCORE_EXPORT medAbstractUndoRedoProcess
{  
public:
    medAbstractUndoRedoProcess();
    virtual ~medAbstractUndoRedoProcess();

public:
    virtual void undo() = 0;
    virtual void redo() = 0;
    virtual void reset() = 0;

public:
    virtual medUndoRedoToolBox* undoRedoToolBox() const;

    medAbstractUndoRedoProcessWrapper* wrapper() const;

private:
    medAbstractUndoRedoProcessPrivate* d;
};
