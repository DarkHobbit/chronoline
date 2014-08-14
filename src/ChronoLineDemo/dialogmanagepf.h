#ifndef DIALOGMANAGEPF_H
#define DIALOGMANAGEPF_H

#include <QDialog>
#include <QItemDelegate>
#include <QList>
#include <QTableWidget>
#include "chronoline.h"

namespace Ui {
    class DialogManagePF;
}

class DialogManagePF : public QDialog {
    Q_OBJECT
public:
    DialogManagePF(QWidget *parent, ChronoLine* cl, QList<long>* periods, QList<long>* evFlags, QList<long>* flagPairs);
    ~DialogManagePF();

protected:
    void readPeriods();
    void readEvFlags();
    void readFlagPairs();
    bool chkSel(QTableWidget* t); // check if any row selected
    virtual void changeEvent(QEvent *e);
    virtual void resizeEvent(QResizeEvent* event);

private slots:
    void on_pbClose_clicked();
    void on_pbEditEvFlag_clicked();
    void on_pbRemoveEvFlag_clicked();

private:
    Ui::DialogManagePF *ui;
    ChronoLine* _cl;
    QList<long>* _periods;
    QList<long>* _evFlags;
    QList<long>* _flagPairs;
};

// This bicycle-delegate allows disable editing for ALL items in table widget,
// instead toggling editable flag for EACH new item! Delegates is power!
// Thanx to Axis - http://axis.bplaced.net/news/460
class NonEditTableColumnDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    NonEditTableColumnDelegate(QObject * parent = 0) : QItemDelegate(parent) {}
    virtual QWidget * createEditor ( QWidget *, const QStyleOptionViewItem &,
                                     const QModelIndex &) const
    {
        return 0;
    }
};

#endif // DIALOGMANAGEPF_H
