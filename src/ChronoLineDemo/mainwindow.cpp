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

#include <QDebug>

QLabel* lbDebug = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    lockRoundRecursion(false)
{
    ui->setupUi(this);
    chronoLine = new ChronoLine(ui->frmChrono);
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(chronoLine);
    ui->frmChrono->setLayout(layout);
    // Events from ChronoLine
    connect(chronoLine,
            SIGNAL(flagDateChanged(long, const QDateTime&)), this, SLOT(anyFlagDateChanged(long, const QDateTime&)));
    connect(chronoLine,
            SIGNAL(pairDatesChanged(long, const QDateTime&, const QDateTime&)), this, SLOT(anyPairDatesChanged(long, const QDateTime&, const QDateTime&)));
    connect(chronoLine, SIGNAL(periodSelected(long)), this, SLOT(anyPeriodSelected(long)));
    connect(chronoLine, SIGNAL(eventFlagSelected(long)), this, SLOT(anyEventFlagSelected(long)));
    connect(chronoLine,
            SIGNAL(flagPairSelected(long,ChronoLineFlagType)), this, SLOT(anyFlagPairSelected(long,ChronoLineFlagType)));
    connect(chronoLine, SIGNAL(selectionRemoved()), this, SLOT(anySelectionRemoved()));
    connect(chronoLine, SIGNAL(actualUnitChanged(ChronoLineUnit)), this, SLOT(clUnitChanged(ChronoLineUnit)));
    connect(chronoLine, SIGNAL(mouseMovedOnScene(QPointF&,QDateTime&)), this, SLOT(onMouseMovedOnScene(QPointF&,QDateTime&)));
    // Initial data
    chronoLine->lockAutoUpdate();
    // Status bar
    int year = QDate::currentDate().year();
    QDate begin = QDate( year, 1, 1); // 01.01.year
    QDate end = begin.addMonths(11).addDays(30); // add 11 month and 30 days -> 31.12.year :)
    ui->edMinDate->setDateTime( QDateTime( begin ) );
    ui->edMaxDate->setDateTime( QDateTime( end ) );
    sl1 = new QLabel(0);
    sl2 = new QLabel(0);
    sl3 = new QLabel(0);
    statusBar()->addWidget(sl1, 1);
    statusBar()->addWidget(sl2, 1);
    statusBar()->addWidget(sl3, 4);
    lbDebug = sl3;
    // Periods/flags debugging
    long idP = chronoLine->addPeriod(QDateTime::currentDateTime().addDays(1), QDateTime::currentDateTime().addDays(2));
    if (idP) periods.push_back(idP);
    idP = chronoLine->addPeriod(QDateTime::currentDateTime().addDays(5), QDateTime::currentDateTime().addDays(6));
    if (idP) periods.push_back(idP);
    idP = chronoLine->addPeriod(
        QDateTime::currentDateTime().addDays(5).addSecs(-2*3600), QDateTime::currentDateTime().addDays(6).addSecs(-2*3600));
    if (idP) periods.push_back(idP);
    idP = chronoLine->addPeriod(
        QDateTime::currentDateTime().addDays(5).addSecs(2*3600), QDateTime::currentDateTime().addDays(6).addSecs(2*3600));
    if (idP) periods.push_back(idP);
    long idF = chronoLine->addEventFlag(QDateTime::currentDateTime().addDays(2).addSecs(12*3600));
    if (idF) evFlags.push_back(idF);
    long idFP = chronoLine->addFlagPair(QDateTime::currentDateTime().addDays(3), QDateTime::currentDateTime().addDays(4));
    if (idFP) flagPairs.push_back(idFP);
    idFP = chronoLine->addFlagPair(QDateTime::currentDateTime().addDays(8), QDateTime::currentDateTime().addDays(9));
    if (idFP) flagPairs.push_back(idFP);
    idF = chronoLine->addEventFlag(QDateTime::currentDateTime().addDays(7));
    if (idF) evFlags.push_back(idF);
    // Ready!
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

void MainWindow::resizeEvent(QResizeEvent*)
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
    if (!chronoLine->updateAll())
        QMessageBox::critical(0, trUtf8("Ошибка"),
            trUtf8("Слишком большая единица измерения для выбранного периода либо неверная дата окончания"));
    sl2->setText(tr("%1 periods, %2 event flags").arg(chronoLine->periodCount()).arg(chronoLine->eventFlagCount()));
}

void MainWindow::on_edMinDate_dateTimeChanged(const QDateTime&)
{
     updateSettings();
}

void MainWindow::on_edMaxDate_dateTimeChanged(const QDateTime&)
{
     updateSettings();
}

void MainWindow::on_cbUnit_currentIndexChanged(const QString&)
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

void MainWindow::anyPeriodSelected(long idPeriod)
{
    QDateTime minDate, maxDate;
    chronoLine->readPeriod(idPeriod, minDate, maxDate);
    lbDebug->setText(tr("Period %1: %2/%3").arg(idPeriod).arg(minDate.toString()).arg(maxDate.toString()));
    updateView();
}

void MainWindow::anyEventFlagSelected(long idFlag)
{
    QDateTime date;
    chronoLine->readEventFlag(idFlag, date);
    lbDebug->setText(tr("Flag %1: %2").arg(idFlag).arg(date.toString()));
    updateView();
}

void MainWindow::anyFlagPairSelected(long idPair, ChronoLineFlagType)
{
    QDateTime minDate, maxDate;
    chronoLine->readFlagPair(idPair, minDate, maxDate);
    lbDebug->setText(tr("Pair %1: %2/%3").arg(idPair).arg(minDate.toString()).arg(maxDate.toString()));
    updateView();
}

void MainWindow::anySelectionRemoved()
{
    lbDebug->setText("");
    updateView();
}

void MainWindow::anyFlagDateChanged(long idFlag, const QDateTime& newDate)
{
    if (!lockRoundRecursion) {
        lockRoundRecursion = true;
        QDateTime roundDate = chronoLine->roundToUnit(newDate, cluDay);
        chronoLine->editEventFlag(idFlag, roundDate);
        updateView();
        lockRoundRecursion = false;
    }
    lbDebug->setText(tr("Flag %1 set to %2").arg(idFlag).arg(newDate.toString()));
}

void MainWindow::anyPairDatesChanged(long idPair, const QDateTime& newMinDate, const QDateTime& newMaxDate)
{
    if (!lockRoundRecursion) {
        lockRoundRecursion = true;
        QDateTime roundMinDate = chronoLine->roundToUnit(newMinDate, cluDay);
        QDateTime roundMaxDate = chronoLine->roundToUnit(newMaxDate, cluDay);
        chronoLine->editFlagPair(idPair, roundMinDate, roundMaxDate);
        updateView();
        lockRoundRecursion = false;
    }
    lbDebug->setText(tr("Pair %1:  %2/%3").arg(idPair).arg(newMinDate.toString()).arg(newMaxDate.toString()));
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

void MainWindow::clUnitChanged(ChronoLineUnit unit)
{
    if (ui) sl1->setText(tr("Units: %1").arg(ui->cbUnit->itemText(unit)));
}

void MainWindow::onMouseMovedOnScene(QPointF& scenePos, QDateTime& sceneDate)
{
    lbDebug->setText(tr("Mouse on (%1,%2) ").arg(scenePos.x()).arg(scenePos.y())+sceneDate.toString("dd.MM.yyyy hh:mm:ss"));
}
