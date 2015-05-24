#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui>
#include <QtWidgets>

#include "compiler.h"

#include "base.h"
#include "primitivesloader.h"

#include "browseprimitivesdialog.h"
#include "programoptionsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), projectDirectory(nullptr), projectBases(nullptr), netsListModel(new QStringListModel(this)), mModified(false), tabIndex(0), mainNetName(nullptr)

{
    QLibrary libGraph("libGraph");
    libGraph.load();
    if (!libGraph.isLoaded()){
        qCritical("Cannot load libGraph");
        qApp->exit(qApp->exec());
    }

    QLibrary libCompiler("libCompiler");
    libCompiler.load();
    if (!libGraph.isLoaded()){
        qCritical("Cannot load libCompiler");
        qApp->exit(qApp->exec());
    }

    QLibrary libPrimitivesLoader("libPrimitivesLoader");
    libPrimitivesLoader.load();
    if (!libPrimitivesLoader.isLoaded()){
        qCritical("Cannot load libPrimitivesLoader");
        qApp->exit(qApp->exec());
    }

    ui->setupUi(this);
    this->showMaximized();

    connect(ui->actionNew_Project, &QAction::triggered, this, &MainWindow::createProject);
    connect(ui->actionOpen_Project, &QAction::triggered, this, &MainWindow::openProject);
    connect(ui->actionSave_Project, &QAction::triggered, this, &MainWindow::saveProject);
    connect(ui->actionClose_Project, &QAction::triggered, this, &MainWindow::closeProject);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exit);

    connect(ui->actionAdd_Net, &QAction::triggered, this, &MainWindow::addNet);
    connect(ui->actionRemove_Net, &QAction::triggered, this, &MainWindow::removeNet);
    connect(ui->actionSave_Current_Net, &QAction::triggered, this, static_cast<void(MainWindow::*)()>(&MainWindow::saveNet));
    connect(ui->actionSave_All_Nets, &QAction::triggered, this, &MainWindow::saveAllNets);
    connect(ui->actionBrowse_Primitives_Bases, &QAction::triggered, this, &MainWindow::browsePrimitives);
    connect(ui->actionProgram_Options, &QAction::triggered, this, &MainWindow::programOptions);

    connect(ui->actionBuild, &QAction::triggered, this, &MainWindow::build);
    connect(ui->actionRun, &QAction::triggered, this, &MainWindow::run);
    connect(ui->actionDebug, &QAction::triggered, this, &MainWindow::debug);

    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::help);
    connect(ui->actionAbout, &QAction::triggered, this, &MainWindow::about);

    connect(this, &MainWindow::projectLoaded, this, &MainWindow::projectLoad);
    connect(this, &MainWindow::projectUnloaded, this, &MainWindow::projectUnload);

    connect(ui->netsListView, &QListView::clicked, this, &MainWindow::openNet);
    connect(ui->editorTabWidget, &QTabWidget::tabCloseRequested, this, &MainWindow::closeNet);

    connect(ui->editorTabWidget, &QTabWidget::currentChanged, this, &MainWindow::checkTabModified);

    connect(this, &MainWindow::netCountModified, this, &MainWindow::enableOrDisableRemoveNet);

    netsListModel->setStringList(netsList);
    ui->netsListView->setModel(netsListModel);


    browsePrimitivesDialog = nullptr;
}

MainWindow::~MainWindow()
{
    delete projectDirectory;
    delete mainNetName;
    delete projectBases;
    delete ui;
}

void MainWindow::setBases(QVector<Base*>* newBases)
{
    QVector<Base*>* tmp = projectBases;
    projectBases = newBases;
    delete tmp;
}

void MainWindow::createProject()
{

    if (projectDirectory){
        closeProject();
        if (projectDirectory)
            return;
    }

    QFileDialog dialog(
                this, tr("Create New Project"),
                QDir::homePath(),
                tr("NETDesigner Project Directories"));
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setFileMode(QFileDialog::Directory);

    if (dialog.exec() == QDialog::Accepted){

        QDir dir = dialog.selectedFiles()[0];

        if (dir.exists()){
            if (!(dir.entryInfoList(QDir::NoDotAndDotDot|QDir::AllEntries).count() == 0)){
                if (!QFile(QString(dir.absolutePath() + "/net.project")).exists()){
                    QErrorMessage msg;
                    msg.showMessage(tr("Invalid directory!"));
                    return;
                }
            }
        } else {
            dir.mkpath(".");
            if (!dir.exists()){
                QErrorMessage msg;
                msg.showMessage(tr("Cannot create given directory!"));
                return;
            }
        }
        QFile file(QString(dir.absolutePath() + "/net.project"));
        file.open(QFile::WriteOnly | QIODevice::Text);
        file.close();
        if (!file.exists()){
            QErrorMessage msg;
            msg.showMessage(tr("Cannot write in given directory!"));
            return;
        }
        projectDirectory = new QString(dir.absolutePath());
        projectBases = new QVector<Base*>();

        solutions = 0;
        loops = 0;
        recursion = 0;
        strategyBF = false;
        backtracking = true;
        maxPathLength = 0;
        executeBack = true;
        oneArrow = false;
        mainNetName = new QString();

        saveProject();

        emit projectLoaded();
    }
}

