#ifndef CHRONOLINE_H
#define CHRONOLINE_H

#include <QColor>
#include <QDateTime>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>
#include <QTimer>
#include "cldefs.h"
#include "clperiod.h"
#include "clflag.h"
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
    bool editPeriod(long idPeriod, const QDateTime& minDate, const QDateTime& maxDate);
    bool removePeriod(long idPeriod);
    inline int periodCount() { return periods.count(); }
    bool readPeriod(long idPeriod, QDateTime& minDate, QDateTime& maxDate);
    // Event flags management
    long addEventFlag(const QDateTime& date, const QColor& color);
    /*long addEventFlag(const QDateTime& date);*/
    bool editEventFlag(long idFlag, const QDateTime& date);
    bool removeEventFlag(long idFlag);
    inline int eventFlagCount() { return evFlags.count(); }
    bool readEventFlag(long idFlag, QDateTime& date);
    // Flags pairs management
    /*long addFlagPair(const QDateTime& minDate, const QDateTime& maxDate, const QColor& color);
    long addFlagPair(const QDateTime& minDate, const QDateTime& maxDate);
    bool editFlagPair(long idPair, const QDateTime& minDate, const QDateTime& maxDate);
    bool removeFlagPair(long idPPair);*/
    // Set range to fit all flags/periods on scene
    bool fitObjectsOnScene(bool shrinkIfNeeded);
protected:
    QGraphicsScene *scene;
    bool           _lockAutoUpdate;
    // Items
    CLTimeLine     *timeLine;
    QMap<long, CLPeriod*> periods;
    QMap<long, CLFlag*>   evFlags;
    long           idSequencer; // generator of ID for periods and flags
    virtual void resizeEvent(QResizeEvent* event);
    // Scale shift while some flag dragged outside scale
    QTimer tmDragger;
    CLFlag* draggingFlag;
    float dragStep;
signals:
    void flagDateChanged(long idFlag, const QDateTime& newDate);
public slots:
    void flagDraggedOutside(FlagDragDirection direction, int newX);
    void flagDragOutsideStop();
    void oneDragShiftStep();
    void transferFlagDateChanged(long idFlag, const QDateTime& newDate);
};

#endif // CHRONOLINE_H
