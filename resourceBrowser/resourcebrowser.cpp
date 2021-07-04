#include "resourcebrowser.h"

#include <QStandardItemModel>
#include <QDirModel>
#include <QFileSystemModel>

#include <QHeaderView>
#include <QTreeView>
#include <QTableView>
#include <QVBoxLayout>

#include <QApplication>
#include <QDebug>
#include <memory>

#include "diroperate.h"
#include "file_op_thread.h"

ResourceBrowser::ResourceBrowser(QWidget *parent) : QWidget(parent)
{
    InitUI();
    DirOperate dirOperate;
    FileOpThread myThread;

    dirOperate.moveToThread(&myThread);

}

ResourceBrowser::~ResourceBrowser()
{

    if( toolWidget) delete toolWidget;
    if( mMainWidget) delete mMainWidget;
    if( mTableView) delete mTableView;
    if( mTreeView) delete mTreeView;

}

void ResourceBrowser::InitUI()
{
    auto fileSysModel = std::make_unique <QFileSystemModel>(new QFileSystemModel);
//    QFileSystemModel *fileSysModel = new QFileSystemModel;
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

    mTableView->setModel(fileSysModel.get());
    //mTableView->setRootIndex(fileSysModel->index(QDir::currentPath()));
    mTableView->setRootIndex(fileSysModel->index(""));
    mTableView->verticalHeader()->setHidden(true);

    mTreeView->setModel(fileSysModel.get());
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

    // ��ǰ�еĵ�һ��index
    QModelIndex firstIndex = index.sibling(index.row(), 0);

    QFileSystemModel *fsm = static_cast<QFileSystemModel* >(mTreeView->model());

    // ѡ����Ŀ¼ʱ�����룬����������
    if(fsm->isDir(firstIndex))
        mTableView->setRootIndex(firstIndex);

}
