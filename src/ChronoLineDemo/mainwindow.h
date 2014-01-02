#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
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

private:

    void updateSettings();
    void updateView();
    Ui::MainWindow *ui;
    ChronoLine *chronoLine;
    QLabel* sl1;
    QLabel* sl2;
};

#endif // MAINWINDOW_H
