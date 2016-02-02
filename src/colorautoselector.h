#ifndef COLORAUTOSELECTOR_H
#define COLORAUTOSELECTOR_H

#include <QColor>
#include <QList>

class ColorAutoSelector
{
public:
    // initialShift used if some ColorAutoSelector instances present in one scene for its separate parts
    explicit ColorAutoSelector(int initialShift = 0);
    void reset();
    // Reserved colors (i.e backround, axiz) not included in autoselected color queue
    bool addReservedColor(const QColor& color);
    bool removeReservedColor(const QColor& color);
    // Get next color
    QColor nextColor();
private:
    int index;
    int _initialShift;
    QList <QColor> reservedColors;
    void incIndex();
};

#endif // COLORAUTOSELECTOR_H
