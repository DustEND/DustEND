#ifndef PROGRESSDELEGATE_H
#define PROGRESSDELEGATE_H

#include <QtWidgets/QItemDelegate>
#include <QModelIndex>
#include <QObject>
#include <QSize>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStyledItemDelegate>

class ProgressDelegate : public QStyledItemDelegate
{
    Q_OBJECT


public:
    ProgressDelegate(QWidget *parent = 0, Qt::Alignment alignment = Qt::AlignCenter) : QStyledItemDelegate(parent), align(alignment) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

private:
    Qt::Alignment align;
};

#endif // PROGRESSDELEGATE_H
