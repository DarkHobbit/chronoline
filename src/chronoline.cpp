#include <iostream>
#include <QtAlgorithms>
#include <QDebug>
#include <QLabel>
#include <QList>
#include <QMessageBox>
#include "chronoline.h"

extern QLabel* lbDebug;

ChronoLine::ChronoLine(QWidget *parent) :
    QGraphicsView(parent),
    _lockAutoUpdate(true),
    idSequencer(0),
    periodColorSel(0),
    flagColorSel(3)
{
    scene = new QGraphicsScene;
    setScene(scene);
    setBackgroundColor(Qt::cyan);
    periodColorSel.addReservedColor(Qt::black);
    periodColorSel.addReservedColor(Qt::cyan);
    flagColorSel.addReservedColor(Qt::black);
    flagColorSel.addReservedColor(Qt::cyan);
    // Workaround for preventing scene drift while drag-n-drop (magic but works)
    scene->setSceneRect(-width()/2, -height()/2, width(), height());
    // Time Line
    timeLine = new CLTimeLine();
    scene->addItem(timeLine);
    _lockAutoUpdate = false;
    connect(timeLine, SIGNAL(needUpdateAll()), this, SLOT(doUpdateAll()));
    connect(timeLine, SIGNAL(actualUnitChanged(ChronoLineUnit)), this, SLOT(clUnitChanged(ChronoLineUnit)));
    connect(timeLine, SIGNAL(rangeChanged(QDateTime,QDateTime)), this, SLOT(clRangeChanged(QDateTime,QDateTime)));
    connect(timeLine, SIGNAL(mouseMovedOnScene(QPointF&,QDateTime&)), this, SLOT(onMouseMovedOnScene(QPointF&,QDateTime&)));
    // Scale shift while some flag dragged outside scale
    tmDragger.setInterval(FLAGDRAG_DATE_SHIFT_PERIOD);
    connect(&tmDragger, SIGNAL(timeout()), this, SLOT(oneDragShiftStep()));
    draggingFlag = 0;
    dragDateStep = 0;
}

bool ChronoLine::updateAll()
{
    if (timeLine->calcScale(childrenRect())) {
        scene->update();
        resize(width()+1, height()+1); // Magic pass for flags redraw
        resize(width()-1, height()-1);
        return true;
    }
    else
        return false;
}

