#include "progressdelegate.h"

#include <QtWidgets/QtWidgets>


void ProgressDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                const QModelIndex &index) const
{
    if (index.column() == 5) {
        QColor highlightColor = option.palette.color(QPalette::Highlight);
        highlightColor = "green";

        QString prs = index.data(Qt::DisplayRole).toString();
        QStyleOptionProgressBar progressBarOption;

        progressBarOption.palette.setColor(QPalette::Highlight, highlightColor);

        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        QString progress;
        for (int i = 0; i < prs.length(); ++i) {
            if (prs[i] != ':') {
                if (prs[i] != 'f' && prs[i] != 'c')
                    progress += prs[i];
            }
            else
                break;
        }

        progressBarOption.maximum = 2600;

        progressBarOption.progress = progress.toInt();
        prs.remove(0,1);
        progressBarOption.text = prs;
        progressBarOption.textVisible = true;
        progressBarOption.textAlignment = Qt::AlignHCenter;
        QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                           &progressBarOption, painter);
    }
}

QWidget *ProgressDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &/*option */,
                                        const QModelIndex &/*index */) const

{
    QProgressBar *progressBar = new QProgressBar(parent);
    return progressBar;
}

void ProgressDelegate::setEditorData(QWidget *editor,
                                        const QModelIndex &index) const
{
    int value = index.data(Qt::DisplayRole).toInt();
    QProgressBar *progressBar = qobject_cast<QProgressBar *>(editor);
    progressBar->setValue(value);
//    progressBar->setFormat();
//    if (value == -1)
//    {
//        progressBar->setFormat(tr("Waiting."));
//        progressBar->setValue(0);
//    }
//    else if (value == -2)
//    {
//        progressBar->setFormat(tr("Pausing."));
//        progressBar->setValue(0);
//    }
//    else
//    {
//        progressBar->setFormat("%p%");
//        progressBar->setValue(value);
//    }
}

void ProgressDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                       const QModelIndex &index) const
{
    QProgressBar *progressBar = qobject_cast<QProgressBar *>(editor);
    QString str = QString::number(progressBar->value());
    model->setData(index, str+"%2", Qt::DisplayRole);
}
