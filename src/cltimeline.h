#ifndef CLTIMELINE_H
#define CLTIMELINE_H

#include <QDateTime>
#include <QGraphicsObject>
#include <QObject>
#include "cldefs.h"

class CLSelectableObject;

class CLTimeLine : public QGraphicsObject
{
    Q_OBJECT
public:
    CLTimeLine();
    void paint(QPainter *p, const QStyleOptionGraphicsItem *item, QWidget *widget);
    virtual QRectF boundingRect() const;
    // Timeline settings
    bool setUnit(const ChronoLineUnit& unit);
    bool setMinDate(const QDateTime& date, bool checkRange);
    bool setMaxDate(const QDateTime& date, bool checkRange);
    bool setRange(const QDateTime& minDate, const QDateTime& maxDate, bool checkRange);
    void zoomIn(float centerRate);
    void zoomOut(float centerRate);
    // Date/time coordinate (viewport x) for date
    int  xForDate(const QDateTime date, const QRect& r);
    QDateTime dateForX(int x);
    // Meazurement unit
    ChronoLineUnit unit();
    // if unit=auto, adjust real unit. Otherwise =unit
    ChronoLineUnit actualUnit();
    QDateTime minDate();
    QDateTime maxDate();
    int xMin();
    int xMax();
   // Rounded minDate for best view (read-only, calculated based on minDate)
    QDateTime leftScaleDate();
    // Calculate range, step, etc.
    bool calcScale(const QRect& r);
    // D/t length beetwen two dates in selected unit (daysTo() and secsTo()-like)
    float unitsTo(const QDateTime& baseDate, const QDateTime& newDate, ChronoLineUnit unit = cluAuto);
    // D/t add num units to baseDate
    QDateTime addUnits(const QDateTime& baseDate, float num, ChronoLineUnit unit = cluAuto);
    // D/t truncate date to actual unit (drop minutes if unit is hour, etc.)
    QDateTime truncToUnit(const QDateTime& baseDate, ChronoLineUnit unit = cluAuto);
    // D/t ceil date to actual unit
    QDateTime ceilToUnit(const QDateTime& baseDate, ChronoLineUnit unit);
    // D/t round date to actual unit (to next, if >=0.5)
    QDateTime roundToUnit(const QDateTime& baseDate, ChronoLineUnit unit = cluAuto);
    // Object (flag, period) selection hadling
    CLSelectableObject* selectedObject;
protected:
    // Input data
    ChronoLineUnit _unit, _actualUnit;
    QDateTime      _minDate, _maxDate;
    // Recalc flag
    bool changed;
    QRect rect;
    // Scale division parameters (calculated by CalcStep)
    int            mainDivCount;
    int            mainDivStep;
    int            x0;
    int            xN;
    QDateTime      _leftScaleDate;
    // Drag data
    int oldDragX;
    // draw date text
    void drawDate(QPainter *p, const QDateTime& date, short level, ChronoLineUnit unit, bool forceDrawParent);
    // check if parent unit text draw needed
    bool parentTextNeeded(const QDateTime& d, ChronoLineUnit nextUnit);
    // mouse handling
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
signals:
    void needUpdateAll();
    void actualUnitChanged(ChronoLineUnit unit);
    void mouseMovedOnScene(QPointF& scenePos, QDateTime& sceneDate);
    void rangeChanged(const QDateTime&, const QDateTime&);
};

#endif // CLTIMELINE_H
