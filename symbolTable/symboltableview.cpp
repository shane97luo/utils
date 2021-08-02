#include "symboltableview.h"

#include <QAction>
#include <QPushButton>
#include <QDebug>
#include <QMessageBox>
#include <QLayout>
#include <functional>

#include "itemdelegate.h"
#include "standarditemergodic.h"

SymbolTableView::SymbolTableView(Symbol2AddrMap &map, QWidget *parent) :
    symbolVarAddrMap(map),
    QWidget(parent)
{
    mTableView = new QTableView;  // 表格
    QVBoxLayout * mainLay = new QVBoxLayout;
    mainLay->addWidget(mTableView);
    this->setLayout(mainLay);

    actInsertRowBefore = new QAction(tr("insert row before"), this);
    connect(actInsertRowBefore, SIGNAL(triggered()),
            this, SLOT(SlotInsertRowBefore()));
    addAction(actInsertRowBefore);

    actInsertRowAfter = new QAction(tr("insert row after"), this);
    connect(actInsertRowAfter, SIGNAL(triggered()),
            this, SLOT(SlotInsertRowAfter()));
    addAction(actInsertRowAfter);

    actDeleteCurrentRow = new QAction(tr("remove current row"), this);
    connect(actDeleteCurrentRow, SIGNAL(triggered()),
            this, SLOT(SlotDelCurrentRow()));
    addAction(actDeleteCurrentRow);

    setContextMenuPolicy(Qt::ActionsContextMenu);

    QStandardItemModel* model = new QStandardItemModel(this);
    mTableView->setModel(model);
    QStringList hHeaders;
    hHeaders<<tr("#")<< tr("Symbol")<< tr("Address")<< tr("Type")
           << tr("InOut")<< tr("Description");

    model->setHorizontalHeaderLabels(hHeaders);

    connect(model, SIGNAL(itemChanged(QStandardItem*)),
            this, SLOT(SlotItemChanged(QStandardItem *)));

    connect(mTableView->model(), SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(SlotDataChanged(const QModelIndex &, const QModelIndex &)));

    QStringList inoutStrList;
    inoutStrList<<tr("IN")<<tr("OUT")<<tr("INOUT")<<tr("TEMP");
    ComboDelegate* inoutDelegate = new ComboDelegate(this);
    inoutDelegate->setItems(inoutStrList);

    QStringList typeStrList;
    typeStrList<<tr("BYTE")<<tr("INT")<<tr("WORD")<<tr("DWORD")<<tr("ARRAY");
    ComboDelegate* typeDelegate = new ComboDelegate(this);
    typeDelegate->setItems(typeStrList);

    ReadOnlyDelegate* mreadonlyDelegate = new ReadOnlyDelegate;

    mTableView->setItemDelegateForColumn(0, mreadonlyDelegate);
    mTableView->setItemDelegateForColumn(3, typeDelegate);
    mTableView->setItemDelegateForColumn(4, inoutDelegate);
    connect(this, SIGNAL(clicked(const QModelIndex &)),
            this, SLOT(SlotClicked(const QModelIndex &)));

    connect(inoutDelegate, SIGNAL(CurrentIndexChangeSignal(int,int)),
            this, SLOT(SlotIndexChange(int, int)));

    mTableView->setColumnWidth(0, 25);

    mCurModelIndex = NULL;

}

SymbolTableView::~SymbolTableView()
{

    if(actInsertRowBefore) delete actInsertRowBefore;
    if(actInsertRowAfter) delete actInsertRowAfter;
    if(actDeleteCurrentRow) delete actDeleteCurrentRow;

    mCurModelIndex = NULL;

    QStandardItemModel *mModel = dynamic_cast<QStandardItemModel *>(mTableView->model());
    if( NULL != mModel) delete mModel;

}

/**
 * @brief SymbolTableView::UpdateSymbols2Tableview
 * @note  更新符号表
 */
