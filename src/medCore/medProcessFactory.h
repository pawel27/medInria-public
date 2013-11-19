/*=========================================================================

 medInria

 Copyright (c) INRIA 2013. All rights reserved.
 See LICENSE.txt for details.
 
  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#pragma once

#include "medCoreExport.h"

#include <dtkCore/dtkAbstractFactory.h>

#include <QtCore>

class medAbstractProcess;
class medProcessFactoryPrivate;
class medProcessFactory;
struct medProcessDetails;


class MEDCORE_EXPORT medProcessFactory : public dtkAbstractFactory
{
    Q_OBJECT

public:
    typedef medAbstractProcess *(*medProcessCreator)(QWidget *parent);

public:
    static medProcessFactory *instance();

    template <typename processType>
    bool registerProcess(QString identifier,
                         QString name,
                         QString description,
                         QStringList categories){
        //we must keep the templated part in the .h file for library users
        medProcessCreator creator = create<processType>;
        return registerProcess(identifier,name,description,categories,creator);
    }

    bool registerProcess(QString identifier,
                         QString name,
                         QString description,
                         QStringList categories,
                         medProcessCreator creator);

    QList<QString> processesFromCategory(const QString& category) const;

    medProcessDetails* processDetailsFromId ( const QString& id )const;

    QHash<QString, medProcessDetails*> processDetailsFromCategory ( const QString& id )const;



public slots:
    medAbstractProcess *createProcess(QString identifier, QWidget *parent=0);


protected:
     medProcessFactory();
    ~medProcessFactory();

private:
    static medProcessFactory *s_instance; /** Singleton holder.*/

    template < typename T >
    static medAbstractProcess* create ( QWidget* parent ) {
    return ( new T(parent) );
    }
private:
    medProcessFactoryPrivate *d;
};

struct MEDCORE_EXPORT medProcessDetails{
    QString name; /** Readable name*/
    QString description; /** (tooltip) short description of the process */
    QStringList categories; /** List of categories the process falls in*/
    medProcessFactory::medProcessCreator creator; /** function pointer allocating memory for the process*/
    medProcessDetails(QString name,QString description, QStringList categories,
                     medProcessFactory::medProcessCreator creator):
        name(name),description(description),categories(categories),
        creator(creator){}
};


