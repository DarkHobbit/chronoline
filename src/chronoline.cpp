#include <iostream>
#include <QMessageBox>
#include "chronoline.h"

#include <QLabel>
extern QLabel* lbDebug;

ChronoLine::ChronoLine(QWidget *parent) :
    QGraphicsView(parent),
    _lockAutoUpdate(true),
    idSequencer(0)
{
    scene = new QGraphicsScene;
    setScene(scene);
    setBackgroundColor(Qt::cyan);
    // Workaround for preventing scene drift while drag-n-drop (magic but works)
    scene->setSceneRect(-width()/2, -height()/2, width(), height());
    // Time Line
    timeLine = new CLTimeLine();
    scene->addItem(timeLine);
    _lockAutoUpdate = false;
    connect(timeLine, SIGNAL(needUpdateAll()), this, SLOT(doUpdateAll()));
    // Scale shift while some flag dragged outside scale
    tmDragger.setInterval(FLAGDRAG_DATE_SHIFT_PERIOD);
    connect(&tmDragger, SIGNAL(timeout()), this, SLOT(oneDragShiftStep()));
    draggingFlag = 0;
    dragDateStep = 0;
}

void ChronoLine::updateAll()
{
    if (timeLine->calcScale(childrenRect())) {
        scene->update();
        resize(width()+1, height()+1); // Magic pass for flags redraw
        resize(width()-1, height()-1);
    }
    else
        QMessageBox::critical(0, QString::fromUtf8("Error"),
            QString::fromUtf8("Слишком большая единица измерения для выбранного периода либо неверная дата окончания"));
}

void ChronoLine::setBackgroundColor(QColor c)
{
    setBackgroundBrush(QBrush(c, Qt::SolidPattern));
    if (!_lockAutoUpdate) updateAll();
}

void ChronoLine::lockAutoUpdate()
{
    _lockAutoUpdate = true;
}

void ChronoLine::unLockAutoUpdate()
{
    _lockAutoUpdate = false;
    updateAll();
}

bool ChronoLine::isAutoUpdateLocked()
{
    return _lockAutoUpdate;
}

void ChronoLine::setUnit(const ChronoLineUnit& unit)
{
    timeLine->setUnit(unit);
    if (!_lockAutoUpdate) updateAll();
}

void ChronoLine::setMinDate(const QDateTime date)
{
    timeLine->setMinDate(date);
    if (!_lockAutoUpdate) updateAll();
}

void ChronoLine::setMaxDate(const QDateTime date)
{
    timeLine->setMaxDate(date);
    if (!_lockAutoUpdate) updateAll();
}

void ChronoLine::zoomIn(float centerRate)
{
    timeLine->zoomIn(centerRate);
    if (!_lockAutoUpdate) updateAll();
}

void ChronoLine::zoomOut(float centerRate)
{
    timeLine->zoomOut(centerRate);
    if (!_lockAutoUpdate) updateAll();
}

ChronoLineUnit ChronoLine::unit()
{
    return timeLine->unit();
}

QDateTime ChronoLine::minDate()
{
    return timeLine->minDate();
}

QDateTime ChronoLine::maxDate()
{
    return timeLine->maxDate();
}

long ChronoLine::addPeriod(const QDateTime& minDate, const QDateTime& maxDate, const QColor& color)
{
    long idPeriod = ++idSequencer; // first ID is 1
    if (minDate>=maxDate) return 0;
    periods[idPeriod] = new CLPeriod(idPeriod, minDate, maxDate, color, timeLine);
    periods[idPeriod]->setParentItem(timeLine);
    if (!_lockAutoUpdate) updateAll();
    return idPeriod;
}

bool ChronoLine::editPeriod(long idPeriod, const QDateTime& minDate, const QDateTime& maxDate)
{
    if (minDate>=maxDate) return false;
    CLPeriod* p = periods[idPeriod];
    if (!p) return false;
    p->setMinDate(minDate);
    p->setMaxDate(maxDate);
    return true;
}

bool ChronoLine::removePeriod(long idPeriod)
{
    CLPeriod* p = periods[idPeriod];
    if (!p) return false;
    periods.remove(idPeriod);
    delete p;
    return true;
}

bool ChronoLine::readPeriod(long idPeriod, QDateTime& minDate, QDateTime& maxDate)
{
    CLPeriod* p = periods[idPeriod];
    if (!p) return false;
    minDate = p->minDate();
    maxDate = p->maxDate();
    return true;
}

long ChronoLine::addEventFlag(const QDateTime& date, const QColor& color)
{
    QRect r = childrenRect();
    timeLine->calcScale(r);
    long idFlag = ++idSequencer; // first ID is 1
    evFlags[idFlag] = new CLFlag(idFlag, date, clftEvent, color, timeLine, this);
    evFlags[idFlag]->setParentItem(timeLine);
    evFlags[idFlag]->setPosByDate(r);
    if (!_lockAutoUpdate) updateAll();
    return idFlag;
}

bool ChronoLine::editEventFlag(long idFlag, const QDateTime& date)
{
    CLFlag* f = evFlags[idFlag];
    if (!f) return false;
    f->setDate(date);
    return true;
}

bool ChronoLine::removeEventFlag(long idFlag)
{
    CLFlag* f = evFlags[idFlag];
    if (!f) return false;
    evFlags.remove(idFlag);
    delete f;
    return true;
}