void MainWindow::openProject()
{
    if (projectDirectory){
        closeProject();
        if (projectDirectory)
            return;
    }

    QString path = QFileDialog::getOpenFileName(this, tr("Open Project"),
                                                QDir::homePath(),
                                                tr("NETDesigner Projects (net.project)"));
    if (path.isEmpty())
        return;

    QFile projectFile(path);

    if (!projectFile.open(QFile::ReadOnly | QIODevice::Text)){
        QErrorMessage msg;
        msg.showMessage(tr("Cannot read from project file!"));
        return;
    }

    QTextStream stream(&projectFile);

    mainNetName = new QString(stream.readLine());

    QString optionsStr = stream.readLine();
    QStringList options = optionsStr.split(":", QString::SkipEmptyParts);

    solutions = options[0].toInt();
    loops = options[1].toInt();
    recursion = options[2].toInt();
    strategyBF = (bool)(options[3].toInt());
    backtracking = (bool)(options[4].toInt());
    maxPathLength = options[5].toInt();
    executeBack = (bool)(options[6].toInt());
    oneArrow = (bool)(options[7].toInt());

    projectBases = new QVector<Base*>();

    projectFile.close();

    path.remove("/net.project");
    projectDirectory = new QString(path);

    QStringList filters;

    filters << "*.net";
    QDir projectDir(*projectDirectory);
    QStringList files = projectDir.entryList(filters, QDir::Files);

    for (auto& file : files){
        file.remove(".net");
        netsList.append(file);
    }

    netsListModel->setStringList(netsList);

    ui->netsListView->setModel(netsListModel);

    emit netCountModified();

    // load primitive bases

    QDir primitivesDir(*projectDirectory);
    if (primitivesDir.cd("Primitives")){
        PrimitivesLoader loader;
        for (const auto& base : primitivesDir.entryInfoList()){
            if (base.fileName() == "." || base.fileName() == "..")
                continue;
            QString infoPath = base.absoluteFilePath();
            infoPath.append("/").append(base.fileName()).append(".info");
            QString srcPath = base.absoluteFilePath();
            srcPath.append("/").append(base.fileName()).append(".cpp");
            projectBases->append(loader.loadBase(new QString(base.fileName()), &infoPath, &srcPath));
        }
    }

    // all done

    emit projectLoaded();

}

void MainWindow::closeProject()
{
    if (ui->actionSave_Project->isEnabled() || ui->actionSave_All_Nets->isEnabled()){
        QMessageBox::StandardButton confirm;
        confirm = QMessageBox::question(
                    this, tr("NETDesigner"), tr("You have unsaved data! Do you want to save before you quit?"),
                    QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (confirm == QMessageBox::Yes){
            saveProject();
            if (ui->actionSave_Project->isEnabled())
                return;
        } else if (confirm == QMessageBox::Cancel){
            return;
        }
    }

    delete browsePrimitivesDialog;

    emit projectUnloaded();
}

void MainWindow::saveProject()
{
    QFile projectFile(QString(*projectDirectory + "/net.project"));

    projectFile.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text);

    QTextStream stream(&projectFile);

    stream << *mainNetName;

    stream << "\n" << solutions << ":" << loops << ":" << recursion << ":" << (int)strategyBF << ":" <<
              (int)backtracking << ":" << maxPathLength << ":" << (int)executeBack << ":" << (int)oneArrow;

    projectFile.close();

    saveAllNets();
    ui->actionSave_Project->setEnabled(false);
}

void MainWindow::exit()
{
    closeProject();
    qApp->exit(0);
}

