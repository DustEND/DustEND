
#include "tablemodel.h"
#include <QDebug>
#include <QPixmap>
//! [0]
TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

TableModel::TableModel(QList< NetComp > pairs, QObject *parent)
    : QAbstractTableModel(parent)
{
    listOfComps = pairs;
}
//! [0]

//! [1]
int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return listOfComps.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}
//! [1]

//! [2]
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= listOfComps.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        NetComp pair = listOfComps.at(index.row());

        if (index.column() == 0)
            return pair.getName();
        else {
            if (index.column() == 1)
                return pair.getIpAddress();
            else {
                if (index.column() == 2)
                    return pair.getCleanTime();
                else {
                    if (index.column() == 3)
                        return pair.getFilterTime();
                    else {
                        if (index.column() == 5) {
                            if (pair.getFilterTime() != "")
                                return QString("f%1").arg(pair.getFilterTime());
                            else {
                                if (pair.getCleanTime() != "")
                                    return QString("c%1").arg(pair.getCleanTime());
                            }
                        }
                        else
                            if (index.column() == 4)
                                return pair.getOsName();
                    }
                }
            }
        }
    }
    if (role == Qt::DecorationRole) {
        NetComp pair = listOfComps.at(index.row());
        if (index.column() == 6) {
            QPixmap pix(10,10);            
            if (pair.getCleanTime() != "") {
                pix.load(":/images/clean.png");
                return pix.scaled(16,16);
            }

            if (pair.getFilterTime() != "") {
                pix.load(":/images/FS8.ico");
                return pix.scaled(16,16);
            }
        }
    }

    if (role == Qt::ToolTipRole) {
        NetComp pair = listOfComps.at(index.row());
        if (pair.getCleanTime() != "") {
            return cleanHint;
        }
        if (pair.getFilterTime() != "") {
            return filterHint;
        }
    }

    return QVariant();
}
//! [2]

//! [3]
QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return nameHeader;
            case 1:
                return ipHeader;
            case 2:
                return cleanHeader;
            case 3:
                return filterHeader;
            case 4:
                return osHeader;
            case 6:
                return cfHeader;
            case 5:
                return timeHeader;

            default:
                return QVariant();
        }
    }
    return QVariant();
}
//! [3]

//! [4]
bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position+rows-1);

    for (int row=0; row < rows; row++) {
        NetComp pair(" ", " ", " ", " ", " ");
        listOfComps.insert(position, pair);
    }

    endInsertRows();
    return true;
}
//! [4]

//! [5]
bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position+rows-1);

    for (int row = 0; row < rows; ++row) {
        listOfComps.removeAt(position);
    }

    endRemoveRows();
    return true;
}
//! [5]

//! [6]
bool TableModel::setData(/*const*/ QModelIndex &index, /*const*/ QString &value, int role)
{
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        NetComp p(" ", " ", " ", " ", " ");
        p = listOfComps.at(row);
        QString valStr = value;

        if (index.column() == 0)
            p.setName(&valStr);
        else {
            if (index.column() == 1)
                p.setIpAddress(&valStr);
            else {
                if (index.column() == 2)
                    p.setCleanTime(&valStr);
                else {
                    if (index.column() == 3)
                        p.setFilterTime(&valStr);
                    else {
                        if (index.column() == 4)
                            p.setOsName(&valStr);
                        else
                            return false;
                    }
                }
            }
        }
        listOfComps.replace(row, p);
        emit(dataChanged(index, index));

        return true;
    }
    return false;
}
//! [6]

//! [7]
Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
//! [7]

//! [8]
QList< NetComp > TableModel::getList()
{
    return listOfComps;
}

void TableModel::clear()
{
    listOfComps.clear();
}

void TableModel::setNameHeader(QString value)
{
    nameHeader = value;
}

void TableModel::setIpHeader(QString value)
{
    ipHeader = value;
}

void TableModel::setCleanHeader(QString value)
{
    cleanHeader = value;
}

void TableModel::setFilterHeader(QString value)
{
    filterHeader = value;
}

void TableModel::setOsHeader(QString value)
{
    osHeader = value;
}

void TableModel::setCFHeader(QString value)
{
    cfHeader = value;
}

void TableModel::setTimeHeader(QString value)
{
    timeHeader = value;
}

void TableModel::setCleanHint(QString value)
{
    cleanHint = value;
}

void TableModel::setFilterHint(QString value)
{
    filterHint = value;
}
//! [8]
