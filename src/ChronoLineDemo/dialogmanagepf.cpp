#include <iostream>
#include <QDateTime>
#include <QMessageBox>
#include "dialogmanagepf.h"
#include "ui_dialogmanagepf.h"
#include "dialogaeddaterange.h"
#include "dialogaedevflag.h"

DialogManagePF::DialogManagePF(QWidget *parent, ChronoLine* cl, QList<long>* periods, QList<long>* evFlags, QList<long>* flagPairs):
    QDialog(parent),
    ui(new Ui::DialogManagePF),
    _cl(cl),
    _periods(periods),
    _evFlags(evFlags),
    _flagPairs(flagPairs)
{
    ui->setupUi(this);
    readPeriods();
    readEvFlags();
    readFlagPairs();
}

DialogManagePF::~DialogManagePF()
{
    delete ui;
}

void DialogManagePF::readPeriods()
{
    // Fill period list
    ui->twPeriods->setRowCount(0);
    ui->twPeriods->setRowCount(_periods->count());
    ui->twPeriods->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twPeriods->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twPeriods->setItemDelegate(new NonEditTableColumnDelegate());
    long id;
    int i=0;
    QDateTime d1, d2;
    foreach (id, *_periods) {
        _cl->readPeriod(id, d1, d2);
        ui->twPeriods->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        ui->twPeriods->setItem(i, 1, new QTableWidgetItem(d1.toString("dd.MM.yyyy hh:mm")));
        ui->twPeriods->setItem(i, 2, new QTableWidgetItem(d2.toString("dd.MM.yyyy hh:mm")));
        i++;
    };
}

void DialogManagePF::readEvFlags()
{
    // Fill event flag list
    ui->twEvFlags->setRowCount(_evFlags->count());
    ui->twEvFlags->setRowCount(_evFlags->count());
    ui->twEvFlags->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twEvFlags->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twEvFlags->setItemDelegate(new NonEditTableColumnDelegate());
    long id;
    int i=0;
    QDateTime d;
    foreach (id, *_evFlags) {
        _cl->readEventFlag(id, d);
        ui->twEvFlags->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        ui->twEvFlags->setItem(i, 1, new QTableWidgetItem(d.toString("dd.MM.yyyy hh:mm")));
        i++;
    };
}

void DialogManagePF::readFlagPairs()
{
    // Fill period list
    ui->twFlagPairs->setRowCount(0);
    ui->twFlagPairs->setRowCount(_flagPairs->count());
    ui->twFlagPairs->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twFlagPairs->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twFlagPairs->setItemDelegate(new NonEditTableColumnDelegate());
    long id;
    int i=0;
    QDateTime d1, d2;
    foreach (id, *_flagPairs) {
        _cl->readFlagPair(id, d1, d2);
        ui->twFlagPairs->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        ui->twFlagPairs->setItem(i, 1, new QTableWidgetItem(d1.toString("dd.MM.yyyy hh:mm")));
        ui->twFlagPairs->setItem(i, 2, new QTableWidgetItem(d2.toString("dd.MM.yyyy hh:mm")));
        i++;
    };
}

void DialogManagePF::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool DialogManagePF::chkSel(QTableWidget* t)
{
    if (t->selectedItems().size()==0) {
        QMessageBox::critical(0, tr("Error"), tr("No rows selected"));
        return false;
    }
    else {
        return true;
    }
}

void DialogManagePF::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    QRect r = childrenRect();
    int w = r.width();
    // Resize period list
    ui->twPeriods->setColumnWidth(0, w/6);
    ui->twPeriods->setColumnWidth(1, w/3);
    ui->twPeriods->setColumnWidth(2, w/3);
    // Resize event flag list
    ui->twEvFlags->setColumnWidth(0, w/4);
    ui->twEvFlags->setColumnWidth(1, w/2);
    // Resize flag pair list
    ui->twFlagPairs->setColumnWidth(0, w/6);
    ui->twFlagPairs->setColumnWidth(1, w/3);
    ui->twFlagPairs->setColumnWidth(2, w/3);
}


void DialogManagePF::on_pbClose_clicked()
{
    close();
}

