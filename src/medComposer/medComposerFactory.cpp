/*=========================================================================

 medInria

 Copyright (c) INRIA 2013 - 2014. All rights reserved.
 See LICENSE.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.

=========================================================================*/

#include <medComposerFactory.h>
#include <medComposerNodeFiltering.h>
#include <medComposerNodeSegmentation.h>
#include <medComposerNodeRegistration.h>
#include <medComposerNodeImageReader.h>
#include <medComposerNodeImageWriter.h>
#include <medComposerNodeView.h>
#include <medComposerNodeImage.h>
#include <medComposer.h>

#include <dtkCore/dtkAbstractView.h>
#include <dtkCore/dtkAbstractViewFactory.h>

#include <dtkLog/dtkLog.h>

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

class medComposerFactoryPrivate
{
public:
    void initialize(void);

public:
    QList<QString> nodes;
    QHash<QString, QString> descriptions;
    QHash<QString, QStringList> tags;
    QHash<QString, QString> types;
};

void medComposerFactoryPrivate::initialize(void)
{

}

// /////////////////////////////////////////////////////////////////
//
// /////////////////////////////////////////////////////////////////

medComposerFactory::medComposerFactory(void) : dtkComposerFactory(), d(new medComposerFactoryPrivate)
{
    d->initialize();

    // Initialize attributes on super class

    d->nodes = QList<QString>(dtkComposerFactory::nodes());
    d->descriptions = QHash<QString, QString>(dtkComposerFactory::descriptions());
    d->tags = QHash<QString, QStringList>(dtkComposerFactory::tags());
    d->types = QHash<QString, QString>(dtkComposerFactory::types());

    // Extend attributes

    d->nodes << "Filtering";
    d->descriptions["Filtering"] = "<p>Filtering process that filters according the plugin one chooses.</p>";
    d->tags["Filtering"] = QStringList() << "filtering";
    d->types["Filtering"] = "medComposerNodeFiltering";

    d->nodes << "Registration";
    d->descriptions["Registration"] = "<p>Registration process.</p>";
    d->tags["Registration"] = QStringList() << "Registration";
    d->types["Registration"] = "medComposerNodeRegistration";

    d->nodes << "Segmentation";
    d->descriptions["Segmentation"] = "<p>Segmentation process.</p>";
    d->tags["Segmentation"] = QStringList() << "Segmentation";
    d->types["Segmentation"] = "medComposerNodeSegmentation";

    d->nodes << "View";
    d->descriptions["View"] = "<p>medAbstractView</p>";
    d->tags["View"] = QStringList() << "view";
    d->types["View"] = "medAbstractView";

    d->nodes << "Reader";
    d->descriptions["Reader"] = "<p>medAbstractImage.</p>";
    d->tags["Reader"] = QStringList() << "image" << "reader";
    d->types["Reader"] = "medComposerNodeImageReader";

    d->nodes << "Image";
    d->descriptions["Image"] = "<p>medAbstractImage.</p>";
    d->tags["Image"] = QStringList() << "image";
    d->types["Image"] = "medComposerNodeImage";

    d->nodes << "Writer";
    d->descriptions["Writer"] = "<p>medAbstractImage.</p>";
    d->tags["Writer"] = QStringList() << "image" << "writer";
    d->types["Writer"] = "medDataImageWriter";
}

medComposerFactory::~medComposerFactory(void)
{
    delete d;

    d = NULL;
}

dtkComposerNode *medComposerFactory::create(const QString& type)
{
    if(type == "medComposerNodeFiltering")
        return new medComposerNodeFiltering;

    if(type == "medComposerNodeSegmentation")
        return new medComposerNodeSegmentation;

    if(type == "medComposerNodeRegistration")
        return new medComposerNodeRegistration;

    if(type == "medAbstractView")
        return new medComposerNodeView;

    if(type == "medComposerNodeImage")
        return new medComposerNodeImage;

    if(type == "medComposerNodeImageReader")
        return new medComposerNodeImageReader;

    if(type == "medDataImageWriter")
        return new medComposerNodeImageWriter;

    return dtkComposerFactory::create(type);
}

QList<QString> medComposerFactory::nodes(void)
{
    return d->nodes;
}

QHash<QString, QString> medComposerFactory::descriptions(void)
{
    return d->descriptions;
}

QHash<QString, QStringList> medComposerFactory::tags(void)
{
    return d->tags;
}

QHash<QString, QString> medComposerFactory::types(void)
{
    return d->types;
}
