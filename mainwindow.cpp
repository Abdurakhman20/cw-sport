#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QStandardItem>
#include <QTableWidgetItem>
#include <QSettings>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tableModel(new DataModel(this))
    , searchTableModel(new DataModel(this))
    , proxyModel(new QSortFilterProxyModel(this))
{
    ui->setupUi(this);

    setSettings();

    ui->tableView->setSortingEnabled(true);

    ui->tableView->setModel(tableModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
}

MainWindow::~MainWindow()
{
    getSettings();
    delete ui;
    delete tableModel;
}

void MainWindow::getSettings() {
    QSettings settings("Settings", "MySettings");

    settings.beginGroup("MainWindowGeometry");
    settings.setValue("size", size());
    settings.setValue("position", pos());
    settings.endGroup();
}

void MainWindow::setSettings() {
    QSettings settings("Settings", "MySettings");

    settings.beginGroup("MainWindowGeometry");
    resize(settings.value("size").toSize());
    move(settings.value("position").toPoint());
    settings.endGroup();
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "OpenFile", QDir::cleanPath("./.."), "Text Files (*.txt *.db)");
    if (!filePath.isEmpty())
        loadFile(filePath);
}

void MainWindow::loadFile(const QString &filePath) {
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,
                             tr("Application!"),
                             tr("Cannot read file %1:\n%2!")
                             .arg(filePath)
                             .arg(file.errorString()));
        return;
    }
    proxyModel->setSourceModel(tableModel);
    ui->tableView->setModel(proxyModel);

    openFileFlag = true;
    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);

    //    qDebug() << tableModel->rowCount();

    int rowCount = tableModel->rowCount();
    for (int row = rowCount-1; row >= 0; row--) {
        tableModel->removeRow(row);
    }

    int i = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(";");
        DataClass newDataClass;

        newDataClass.setID(fields[0].toInt());
        newDataClass.setName(fields[1]);
        newDataClass.setCity(fields[2]);
        newDataClass.setAddress(fields[3]);
        newDataClass.setBuildingDate(fields[4]);
        newDataClass.setCapacity(fields[5].toInt());
        newDataClass.setWorkingHours(fields[6]);
        newDataClass.setWebsiteAddress(fields[7]);
        tableModel->insertRow(i, newDataClass);
        i++;
    }

    initTable();
    QApplication::restoreOverrideCursor();
}

void MainWindow::initTable() {
    QList<DataClass> dataClasses = tableModel->getData();

    for (int row = 0; row < tableModel->rowCount(); row++) {
        tableModel->setData(tableModel->index(row, 0), dataClasses[row].getName());
        tableModel->setData(tableModel->index(row, 1), dataClasses[row].getCity());
        tableModel->setData(tableModel->index(row, 2), dataClasses[row].getAddress());
        tableModel->setData(tableModel->index(row, 3), dataClasses[row].getBuildingDate());
        tableModel->setData(tableModel->index(row, 4), dataClasses[row].getCapacity());
        tableModel->setData(tableModel->index(row, 5), dataClasses[row].getWorkingHours());
        tableModel->setData(tableModel->index(row, 6), dataClasses[row].getWebsiteAddress());
    }
}

void MainWindow::on_actionSave_triggered()
{
    QString filePathAndName = QFileDialog::getSaveFileName(this);

        if (!filePathAndName.isEmpty())
            saveFile(filePathAndName);
}

void MainWindow::saveFile(const QString &filePathAndName) {
    QFile file(filePathAndName);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this,
                             tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(filePathAndName).arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    QList<DataClass> dataClasses = tableModel->getData();

    for (DataClass dataClass : dataClasses) {

        out << dataClass.getID() << ";"
            << dataClass.getName() << ";"
            << dataClass.getCity() << ';'
            << dataClass.getAddress() << ";"
            << dataClass.getBuildingDate() << ";"
            << dataClass.getCapacity() << ";"
            << dataClass.getWorkingHours() << ";"
            << dataClass.getWebsiteAddress() << "\n";
    }
}

