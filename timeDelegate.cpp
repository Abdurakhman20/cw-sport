#include "timeDelegate.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLineEdit>

QWidget* TimeDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QLineEdit* editor = new QLineEdit(parent);
    QRegularExpression regex("\\d{1,2}\\.\\d{2}-\\d{1,2}\\.\\d{2}");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, editor);
    editor->setValidator(validator);
    return editor;
}

void TimeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void TimeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}
