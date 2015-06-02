#include "browseprimitivesdialog.h"
#include "ui_browseprimitivesdialog.h"

#include <Qt>
#include <QtWidgets>

#include "mainwindow.h"

#include "primitivesloader.h"
#include "base.h"
#include "primitive.h"

BrowsePrimitivesDialog::BrowsePrimitivesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BrowsePrimitivesDialog), mBases(nullptr), projectPrimitivesPath(nullptr),
    primitivesLoader(new PrimitivesLoader()),
    basesListModel(new QStringListModel()), primitivesListModel(new QStringListModel())
{
    ui->setupUi(this);

    mParent = (MainWindow*) parent;

    connect(ui->loadButton, &QPushButton::clicked, this, &BrowsePrimitivesDialog::loadBase);
    connect(ui->unloadButton, &QPushButton::clicked, this, &BrowsePrimitivesDialog::unloadBase);
    connect(this, &BrowsePrimitivesDialog::basesListChanged, this, &BrowsePrimitivesDialog::setBasesList);
    connect(ui->basesListView, &QListView::clicked, this, &BrowsePrimitivesDialog::setPrimitivesList);
    connect(ui->buttonBox, &QDialogButtonBox::clicked, this, &BrowsePrimitivesDialog::handleButtonBox);
}

BrowsePrimitivesDialog::BrowsePrimitivesDialog(QVector<Base*>*& bases, QString* projectDirectory, QWidget* parent) : BrowsePrimitivesDialog(parent)
{
    mBases = new QVector<Base*>(*bases);
    projectPrimitivesPath = new QString(*projectDirectory);
    projectPrimitivesPath->append("/Primitives");

    basesList = new QStringList();
    primitivesList = new QStringList();
    basesList->clear();
    for (const auto& base : *mBases){
        basesList->append(*(base->getName()));
    }
    emit basesListChanged();
}

BrowsePrimitivesDialog::~BrowsePrimitivesDialog()
{
    delete ui;
    delete basesList;
    delete primitivesList;
}

void BrowsePrimitivesDialog::setUnloadEnabled(bool enabled)
{
    ui->unloadButton->setEnabled(enabled);
}

void BrowsePrimitivesDialog::loadBase()
{
    QFileDialog dialog(
                this, tr("Load Primitives Base"),
                QDir::homePath(),
                tr("NETDesigner Base Primitives Directories"));
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    QString filePath = QFileDialog::getExistingDirectory(this, tr("Load Primitives Base"),
                                                         QDir::homePath(),
                                                         QFileDialog::ShowDirsOnly);
    QDir origin(filePath);
    QDir path(*projectPrimitivesPath);
    if (!path.exists())
        path.mkpath(".");
    if (!path.exists()){
        qCritical("Could noy create directory %s", path.absolutePath().toLocal8Bit().constData());
        return;
    }
    if (!path.cd(origin.dirName())){
        if (!path.mkdir(origin.dirName())){
            qCritical("Could not create Base folder Primitives in project directory!");
            return;
        } else {
            path.cd(origin.dirName());
        }
    }
    if (!QFile::copy(origin.absoluteFilePath(origin.dirName() + ".info"), path.absoluteFilePath(origin.dirName() + ".info"))){
        qCritical("Could not copy info file!");
        return;
    }
    if (!QFile::copy(origin.absoluteFilePath(origin.dirName() + ".cpp"), path.absoluteFilePath(origin.dirName() + ".cpp"))){
        qCritical("Could not src info file!");
        return;
    }

    mBases->append(primitivesLoader->loadBase(new QString(origin.dirName()),
                                              new QString(path.absoluteFilePath(origin.dirName()).append(".info")),
                                              new QString(path.absoluteFilePath(origin.dirName()).append(".cpp"))));

    basesList->append(origin.dirName());
    emit basesListChanged();
}

void BrowsePrimitivesDialog::unloadBase()
{
    QString baseName = ui->basesListView->currentIndex().data().toString();
    QDir dir(projectPrimitivesPath->append("/") + baseName);
    if (!dir.removeRecursively()){
        qCritical("Could not remove Base directory in project's primitives location!");
        return;
    }
    Base* toRemove;
    for (const auto& base : *mBases){
        if (base->getName() == baseName){
            toRemove = base;
            break;
        }
    }
    mBases->removeAll(toRemove);

    basesList->removeAll(ui->basesListView->currentIndex().data().toString());
    emit basesListChanged();
}

void BrowsePrimitivesDialog::setBasesList()
{
    basesListModel->setStringList(*basesList);
    ui->basesListView->setModel(basesListModel);

    setPrimitivesList(ui->basesListView->currentIndex());
}

void BrowsePrimitivesDialog::setPrimitivesList(const QModelIndex& index)
{
    Base* currentBase = nullptr;
    for (const auto& base : *mBases){
        if (base->getName() == index.data().toString()){
            currentBase = base;
            break;
        }
    }
    primitivesList->clear();
    if (currentBase){
        for (const auto& primitive : *currentBase->primitives()){
            primitivesList->append(*primitive->definition());
        }
    }
    primitivesListModel->setStringList(*primitivesList);
    ui->primitivesListView->setModel(primitivesListModel);

    if (currentBase)
        setUnloadEnabled(true);
    else
        setUnloadEnabled(false);
}

void BrowsePrimitivesDialog::handleButtonBox(QAbstractButton* button)
{

    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok){
        mParent->setBases(mBases);
        this->close();
    } else if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Apply){
        mParent->setBases(mBases);
    } else if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Cancel) {
        this->close();
    }
}
