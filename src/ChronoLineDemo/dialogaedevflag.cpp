#include "dialogaedevflag.h"
#include "ui_dialogaedevflag.h"

DialogAEDEvFlag::DialogAEDEvFlag(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAEDEvFlag)
{
    ui->setupUi(this);
}

DialogAEDEvFlag::~DialogAEDEvFlag()
{
    delete ui;
}

bool DialogAEDEvFlag::setData(const QDateTime& date)
{
    ui->edDate->setDateTime(date);
    return true;
}

void DialogAEDEvFlag::getData(QDateTime& date)
{
    date = ui->edDate->dateTime();
}

void DialogAEDEvFlag::changeEvent(QEvent *e)
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
