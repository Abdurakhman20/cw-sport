#include "dataModel.h"

DataModel::DataModel(QObject *parent)
    : QAbstractTableModel{parent}
    ,numOfColumns(7)
{}

int DataModel::rowCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : dataClasses.size();
}
int DataModel::columnCount(const QModelIndex &parent) const {
    return parent.isValid() ? 0 : numOfColumns;
}

QVariant DataModel::data(const QModelIndex &index, int role) const {
    if(!checkIndex(index, CheckIndexOption::IndexIsValid))
        return QVariant();
    int row = index.row();
    int col = index.column();

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        if (col == 0)
            return dataClasses[row].getName();
        if (col == 1)
            return dataClasses[row].getCity();
        if (col == 2)
            return dataClasses[row].getAddress();
        if (col == 3)
            return dataClasses[row].getBuildingDate();
        if (col == 4)
            return dataClasses[row].getCapacity();
        if (col == 5)
            return dataClasses[row].getWorkingHours();
        if (col == 6)
            return dataClasses[row].getWebsiteAddress();
        break;


    default:
        return QVariant();
    }
    return QVariant();
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (!checkIndex(index, CheckIndexOption::IndexIsValid))
        return false;

    int col = index.column();

    if (role == Qt::EditRole) {
        if (col == 0)
            dataClasses[index.row()].setName(value.toString());
        if (col == 1)
            dataClasses[index.row()].setCity(value.toString());
        if (col == 2)
            dataClasses[index.row()].setAddress(value.toString());
        if (col == 3)
            dataClasses[index.row()].setBuildingDate(value.toString());
        if (col == 4)
            dataClasses[index.row()].setCapacity(value.toInt());
        if (col == 5)
            dataClasses[index.row()].setWorkingHours(value.toString());
        if (col == 6)
            dataClasses[index.row()].setWebsiteAddress(value.toString());

        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        return true;
    }
    return false;
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const {
    if (index.isValid())
        return  Qt::ItemIsEnabled |
                Qt::ItemIsSelectable |
                Qt::ItemIsEditable |
                Qt::ItemIsDragEnabled;
    else
        return Qt::NoItemFlags;
}

bool DataModel::insertRow(int position, const DataClass &value) {
    beginInsertRows(QModelIndex(), position, position);

    dataClasses.insert(position, value);

    endInsertRows();

    return true;
}

bool DataModel::removeRow(const int position) {
    if (dataClasses.isEmpty()) {
        return false;
    }

    beginRemoveRows(QModelIndex(), position, position);

    dataClasses.removeAt(position);

    endRemoveRows();

    return true;
}

const QList<DataClass> &DataModel::getData() const {
    return dataClasses;
}

QModelIndex DataModel::index(int row, int column, const QModelIndex &) const {
    return createIndex(row, column);
}

QVariant DataModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0:
            return QString(tr("Name"));
        case 1:
            return QString(tr("City"));
        case 2:
            return QString("Address");
        case 3:
            return QString("Building Date");
        case 4:
            return QString("Capacity");
        case 5:
            return QString("Working Hours");
        case 6:
            return QString("Website Address");

        }
    }

    if (orientation == Qt::Vertical && role == Qt::DisplayRole)
        return QString::number(section + 1);

    return QVariant();

}

void DataModel::onColumnSelected(int index) {//4
    selectedColumn = index;
}

int DataModel::getSelectedColumn() {//5
    return selectedColumn;
}
