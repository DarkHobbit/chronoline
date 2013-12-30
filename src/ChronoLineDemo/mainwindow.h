#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void changeEvent(QEvent *e);
    
private slots:
    void on_action_Add_Period_activated();
    void on_action_Add_Event_Flag_activated();
    void on_actionE_xit_activated();

    void on_edMinDate_dateTimeChanged(const QDateTime &dateTime);

    void on_edMaxDate_dateTimeChanged(const QDateTime &dateTime);

    void on_cbUnit_currentIndexChanged(const QString &arg1);

private:

    void updateSettings();
    Ui::MainWindow *ui;
    ChronoLine *chronoLine;
};

#endif // MAINWINDOW_H
