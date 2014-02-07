#include <iostream>
#include <QDateTime>
#include <QMessageBox>
#include "dialogmanagepf.h"
#include "ui_dialogmanagepf.h"
#include "dialogaedperiod.h"
#include "dialogaedevflag.h"

DialogManagePF::DialogManagePF(QWidget *parent, ChronoLine* cl, QVector<long>* periods, QVector<long>* evFlags):
    QDialog(parent),
    ui(new Ui::DialogManagePF),
    _cl(cl),
    _periods(periods),
    _evFlags(evFlags)
{
    ui->setupUi(this);
    long id;
    int i;
    QDateTime d1, d2;
    // Fill period list
    ui->twPeriods->setRowCount(periods->count());
    ui->twPeriods->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twPeriods->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twPeriods->setItemDelegate(new NonEditTableColumnDelegate());
    i=0;
    foreach (id, *periods) {
        _cl->readPeriod(id, d1, d2);
        ui->twPeriods->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        ui->twPeriods->setItem(i, 1, new QTableWidgetItem(d1.toString("dd.MM.yyyy hh:mm")));
        ui->twPeriods->setItem(i, 2, new QTableWidgetItem(d2.toString("dd.MM.yyyy hh:mm")));
        i++;
    };
    // Fill event flag list
    ui->twEvFlags->setRowCount(evFlags->count());
    ui->twEvFlags->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->twEvFlags->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->twEvFlags->setItemDelegate(new NonEditTableColumnDelegate());
    i=0;
    foreach (id, *evFlags) {
        _cl->readEventFlag(id, d1);
        ui->twEvFlags->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        ui->twEvFlags->setItem(i, 1, new QTableWidgetItem(d1.toString("dd.MM.yyyy hh:mm")));
        i++;
    };
}

DialogManagePF::~DialogManagePF()
{
    delete ui;
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
}


void DialogManagePF::on_pbClose_clicked()
{
    close();
}

void DialogManagePF::on_pbEditPeriod_clicked()
{
    if (ui->twPeriods->selectedItems().size()==0) return;
    long idP = ui->twPeriods->selectedItems()[0]->text().toLong();
    QDateTime minDate = QDateTime::fromString(ui->twPeriods->selectedItems()[1]->text(), "dd.MM.yyyy hh:mm");
    QDateTime maxDate = QDateTime::fromString(ui->twPeriods->selectedItems()[2]->text(), "dd.MM.yyyy hh:mm");
    DialogAEDPeriod* dlg = new DialogAEDPeriod(0);
    dlg->setEditMode(true);
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

void DialogManagePF::on_pbRemovePeriod_clicked()
{
    if (ui->twPeriods->selectedItems().size()==0) return;
    long idP = ui->twPeriods->selectedItems()[0]->text().toLong();
    if (QMessageBox::question(0,
            QString::fromUtf8("Подтверждение"),
            QString::fromUtf8("Вы действительно хотите удалить период?"),
            QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        _cl->removePeriod(idP);
}

void DialogManagePF::on_pbEditEvFlag_clicked()
{
    if (ui->twEvFlags->selectedItems().size()==0) return;
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

void DialogManagePF::on_pbRemoveEvFlag_clicked()
{
    if (ui->twEvFlags->selectedItems().size()==0) return;
    long idF = ui->twEvFlags->selectedItems()[0]->text().toLong();
    if (QMessageBox::question(0,
            QString::fromUtf8("Подтверждение"),
            QString::fromUtf8("Вы действительно хотите удалить флаг?"),
            QMessageBox::Yes, QMessageBox::No)==QMessageBox::Yes)
        _cl->removeEventFlag(idF);
}
