#include <QWidget>
#include <medAbstractData.h>

class medAbstractProcess : public QObject
{
    Q_OBJECT

public:
    virtual QWidget *toolbox() = 0;
    virtual void setInputs(QList <medAbstractData *> inputs) = 0;
    
    virtual QList <medAbstractData *> outputs() = 0;
};
