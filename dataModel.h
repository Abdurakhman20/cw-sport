#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractTableModel>
#include <QList>

#include "dataClass.h"


class DataModel : public QAbstractTableModel
{
    Q_OBJECT
    QList<DataClass> dataClasses;
    const int numOfColumns;
public:
    explicit DataModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRow(int position, const DataClass &value);
    bool removeRow(const int position);
    void sort(int column, Qt::SortOrder order) override;
    const QList<DataClass> &getData() const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
};
#endif // DATAMODEL_H
