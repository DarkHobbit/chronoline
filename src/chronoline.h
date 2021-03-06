#ifndef CHRONOLINE_H
#define CHRONOLINE_H

#include <QColor>
#include <QDateTime>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QEvent>
#include <QMap>
#include <QTimer>
#include <QWheelEvent>
#include "cldefs.h"
#include "clperiod.h"
#include "clflag.h"
#include "clflagpair.h"
#include "cltimeline.h"
#include "colorautoselector.h"

class ChronoLine : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ChronoLine(QWidget *parent = 0);
    bool updateAll();
    // Global settings
    void setBackgroundColor(QColor c);
    void lockAutoUpdate();
    void unLockAutoUpdate();
    bool isAutoUpdateLocked();
    void setMinUnit(const ChronoLineUnit& unit);
    // Timeline settings
    bool setUnit(const ChronoLineUnit& unit);
    bool setMinDate(const QDateTime& date);
    bool setMaxDate(const QDateTime& date);
    bool setRange(const QDateTime& minDate, const QDateTime& maxDate);
    ChronoLineUnit unit();
    ChronoLineUnit actualUnit();
    QDateTime minDate();
    QDateTime maxDate();
    void zoomIn(float centerRate);
    void zoomOut(float centerRate);
    // Periods management
    long addPeriod(const QDateTime& minDate, const QDateTime& maxDate, const QColor& color);
    long addPeriod(const QDateTime& minDate, const QDateTime& maxDate);
    bool editPeriod(long idPeriod, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color);
    bool editPeriod(long idPeriod, const QDateTime& minDate, const QDateTime& maxDate);
    bool removePeriod(long idPeriod);
    inline int periodCount() { return periods.count(); }
    bool readPeriod(long idPeriod, QDateTime& minDate, QDateTime& maxDate);
    // Event flags management
    long addEventFlag(const QDateTime& date, const QColor& color);
    long addEventFlag(const QDateTime& date);
    bool editEventFlag(long idFlag, const QDateTime& date, const QColor& color);
    bool editEventFlag(long idFlag, const QDateTime& date);
    bool removeEventFlag(long idFlag);
    inline int eventFlagCount() { return evFlags.count(); }
    bool readEventFlag(long idFlag, QDateTime& date);
    // Flags pairs management
    long addFlagPair(const QDateTime& minDate, const QDateTime& maxDate, const QColor& color);
    long addFlagPair(const QDateTime& minDate, const QDateTime& maxDate);
    bool editFlagPair(long idPair, const QDateTime& minDate, const QDateTime& maxDate, const QColor& color);
    bool editFlagPair(long idPair, const QDateTime& minDate, const QDateTime& maxDate);
    bool removeFlagPair(long idPair);
    inline int flagPairCount() { return flagPairs.count(); }
    bool readFlagPair(long idFlagPair, QDateTime& minDate, QDateTime& maxDate);
    // Clear all objects
    void clearAll();
    // Set range to fit all flags/periods on scene
    bool fitObjectsOnScene(bool shrinkIfNeeded);
    // D/t truncate date to actual unit (drop minutes if unit is hour, etc.)
    QDateTime truncToUnit(const QDateTime& baseDate, ChronoLineUnit unit);
    // D/t ceil date to actual unit
    QDateTime ceilToUnit(const QDateTime& baseDate, ChronoLineUnit unit);
    // D/t round date to actual unit (to next, if >=0.5)
    QDateTime roundToUnit(const QDateTime& baseDate, ChronoLineUnit unit);
    ColorAutoSelector periodColorSel, flagColorSel;
protected:
    QGraphicsScene *scene;
    bool           _lockAutoUpdate;
    // Items
    CLTimeLine     *timeLine;
    QMap<long, CLPeriod*> periods;
    QMap<long, CLFlag*>   evFlags;
    QMap<long, CLFlagPair*>   flagPairs;
    long           idSequencer; // generator of ID for periods and flags
    virtual void resizeEvent(QResizeEvent* event);
    // Scale shift while some flag dragged outside scale
    QTimer tmDragger;
    CLFlag* draggingFlag;
    float dragDateStep;
    bool event(QEvent *event);
    void wheelEvent(QWheelEvent* event);
    void mousePressEvent(QMouseEvent*event);
    void mouseDoubleClickEvent(QMouseEvent* event);
private:
    void selectNextObject(const CLSelectableList& candToSel, const QDateTime& date);
    void requestEditSelectedObject();
signals:
    // Global signals
    void actualUnitChanged(ChronoLineUnit unit);
    void mouseMovedOnScene(QPointF& scenePos, QDateTime& sceneDate);
    void rangeChanged(const QDateTime&, const QDateTime&);
    // Drag-n-drop signals
    void flagDateChanged(long idFlag, const QDateTime& newDate);
    void pairDatesChanged(long idPair, const QDateTime& newMinDate, const QDateTime& newMaxDate);
    // Object selection signals
    void periodSelected(long idPeriod);
    void eventFlagSelected(long idFlag);
    void flagPairSelected(long idPair, ChronoLineFlagType fType);
    void selectionRemoved();
    // Object editing signals
    void periodEditRequest(long idPeriod);
    void eventFlagEditRequest(long idFlag);
    void flagPairEditRequest(long idPair);
    // Object popup menu signals
    void scenePopupMenuRequest(const QDateTime& date);
    void periodPopupMenuRequest(long idPeriod);
    void eventFlagPopupMenuRequest(long idFlag);
    void flagPairPopupMenuRequest(long idPair);
public slots:
    void flagDraggedOutside(FlagDragDirection direction, int newX);
    void flagDragOutsideStop();
    void oneDragShiftStep();
    void receiveFlagDateChanged(const QDateTime& newDate);
    void transferFlagDateChanged(CLFlag* f, const QDateTime& newDate);
    void clUnitChanged(ChronoLineUnit unit);
    void clRangeChanged(const QDateTime& minD, const QDateTime& maxD);
    void onMouseMovedOnScene(QPointF& scenePos, QDateTime& sceneDate);
    void doUpdateAll();
};

#endif // CHRONOLINE_H
