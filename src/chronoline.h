#ifndef CHRONOLINE_H
#define CHRONOLINE_H

#include <QDateTime>
#include <QGraphicsView>
#include <QGraphicsScene>
#include "cldefs.h"
#include "cltimeline.h"

class ChronoLine : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChronoLine(QWidget *parent = 0);
    void updateAll();
    // Global settings
    void setBackgroundColor(QColor c);
    // Timeline settings
    void setUnit(const ChronoLineUnit& unit);
    void setMinDate(const QDateTime date);
    void setMaxDate(const QDateTime date);
    ChronoLineUnit unit();
    QDateTime minDate();
    QDateTime maxDate();
protected:
    QGraphicsScene *scene;
    CLTimeLine     *timeLine;
signals:
public slots:
};

#endif // CHRONOLINE_H
