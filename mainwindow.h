#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <vector>
#include <QSortFilterProxyModel>

#include "dataClass.h"
#include "dataModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_tableView_customContextMenuRequested(const QPoint &pos);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_comboBox_activated(int index);

private:
    Ui::MainWindow *ui;
    DataModel *tableModel;
    DataModel *searchTableModel;
    QSortFilterProxyModel *proxyModel;
    bool openFileFlag = false;

    void getSettings();
    void setSettings();
    void initTable();
    void setHeaders();
    void loadFile(const QString &filePathAndName);
    void saveFile(const QString &filePathAndName);

    void showContextMenu(const QPoint& pos);

};

#endif // MAINWINDOW_H
