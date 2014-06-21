#ifndef DIALOGAEDDATERANGE_H
#define DIALOGAEDDATERANGE_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
    class DialogAEDDateRange;
}

class DialogAEDDateRange : public QDialog {
    Q_OBJECT
public:
    DialogAEDDateRange(QWidget *parent = 0);
    ~DialogAEDDateRange();
    bool setData(const QDateTime& minDate, const QDateTime& maxDate);
    void getData(QDateTime& minDate, QDateTime& maxDate);
protected:
    void changeEvent(QEvent *e);

private:
    Ui::DialogAEDDateRange *ui;
};

#endif // DIALOGAEDDATERANGE_H