void ChronoLine::setBackgroundColor(QColor c)
{
    periodColorSel.removeReservedColor(backgroundBrush().color());
    flagColorSel.removeReservedColor(backgroundBrush().color());
    setBackgroundBrush(QBrush(c, Qt::SolidPattern));
    periodColorSel.addReservedColor(c);
    flagColorSel.addReservedColor(c);
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

void ChronoLine::setMinUnit(const ChronoLineUnit &unit)
{
    timeLine->setMinUnit(unit);
}

bool ChronoLine::setUnit(const ChronoLineUnit& unit)
{
    bool res = timeLine->setUnit(unit);
    if (res && !_lockAutoUpdate) updateAll();
    return res;
}

bool ChronoLine::setMinDate(const QDateTime& date)
{
    bool res = timeLine->setMinDate(date, !_lockAutoUpdate);
    if (res && !_lockAutoUpdate) updateAll();
    return res;
}

bool ChronoLine::setMaxDate(const QDateTime& date)
{
    bool res = timeLine->setMaxDate(date, !_lockAutoUpdate);
    if (res && !_lockAutoUpdate) updateAll();
    return res;
}

bool ChronoLine::setRange(const QDateTime& minDate, const QDateTime& maxDate)
{
    bool res = timeLine->setRange(minDate, maxDate, !_lockAutoUpdate);
    if (res && !_lockAutoUpdate) updateAll();
    return res;
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

ChronoLineUnit ChronoLine::actualUnit()
{
    return timeLine->actualUnit();
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

long ChronoLine::addPeriod(const QDateTime& minDate, const QDateTime& maxDate)
{
    return addPeriod(minDate, maxDate, periodColorSel.nextColor());
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
    evFlags[idFlag] = new CLFlag(idFlag, date, clftEvent, color, timeLine, this, 0);
    evFlags[idFlag]->setParentItem(timeLine);
    evFlags[idFlag]->setPosByDate(r);
    if (!_lockAutoUpdate) updateAll();
    return idFlag;
}

long ChronoLine::addEventFlag(const QDateTime& date)
{
    return addEventFlag(date, flagColorSel.nextColor());
}

bool ChronoLine::editEventFlag(long idFlag, const QDateTime& date)
{
    CLFlag* f = evFlags.value(idFlag);
    if (!f) {
        return false;
    }
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

long ChronoLine::addFlagPair(const QDateTime& minDate, const QDateTime& maxDate)
{
    return addFlagPair(minDate, maxDate, flagColorSel.nextColor());
}

bool ChronoLine::editFlagPair(long idPair, const QDateTime& minDate, const QDateTime& maxDate)
{
    if (minDate>=maxDate) return false;
    CLFlagPair* p = flagPairs[idPair];
    if (!p) return false;
    p->setMinDate(minDate);
    p->setMaxDate(maxDate);
    return true;
}

bool ChronoLine::removeFlagPair(long idPair)
{
    CLFlagPair* p = flagPairs[idPair];
    if (!p) return false;
    flagPairs.remove(idPair);
    delete p;
    return true;
}

bool ChronoLine::readFlagPair(long idPair, QDateTime& minDate, QDateTime& maxDate)
{
    CLFlagPair* p = flagPairs[idPair];
    if (!p) return false;
    minDate = p->minDate();
    maxDate = p->maxDate();
    return true;
}

void ChronoLine::clearAll()
{
    foreach (CLPeriod* p, periods) {
        delete p;
        periods.remove(periods.key(p));
    }
    foreach (CLFlag* f, evFlags) {
        delete f;
        evFlags.remove(evFlags.key(f));
    }
    foreach (CLFlagPair* p, flagPairs) {
        delete p;
        flagPairs.remove(flagPairs.key(p));
    }
    idSequencer = 0;
    periodColorSel.reset();
    flagColorSel.reset();
}

bool ChronoLine::fitObjectsOnScene(bool shrinkIfNeeded)
{
    if (!evFlags.count() && !periods.count() && !flagPairs.count())
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

QDateTime ChronoLine::truncToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    return timeLine->truncToUnit(baseDate, unit);
}

QDateTime ChronoLine::ceilToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    return timeLine->ceilToUnit(baseDate, unit);
}

QDateTime ChronoLine::roundToUnit(const QDateTime& baseDate, ChronoLineUnit unit)
{
    return timeLine->roundToUnit(baseDate, unit);
}

void ChronoLine::resizeEvent(QResizeEvent* event)
{
    QRect r = childrenRect();
    timeLine->calcScale(r);
    foreach (const long id, evFlags.keys()) {
        CLFlag* f = evFlags.value(id);
        if (f) f->setPosByDate(r); else qDebug() << "RSZ: invalid flag in set";
    }
    foreach (const long id, flagPairs.keys()) {
        CLFlagPair* p = flagPairs.value(id);
        if (p) p->setPosByDates(r); else qDebug() << "RSZ: invalid pair in set";
    }
    QGraphicsView::resizeEvent(event);
}

bool selObjLessThan(CLSelectableObject*& a, CLSelectableObject*& b)
{
    return ((CLPeriod*)a)->level() < ((CLPeriod*)b)->level();
}

bool ChronoLine::event(QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Tab) {
            CLSelectableList candToSel;
            // Candidates - periods
            for (QMap<long, CLPeriod*>::iterator i=periods.begin(); i!=periods.end(); i++)
                candToSel.push_back(i.value());
            // Sort period list by visual level (old-style implementation)
            qSort(candToSel.begin(), candToSel.end(), selObjLessThan);
            // Candidates - flags and pairs
            foreach (CLFlag* f, evFlags.values())
                candToSel.push_back(f);
            foreach (CLFlagPair* p, flagPairs.values())
                candToSel.push_back(p);
            selectNextObject(candToSel, QDateTime());
            return true;
        }
    }
    else
        return QWidget::event(event);
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
            transferFlagDateChanged(draggingFlag, newDate);
        else
            flagDragOutsideStop();
        updateAll();
    }
}

