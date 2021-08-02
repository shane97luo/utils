#include "itemdelegate.h"

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QTimeEdit>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QApplication>
#include <QDebug>

ComboDelegate::ComboDelegate(QObject *parent) :
    QItemDelegate(parent)
{
    nOption = 0;

}

ComboDelegate::~ComboDelegate()
{

}

void ComboDelegate::setItems(QStringList items)
{
    myItems = items;
}

QWidget *ComboDelegate::createEditor(QWidget *parent,const QStyleOptionViewItem &/*option*/,
                                     const QModelIndex &/*index*/) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->addItems(myItems);
    editor->installEventFilter(const_cast<ComboDelegate*>(this));
    return editor;
}

void ComboDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *combox = static_cast<QComboBox*>(editor);
    int icurIndex = combox->findText(value);
    combox->setCurrentIndex(icurIndex);

}

void ComboDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    QComboBox *combox = static_cast<QComboBox*>(editor);
    QString value = combox->currentText();
    model->setData(index, value);
    emit CurrentIndexChangeSignal(index.row(), index.column());

}

void ComboDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
    QComboBox *combox = static_cast<QComboBox*>(editor);

    combox->showPopup();// 直接弹出下拉框， 无此句话，双击后再点击才会弹出下拉框

}

/****************************************************
 * CheckBoxDelegate
 ****************************************************/
static QRect CheckBoxRect(const QStyleOptionViewItem &viewItemStyleOptions)/*const*/
{
    //绘制按钮所需要的参数
    QStyleOptionButton checkBoxStyleOption;
    //按照给定的风格参数 返回元素子区域
    QRect checkBoxRect = QApplication::style()->subElementRect(
                QStyle::SE_CheckBoxIndicator, &checkBoxStyleOption);
    //返回QCheckBox坐标
    QPoint checkBoxPoint(viewItemStyleOptions.rect.x() +
                         viewItemStyleOptions.rect.width()/2 - checkBoxRect.width()/2,
                         viewItemStyleOptions.rect.y() +
                         viewItemStyleOptions.rect.height()/2 - checkBoxRect.height()/2);
    //返回QCheckBox几何形状
    return QRect(checkBoxPoint, checkBoxRect.size());
}

CheckBoxDelegate::CheckBoxDelegate(QObject *parent):
    QStyledItemDelegate(parent)
{

}

void CheckBoxDelegate::setColumn(int col)
{
    column = col;
}

// 绘制复选框
void CheckBoxDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{
    bool checked = index.model()->data(index, Qt::DisplayRole).toBool();

    if(index.column() == column){
        QStyleOptionButton checkBoxStyleOption;
        checkBoxStyleOption.state |= QStyle::State_Enabled;
        checkBoxStyleOption.state |= checked? QStyle::State_On : QStyle::State_Off;
        checkBoxStyleOption.rect = CheckBoxRect(option);

        QApplication::style()->drawControl(QStyle::CE_CheckBox,&checkBoxStyleOption,painter);
    }else{
        QStyledItemDelegate::paint(painter, option, index);
    }
}

// 响应鼠标事件，更新数据
bool CheckBoxDelegate::editorEvent(QEvent *event,
                                   QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option,
                                   const QModelIndex &index) {
    if(index.column() == column){
        if((event->type() == QEvent::MouseButtonRelease) ||
                (event->type() == QEvent::MouseButtonDblClick)){
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            if(mouseEvent->button() != Qt::LeftButton ||
                    !CheckBoxRect(option).contains(mouseEvent->pos())){
                return true;
            }
            if(event->type() == QEvent::MouseButtonDblClick){
                return true;
            }
        }else if(event->type() == QEvent::KeyPress){
            if(static_cast<QKeyEvent*>(event)->key() != Qt::Key_Space &&
                    static_cast<QKeyEvent*>(event)->key() != Qt::Key_Select){
                return false;
            }
        }else{
            return false;
        }

        bool checked = index.model()->data(index, Qt::DisplayRole).toBool();
        int data = checked ? 0 : 1;     //互逆
        return model->setData(index, data, Qt::EditRole);
    }else{
        return QStyledItemDelegate::editorEvent(event, model, option, index);
    }
}
