#include "logdelete.h"

LogDelete::LogDelete(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();
}

void LogDelete::init()
{
    qDebug() <<"LogDelete thread start!!";
    connect(&LogTimer, SIGNAL(timeout()), this, SLOT(logDeleteSlot()));
    LogTimer.start(1*60*60*1000);//1小时检测一次
}

void LogDelete::logDeleteSlot()
{
    QDir dir("/log");
    QFileInfoList list = dir.entryInfoList();
    int i = 0;
    do{
        QFileInfo fileInfo = list.at(i);
        QString fileName = fileInfo.fileName();//文件名（带后缀）
        if(fileName=="."||fileName==".."){
            i++;
            continue;
        }
        QString baseName = fileInfo.baseName();//文件名（不带后缀）
        if(QDateTime::fromString(baseName.left(10),"yyyy-MM-dd").daysTo(QDateTime::currentDateTime()) > 15){
            qDebug() << "log delete file "+fileInfo.fileName();
            QFile(fileInfo.filePath()).remove();
        }
        i++;
    }while(i<list.size());
}
