/*=========================================================================
 
 medInria
 
 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.
 
 =========================================================================*/

#include "medAbstractProcess.h"
#include "medProcessFactory.h"

class medProcessFactoryPrivate
{
public:
    typedef QHash<QString,
    medProcessDetails*> medProcessCreatorHash;
    
    medProcessCreatorHash creators;
    
};

medProcessFactory *medProcessFactory::instance(void)
{
    if(!s_instance)
        s_instance = new medProcessFactory;
    
    return s_instance;
}


bool medProcessFactory::registerProcess(QString identifier,
                                        QString name,
                                        QString description,
                                        QStringList categories,
                                        medProcessCreator creator)
{
    if(!d->creators.contains(identifier))
    {
        medProcessDetails* holder = new medProcessDetails(name,
                                                          description,
                                                          categories,
                                                          creator);
        d->creators.insert( identifier,
                           holder);
        return true;
    }
    return false;
}

QList<QString> medProcessFactory::processesFromCategory( const QString& category)const
{
    QList<QString> ids;
    typedef medProcessFactoryPrivate::medProcessCreatorHash::iterator creator_iterator;
    creator_iterator i = d->creators.begin();
    while (i != d->creators.end())
    {
        if (i.value()->categories.contains(category))
        {
            ids << i.key();
        }
        ++i;
    }
    return ids;
}

medAbstractProcess *medProcessFactory::createProcess(QString identifier,
                                                     QWidget *parent)
{
    if(!d->creators.contains(identifier))
        return NULL;
    
    medAbstractProcess *process = (d->creators[identifier])->creator(parent);
    
    return process;
}

medProcessDetails * medProcessFactory::processDetailsFromId(const QString &id) const
{
    return d->creators.value(id);
}


QHash<QString, medProcessDetails *> medProcessFactory::processDetailsFromCategory(const QString &cat) const
{
    QHash<QString, medProcessDetails *> pcsDetails;
    typedef medProcessFactoryPrivate::medProcessCreatorHash::iterator creator_iterator;
    creator_iterator i = d->creators.begin();
    while (i != d->creators.end())
    {
        if (i.value()->categories.contains(cat))
        {
            pcsDetails.insert(i.key(),i.value());
        }
        ++i;
    }
    return pcsDetails;
}

medProcessFactory::medProcessFactory(void) : dtkAbstractFactory(), d(new medProcessFactoryPrivate)
{
    
}

medProcessFactory::~medProcessFactory(void)
{
    //delete details.
    foreach (medProcessDetails * detail, d->creators.values())
    {
        delete detail;
        detail = NULL;
    }
    
    delete d;
    
    d = NULL;
}

medProcessFactory *medProcessFactory::s_instance = NULL;



