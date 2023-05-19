#include "dateDelegate.h"
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QLineEdit>

QWidget* DateDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QLineEdit* editor = new QLineEdit(parent);
    QRegularExpression regex("[0-3][0-9]-[0-1][0-9]-[0-9]{4}");
    QRegularExpressionValidator* validator = new QRegularExpressionValidator(regex, editor);
    editor->setValidator(validator);
    return editor;
}

void DateDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    QString value = index.model()->data(index, Qt::EditRole).toString();
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}

void DateDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const {
    QLineEdit* lineEdit = static_cast<QLineEdit*>(editor);
    QString value = lineEdit->text();
    model->setData(index, value, Qt::EditRole);
}
