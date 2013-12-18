#include <QDateTime>
#include <QVBoxLayout>
#include "../cldefs.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chronoLine = new ChronoLine(ui->frmChrono);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chronoLine);
    ui->frmChrono->setLayout(layout);
    // Initial data
    ui->edMinDate->setDateTime(QDateTime::currentDateTime());
    ui->edMaxDate->setDateTime(QDateTime::currentDateTime().addDays(7));
    // Periods debugging
    chronoLine->addPeriod(QDateTime::currentDateTime().addDays(2), QDateTime::currentDateTime().addDays(3)); //==>
    chronoLine->addPeriod(QDateTime::currentDateTime().addDays(5), QDateTime::currentDateTime().addDays(6)); //==>

    update_line();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::on_actionE_xit_activated()
{
    close();
}

void MainWindow::update_line()
{
    chronoLine->setMinDate(ui->edMinDate->dateTime());
    chronoLine->setMaxDate(ui->edMaxDate->dateTime());
    chronoLine->setUnit((ChronoLineUnit)ui->cbUnit->currentIndex());
    chronoLine->updateAll();
}

void MainWindow::on_edMinDate_dateTimeChanged(const QDateTime &dateTime)
{
    update_line();
}

void MainWindow::on_edMaxDate_dateTimeChanged(const QDateTime &dateTime)
{
    update_line();
}

void MainWindow::on_cbUnit_currentIndexChanged(const QString &arg1)
{
    update_line();
}