bool ChronoLine::readEventFlag(long idFlag, QDateTime& date)
{
    CLFlag* f = evFlags[idFlag];
    if (!f) return false;
    date = f->date();
    return true;
}

long ChronoLine::addFlagPair(const QDateTime& minDate, const QDateTime& maxDate, const QColor& color)
{
    QRect r = childrenRect();
    timeLine->calcScale(r);
    long idPair = ++idSequencer; // first ID is 1
    idSequencer +=2; // 3 IDs needed: for pair and for both its flags
    if (minDate>=maxDate) return 0;
    flagPairs[idPair] = new CLFlagPair(idPair, minDate, maxDate, color, timeLine, this);
    flagPairs[idPair]->setParentItem(timeLine);
    flagPairs[idPair]->setPosByDates(r);
    if (!_lockAutoUpdate) updateAll();
    return idPair;

}

bool ChronoLine::fitObjectsOnScene(bool shrinkIfNeeded)
{
    if (!evFlags.count() && !periods.count() /*&& !flagPairs.count()*/)
        return false; // can't fit if no objects
    QDateTime minD, maxD;
    if (shrinkIfNeeded)
    {
        if (evFlags.count()>0) {

            minD = evFlags.begin().value()->date();
            maxD = evFlags.begin().value()->date();
        }
        else
        if (periods.count()>0) {
            minD = periods.begin().value()->minDate();
            maxD = periods.begin().value()->maxDate();
        }
        else
        if (flagPairs.count()>0) {
            minD = flagPairs.begin().value()->minDate();
            maxD = flagPairs.begin().value()->maxDate();
        }
    }
    else {
        minD = minDate();
        maxD = maxDate();
    }
    for (QMap<long, CLFlag*>::iterator i=evFlags.begin(); i!=evFlags.end(); i++) {
        if (i.value()->date()<minD) minD = i.value()->date();
        if (i.value()->date()>maxD) maxD = i.value()->date();
    }
    for (QMap<long, CLPeriod*>::iterator i=periods.begin(); i!=periods.end(); i++) {
        if (i.value()->minDate()<minD) minD = i.value()->minDate();
        if (i.value()->maxDate()>maxD) maxD = i.value()->maxDate();
    }
    for (QMap<long, CLFlagPair*>::iterator i=flagPairs.begin(); i!=flagPairs.end(); i++) {
        if (i.value()->minDate()<minD) minD = i.value()->minDate();
        if (i.value()->maxDate()>maxD) maxD = i.value()->maxDate();
    }
    minD = timeLine->addUnits(minD, -1);
    maxD = timeLine->addUnits(maxD, 1);
    setMinDate(minD);
    setMaxDate(maxD);
    return true;
}

void ChronoLine::resizeEvent(QResizeEvent* event)
{
    QRect r = childrenRect();
    timeLine->calcScale(r);
    foreach (const long id, evFlags.keys()) {
        CLFlag* p = evFlags.value(id);
        p->setPosByDate(r);
    }
    foreach (const long id, flagPairs.keys()) {
        CLFlagPair* p = flagPairs.value(id);
        p->setPosByDates(r);
    }
    // TODO pairs
    QGraphicsView::resizeEvent(event);
}

void ChronoLine::flagDraggedOutside(FlagDragDirection direction, int newX)
{
    draggingFlag = dynamic_cast<CLFlag*>(sender());
    float dX = 0;
    if (direction==fdLeft) {
        dX = (float)(newX-timeLine->xMin())/LEFT_DIV_MARGIN;
        dragDateStep = -MIN_FLAGDRAG_DATE_SHIFT+(MAX_FLAGDRAG_DATE_SHIFT-MIN_FLAGDRAG_DATE_SHIFT)*dX;
    }
    else {
        dX = (float)(newX-timeLine->xMax())/RIGHT_DIV_MARGIN;
        dragDateStep = MIN_FLAGDRAG_DATE_SHIFT+(MAX_FLAGDRAG_DATE_SHIFT-MIN_FLAGDRAG_DATE_SHIFT)*dX;
    }
    //lbDebug->setText(QString("%1 %2 %3 %4").arg(newX).arg(timeLine->xMin()).arg(dX).arg(dragStep));
    tmDragger.start();
}

void ChronoLine::flagDragOutsideStop()
{
    tmDragger.stop();
    draggingFlag = 0;
    dragDateStep = 0;
}

void ChronoLine::oneDragShiftStep()
{
    if (draggingFlag) {
        setMinDate(timeLine->addUnits(minDate(), dragDateStep));
        setMaxDate(timeLine->addUnits(maxDate(), dragDateStep));
        QDateTime newDate = (dragDateStep<0) ? minDate() : maxDate();
        if (draggingFlag->setDate(newDate))
            emit flagDateChanged(draggingFlag->id(), newDate);
        else
            flagDragOutsideStop();
        updateAll();
    }
}

void ChronoLine::transferFlagDateChanged(long idFlag, const QDateTime& newDate)
{
    emit flagDateChanged(idFlag, newDate);
}

// Scale range changing by mouse wheel
void ChronoLine::wheelEvent(QWheelEvent* event)
{
    int centerX = event->pos().x();
    float centerRate = (float)centerX/width();
//    std::cout << "wheel " << event->pos().x() << " " << width() << " " << event->delta() <<  std::endl;
    if (event->delta()>0)
        zoomIn(centerRate);
    else
        zoomOut(centerRate);
    event->accept();
}

void ChronoLine::doUpdateAll()
{
    updateAll();
}
