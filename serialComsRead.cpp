#include "serialComsRead.h"

SerialComsRead::SerialComsRead(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&m_thread);
    m_thread.start();
    vari2_XL = "";
    vari1 = "";
}
/*******************************************/
//线程开始
/*******************************************/
void SerialComsRead::serialComStart()
{
    qDebug() << "barcode thread start!!!"<<vari1<<vari2_XL;
    comInit();
    QTimer::singleShot(1000,this,SLOT(timerReadComFunc()));
    timerCount = 0;
    scanCodeCount = 0;
    revComBuf = "";
    startScanCodeFlag = false;
    revStartFlag = false;
    timeoutCount = 0;
    boltSN = "";
    packSN = "";
    airResultValidFlag = false;
}

/*******************************************/
//RS232 com initialize
/*******************************************/
bool SerialComsRead::comInit()
{
    QString portName ;
    qDebug()<<"this is cominit";
    system("echo 1 > /root/gpio/COM1 &");
    if(vari1.mid(0,1) == "M")
    {
        portName = "ttyS1";   //获取串口名
    }
    else
    {
        portName = "ttyS1";   //获取串口名
    }
    myCom = new QextSerialPort("/dev/" + portName);
    qDebug()<<"this is the carcode"<<Gcarcode;
    myCom->setBaudRate((BaudRateType)Gcarcode);
//    myCom->setBaudRate((BaudRateType)9600);
//    myCom->setBaudRate((BaudRateType)115200);
    //设置数据位
    myCom->setDataBits((DataBitsType)8);
    //设置校验
    myCom->setParity(PAR_NONE);
    //设置停止位
    myCom->setStopBits(STOP_1);
    //设置数据流控制
    myCom->setFlowControl(FLOW_OFF);
    //设置延时
    myCom->setTimeout(10);

    if(myCom->open(QIODevice::ReadWrite))
    {
        qDebug()<<"barcode serial open ok"<<portName;
        connect(myCom, SIGNAL(readyRead()), this, SLOT(slot_read_com3()));
        return true;
    }
    else
    {
        qDebug()<<"barcode serial open fail "<<portName;
        return false;
    }
}

/**************************************/
//timer read com
/**************************************/
void SerialComsRead::timerReadComFunc()
{
    if(revStartFlag) //receive serial data timeout
    {
        timeoutCount ++;
        if(timeoutCount > 3)
        {
            revStartFlag = false;
            timeoutCount = 0;
            revComBuf.clear();
            qDebug()<< "#################################clear revcomBuf";
        }
    }
    if(startScanCodeFlag)
    {
        timerCount++;
        if(timerCount > 2)
        {
            timerCount = 0;
            scanCodeCount++;
            sendSerialCMD();
        }
        if(scanCodeCount > 3)
        {
            emit sendCodeToPLC("Scan Error");
            timerCount = 0;
            scanCodeCount = 0;
            startScanCodeFlag = false;
        }
    }
    slot_read_com3();
    QTimer::singleShot(1000,this,SLOT(timerReadComFunc()));
}
/***********************************/
//serial com send CMD
//
/*************************************/
void SerialComsRead::sendSerialCMD()
{
    QByteArray sendBuf;
    revComBuf.clear();
    revStartFlag = false;
    sendBuf[0] = '+';
    myCom->write(sendBuf,1);
    emit sendCodeToMainwindow("99","NULL");
    qDebug() << "send+++++++++++++++:" << sendBuf <<scanCodeCount;

}
/*******************************************/
//start barcode scanning
/*******************************************/
void SerialComsRead::revAutoBarcodeCmd()
{
    if(!startScanCodeFlag)
    {
        startScanCodeFlag = true;
        timerCount = 0;
        scanCodeCount = 0;
        sendSerialCMD();
    }
}
/*******************************************/
//receive air test cmd
/*******************************************/
void SerialComsRead::revManuulCmd(QString sn,QString code,bool start_en)
{
    if(start_en)
    {
        boltSN = sn;
        packSN = code;
        airResultValidFlag = true;
        revComBuf.clear();
    }
    else
    {
        airResultValidFlag = false;
    }
}

