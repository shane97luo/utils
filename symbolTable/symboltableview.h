#ifndef SYMBOLTABLEVIEW_H
#define SYMBOLTABLEVIEW_H

#include <QTableView>
#include <QStandardItemModel>

#include "poubase.h"

class QAction;
class ComboDelegate;
class ReadOnlyDelegate;

#define MAXCOL  6

class SymbolTableView : public QWidget
{
    Q_OBJECT
public:
    explicit SymbolTableView(Symbol2AddrMap &map, QWidget *parent = 0);
    ~SymbolTableView();

    void UpdateSymbols2Tableview();

protected:

    bool callpack_collectValAddr(QStandardItem* item, QMap<QString, QStringList> &valAddrmap);
    void updateVarAddrMap();

private:
    QTableView *mTableView;  // 表格

    QAction *actInsertRowBefore, *actInsertRowAfter;
    QAction *actDeleteCurrentRow;

    QModelIndex *mCurModelIndex;    // 当前选中

    Symbol2AddrMap &symbolVarAddrMap;

signals:

protected slots:

    void SlotInsertRowBefore();
    void SlotInsertRowAfter();
    void SlotDelCurrentRow();

    void SlotClicked(const QModelIndex &index);
    void SlotIndexChange(int iRow, int iColumn);

    void SlotItemChanged(QStandardItem*);

    void SlotDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);

};

#endif // SYMBOLTABLEVIEW_H
