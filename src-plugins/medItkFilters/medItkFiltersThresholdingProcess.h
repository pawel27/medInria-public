/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medItkFiltersProcessBase.h>
#include <medAbstractData.h>

#include <medItkFiltersPluginExport.h>

class medItkFiltersThresholdingProcessPrivate;
class medAbstractData;

class medItkFiltersPlugin_EXPORT medItkFiltersThresholdingProcess : public medItkFiltersProcessBase
{
    Q_OBJECT
    
public:
    medItkFiltersThresholdingProcess(medItkFiltersThresholdingProcess * parent = 0);
    virtual ~medItkFiltersThresholdingProcess(void);

    static bool registered ( void );
    
public slots:
    int update ( void );

public:
    QList<medAbstractParameter*> parameters();

private:
    medItkFiltersThresholdingProcessPrivate *d;
};

dtkAbstractProcess * createmedItkFiltersThresholdingProcess(void);


