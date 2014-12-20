#include <QPen>
#include "clselectableobject.h"

CLSelectableObject::CLSelectableObject(CLTimeLine* timeLine)
    :_timeLine(timeLine)
{
}

void CLSelectableObject::setPenWidth(QPen& pen)
{
    if (_timeLine->selectedObject==this)
        pen.setWidth(2);
    else
        pen.setWidth(1);
}
