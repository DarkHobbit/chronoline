#ifndef CLSELECTABLEOBJECT_H
#define CLSELECTABLEOBJECT_H

#include "cltimeline.h"

class CLSelectableObject
{
public:
    CLSelectableObject(CLTimeLine* timeLine);
    virtual bool matchDate(const QDateTime& d)=0;
    void setPenWidth(QPen& pen);
protected:
    CLTimeLine* _timeLine;
};

class CLSelectableList: public QList<CLSelectableObject*>
{};

#endif // CLSELECTABLEOBJECT_H
