#ifndef TIMEDELEGATE_H
#define TIMEDELEGATE_H

#include <QStyledItemDelegate>

class TimeDelegate : public QStyledItemDelegate {
public:
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;
};

#endif // TIMEDELEGATE_H
