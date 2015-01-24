#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QList>
#include "../chronoline.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
protected:
    virtual void changeEvent(QEvent *e);
    virtual void resizeEvent (QResizeEvent* e);
private slots:
    void on_action_Add_Period_triggered();
    void on_action_Add_Event_Flag_triggered();
    void on_actionE_xit_triggered();
    void on_action_Manage_Periods_Flags_triggered();
    void on_edMinDate_dateTimeChanged(const QDateTime &dateTime);
    void on_edMaxDate_dateTimeChanged(const QDateTime &dateTime);
    void on_cbUnit_currentIndexChanged(const QString &arg1);
    void on_action_Fit_objects_on_scene_triggered();
    void on_action_Set_scale_Range_triggered();
    void on_action_Zoom_In_triggered();
    void on_actionZoom_Out_triggered();
    void on_action_Add_Pair_Of_Flags_triggered();
public slots:
    void anyPeriodSelected(long idPeriod);
    void anyEventFlagSelected(long idFlag);
    void anyFlagPairSelected(long idPair, ChronoLineFlagType fType);
    void anyFlagDateChanged(long idFlag, const QDateTime& newDate);
    void anySelectionRemoved();
    void clUnitChanged(ChronoLineUnit unit);
    void onMouseMovedOnScene(QPointF& scenePos, QDateTime& sceneDate);
private:
    void updateSettings();
    void updateView();
    Ui::MainWindow *ui;
    ChronoLine *chronoLine;
    QLabel *sl1, *sl2, *sl3;
    QList<long> periods;
    QList<long> evFlags;
    QList<long> flagPairs;
};

#endif // MAINWINDOW_H
