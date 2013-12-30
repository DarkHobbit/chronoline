#ifndef CHRONOLINE_H
#define CHRONOLINE_H

#include <QColor>
#include <QDateTime>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>
#include "cldefs.h"
#include "clperiod.h"
#include "cltimeline.h"

class ChronoLine : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChronoLine(QWidget *parent = 0);
    void updateAll();
    // Global settings
    void setBackgroundColor(QColor c);
    void lockAutoUpdate();
    void unLockAutoUpdate();
    bool isAutoUpdateLocked();
    // Timeline settings
    void setUnit(const ChronoLineUnit& unit);
    void setMinDate(const QDateTime date);
    void setMaxDate(const QDateTime date);
    ChronoLineUnit unit();
    QDateTime minDate();
    QDateTime maxDate();
    // Periods management
    long addPeriod(const QDateTime& minDate, const QDateTime& maxDate, const QColor& color);
    /*long addPeriod(const QDateTime& minDate, const QDateTime& maxDate);*/
    /*bool editPeriod(long idPeriod, const QDateTime& minDate, const QDateTime& maxDate);
    bool removePeriod(long idPeriod);*/
protected:
    QGraphicsScene *scene;
    bool           _lockAutoUpdate;
    CLTimeLine     *timeLine;
    long           idSequencer; // generator of ID for periods and flags
    QMap<long, CLPeriod*> periods;
signals:
public slots:
};

#endif // CHRONOLINE_H
