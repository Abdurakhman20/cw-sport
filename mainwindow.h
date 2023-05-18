#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <vector>
#include <QSortFilterProxyModel>
#include <QRegExp>
#include <QTranslator>
#include "dataClass.h"
#include "dataModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/// Делаем предварительное(опережающее, упреждающее) объявление
class QActionGroup;
class QLabel;


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

    void on_comboBox_activated(int index);

    void on_lineEdit_textChanged(const QString &arg1);

    /// Слот (метод) переключает язык программы
    void switchLanguage(QAction *action);


    void on_actionAbout_the_Developer_triggered();

private:
    Ui::MainWindow *ui;
    DataModel *tableModel;
    QSortFilterProxyModel *proxyModel;

    bool openFileFlag = false;
    void getSettings();
    void setSettings();
    void initTable();
    void setHeaders();
    void loadFile(const QString &filePathAndName);
    void saveFile(const QString &filePathAndName);
    void showContextMenu(const QPoint& pos);


    QTranslator appTranslator;
    QTranslator qtTranslator;

    /// Список доступных переводов в меню будет формироваться динамически
    QActionGroup *languageActionGroup;
    QLabel *label;
    /// Переменная где будет храниться путь к файлам перевода "*.qm"
    QString qmPath;

    /// Метод для создания языкового меню
    void createLanguageMenu();

};

#endif // MAINWINDOW_H
