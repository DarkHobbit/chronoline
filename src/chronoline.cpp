#include <QMessageBox>
#include "chronoline.h"

ChronoLine::ChronoLine(QWidget *parent) :
    QGraphicsView(parent), idSequencer(0)
{
    scene = new QGraphicsScene;
    setScene(scene);
    setBackgroundColor(Qt::cyan);
    // Time Line
    timeLine = new CLTimeLine();
//    timeLine->setPos(QPointF(0, 0 /*height()/2*/));
    scene->addItem(timeLine);
}

void ChronoLine::updateAll()
{
    if (timeLine->calcScale(childrenRect()))
        scene->update();
    else
        QMessageBox::critical(0, QString::fromUtf8("Ошибка"),
            QString::fromUtf8("Слишком большая единица измерения для выбранного периода либо неверная дата окончания"));
}

void ChronoLine::setBackgroundColor(QColor c)
{
    setBackgroundBrush(QBrush(c, Qt::SolidPattern));
}

void ChronoLine::setUnit(const ChronoLineUnit& unit)
{
    timeLine->setUnit(unit);
}

void ChronoLine::setMinDate(const QDateTime date)
{
    timeLine->setMinDate(date);
}

void ChronoLine::setMaxDate(const QDateTime date)
{
    timeLine->setMaxDate(date);
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

long ChronoLine::addPeriod(const QDateTime& minDate, const QDateTime& maxDate)
{
    long idPeriod = ++idSequencer; // first ID is 1
    if (minDate>=maxDate) return 0;
    periods[idPeriod] = new CLPeriod(minDate, maxDate, timeLine);
    periods[idPeriod]->setParentItem(timeLine);
    return idPeriod;
}