void MainWindow::on_tableView_customContextMenuRequested(const QPoint &pos)
{
    QMenu *menu=new QMenu(this);
    QModelIndex index=ui->tableView->indexAt(pos);

    QAction *deleteAction = new QAction("Удалить строчку", this);
    QAction *addAction = new QAction(tr("Добавить строчку"), this);

    menu->addAction(deleteAction);
    menu->addAction(addAction);
    QAction *selectedItem = menu->exec(ui->tableView->viewport()->mapToGlobal(pos));

    if (selectedItem == deleteAction)
        tableModel->removeRow(index.row());
    else if(selectedItem == deleteAction) {
        DataClass newDataClass;
        newDataClass.setID(tableModel->rowCount()+1);
        tableModel->insertRow(tableModel->rowCount(),newDataClass);
    }
}

// Search

void MainWindow::on_comboBox_activated(int index)
{
    tableModel->onColumnSelected(index);
}

void MainWindow::on_pushButton_clicked()
{
    QList<DataClass> dataClasses = tableModel->getData();
    QString searchText = ui->textEdit->toPlainText();
    if(searchText == "") return;
    ui->textEdit->clear();
    int columnInd = tableModel->getSelectedColumn();

    int count = 0;
    for (int row = 0; row < tableModel->rowCount(); row++) {
        QString value;
        switch (columnInd) {
        case 0:
            value = dataClasses[row].getName();
            break;
        case 1:
            value = dataClasses[row].getCity();
            break;
        case 2:
            value = dataClasses[row].getAddress();
            break;
        case 3:
            value = dataClasses[row].getBuildingDate();
            break;
        case 4:
            value = dataClasses[row].getCapacity();
            break;
        case 5:
            value = dataClasses[row].getWorkingHours();
            break;
        case 6:
            value = dataClasses[row].getWebsiteAddress();
            break;
        default:
            break;
        }

        if(searchText == value) {
            DataClass newDataClass;
            newDataClass.setID(count);
            newDataClass.setName(dataClasses[row].getName());
            newDataClass.setCity(dataClasses[row].getCity());
            newDataClass.setAddress(dataClasses[row].getAddress());
            newDataClass.setBuildingDate(dataClasses[row].getBuildingDate());
            newDataClass.setCapacity(dataClasses[row].getCapacity());
            newDataClass.setWorkingHours(dataClasses[row].getWorkingHours());
            newDataClass.setWebsiteAddress(dataClasses[row].getWebsiteAddress());
            searchTableModel->insertRow(count, newDataClass);
            count++;

            for(int newRow = 0; newRow < searchTableModel->rowCount(); newRow++) {
                searchTableModel->setData(searchTableModel->index(newRow, 0), dataClasses[row].getName());
                searchTableModel->setData(searchTableModel->index(newRow, 1), dataClasses[row].getCity());
                searchTableModel->setData(searchTableModel->index(newRow, 2), dataClasses[row].getAddress());
                searchTableModel->setData(searchTableModel->index(newRow, 3), dataClasses[row].getBuildingDate());
                searchTableModel->setData(searchTableModel->index(newRow, 4), dataClasses[row].getCapacity());
                searchTableModel->setData(searchTableModel->index(newRow, 5), dataClasses[row].getWorkingHours());
                searchTableModel->setData(searchTableModel->index(newRow, 6), dataClasses[row].getWebsiteAddress());
            }

        }
    }
    if(searchTableModel->rowCount() == 0){
        QMessageBox::warning(this, tr("Application"), tr("Couldn't find a company with that value!"));
        return;
    }
    ui->tableView->setModel(searchTableModel);
}


void MainWindow::on_pushButton_2_clicked()
{
    int rowCount = searchTableModel->rowCount();
    for (int row = rowCount-1; row >= 0; row--) {
        searchTableModel->removeRow(row);
    }

    ui->tableView->setModel(tableModel);
}




