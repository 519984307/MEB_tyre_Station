#ifndef RFID_H
#define RFID_H

#include"GlobalVarible.h"
#include <QObject>
#include <qthread.h>
#include <QtNetwork/QtNetwork>
#include <QtNetwork/QAbstractSocket>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QTimer>



class RFID : public QObject
{
    Q_OBJECT
public:
    explicit RFID(QObject *parent = 0);
    QThread m_thread;
    void init();
    QTcpSocket *m_rfidSocket;
    QTimer *timer;
    bool RFIDIsConnect;
    QString RFIDIP;
    int RFIDPort;

signals:
    void sendRfidToCheck(QByteArray);
    void sendRFIDStatusToUI(bool);
    void sendRFIDToMainWindow(QString,QString);

public slots:
    void recvMess();
    void disConnectDo();
    void displayError(QAbstractSocket::SocketError);
    void newConnect();
    void timerHandle();
};

#endif // RFID_H
