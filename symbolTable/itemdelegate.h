#ifndef ITEM_DELEGATE_H
#define ITEM_DELEGATE_H

#include <QWidget>
#include <QComboBox>
#include <QItemDelegate>
#include <QStyledItemDelegate>

/********************************************************
 * 只读Item
*********************************************************/
class ReadOnlyDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    ReadOnlyDelegate(QObject *parent = 0): QItemDelegate(parent){}

    void setItems(QStringList items);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const
    {
        return NULL;
    }
};

/************************************************************
 *  CheckBox Item
*************************************************************/
class CheckBoxDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CheckBoxDelegate(QObject *parent = 0);

    void setColumn(int col);

protected:
    void paint(QPainter* painter,const QStyleOptionViewItem& option,
               const QModelIndex& index) const;
    bool editorEvent(QEvent *event,QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,const QModelIndex &index);

private:
    int column; //设置复选框的列
};


/************************************************************
 *
 * 下拉列表 Item
 *
*************************************************************/
class ComboDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit ComboDelegate(QObject *parent = 0);
    ~ComboDelegate();

    void setItems(QStringList items);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;

    int nOption;

private:
    QStringList myItems;    //下拉框列表项

signals:

    void CurrentIndexChangeSignal(int iRow, int iCloumn) const;

public slots:

};

#endif // ITEM_DELEGATE_H
