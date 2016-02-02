#include "colorautoselector.h"

const int COLOR_COUNT = 16;

const QColor baseColors[COLOR_COUNT] =
{
    Qt::red, /*Qt::green,*/ Qt::blue, Qt::cyan, Qt::magenta, Qt::yellow,
    Qt::white, Qt::gray, Qt::black,
    Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow,
    Qt::lightGray, Qt::darkGray
};


ColorAutoSelector::ColorAutoSelector(int initialShift) :
    index(initialShift-1),
    _initialShift(initialShift),
    reservedColors()
{
    incIndex();
}

void ColorAutoSelector::reset()
{
    index = _initialShift-1;
}

bool ColorAutoSelector::addReservedColor(const QColor& color)
{
    if (reservedColors.indexOf(color)==-1) {
        reservedColors.push_back(color);
        return true;
    }
    else return false;
}

bool ColorAutoSelector::removeReservedColor(const QColor& color)
{
    if (reservedColors.indexOf(color)!=-1) {
        reservedColors.removeOne(color);
        return true;
    }
    else return false;
}

QColor ColorAutoSelector::nextColor()
{
    incIndex();
    while (reservedColors.indexOf(baseColors[index])!=-1)
        incIndex();
    return baseColors[index];
}

void ColorAutoSelector::incIndex()
{
    index++;
    if (index>=COLOR_COUNT) index = 0;
}
