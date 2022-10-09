#include "rfid.h"

RFID::RFID(QObject *parent) : QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();
    timer = new QTimer;
    RFIDIsConnect = false;
}
/******************初始化通讯***********************/
void RFID::init()
{
    qDebug()<<"RFID start";
    m_rfidSocket = new QTcpSocket;
    connect(m_rfidSocket,SIGNAL(readyRead()),this,SLOT(recvMess()));
    //    connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(connectedDo()));
    connect(m_rfidSocket,SIGNAL(disconnected()),this,SLOT(disConnectDo()));
//    connect(&pingTimer,SIGNAL(timeout()),this,SLOT(pingTimers()));
//    connect(m_rfidSocket,&QTcpSocket::readyRead,this,&RFID::recvMess);
//    connect(m_rfidSocket,&QTcpSocket::disconnected,this,&RFID::disConnectDo);
    connect(m_rfidSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(timer,SIGNAL(timeout()),this,SLOT(timerHandle()),Qt::DirectConnection);
    timer->start(5000);
}
/******************创建通讯***********************/
void RFID::newConnect()
{
    m_rfidSocket->abort();
    m_rfidSocket->connectToHost(GRFIDIP,GRFIDPort);
    m_rfidSocket->waitForDisconnected(2000);
    if(m_rfidSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug()<<"RFIDLinkSuccess";
        RFIDIsConnect = true;
    }
}
/******************接收数据***********************/
void RFID::recvMess()
{
//    QByteArray tmp_recvMess;
//    tmp_recvMess = m_rfidSocket->readAll();
//    qDebug()<<"recvRFIDMess"<<tmp_recvMess.toHex();
    QString hex = QString(m_rfidSocket->readAll());
    QByteArray ret;
    QString &org = hex;
//    qDebug()<<"rfid"<<hex;
    hex = hex.trimmed();
    int n =2;
    int size= org.size();
    int space= qRound(size*1.0/n+0.5)-1;
    if(space<=0)
        return ;
    for(int i=0,pos=n;i<space;++i,pos+=(n+1))
    {
        org.insert(pos,' ');
    }
    QStringList sl=hex.split(" ");
    foreach(QString s,sl)
    {
        if(!s.isEmpty())
            ret.append((char)s.toInt(0,16)&0xFF);
    }
    QString pin = QString(ret);
    RFIDVin = pin.mid(2,14);
    RFIDG9  = pin.mid(16,2);
    qDebug()<<"RFIDVIN"<<RFIDVin<<"G9"<<RFIDG9;
    emit sendRFIDToMainWindow(RFIDVin,"233");
}
/******************异常断开***********************/
void RFID::disConnectDo()
{
    qDebug()<<"RFIDConnectDisConnect";
    RFIDIsConnect = false;
}
/******************打印错误信息***********************/
void RFID::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<"RFIDError"<<m_rfidSocket->errorString();
    RFIDIsConnect = false;
}
/******************定时查询连接状态***********************/
void RFID::timerHandle()
{
    qDebug()<<"test";
    if(!RFIDIsConnect)
    {
        newConnect();
    }
}
