#ifndef DIALOGMANAGEPF_H
#define DIALOGMANAGEPF_H

#include <QDialog>

namespace Ui {
    class DialogManagePF;
}

class DialogManagePF : public QDialog {
    Q_OBJECT
public:
    DialogManagePF(QWidget *parent = 0);
    ~DialogManagePF();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogManagePF *ui;
};

#endif // DIALOGMANAGEPF_H