void ChronoLine::receiveFlagDateChanged(const QDateTime& newDate)
{
    transferFlagDateChanged(dynamic_cast<CLFlag*>(sender()), newDate);
}

void ChronoLine::transferFlagDateChanged(CLFlag* f, const QDateTime& newDate)
{
    if (f->fType()==clftEvent)
        emit flagDateChanged(f->id(), newDate);
    else {
        CLFlagPair* p = dynamic_cast<CLFlagPair*>(f->pair());
        if (p)
            emit pairDatesChanged(p->id(), p->minDate(), p->maxDate());
    }
}

// Scale range changing by mouse wheel
void ChronoLine::wheelEvent(QWheelEvent* event)
{
    int centerX = event->pos().x();
    float centerRate = (float)centerX/width();
    if (event->delta()>0)
        zoomIn(centerRate);
    else
        zoomOut(centerRate);
    updateAll();
    event->accept();
}

void ChronoLine::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    // Search for all objects under cursor
    QPointF sc = mapToScene(event->pos());
    QDateTime mDate = timeLine->dateForX(sc.x());
    CLSelectableList candToSel;
    // Candidates - periods
    if (sc.y()<0) {
        for (QMap<long, CLPeriod*>::iterator i=periods.begin(); i!=periods.end(); i++)
            if (i.value()->matchDate(mDate)) candToSel.push_back(i.value());
        // Sort period list by visual level (lambda-based implementation)
        /*qSort(candToSel.begin(), candToSel.end(), [](CLSelectableObject*& a, CLSelectableObject*& b)
            { return ((CLPeriod*)a)->level() < ((CLPeriod*)b)->level();} );*/
        // Sort period list by visual level (old-style implementation)
        qSort(candToSel.begin(), candToSel.end(), selObjLessThan);
    }
    // Candidates - flags and pairs
    else {
        foreach (CLFlag* f, evFlags.values())
                if (f->matchDate(mDate)) candToSel.push_back(f);
        foreach (CLFlagPair* p, flagPairs.values())
                if (p->matchDate(mDate)) candToSel.push_back(p);
    };
    selectNextObject(candToSel, mDate);
}

void ChronoLine::selectNextObject(const CLSelectableList &candToSel, const QDateTime& date)
{
    // Select period, flag or pair
    if (candToSel.count()>0) {
       CLSelectableObject* so = 0;
        for (int i=0; i<candToSel.count(); i++) {
            if (candToSel[i]==timeLine->selectedObject) {
                if (i==candToSel.count()-1)
                    so = candToSel[0];
                else
                   so = candToSel[i+1];
                break;
            }
        }
        if (!so) so = candToSel[0];
        timeLine->selectedObject = so;
        // Send appropriate signal to application
        CLPeriod* p = dynamic_cast<CLPeriod*>(so);
        if (p) {
            emit periodSelected(p->id());
        }
        else {
            CLFlag* f = dynamic_cast<CLFlag*>(so);
            if (f) {
                emit eventFlagSelected(f->id());
            }
            else {
                CLFlagPair* fp = dynamic_cast<CLFlagPair*>(so);
                if (fp) {
                    if (date.isValid())
                        emit flagPairSelected(fp->id(), fp->matchedFlag(date));
                    else
                        emit flagPairSelected(fp->id(), clftPairBeg);

                }
            }
        }
    }
    else {
        timeLine->selectedObject = 0;
        emit selectionRemoved();
    }
    update();
}

void ChronoLine::clUnitChanged(ChronoLineUnit unit)
{
    if (!_lockAutoUpdate) emit actualUnitChanged(unit);
}

void ChronoLine::clRangeChanged(const QDateTime &minD, const QDateTime &maxD)
{
    if (!_lockAutoUpdate) emit rangeChanged(minD, maxD);
}

void ChronoLine::onMouseMovedOnScene(QPointF& scenePos, QDateTime& sceneDate)
{
    emit mouseMovedOnScene(scenePos, sceneDate);
}

void ChronoLine::doUpdateAll()
{
    updateAll();
}
