// /////////////////////////////////////////////////////////////////
// Generated by dtkPluginGenerator
// /////////////////////////////////////////////////////////////////

#ifndef V3DVIEWPUBLIC_H
#define V3DVIEWPUBLIC_H

#include <medCore/medAbstractView.h>

#include "v3dViewPublicPluginExport.h"

class v3dViewPublicPrivate;
class vtkViewImage2D;
class vtkViewImage3D;

class V3DVIEWPUBLICPLUGIN_EXPORT v3dViewPublic : public medAbstractView
{
    Q_OBJECT

public:
             v3dViewPublic(void);
    virtual ~v3dViewPublic(void);

    virtual QString description(void) const;

    static bool registered(void);

public:
    void reset(void);
    void update(void);
	
    void   link(dtkAbstractView *other);
    void unlink(dtkAbstractView *other);
	
    void *view(void);
	
    void setData(dtkAbstractData *data);
    void *data (void);
	
    QList<dtkAbstractView *> linkedViews (void);
	
    // QWidget *receiverWidget(void);
    QWidget *widget(void);
	
    // access method to internal members for v3dView**Interactor classes
    vtkViewImage2D *viewAxial(void);
    vtkViewImage3D *view3D(void);
	
public slots:
    void onPropertySet         (QString key, QString value);
    void onOrientationPropertySet           (QString value);
    void on3DModePropertySet                (QString value);
    void onPresetPropertySet                (QString value);
    void onShowScalarBarPropertySet         (QString value);
    void onLookupTablePropertySet           (QString value);
    void onShowAxisPropertySet              (QString value);
    void onShowRulerPropertySet             (QString value);
    void onShowAnnotationsPropertySet       (QString value);
    void onMouseInteractionPropertySet      (QString value);
    void onCroppingPropertySet              (QString value);
    void onDaddyPropertySet                 (QString value);
    void onWindowingLinkedPropertySet       (QString value);
    void onPositionLinkedPropertySet        (QString value);

    
public slots:
    void linkPosition  (dtkAbstractView *view, bool value);
    void linkCamera    (dtkAbstractView *view, bool value);
    void linkWindowing (dtkAbstractView *view, bool value);

    void onZSliderValueChanged (int);
    void onMousePressEvent(QMouseEvent *event);
    void switchToAxial(void);
    void switchToSagittal(void);
    void switchToCoronal(void);
    void switchToVR(void);
    void switchToMPR(void);
    void onMenuZoomTriggered (void);
    void onMenuWindowLevelTriggered (void);
	void onPlayButtonClicked (bool value);
    

private:
    v3dViewPublicPrivate *d;
};

dtkAbstractView *createV3dViewPublic(void);

#endif
