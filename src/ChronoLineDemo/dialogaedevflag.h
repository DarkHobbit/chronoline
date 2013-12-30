#ifndef DIALOGAEDEVFLAG_H
#define DIALOGAEDEVFLAG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
    class DialogAEDEvFlag;
}

class DialogAEDEvFlag : public QDialog {
    Q_OBJECT
public:
    DialogAEDEvFlag(QWidget *parent = 0);
    ~DialogAEDEvFlag();
    bool setData(const QDateTime& date);
    void getData(QDateTime& date);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogAEDEvFlag *ui;
};

#endif // DIALOGAEDEVFLAG_H
