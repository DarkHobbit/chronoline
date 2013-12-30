#include "dialogmanagepf.h"
#include "ui_dialogmanagepf.h"

DialogManagePF::DialogManagePF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogManagePF)
{
    ui->setupUi(this);
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
