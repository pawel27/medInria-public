// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#include "v3dView.h"
#include "v3dViewFuseInteractor.h"
#include "v3dViewMeshInteractor.h"
#include "v3dView4DInteractor.h"
#include "v3dViewPlugin.h"

#ifndef DISABLE_TTK_DEPENDENT_CODE
  #include "v3dViewFiberInteractor.h"
  #include "v3dViewTensorInteractor.h"
#endif

#include <dtkCore/dtkLog.h>

// /////////////////////////////////////////////////////////////////
// v3dViewPluginPrivate
// /////////////////////////////////////////////////////////////////

class v3dViewPluginPrivate 
{
public:
    // Class variables go here.
};

// /////////////////////////////////////////////////////////////////
// v3dViewPlugin
// /////////////////////////////////////////////////////////////////

v3dViewPlugin::v3dViewPlugin(QObject *parent) : dtkPlugin(parent), d(new v3dViewPluginPrivate)
{

}

v3dViewPlugin::~v3dViewPlugin(void)
{
    delete d;

    d = NULL;
}

bool v3dViewPlugin::initialize(void)
{
    if (!v3dView::registered()) dtkWarning() << "Unable to register v3dView type";

#ifndef DISABLE_TTK_DEPENDENT_CODE
    if (!v3dViewFiberInteractor::registered()) dtkWarning() << "Unable to register v3dViewFiberInteractor type";
#endif

    if (!v3dViewFuseInteractor::registered()) dtkWarning() << "Unable to register v3dViewFuseInteractor type";
    if (!v3dViewMeshInteractor::registered()) dtkWarning() << "Unable to register v3dViewMeshInteractor type";
    if (!v3dView4DInteractor::registered()) dtkWarning() << "Unable to register v3dView4DInteractor type";

#ifndef DISABLE_TTK_DEPENDENT_CODE
    if (!v3dViewTensorInteractor::registered()) dtkWarning() << "Unable to register v3dViewTensorInteractor type";
#endif
      
    return true;
}

bool v3dViewPlugin::uninitialize(void)
{
    return true;
}

QString v3dViewPlugin::name(void) const
{
    return "v3dViewPlugin";
}

QString v3dViewPlugin::description(void) const
{
    return "v3dView";
}

QStringList v3dViewPlugin::tags(void) const
{
    return QStringList() << "v3d" << "view";
}

QStringList v3dViewPlugin::types(void) const
{
    return QStringList() << "v3dView" << "v3dViewFiberInteractor" << "v3dViewFuseInteractor" << "v3dViewMeshInteractor"  << "v3dViewTensorInteractor" << "v3dView4DInteractor";
}

Q_EXPORT_PLUGIN2(v3dViewPlugin, v3dViewPlugin)
