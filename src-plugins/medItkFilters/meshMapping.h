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
#include <medAbstractFilteringProcess.h>
#include <medAbstractData.h>

#include <medItkFiltersPluginExport.h>

#include "itkImage.h"


class meshMappingPrivate;

typedef itk::Image<unsigned char, 3>  MaskType;

class medItkFiltersPlugin_EXPORT meshMapping : public medAbstractFilteringProcess
{
    Q_OBJECT
    
public:
    meshMapping(medItkFiltersProcessBase *parent = NULL);
    virtual ~meshMapping();
        
    static bool registered();
	
	QList<medAbstractParameter*> parameters();
    
public slots:
    
    //! Method to actually start the filter
    int update();
    
    
private:
    meshMappingPrivate *d;
};

dtkAbstractProcess *createMeshMapping();


