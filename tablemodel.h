#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QPair>
#include <QList>

#include "netcomp.h"

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent=0);
    TableModel(QList< NetComp > listofComps, QObject *parent=0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(QModelIndex &index, QString &value, int role=Qt::EditRole);
    bool insertRows(int position, int rows, const QModelIndex &index=QModelIndex());
    bool removeRows(int position, int rows, const QModelIndex &index=QModelIndex());    
    QList< NetComp > getList();
    void clear();

    void setNameHeader(QString value);
    void setIpHeader(QString value);
    void setCleanHeader(QString value);
    void setFilterHeader(QString value);
    void setOsHeader(QString value);
    void setCFHeader(QString value);
    void setTimeHeader(QString value);
    void setCleanHint(QString value);
    void setFilterHint(QString value);

private:
    QList< NetComp > listOfComps;
    QString nameHeader;
    QString ipHeader;
    QString cleanHeader;
    QString filterHeader;
    QString osHeader;
    QString cfHeader;
    QString timeHeader;
    QString cleanHint;
    QString filterHint;
};

#endif
