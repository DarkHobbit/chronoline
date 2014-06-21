#include "dialogaeddaterange.h"
#include "ui_dialogaeddaterange.h"

DialogAEDDateRange::DialogAEDDateRange(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAEDDateRange)
{
    ui->setupUi(this);
}

DialogAEDDateRange::~DialogAEDDateRange()
{
    delete ui;
}

bool DialogAEDDateRange::setData(const QDateTime& minDate, const QDateTime& maxDate)
{
    if (minDate>=maxDate) return false;
    ui->edMinDate->setDateTime(minDate);
    ui->edMaxDate->setDateTime(maxDate);
    return true;
}

void DialogAEDDateRange::getData(QDateTime& minDate, QDateTime& maxDate)
{
    minDate = ui->edMinDate->dateTime();
    maxDate = ui->edMaxDate->dateTime();
}

void DialogAEDDateRange::changeEvent(QEvent *e)
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