void MainWindow::addNet()
{
    QString name = QInputDialog::getText(this, "Add Net", "Net name:");

    if (name.isEmpty() || name.contains(QRegularExpression("~`!@#$%^&*()_=+[{]}\\|\"\'<>/?")) ||
            !name.at(0).isLetter()){
        QErrorMessage msg;
        msg.showMessage(tr("Invalid Net Name!"));
        return;
    }

    QFile net(QString(*projectDirectory + "/" + name + ".net"));

    net.open(QFile::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    net.close();

    if (!net.exists()){
        QErrorMessage msg;
        msg.showMessage(tr("Could not write net!"));
        return;
    }

    netsList.append(name);

    emit netCountModified();
}

void MainWindow::openNet(QModelIndex index)
{

    for (int i = 0; i < ui->editorTabWidget->count(); ++i) {
        if (ui->editorTabWidget->tabText(i) == QString("&" + index.data().toString()))
        {
            ui->editorTabWidget->setCurrentIndex(i);
            return;
        }
    }

    QString netName = index.data().toString();

    Editor* editor = new Editor(ui->editorTabWidget);

    QFile file(QString(*projectDirectory + "/" +
                       netName + ".net").remove("&").remove("*"));

    file.open(QFile::Text | QFile::ReadOnly);

    QTextStream stream(&file);

    editor->load(stream);

    file.close();

    connect(editor, &Editor::modification, this, &MainWindow::modified);

    int newTabIndex = ui->editorTabWidget->addTab(editor, netName);
    ui->editorTabWidget->setCurrentIndex(newTabIndex);

    emit netCountModified();
}

void MainWindow::closeNet(int index)
{
    Editor* editor = ((Editor*)ui->editorTabWidget->widget(index));
    if (editor->isModified()){
        QMessageBox::StandardButton confirm;
        confirm = QMessageBox::question(this, tr("NETDesigner"), tr("Net modified. Do you want to save your progress?"),
                                        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

        if (confirm == QMessageBox::Yes){
            saveNet();
        }
        if (confirm == QMessageBox::No){
            saved();
        }
        if (confirm == QMessageBox::Cancel)
            return;
    }
    disconnect(editor, &Editor::modification, this, &MainWindow::modified);
    ui->editorTabWidget->removeTab(index);

    emit netCountModified();
}

void MainWindow::removeNet()
{
    QMessageBox::StandardButton confirm;
    confirm = QMessageBox::question(
                this, tr("NETDesigner"), tr("Are you sure you want to delete this net?"),
                QMessageBox::Yes|QMessageBox::No);
    QString netName = QString(ui->editorTabWidget->tabText(ui->editorTabWidget->currentIndex())).remove("&").remove("*");
    if (confirm == QMessageBox::Yes){
        QFile file(QString(*projectDirectory + "/" + netName + ".net"));
        if (!file.remove()){
            QErrorMessage msg;
            msg.showMessage(tr("Cannot delete net!"));
            return;
        }
    } else if (confirm == QMessageBox::No){
        return;
    }
    netsList.removeAll(netName);
    QWidget* currentWidget = ui->editorTabWidget->currentWidget();
    closeNet(ui->editorTabWidget->currentIndex());
    if (currentWidget == ui->editorTabWidget->currentWidget())
        return;

    emit netCountModified();
}

void MainWindow::saveAllNets()
{
    for (int i = 0; i < ui->editorTabWidget->tabBar()->count(); i++){
        Editor* editor = (Editor*)ui->editorTabWidget->widget(i);
        if (editor->isModified())
            saveNet(editor);
    }
    ui->actionSave_All_Nets->setEnabled(false);
}

void MainWindow::saveNet()
{
    saveNet((Editor*)ui->editorTabWidget->currentWidget());

    bool unsaved = false;
    for(int i = 0; i < ui->editorTabWidget->count(); ++i){
        if (((Editor*)(ui->editorTabWidget->widget(i)))->isModified())
            unsaved = true;
    }
    if (!unsaved)
        ui->actionSave_All_Nets->setEnabled(false);
}

void MainWindow::saveNet(Editor *editor)
{
    QFile file(QString(*projectDirectory + "/"
                       + ui->editorTabWidget->tabText(
                           ui->editorTabWidget->currentIndex()) + ".net").remove("&").remove("*"));
    file.open(QFile::Text | QFile::WriteOnly | QFile::Truncate);

    QTextStream stream(&file);

    editor->save(stream);

    file.close();

    saved();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    closeProject();

    if (projectDirectory)
        event->ignore();
    else
        event->accept();
}

void MainWindow::browsePrimitives()
{
    if (!browsePrimitivesDialog)
        browsePrimitivesDialog = new BrowsePrimitivesDialog(projectBases, projectDirectory, this);
    browsePrimitivesDialog->exec();
}

void MainWindow::programOptions()
{
    QStringList netsList;

    for (int i = 0; i < ui->netsListView->model()->rowCount(); ++i){
        netsList << ui->netsListView->model()->index( i, 0).data(Qt::DisplayRole).toString();
    }

    ProgramOptionsDialog dialog(solutions, loops, recursion, strategyBF, backtracking,maxPathLength, executeBack,
                                oneArrow, mainNetName, &netsList);
    if (dialog.exec() == QDialog::Accepted){
        dialog.getResults(solutions,loops,recursion,strategyBF,backtracking,maxPathLength,executeBack,oneArrow,mainNetName);
        ui->actionSave_Project->setEnabled(true);
    }
}

void MainWindow::build()
{
    Compiler compiler(projectBases, projectDirectory);
    QTextStream output;
    compiler.build(output);
}

void MainWindow::run()
{

}

void MainWindow::debug()
{

}

void MainWindow::enableOrDisableRemoveNet()
{
    if (ui->editorTabWidget->count() > 0){
        ui->actionRemove_Net->setEnabled(true);
    } else {
        ui->actionRemove_Net->setEnabled(false);
    }

    netsListModel->setStringList(netsList);
    ui->netsListView->setModel(netsListModel);
}

void MainWindow::projectLoad()
{
    ui->actionClose_Project->setEnabled(true);

    ui->menuProject->setEnabled(true);
    ui->actionAdd_Net->setEnabled(true);
    ui->actionBrowse_Primitives_Bases->setEnabled(true);
    ui->actionProgram_Options->setEnabled(true);

    ui->menuBuild_Run->setEnabled(true);
    ui->actionBuild->setEnabled(true);
    ui->actionRun->setEnabled(true);
    //ui->actionDebug->setEnabled(true);
}

void MainWindow::projectUnload()
{
    delete projectDirectory;
    projectDirectory = nullptr;
    delete projectBases;
    projectBases = nullptr;
    ui->actionClose_Project->setEnabled(false);
    ui->menuProject->setEnabled(false);
    ui->actionAdd_Net->setEnabled(false);
    ui->actionBrowse_Primitives_Bases->setEnabled(false);
    ui->actionProgram_Options->setEnabled(false);

    ui->menuBuild_Run->setEnabled(false);
    ui->actionBuild->setEnabled(false);
    ui->actionRun->setEnabled(false);
    ui->actionDebug->setEnabled(false);

    ui->editorTabWidget->clear();

    netsList.clear();
    emit netCountModified();
}

void MainWindow::help()
{
    QMessageBox::about(this, tr("NETDesigner help"), tr("Left mouse button anywhere on the canvas creates a node. Left-clicking that node again edits that node. Right-click to delete. Hover on any node to make the add-arrow button show. Click it and click on another node to create an arrow. Right-click the arrow to delete it, left-click to edit"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About NETDesigner"), tr("NETDesigner is an IDE for the Net language. The IDE is developed by Ivan Bratoev as a thesis project for his bachelor's degree in Computer Science in Ruse University Angel Kanchev"));
}

void MainWindow::modified()
{
    if (!mModified){
        ui->actionSave_All_Nets->setEnabled(true);
        checkTabModified(ui->editorTabWidget->currentIndex());

        int index = ui->editorTabWidget->currentIndex();
        int listViewIndex = netsList.indexOf(ui->editorTabWidget->tabText(index).remove("&"));
        ui->editorTabWidget->setTabText(index, "*" + ui->editorTabWidget->tabText(index));
        netsList.removeAt(listViewIndex);
        netsList.insert(listViewIndex, ui->editorTabWidget->tabText(index).remove("&"));

        netsListModel->setStringList(netsList);
        ui->netsListView->setModel(netsListModel);
        mModified = true;
    }
}

void MainWindow::saved()
{
    if (mModified){
        int index = ui->editorTabWidget->currentIndex();
        int listViewIndex = netsList.indexOf(ui->editorTabWidget->tabText(index).remove("&"));
        ui->editorTabWidget->setTabText(index, ui->editorTabWidget->tabText(index).remove("*"));
        netsList.removeAt(listViewIndex);
        netsList.insert(listViewIndex, ui->editorTabWidget->tabText(index).remove("&"));

        netsListModel->setStringList(netsList);
        ui->netsListView->setModel(netsListModel);

        mModified = false;

        checkTabModified(ui->editorTabWidget->currentIndex());
    }
}

void MainWindow::checkTabModified(int index)
{
    if (index == -1)
        return;
    if (((Editor*)ui->editorTabWidget->widget(index))->isModified())
        ui->actionSave_Current_Net->setEnabled(true);
    else
        ui->actionSave_Current_Net->setEnabled(false);
}
