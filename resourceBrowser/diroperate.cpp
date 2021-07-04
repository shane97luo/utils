#include "diroperate.h"

#include <QDir>
#include <QFile>
#include <QMessageBox>

#include <QThread>
#include <QDebug>

DirOperate::DirOperate(QObject *parent) : QObject(parent)
{

}

DirOperate::~DirOperate()
{

}

/**
 * @brief DirOperate::SlotCopyFile
 *          copy a file
 * @param src_file:  source file
 * @param to_dir:    destination
 * @param cover:     cover file if there exist file
 * @return
 */
bool DirOperate::CopyFile(const QString &src_file, const QString &dest_file, bool covered) const
{

    if (src_file == dest_file)
        return true;

    if (!QFile::exists(src_file))
        return false;

    QDir *create_dir = new QDir;

    if ( create_dir->exists()) {

        if (covered) {
            create_dir->remove(dest_file);
            return QFile::copy(src_file, dest_file);

        } else
        {
            //QMessageBox::StandardButton re =
            QMessageBox::warning(NULL, "Warn",
                                 "there already existing file ",
                                 QMessageBox::Yes | QMessageBox::No);

            //if(QMessageBox::Yes == re )
            return  false;

        }

    }

    return QFile::copy(src_file, dest_file);

}

/**
 * @brief DirOperate::CopyDir
 *          copy a dir
 * @param src_dir
 * @param dest_dir
 * @param covered
 * @return
 */
bool DirOperate::CopyDir(const QString &src_dir, const QString &dest_dir, bool covered) const
{
    // conditions judge
    QDir sourceDir(src_dir);
    QDir targetDir(dest_dir);
    if(!targetDir.exists()){ // create dest dir if not exist
        if(!targetDir.mkdir(targetDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = sourceDir.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList) {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        if(fileInfo.isDir()){    // if dir, recursion
            if(!CopyDir(fileInfo.filePath(),
                        targetDir.filePath(fileInfo.fileName()), covered))
                return false;
        }
        else{ // if covered is true, remove existing files
            if(covered && targetDir.exists(fileInfo.fileName())){
                targetDir.remove(fileInfo.fileName());
            }

            // copy file
            if(!QFile::copy(fileInfo.filePath(),
                            targetDir.filePath(fileInfo.fileName()))){
                return false;
            }
        }
    }

    return true;

}

void DirOperate::SlotCopyFile(const QString &src_file, const QString &dest_file, bool covered)
{
    CopyFile( src_file, dest_file,covered);
}

void DirOperate::SlotCopyDir(const QString &src_dir, const QString &dest_dir, bool covered)
{
    CopyFile( src_dir, dest_dir, covered);
}

void DirOperate::SlotTest()
{
    qDebug()<<"fun [DirOperate::SlotTest()] cur thread id:"<<QThread::currentThread()<<" fun line:"<<__func__;
}