/***********************************/
//receive serial com data
//
/*************************************/
void SerialComsRead::slot_read_com3()
{
    int revLen,i;
    QString serialNums;
    QByteArray recBuf;
    usleep(50000);
    revLen=myCom->bytesAvailable();
    if(revLen>0)
    {
        if(!revStartFlag)
        {
            revStartFlag = true;
        }
        timeoutCount = 0;
        recBuf = myCom->readAll();
        revComBuf.append(recBuf);
        qDebug()<<"receive 11111 ::"<<recBuf.toHex();
        qDebug()<<"receive 22222 ::"<<revLen<<revComBuf.size()<<revComBuf;
        RFIDVin = revComBuf.left(revLen - 1);
        emit sendCodeToMainwindow("98",RFIDVin);
        if(revComBuf[revComBuf.size() - 1] == '=')
        {
            if(revComBuf[revComBuf.size() - 2] == '*')
            {
               qDebug()<<"recComBuf ***"<<Station;
               if(Station == "VM010-1" || Station == "VM010-2")
               {
                   serialNums = revComBuf;
                   emit sendCodeToPLC(serialNums);
                   emit sendCodeToMainwindow("98",serialNums);
                   startScanCodeFlag = false;
                   timerCount = 0;
                   scanCodeCount = 0;
                   qDebug() << "vm010 atuo tiaoma qiang"<< serialNums;
               }
            }
        }
        for(i=0;i<revComBuf.size();i++)
        {
            if(vari2_XL == "XL") //air test
            {
                if((unsigned char)revComBuf[i] == 0x0A)
                {
                    if(revComBuf.size() >= 88)
                    {
                        qDebug()<<"this is rec com  buf"<<recBuf.toHex();
                        if(airResultValidFlag)//air test
                        {
                            airTestResult(QString(revComBuf.right(85)));
                        }
                        revComBuf.clear();
                        revStartFlag = false;
                    }
                }
            }
            else
            {
                if((unsigned char)revComBuf[i] == 0x0A)
                {
                    if(startScanCodeFlag)
                    {
                        if(i==52)
                        {
                            if(Station == "VM010-1" || Station == "VM010-2")
                            {
                                serialNums = revComBuf;
                            }
                            else
                            {
//                                serialNums = revComBuf.mid(1,14).trimmed();
                                serialNums = revComBuf;

                            }
                            emit sendCodeToPLC(serialNums);
                            emit sendCodeToMainwindow("98",serialNums);
                            startScanCodeFlag = false;
                            timerCount = 0;
                            scanCodeCount = 0;
                            qDebug() << "atuo tiaoma qiang"<< serialNums;
                        }
                    }
                    else
                    {
                        if(i>1)
                            serialNums = QString(revComBuf.mid(0,i-1));
                        else
                            serialNums = QString(revComBuf.mid(0,i));
//                        if(GscanRegExp == "Null" && GscanRegExp2 == "Null")
//                        {
//                            emit sendCmdToStep(1,serialNums,2);
//                            emit sendCodeToMainwindow("1",serialNums);
//                        }
//                        else if(GscanRegExp == serialNums.mid(0,GscanRegExp.size()) || GscanRegExp2 == serialNums.mid(0,GscanRegExp2.size()))
//                        {
//                            emit sendCmdToStep(1,serialNums,2);
//                            emit sendCodeToMainwindow("1",serialNums);
//                        }
//                        else
//                        {
//                            emit sendCodeToMainwindow("huhuhu","301");
//                        }
                        bool scanRegExpFlag = false;
                        bool datediffFlag = false;
                        if(GscanRegExp == "Null" && GscanRegExp2 == "Null")
                        {
                            emit sendCmdToStep(1,serialNums,2);
                            emit sendCodeToMainwindow("1",serialNums);
                        }
                        else
                        {
                            QStringList aaa = GscanRegExp.split(",");
                            QStringList bbb = GscanRegExp2.split(",");
                            for(int i = 0; i < aaa.size(); i++)
                            {
                                qDebug()<<"bbb"<<i<<aaa[i];
                                if(serialNums.mid(0,aaa[i].size()) == aaa[i])
                                {
                                    qDebug()<<"aaa ture";
                                    scanRegExpFlag = true;
                                    break;
                                }
                            }
                            for(int i = 0; i < bbb.size(); i++)
                            {
                                qDebug()<<"bbb"<<i<<bbb[i];
                                if(serialNums.mid(0,bbb[i].size()) == bbb[i])
                                {
                                    qDebug()<<"bbb ture";
                                    scanRegExpFlag = true;
                                    break;
                                }
                            }
                            if(GdateDiff == 0)
                            {
                                datediffFlag = true;
                            }
                            else
                            {
//                                QDateTime localtime = QDateTime::currentDateTime();
//                                QString timeBuffertemp;
//                                QDateTime timeTemp;
//                                QString aaa = "20";
//                                QString bbb = serialNums.mid(10,8); //从哪一位开始是时间
//                                timeBuffertemp = aaa.append(bbb);
//                                timeTemp = QDateTime::fromString(timeBuffertemp, "yyyy.MM.dd");
//                                int timediff = timeTemp.daysTo(localtime);
//                                qDebug()<<timediff<<localtime<<timeTemp;
//                                if(timediff < 25)   //时间间隔判断
//                                {
//                                    datediffFlag = true;
//                                }
//                                else
//                                {
//                                    datediffFlag = false;
//                                }
                                QDateTime localtime = QDateTime::currentDateTime();
                                QDateTime timeTemp;
                                QString aaa = "20";
                                QString bbb = serialNums.mid(38,8); //从哪一位开始是时间
                                QString day = bbb.mid(0,2);
                                QString month = bbb.mid(3,2);
                                QString year = aaa.append(bbb.mid(6,2));
                                QString datebuff = year+"-"+month+"-"+day;
                                timeTemp = QDateTime::fromString(datebuff, "yyyy-MM-dd");
                                int timediff = timeTemp.daysTo(localtime);
                                qDebug()<<timediff<<localtime<<timeTemp<<datebuff;
                                if(timediff < 25)   //时间间隔判断
                                {
                                    datediffFlag = true;
                                }
                                else
                                {
                                    datediffFlag = false;
                                }
                            }
                            if(scanRegExpFlag && datediffFlag)
                            {
                                emit sendCmdToStep(1,serialNums,2);
                                emit sendCodeToMainwindow("1",serialNums);
                            }
                            else
                            {
                                emit sendCodeToMainwindow("huhuhu","301");
                            }
                        }
//                        emit sendCmdToStep(1,serialNums,2);
//                        emit sendCodeToMainwindow("1",serialNums);
                        qDebug() << "mannul tiaoma qiang"<< serialNums;
                    }
                    revComBuf.clear();
                    revStartFlag = false;
                }
//                else if((unsigned char)revComBuf[i] == '=')
//                {
//                    qDebug()<<"recComBuf ===="<<i;
//                    if((unsigned char)revComBuf[i-1] == '*')
//                    {
//                        if(Station == "VM010-1" || Station == "VM010-2")
//                        {
//                            serialNums = revComBuf;
//                            emit sendCodeToPLC(serialNums);
//                            emit sendCodeToMainwindow("98",serialNums);
//                            startScanCodeFlag = false;
//                            timerCount = 0;
//                            scanCodeCount = 0;
//                            qDebug() << "vm010 atuo tiaoma qiang"<< serialNums;
//                        }
//                    }
//                }
            }
        }
    }
}
/***********************************/
//receive serial com data
// 4 STAT= 0 KAN= 2 OP= 0 PP= 1 DP=     0  G DT=   0.0 VL=    2.20 PF=-0.271 PM=-0.033 G
//KAN 通道号 上面为1 下面为2
//PP 程序号
//VL 泄漏率（cm3/min）
//PM 测量压力 （bar）
//A=NOK G=OK
/*************************************/
void SerialComsRead::airTestResult(QString buf)
{
    QString STATE_blc,MI_blc,WI_blc,Program,Channel;
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    Channel = buf.mid(12,2).trimmed();//KAN
    Program = buf.mid(24,2).trimmed();//pp
    MI_blc = buf.mid(53,8).trimmed();//VL
    WI_blc = buf.mid(75,6).trimmed();//PM
    STATE_blc = buf.mid(82,1);//A and G
    qDebug()<<"&(&((((((((((((("<<Channel<<Program<<MI_blc<<WI_blc<<STATE_blc;
    if(STATE_blc == "G")
    {
        STATE_blc = "OK";
    }
    else
    {
        STATE_blc = "NOK";
    }
    DATA_STRUCT demo;
    demo.data_model[0] = time.toString("yyyy-MM-dd");
    demo.data_model[1] = time.toString("hh:mm:ss");
    demo.data_model[2] = STATE_blc;
    demo.data_model[3] = MI_blc;
    demo.data_model[4] = WI_blc;
    demo.data_model[5] = boltSN;
    demo.data_model[6] = packSN;
    demo.data_model[7] = "0";
    demo.data_model[8] = "Curve is null";//CurveStr.mid(32);
    demo.data_model[9] = Program;
    demo.data_model[10] = Channel;
    demo.data_model[11] = "1";
    if(STATE_blc == "OK")
    {
        demo.data_model[12] = QString::number(BoltOrder[0]++);
    }
    else
    {
        demo.data_model[12] = QString::number(BoltOrder[0]);
    }
    demo.data_model[13] = "null";//QString::number(torque_max);
    demo.data_model[14] = "null";//QString::number(torque_min);
    demo.data_model[15] = "0";
    QVariant DataVar;
    DataVar.setValue(demo);
    emit sendResultTomainwindow(MI_blc,WI_blc,STATE_blc,Channel.toInt()-1);
    emit sendResultToSQL(DataVar);
}












