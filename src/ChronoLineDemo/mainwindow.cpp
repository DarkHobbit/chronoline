#include <QtCore/QDateTime>
#include <QtGui/QMessageBox>
#include <QtGui/QVBoxLayout>
#include "../cldefs.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaedperiod.h"

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
    chronoLine->lockAutoUpdate();
    ui->edMinDate->setDateTime(QDateTime::currentDateTime());
    ui->edMaxDate->setDateTime(QDateTime::currentDateTime().addDays(7));
    // Periods debugging
    chronoLine->addPeriod(QDateTime::currentDateTime().addDays(1), QDateTime::currentDateTime().addDays(2));
    chronoLine->addPeriod(QDateTime::currentDateTime().addDays(5), QDateTime::currentDateTime().addDays(6));
    chronoLine->unLockAutoUpdate();
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

void MainWindow::updateSettings()
{
    chronoLine->setMinDate(ui->edMinDate->dateTime());
    chronoLine->setMaxDate(ui->edMaxDate->dateTime());
    chronoLine->setUnit((ChronoLineUnit)ui->cbUnit->currentIndex());
}

void MainWindow::on_edMinDate_dateTimeChanged(const QDateTime &dateTime)
{
     updateSettings();
}

void MainWindow::on_edMaxDate_dateTimeChanged(const QDateTime &dateTime)
{
     updateSettings();
}

void MainWindow::on_cbUnit_currentIndexChanged(const QString &arg1)
{
     updateSettings();
}

void MainWindow::on_action_Add_Period_activated()
{
    DialogAEDPeriod* dlg = new DialogAEDPeriod(0);
    dlg->setData(QDateTime::currentDateTime().addDays(5), QDateTime::currentDateTime().addDays(6));
    dlg->exec();
    if (dlg->result()==QDialog::Accepted) {
        QDateTime minDate, maxDate;
        dlg->getData(minDate, maxDate);
        chronoLine->addPeriod(minDate, maxDate);
    }
    delete dlg;
    chronoLine->updateAll();
}
