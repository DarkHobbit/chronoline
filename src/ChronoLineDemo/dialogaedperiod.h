#ifndef DIALOGAEDPERIOD_H
#define DIALOGAEDPERIOD_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
    class DialogAEDPeriod;
}

class DialogAEDPeriod : public QDialog {
    Q_OBJECT
public:
    DialogAEDPeriod(QWidget *parent = 0);
    ~DialogAEDPeriod();
    bool setData(const QDateTime& minDate, const QDateTime& maxDate);
    void getData(QDateTime& minDate, QDateTime& maxDate);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogAEDPeriod *ui;
};

#endif // DIALOGAEDPERIOD_H
