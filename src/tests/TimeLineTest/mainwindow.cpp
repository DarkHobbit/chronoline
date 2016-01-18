#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    l = new CLTimeLine();
}

MainWindow::~MainWindow()
{
    delete l;
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    // round/trunc tests
    testAllRounds(QDateTime::fromString("31.01.2015 12:20", "dd.MM.yyyy hh:mm"));
    ui->lwLog->addItem("");
    testAllRounds(QDateTime::fromString("31.08.2015 17:40", "dd.MM.yyyy hh:mm"));
    ui->lwLog->addItem("");
    // unitsTo test
    testUnitsTo(QDateTime::fromString("30.01.2015 12:20", "dd.MM.yyyy hh:mm"),
        QDateTime::fromString("02.02.2015 12:20", "dd.MM.yyyy hh:mm"));
    testUnitsTo(QDateTime::fromString("30.01.2015 12:20", "dd.MM.yyyy hh:mm"),
        QDateTime::fromString("30.04.2015 12:20", "dd.MM.yyyy hh:mm"));
    testUnitsTo(QDateTime::fromString("30.01.2015 12:20", "dd.MM.yyyy hh:mm"),
        QDateTime::fromString("15.07.2015 12:20", "dd.MM.yyyy hh:mm"));
    ui->lwLog->addItem("");
    // addUnits test
    testAddUnits(s2d("26.12.2015 17:54:26"), 1, cluMonth);
    testAddUnits(s2d("26.12.2015 17:54:26"), 0.1, cluMonth);
    testAddUnits(s2d("26.12.2015 17:54:26"), 0.03, cluMonth);
}

void MainWindow::testAllRounds(const QDateTime &d)
{
    ui->lwLog->addItem(QString("Date: %1").arg(d.toString()));
    ui->lwLog->addItem(QString("Trunc to hour: %1").arg(l->truncToUnit(d, cluHour).toString()));
    ui->lwLog->addItem(QString("Round to hour: %1").arg(l->roundToUnit(d, cluHour).toString()));
    ui->lwLog->addItem(QString("Trunc to day: %1").arg(l->truncToUnit(d, cluDay).toString()));
    ui->lwLog->addItem(QString("Round to day: %1").arg(l->roundToUnit(d, cluDay).toString()));
    ui->lwLog->addItem(QString("Trunc to week: %1").arg(l->truncToUnit(d, cluWeek).toString()));
    ui->lwLog->addItem(QString("Round to week: %1").arg(l->roundToUnit(d, cluWeek).toString()));
    ui->lwLog->addItem(QString("Trunc to month: %1").arg(l->truncToUnit(d, cluMonth).toString()));
    ui->lwLog->addItem(QString("Round to month: %1").arg(l->roundToUnit(d, cluMonth).toString()));
    ui->lwLog->addItem(QString("Trunc to quarter: %1").arg(l->truncToUnit(d, cluQuarter).toString()));
    ui->lwLog->addItem(QString("Round to quarter: %1").arg(l->roundToUnit(d, cluQuarter).toString()));
    ui->lwLog->addItem(QString("Trunc to year: %1").arg(l->truncToUnit(d, cluYear).toString()));
    ui->lwLog->addItem(QString("Round to year: %1").arg(l->roundToUnit(d, cluYear).toString()));
}

void MainWindow::testUnitsTo(const QDateTime &d1, const QDateTime &d2)
{
    ui->lwLog->addItem(QString("Dates: %1, %2").arg(d1.toString()).arg(d2.toString()));
    ui->lwLog->addItem(QString("Distance: %1 months").arg(l->monthsTo(d1, d2)));
}

void MainWindow::testAddUnits(const QDateTime &d, float addition, ChronoLineUnit unit)
{
    ui->lwLog->addItem(QString("Date: %1, add %2 units %3").arg(d.toString()).arg(addition).arg(unit));
    ui->lwLog->addItem(QString("Result date: %1").arg(l->addUnits(d, addition, unit).toString()));
}


