#include "../../cltimeline.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    testAllRounds(QDateTime::fromString("31.01.2015 12:20", "dd.MM.yyyy hh:mm"));
    ui->lwLog->addItem("");
    testAllRounds(QDateTime::fromString("31.08.2015 17:40", "dd.MM.yyyy hh:mm"));
}

void MainWindow::testAllRounds(const QDateTime &d)
{
    CLTimeLine* l = new CLTimeLine();
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
    // TODO week
    delete l;
}
