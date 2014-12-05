/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include <medItkFiltersProcessBase.h>
#include <medAbstractData.h>

#include <medItkFiltersPluginExport.h>

class medMeshToolsPrivate;

class medItkFiltersPlugin_EXPORT medMeshTools : public medAbstractFilteringProcess
{
    Q_OBJECT
    
public:
    medMeshTools(medItkFiltersProcessBase *parent = NULL);
    virtual ~medMeshTools();
    
    static bool registered();
    
public slots:
    
    //! Input data to the plugin is set through here
    void setInput(medAbstractData *data, int channel = 0);
    
    //! Parameters are set through here, channel allows to handle multiple parameters
    void setParameter(double data, int channel);
    
    //! Method to actually start the filter
    int update();
	
	QList<medAbstractParameter*> parameters();
    
    ////! The output will be available through here
    //medAbstractData *output();

    int getNumberOfTriangles();
    
private:
    medMeshToolsPrivate *d;
};

dtkAbstractProcess *createMedMeshTools();


