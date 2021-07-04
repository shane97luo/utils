#ifndef DIROPERATE_H
#define DIROPERATE_H

#include <QObject>

class DirOperate : public QObject
{
    Q_OBJECT
public:
    explicit DirOperate(QObject *parent = nullptr);
    ~DirOperate();

protected:
    bool CopyFile(const QString &src_file, const QString &dest_file, bool covered) const;
    bool CopyDir(const QString &src_dir, const QString &dest_dir, bool covered = false) const;

private:

signals:

public slots:

    void SlotCopyFile(const QString &src_file, const QString &dest_file, bool covered = false);
    void SlotCopyDir(const QString &src_dir, const QString &dest_dir, bool covered = false);

    void SlotTest();

};

#endif // DIROPERATE_H
