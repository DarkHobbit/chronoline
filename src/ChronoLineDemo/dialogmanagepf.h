#ifndef DIALOGMANAGEPF_H
#define DIALOGMANAGEPF_H

#include <QDialog>
#include <QItemDelegate>
#include <QVector>
#include "chronoline.h"

namespace Ui {
    class DialogManagePF;
}

class DialogManagePF : public QDialog {
    Q_OBJECT
public:
    DialogManagePF(QWidget *parent, ChronoLine* cl, QVector<long>* periods, QVector<long>* evFlags);
    ~DialogManagePF();

protected:
    virtual void changeEvent(QEvent *e);
    virtual void resizeEvent(QResizeEvent* event);

private slots:
    void on_pbClose_clicked();

    void on_pbEditPeriod_clicked();

    void on_pbRemovePeriod_clicked();

    void on_pbEditEvFlag_clicked();

    void on_pbRemoveEvFlag_clicked();

private:
    Ui::DialogManagePF *ui;
    ChronoLine* _cl;
    QVector<long>* _periods;
    QVector<long>* _evFlags;
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
