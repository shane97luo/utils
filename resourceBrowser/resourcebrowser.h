#ifndef RESOURCEBROWSER_H
#define RESOURCEBROWSER_H

#include <QSplitter>
#include <QWidget>

class QTreeView;
class QTableView;

class ResourceBrowser : public QWidget
{
    Q_OBJECT
public:
    explicit ResourceBrowser(QWidget *parent = nullptr);
    ~ResourceBrowser();

protected:
    void InitUI();

    void enterDir();

private:

    QWidget *toolWidget;

    QSplitter *mMainWidget;
    QTableView *mTableView;
    QTreeView *mTreeView;

signals:

public slots:
    // signal
    // void doubleClicked(const QModelIndex &index);

    void SlotEnterTreeviewDirCurIndex(const QModelIndex &index);
    void SlotEnterTableviewDirCurIndex(const QModelIndex &index);

};

#endif // RESOURCEBROWSER_H
