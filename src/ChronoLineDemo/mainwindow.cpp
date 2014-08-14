#include <iostream>
#include <QtCore/QDateTime>
#include <QMessageBox>
#include <QVBoxLayout>
#include "../cldefs.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dialogaeddaterange.h"
#include "dialogaedevflag.h"
#include "dialogmanagepf.h"

QLabel* lbDebug = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    chronoLine = new ChronoLine(ui->frmChrono);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chronoLine);
    ui->frmChrono->setLayout(layout);
    connect(
        chronoLine, SIGNAL(flagDateChanged(long, const QDateTime&)),
                this, SLOT(anyFlagDateChanged(long, const QDateTime&)));
    // Initial data
    chronoLine->lockAutoUpdate();
    ui->edMinDate->setDateTime(QDateTime::currentDateTime());
    ui->edMaxDate->setDateTime(QDateTime::currentDateTime().addDays(7));
    // Status bar
    sl1 = new QLabel(0);
    sl2 = new QLabel(0);
    sl3 = new QLabel(0);
    statusBar()->addWidget(sl1, 1);
    statusBar()->addWidget(sl2, 1);
    statusBar()->addWidget(sl3, 4);
    lbDebug = sl3;
    // Periods debugging
    long idP = chronoLine->addPeriod(QDateTime::currentDateTime().addDays(1), QDateTime::currentDateTime().addDays(2), Qt::magenta);
    if (idP) periods.push_back(idP);
    idP = chronoLine->addPeriod(QDateTime::currentDateTime().addDays(5), QDateTime::currentDateTime().addDays(6), Qt::white);
    if (idP) periods.push_back(idP);
    long idF = chronoLine->addEventFlag(QDateTime::currentDateTime().addDays(3), Qt::red);
    if (idF) evFlags.push_back(idF);
    chronoLine->unLockAutoUpdate();
    updateView();
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

void MainWindow::resizeEvent(QResizeEvent* e)
{
    chronoLine->updateAll(); // TODO dup?
}

void MainWindow::on_actionE_xit_triggered()
{
    close();
}

void MainWindow::updateSettings()
{
    chronoLine->setMinDate(ui->edMinDate->dateTime());
    chronoLine->setMaxDate(ui->edMaxDate->dateTime());
    chronoLine->setUnit((ChronoLineUnit)ui->cbUnit->currentIndex());
}

void MainWindow::updateView()
{
    chronoLine->updateAll();
    sl1->setText(QString("%1 periods").arg(chronoLine->periodCount()));
    sl2->setText(QString("%1 event flags").arg(chronoLine->eventFlagCount()));
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

void MainWindow::on_action_Add_Period_triggered()
{
    DialogAEDDateRange* dlg = new DialogAEDDateRange(0);
    dlg->setWindowTitle(tr("Add Period"));
    dlg->setData(QDateTime::currentDateTime().addDays(5), QDateTime::currentDateTime().addDays(6));
    dlg->exec();
    if (dlg->result()==QDialog::Accepted) {
        QDateTime minDate, maxDate;
        dlg->getData(minDate, maxDate);
        long idP = chronoLine->addPeriod(minDate, maxDate, Qt::yellow);
        if (idP) periods.push_back(idP);
    }
    delete dlg;
    updateView();
}

void MainWindow::on_action_Add_Event_Flag_triggered()
{
    DialogAEDEvFlag* dlg = new DialogAEDEvFlag(0);
    dlg->setData(QDateTime::currentDateTime().addDays(4));
    dlg->exec();
    if (dlg->result()==QDialog::Accepted) {
        QDateTime date;
        dlg->getData(date);
        long idF = chronoLine->addEventFlag(date, Qt::yellow);
        if (idF) evFlags.push_back(idF);
    }
    delete dlg;
    updateView();
}

void MainWindow::on_action_Manage_Periods_Flags_triggered()
{
    DialogManagePF* dlg = new DialogManagePF(0, chronoLine, &periods, &evFlags, &flagPairs);
    dlg->exec();
    delete dlg;
    updateView();
}

void MainWindow::anyFlagDateChanged(long idFlag, const QDateTime& newDate)
{
    lbDebug->setText(QString("Flag %1 set to %2").arg(idFlag).arg(newDate.toString()));
}

void MainWindow::on_action_Fit_objects_on_scene_triggered()
{
    chronoLine->fitObjectsOnScene(true);
}

void MainWindow::on_action_Set_scale_Range_triggered()
{
    DialogAEDDateRange* dlg = new DialogAEDDateRange(0);
    dlg->setWindowTitle(tr("Set scale range"));
    dlg->setData(chronoLine->minDate(), chronoLine->maxDate());
    dlg->exec();
    if (dlg->result()==QDialog::Accepted) {
        QDateTime minDate, maxDate;
        dlg->getData(minDate, maxDate);
        chronoLine->setMinDate(minDate);
        chronoLine->setMaxDate(maxDate);
    }
    delete dlg;
    updateView();
}

void MainWindow::on_action_Zoom_In_triggered()
{
    chronoLine->zoomIn(0.5);
}


void MainWindow::on_actionZoom_Out_triggered()
{
    chronoLine->zoomOut(0.5);
}

void MainWindow::on_action_Add_Pair_Of_Flags_triggered()
{
    DialogAEDDateRange* dlg = new DialogAEDDateRange(0);
    dlg->setWindowTitle(tr("Add Pair of Flags"));
    dlg->setData(QDateTime::currentDateTime().addDays(3), QDateTime::currentDateTime().addDays(4));
    dlg->exec();
    if (dlg->result()==QDialog::Accepted) {
        QDateTime minDate, maxDate;
        dlg->getData(minDate, maxDate);
        long idP = chronoLine->addFlagPair(minDate, maxDate, Qt::magenta);
        if (idP) flagPairs.push_back(idP);
    }
    delete dlg;
    updateView();

}