void DialogManagePF::on_pbEditEvFlag_clicked()
{
    if (ui->tabber->currentWidget()->objectName()=="tabPeriods") {
        if (!chkSel(ui->twPeriods)) return;
        long idP = ui->twPeriods->selectedItems()[0]->text().toLong();
        QDateTime minDate = QDateTime::fromString(ui->twPeriods->selectedItems()[1]->text(), "dd.MM.yyyy hh:mm");
        QDateTime maxDate = QDateTime::fromString(ui->twPeriods->selectedItems()[2]->text(), "dd.MM.yyyy hh:mm");
        DialogAEDDateRange* dlg = new DialogAEDDateRange(0);
        dlg->setWindowTitle(tr("Edit Period"));
        dlg->setData(minDate, maxDate);
        dlg->exec();
        if (dlg->result()==QDialog::Accepted) {
            dlg->getData(minDate, maxDate);
            _cl->editPeriod(idP, minDate, maxDate);
            ui->twPeriods->selectedItems()[1]->setText(minDate.toString("dd.MM.yyyy hh:mm"));
            ui->twPeriods->selectedItems()[2]->setText(maxDate.toString("dd.MM.yyyy hh:mm"));
        }
        delete dlg;
    }
    else if (ui->tabber->currentWidget()->objectName()=="tabEvFlags") {
        if (!chkSel(ui->twEvFlags)) return;
        long idF = ui->twEvFlags->selectedItems()[0]->text().toLong();
        QDateTime date = QDateTime::fromString(ui->twEvFlags->selectedItems()[1]->text(), "dd.MM.yyyy hh:mm");
        DialogAEDEvFlag* dlg = new DialogAEDEvFlag(0);
        dlg->setEditMode(true);
        dlg->setData(date);
        dlg->exec();
        if (dlg->result()==QDialog::Accepted) {
            dlg->getData(date);
            _cl->editEventFlag(idF, date);
            ui->twEvFlags->selectedItems()[1]->setText(date.toString("dd.MM.yyyy hh:mm"));
        }
        delete dlg;
    }
    else if (ui->tabber->currentWidget()->objectName()=="tabFlagPairs") {
        if (!chkSel(ui->twFlagPairs)) return;
        long idP = ui->twFlagPairs->selectedItems()[0]->text().toLong();
        QDateTime minDate = QDateTime::fromString(ui->twFlagPairs->selectedItems()[1]->text(), "dd.MM.yyyy hh:mm");
        QDateTime maxDate = QDateTime::fromString(ui->twFlagPairs->selectedItems()[2]->text(), "dd.MM.yyyy hh:mm");
        DialogAEDDateRange* dlg = new DialogAEDDateRange(0);
        dlg->setWindowTitle(tr("Edit Period"));
        dlg->setData(minDate, maxDate);
        dlg->exec();
        if (dlg->result()==QDialog::Accepted) {
            dlg->getData(minDate, maxDate);
            _cl->editFlagPair(idP, minDate, maxDate);
            ui->twFlagPairs->selectedItems()[1]->setText(minDate.toString("dd.MM.yyyy hh:mm"));
            ui->twFlagPairs->selectedItems()[2]->setText(maxDate.toString("dd.MM.yyyy hh:mm"));
        }
        delete dlg;
    }
    else QMessageBox::critical(0, tr("Error"), tr("Internal error!"));
}

void DialogManagePF::on_pbRemoveEvFlag_clicked()
{
    if (ui->tabber->currentWidget()->objectName()=="tabPeriods") {
        if (!chkSel(ui->twPeriods)) return;
        long idP = ui->twPeriods->selectedItems()[0]->text().toLong();
        if (QMessageBox::question(0,
                QString::fromUtf8("Подтверждение"),
                QString::fromUtf8("Вы действительно хотите удалить период?"),
                QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        {
            _cl->removePeriod(idP);
            _periods->removeOne(idP);
            readPeriods();
        }
    }
    else if (ui->tabber->currentWidget()->objectName()=="tabEvFlags") {
        if (!chkSel(ui->twEvFlags)) return;
        long idF = ui->twEvFlags->selectedItems()[0]->text().toLong();
        if (QMessageBox::question(0,
                QString::fromUtf8("Подтверждение"),
                QString::fromUtf8("Вы действительно хотите удалить флаг события?"),
                QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        {
            _cl->removeEventFlag(idF);
            _evFlags->removeOne(idF);
            readEvFlags();
        }
    }
    else if (ui->tabber->currentWidget()->objectName()=="tabFlagPairs") {
        if (!chkSel(ui->twFlagPairs)) return;
        long idP = ui->twFlagPairs->selectedItems()[0]->text().toLong();
        if (QMessageBox::question(0,
                QString::fromUtf8("Подтверждение"),
                QString::fromUtf8("Вы действительно хотите удалить пару флагов?"),
                QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        {
            _cl->removeFlagPair(idP);
            _flagPairs->removeOne(idP);
            readFlagPairs();
        }
    }
    else QMessageBox::critical(0, tr("Error"), tr("Internal error!"));
}