void SymbolTableView::UpdateSymbols2Tableview()
{

    QStandardItemModel *mModel = dynamic_cast<QStandardItemModel *>(mTableView->model());
    if( NULL == mModel) return;

    mModel->setRowCount(0);

    ComboDelegate *typeDelegate = dynamic_cast<ComboDelegate *> (mTableView->itemDelegateForColumn(3));
    ComboDelegate *inoutDelegate = dynamic_cast<ComboDelegate *> (mTableView->itemDelegateForColumn(4));

    mTableView->setItemDelegateForColumn(3, NULL);
    mTableView->setItemDelegateForColumn(4, NULL);

    Symbol2AddrMap::iterator iter;
    for(iter = symbolVarAddrMap.begin(); iter!=symbolVarAddrMap.end(); iter++) {

        QList<QStandardItem*> itemList;
        QStandardItem *item = new QStandardItem("");
        itemList<<item;
        item = new QStandardItem(iter.key());
        itemList<<item;
        item = new QStandardItem(iter.value().at(0));
        itemList<<item;
        item = new QStandardItem(iter.value().at(1));
        itemList<<item;
        item = new QStandardItem(iter.value().at(2));
        itemList<<item;
        item = new QStandardItem(iter.value().at(3));
        itemList<<item;

        mModel->appendRow(itemList);

    }

    mTableView->setItemDelegateForColumn(3, typeDelegate);
    mTableView->setItemDelegateForColumn(4, inoutDelegate);

}

/**
 * @brief SymbolTableView::callpack_collectValAddr
 * @note        遍历元素的函数, 回调
 * @param item          待处理的item
 * @param valAddrmap    符号表信息储存
 * @return
 */
bool SymbolTableView::callpack_collectValAddr(QStandardItem *item,
                                              QMap<QString, QStringList> &valAddrmap)
{
    if(item->index().column() == 1) {

        QModelIndex valName = item->index();
        QStringList valInfoList;
        QString addr = valName.sibling(valName.row(), 2).data(Qt::DisplayRole).toString();
        QString type = valName.sibling(valName.row(), 3).data(Qt::DisplayRole).toString();
        QString inout = valName.sibling(valName.row(), 4).data(Qt::DisplayRole).toString();
        QString description = valName.sibling(valName.row(), 5).data(Qt::DisplayRole).toString();
        valInfoList<<addr<<type<<inout<<description;
        valAddrmap.insert(item->text(), valInfoList);
    }

    return true;
}

/**
 * @brief SymbolTableView::updateVarAddrMap
 * @note 更新符号地址映射表
 */
void SymbolTableView::updateVarAddrMap()
{

    symbolVarAddrMap.clear();

    // 遍历回调,利用bind函数，产生可调用实体 用pFun承接。
    StandardItemErgodic::ergodicAllItem(
                qobject_cast<QStandardItemModel*>(mTableView->model()),
                std::bind(&SymbolTableView::callpack_collectValAddr, this,
                          std::placeholders::_1, std::ref(symbolVarAddrMap)) );

}

void SymbolTableView::SlotInsertRowBefore()
{

    if(mCurModelIndex && mCurModelIndex->isValid()){
        int nRow = mCurModelIndex->row();
        QModelIndex ins;

        mTableView->model()->insertRow(nRow, ins);
    } else {
        QModelIndex ins;
        mTableView->model()->insertRow(mTableView->model()->rowCount(), ins);
    }

}

void SymbolTableView::SlotInsertRowAfter()
{

    if(mCurModelIndex && mCurModelIndex->isValid() ){
        int nRow = mCurModelIndex->row();
        QModelIndex ins;

        mTableView->model()->insertRow(nRow+1, ins);
    } else {
        QModelIndex ins;
        mTableView->model()->insertRow(mTableView->model()->rowCount(), ins);
    }

}

void SymbolTableView::SlotDelCurrentRow()
{
    QMessageBox::StandardButton reply =
            QMessageBox::question(NULL,
                                  tr("Delete"),
                                  tr("Are you sure delete cur records?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if(reply == QMessageBox::No)
        return;

    QItemSelectionModel *selections = mTableView->selectionModel();
    QModelIndexList selected = selections->selectedIndexes();

    foreach (QModelIndex index, selected) {
        mTableView->model()->removeRow(index.row());
    }

}

void SymbolTableView::SlotClicked(const QModelIndex &index)
{
    mCurModelIndex = const_cast<QModelIndex*> (&index);

}

void SymbolTableView::SlotIndexChange(int iRow, int iColumn)
{
    qDebug()<<"Row = "<<iRow << "Column = "<<iColumn;
}

void SymbolTableView::SlotItemChanged(QStandardItem *)
{
    qDebug()<< __func__ << "Item changed";
}

void SymbolTableView::SlotDataChanged(const QModelIndex &topLeft,
                                      const QModelIndex &bottomRight)
{
    qDebug()<< __func__ << "Data changed"<<topLeft.data(Qt::DisplayRole).toString();

    updateVarAddrMap();

}

