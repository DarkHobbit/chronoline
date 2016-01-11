#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTime>
#include <QMainWindow>
#include "../../cltimeline.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CLTimeLine* l;
protected:
    virtual void showEvent(QShowEvent*);
    void testAllRounds(const QDateTime& d);
    void testUnitsTo(const QDateTime& d1, const QDateTime& d2);
};

#endif // MAINWINDOW_H
