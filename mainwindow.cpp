#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dateDelegate.h"
#include "timeDelegate.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QStandardItem>
#include <QTableWidgetItem>
#include <QSettings>
#include <QLibraryInfo>
#include <QLabel>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , tableModel(new DataModel(this))
    , proxyModel(new QSortFilterProxyModel(this))
    , languageActionGroup(nullptr)
    , label(new QLabel(this))

{
    ui->setupUi(this);

    setSettings();

    ui->tableView->setSortingEnabled(true);

    ui->tableView->setModel(tableModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->actionSave->setEnabled(false); /// Здесь мы отключаем пункт меню save, до открытия файла
    ui->actionClose->setEnabled(false);/// Здесь мы отключаем пункт меню close, до открытия файла


    connect(ui->actionClose, &QAction::triggered,
        this, &MainWindow::close);
    connect(ui->actionAboutQt, &QAction::triggered,
        qApp, &QApplication::aboutQt);


    qApp->installTranslator(&appTranslator);
    qApp->installTranslator(&qtTranslator);

    qmPath = qApp->applicationDirPath() + "/translations";
    createLanguageMenu();

}

MainWindow::~MainWindow()
{
    getSettings();
    delete ui;
    delete tableModel;
}


void MainWindow::createLanguageMenu()
{
    /// Создаём список пунктов выбора языка в динамической памяти
    languageActionGroup = new QActionGroup(this);

    /// Ставим связь пунктов меню со слотом смены языка приложения
    connect(languageActionGroup, &QActionGroup::triggered,
            this, &MainWindow::switchLanguage);

    /// Определяем каталог, где лежат файлы переводов "*.qm"
    QDir dir(qmPath);

    /**
     * Получаем список файлов "*.qm" в каталоге, которые относятся
     * к нашей программе по шаблону "multilang_*.qm",
     * где "multilang" - название нашего приложения
     * "_" - разделитель
     * "*" - означает любой символ или группа символ произвольной длинны
     * ".qm" - расширение файла
     */
    QStringList fileNames =
            dir.entryList(QStringList("sport-project_*.qm"));

    /**
     * Количество пунктов меню нам заранее не известно,
     * но так как оно зависит от количества файлов "*.qm"
     * в каталоге "translations",
     * то мы можем сформировать в меню столько пунктов,
     * сколько файлов у нас, используя цикл.
     */
    for (int i = 0; i < fileNames.size(); i++) {
        /// Получаем i-ую локаль
        QString locale = fileNames[i];
        /// Удаляем символы в строке сначала и по символ "_" включительно
        locale.remove(0, locale.indexOf('_') + 1);
        /// Удаляем символы в строке начиная с символа точки
        locale.truncate(locale.lastIndexOf('.'));

        /// Создаём временную переменную перевода для языкового пункта меню
        QTranslator translator;
        /// Загружаем перевод из файла перевода "*.qm"
        translator.load(fileNames[i], qmPath);

        /**
         * Создаём имя i-ого пункта меню с названием языка путём
         * перевода в linguist заменив English на соответствующий язык
         */
        QString language = translator.translate("MainWindow",
                                                "English");

        /**
         * Создаём пункт в меню с i-ым языком по маске "&%1 %2",
         * где "&" - символ быстрого доступа к пункту меню через сочетание
         * клавиш Alt+символ_перед_которым_стоит_&;
         * "%1" - номер i-ого пункта меню по порядку.
         * "%2" - название языка, которое будет отображаться в меню.
         */
        QAction *action = new QAction(tr("&%1 %2")
                                      .arg(i + 1)
                                      .arg(language),
                                      this);

        /**
         * Задаём свойства для i-ого пункта меню.
         * Возможность держать пункт меню выбранным
         * пока пользователем не будет выбран другой пункт меню.
         */
        action->setCheckable(true);
        /// Задаём внутренние данные хранимые в i-ом пункте.
        /// В нашем случае это локаль вида "ru_RU"
        action->setData(locale);

        /// Добавляем i-ый пункт в меню на нашей форме "mainwindow.ui"
        ui->menuLanguage->addAction(action);
        /// Добавляем i-ый пункт в единую группу пунктов
        languageActionGroup->addAction(action);

        /// Задаём  английский язык в качестве выбранного по умолчанию
        if (language == "English")
            action->setChecked(true);
    }
}

void MainWindow::switchLanguage(QAction *action)
{
    /**
     * Определяем локаль которую выбрал пользователь.
     * Например "ru_RU" для русского языка в России или
     * "en_US" для английского в США
     */
    QString locale = action->data().toString();

    /// Загружаем тот перевод который выбрал пользователь
    appTranslator.load("sport-project_" + locale, qmPath);
    locale.chop(3);
    /**
     * Для переводов стандартных сообщений Qt можно запросить у системы
     * их местонахождение через вызов "QLibraryInfo::TranslationsPath"
     */
    qtTranslator.load("qt_" + locale + ".qm", QLibraryInfo::location(
                          QLibraryInfo::TranslationsPath));



    /**
     * Взываем "retranslateUi()" для обновления интерфейса приложения
     * используя загруженный из соответствующего файла "qm" язык
     */
    ui->retranslateUi(this);

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

    /// Установить делегата для столбца с датой
    DateDelegate* dateDelegate = new DateDelegate();
    ui->tableView->setItemDelegateForColumn(3, dateDelegate);

    TimeDelegate* timeDelegate = new TimeDelegate();
    ui->tableView->setItemDelegateForColumn(5, timeDelegate);

    openFileFlag = true;
    QTextStream in(&file);
    in.setCodec("UTF-8");

    QString check = in.readLine();
    if(check != "adr_usm") {
        QMessageBox::warning(this, tr("Application!"), tr("Unsupported file!"));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

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
    ui->actionSave->setEnabled(openFileFlag);
    ui->actionClose->setEnabled(openFileFlag);
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
    out.setCodec("UTF-8");
    QList<DataClass> dataClasses = tableModel->getData();
    out << "adr_usm\n";

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

    QAction *deleteAction = new QAction(tr("Remove row"), this);
    QAction *addAction = new QAction(tr("Add row"), this);

    menu->addAction(deleteAction);
    menu->addAction(addAction);
    if(!openFileFlag) {
        menu->setEnabled(false);
    }
    QAction *selectedItem = menu->exec(ui->tableView->viewport()->mapToGlobal(pos));
    if (selectedItem == deleteAction)
        tableModel->removeRow(index.row());
    else if(selectedItem == addAction) {
        DataClass newDataClass;
        newDataClass.setID(tableModel->rowCount()+1);
        tableModel->insertRow(tableModel->rowCount(),newDataClass);

    }
}

// Search

void MainWindow::on_comboBox_activated(int index)
{
    proxyModel->setFilterKeyColumn(index);
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{
    QRegularExpression regex(arg1, QRegularExpression::CaseInsensitiveOption);
       proxyModel->setFilterRegularExpression(regex);

}


void MainWindow::on_actionAbout_the_Developer_triggered()
{
    QMessageBox::about(this, tr("About the developer"),
            tr("Developer: Usmanov Abdurakhman Khasmagamedovich\n"
            "E-mail: ausmanov706@gmail.com\n"
            "Faculty: Information Systems and Technologies\n"
            "Group: ICTMS-2-5"));
}


void MainWindow::on_actionNew_triggered()
{
    /// Запрашиваем у пользователя путь и имя нового файла через диалоговое окно
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"),
                                                    QDir::cleanPath("./.."), tr("Text files (*.txt *db)"));
    /// Если пользователь не выбрал имя файла, выходим из функции
    if (fileName.isEmpty()) {
        return;
    }
    /// Создаем новый файл с заданным именем в указанной директории
    QFile file(fileName);
    /// Если не удалось создать файл, выводим сообщение об ошибке и выходим из функции
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Application"), tr("Cannot create file %1:\n%2").arg(file.errorString()));
        return;
    }
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out << "adr_usm\n";
}

