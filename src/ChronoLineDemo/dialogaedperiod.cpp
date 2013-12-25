#include "dialogaedperiod.h"
#include "ui_dialogaedperiod.h"

DialogAEDPeriod::DialogAEDPeriod(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAEDPeriod)
{
    ui->setupUi(this);
}

DialogAEDPeriod::~DialogAEDPeriod()
{
    delete ui;
}

bool DialogAEDPeriod::setData(const QDateTime& minDate, const QDateTime& maxDate)
{
    if (minDate>=maxDate) return false;
    ui->edMinDate->setDateTime(minDate);
    ui->edMaxDate->setDateTime(maxDate);
    return true;
}

void DialogAEDPeriod::getData(QDateTime& minDate, QDateTime& maxDate)
{
    minDate = ui->edMinDate->dateTime();
    maxDate = ui->edMaxDate->dateTime();
}

void DialogAEDPeriod::changeEvent(QEvent *e)
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
