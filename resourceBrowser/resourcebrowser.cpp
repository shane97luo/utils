#include "resourcebrowser.h"

#include <QStandardItemModel>
#include <QDirModel>
#include <QFileSystemModel>
#include <QDebug>
#include <QHeaderView>

#include <QTreeView>
#include <QTableView>
#include <QVBoxLayout>

ResourceBrowser::ResourceBrowser(QWidget *parent) : QWidget(parent)
{
    InitUI();

}

ResourceBrowser::~ResourceBrowser()
{

}

void ResourceBrowser::InitUI()
{
    QFileSystemModel *fileSysModel = new QFileSystemModel;
    fileSysModel->setRootPath(QString(""));

    toolWidget = new QWidget(this);
    toolWidget->setMaximumHeight(70);
    toolWidget->setMinimumHeight(70);

    mMainWidget = new QSplitter(this);

    mTableView = new QTableView(mMainWidget);
    mTreeView = new QTreeView(mMainWidget);
    mMainWidget->addWidget(mTreeView);
    mMainWidget->addWidget(mTableView);

//    mMainWidget->layout()->setSpacing(0);
    mMainWidget->setStretchFactor(0, 1);
    mMainWidget->setStretchFactor(1, 4);

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(toolWidget);
    lay->addWidget(mMainWidget);
    this->setLayout(lay);

    mTableView->setModel(fileSysModel);
    //mTableView->setRootIndex(fileSysModel->index(QDir::currentPath()));
    mTableView->setRootIndex(fileSysModel->index(""));
    mTableView->verticalHeader()->setHidden(true);

    mTreeView->setModel(fileSysModel);
    mTreeView->setColumnHidden(1, true);
    mTreeView->setColumnHidden(2, true);
    mTreeView->setColumnHidden(3, true);
    mTreeView->setRootIndex(fileSysModel->index(""));

    connect(mTreeView, &QTreeView::clicked, this,
            &ResourceBrowser::SlotEnterTableviewDirCurIndex);

    connect(mTreeView, &QTableView::doubleClicked, this,
            &ResourceBrowser::SlotEnterTableviewDirCurIndex);

    this->resize(1000, 700);

}

void ResourceBrowser::SlotEnterTreeviewDirCurIndex(const QModelIndex &index)
{

}

void ResourceBrowser::SlotEnterTableviewDirCurIndex(const QModelIndex &index)
{
    qDebug()<<"index: "<<  index.row()<< index.column();

    // 当前行的第一个index
    QModelIndex firstIndex = index.sibling(index.row(), 0);

    QFileSystemModel *fsm = static_cast<QFileSystemModel* >(mTreeView->model());

    // 选中是目录时，进入，否则不做动作
    if(fsm->isDir(firstIndex))
        mTableView->setRootIndex(firstIndex);

}
