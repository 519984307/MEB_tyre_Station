#include "connectplc.h"
#include "GlobalVarible.h"

ConnectPLC::ConnectPLC(QObject *parent) :
    QObject(parent)
{
    this->moveToThread(&plc_thread);
    plc_thread.start();
    channel_Num = 0;
//    vinCode = "";
}

void ConnectPLC::PLCStart()
{
    qDebug() << "connect plc thread start<><><><><>channel_Num::"<<channel_Num;
    PLC_IP = "";
    QVariantMap tempMap = parsejson->parseJson();
    if(tempMap["errorCode"] != 1)
    {
        PLC_IP      = tempMap["plcIP"].toString();
    }
    stationAddr = 0;
    setUpCommFlag = 2;
    sendCmd = 0;
    codeNum = 0;
    timeSkip = true;
    timeSkipCount = 0;
    trayIsAction = 0;
//    TightIsArrive = 0;
//    TightNotArrive = false;
    aliveCount = 1;
    tmpPackNum = 0;
    whichChannel = 0;
    packUplodeStatus1 = true;
    packUplodeStatus2 = true;
    isReadRfidFlag = true;
    plcAckFlag = true;
    glueTightClearFlag = false;
    packUplodeTemp1 = "1";
    packUplodeTemp2 = "1";
    plcRecvFlag = true;
    plcLinkFlag = false;
    manualCodeFlag = false;//处理手动输入条码
    currentProNum = 0;//本次拧紧的程序号
    rfidReadOutTemp = 0;//记录本次的RFID超时信号
//    tightenNOKResult = 1;
    startCmdBuf1 = "\x03**\x16\x11\xE0***\x2E*\xC1\x02\x01*\xC2\x02\x03*\xC0\x01\x09";     //set up communication
    if(PLC_IP == "192.168.1.13")
    {
        qDebug()<<"other plc";
        startCmdBuf1 = "\x03**\x16\x11\xE0***\x01*\xC1\x02\x01*\xC2\x02\x03\x02\xC0\x01\x09";
    }
//    startCmdBuf1 = "\x03**\x16\x11\xE0***\x01*\xC1\x02\x01*\xC2\x02\x03\x02\xC0\x01\x09";
    startCmdBuf2 = "\x03**\x19\x02\xF0\x80\x32\x01**\xFF\xFF*\x08**\xF0**\x03*\x03\x01*";  //set up communication
//        startCmdBuf1 = "\x03**\x16\x11\xE0***\x01*\xC1\x02\x10*\xC2\x02\x03\x02\xC0\x01\x0A";
//        startCmdBuf2 = "\x03**\x19\x02\xF0\x80\x32\x01**\xCC\xC1*\x08**\xF0**\x01*\x01\x03\xC0";  //set up communication
    readCmdBuf   = "";
    writeCmdBuf  = "";
//    RFIDTemppp = "";
//    clearPLCFlag = false;
//    clearPlcTrueFlag = false;
    sendCount = 0;
    readAddr = 0;
    writeAddr = 0;
    isOneCount = 0;
    trayfinishBoundCount = 0;
    nextTrayflag = false;
    trayProtectFlag = false;
    fansongFinishFlag = false;//反松完成
    reqProductTypeAddr = 0;
    reqBoundAddr = 0;
    conditionType = 0;
//    packStatusTrueFlag = false;
    readPLCFlag = false;     //开始读取PLC有效标志位
    statusValidFlag = false; //读到PLC有效标志位
    statusValidFlagTemp = false;
    writePLCFlag = false;

    writeProductTypeFlag = true;
//    writeProductFinishFlag = true;
    reqBindingSNFlag = false;
    reqProductTypeFlag = false;
    reqProductTrayFlag = false;
    getProductTypeFlag = false;
    getGlueProductTypeFlag = false;
    getDistroyStatus = false;
    onlyRunFirst = true;
    reqTightenPositionFlag = false;
    getTightenPositionFlag = false;
    getGluePositionFlag = false;
    getGlueBindSuccess = false;
//    reqRobotTightenReadyFlag = false;
//    getRobotTightenReadyFlag = false;
    reqBoxPositionFlag = false;
//    sendFisChangeFlag = false;
    dutouStatusPosition = false;
    getBoxPositionFlag = false;
    reqBoxFinishFlag = false;
    getBoxFinishFlag = false;
    getStationFinishFlag = false;
    getTightenFinishFlag = false;
    boxErrorFlag = false;
    getTightErrorFlag = false;
    getStationErrorFinishFlag = false;
    reqDutouPositionFlag = false;
    getDutoupositionFlag = false;
    reqDutouRecordDataFlag = false;
    getDutouRecordDataFlag = false;
    getDutouRecordFinishFlag = false;
//    getBindingSNFlag = false;
    ackBoundPackFlag = false;
//    tightProtFlag = false;
    ackProductTypeFlag = false;
    getUpdownBoxPositionFlag = false;
    getScanBarcodeFlag = false;
    getScanBarcodeVM010Flag = false;
    getRFIDCodeVM010Flag = false;
    getUpdownBoxRecordDataFlag = false;
    getTrayType = false;
    tellTrayReadyFlag = false;
    getTrayReadyFlag = false;
    reqModuleBoxNumFlag = false;
    getModuleBoxNumFlag = false;
    getRfidUpDowmFlag = false;
    reqModuleRecordDataFlag = false;
    getModuleRecordDataFlag = false;
    getModuleRecordFinishFlag = false;
    tellRollerBedReadyFlag = false;
    getRollerBedReadyFlag = false;
    reqModuleTypeFlag = false;
    getModuleTypeFlag = false;
    tellModuleMoveFinishFlag = false;
    getModuleMoveFinishFlag = false;
    reqPLCMoveTrayFlag = false;
    getPLCMoveTrayFlag = false;
//    isFirstFlag = true;

    startWorkFlag = false;
    startWorkWriteFlag = false;
    pack1StatusFlag = false;
/**********************************************/
    pack1StatusWriteFlag = true;
/**********************************************/
    PLCWorkAllStep = 0;
//    tightenCount = 0;
    boxCount = 0;
    dutouCount = 0;
    moduleCount = 0;
    PLC_StepNum = 0;
    stationFinishAddr = 0;
    trayFinishAddr = 0;
    mannulNum = 99;
    moveTrayStep = 99;
    pack1StatusNum = 0;
    startWorkNum = 0;
    glueStartWorkNum = 0;
    glueStartWorkAddr = 0;
    glueTightPackStatusAddr = 0;
    glueTightProductTypeAddr = 0;
    writeToPlcPackSNAddr = 0;
    writeToPlcPack1SNAddr = 0;
    writeToPlcPack2SNAddr = 0;
    regExpVm1 = "";
    regExpVm2 = "";
    glueStartWorkFlag = false;
    glueTightPackStatusFlag = false;
    glueTightProductTypeFlag = false;
    tightenTimes = 0;
    W500EnableFlag = false;
    curStepNum = 0;
    strBoxCode = "";
    strBoxCodeTemp = "";
    strBoxCodeFlag = false;
    strBoxCoderegExpFlag = false;
    scanResult = 1;
    tighten_Num = 2;
    box_Num = 0;
    dutou_Num = 0;
    module_Num = 0;
    module_typeNum = 0;
    moduleMessage = "";
    readRFIDResult = 0;
    readRFIDCount = 0;

    PLCPackType = "0";//product type 拧紧位置号
    PLCPackAGVSN = "";
    PLCPack1SN = "";
    PLCPack2SN = "";
    PLCPack1Status = "";
    PLCPack2Status = "";
    dotouPositionNum1 = "";
    dotouPositionNum2 = "";

    w500_IP = "";
    ackCode = 0;
    RFIDCode = "";
    updataTraySN = "";
    updataTrayTemp = "";
    RFIDCodeTemp = "";
    RFIDCodeTemp1 = "";
    pack12Num = 0;
    pack1Result = 0;
    pack2Result = 0;
    countInitValue = 0;
    countValue = 0;
    workSecondePackFlag = false;
    PLC_AllStep = 0;
    tightenTestFlag = false;
    moduleBuf = "";
    parsejson = new ParseJson;
    isStartStationHand = false;
    codeErrorFlag = false;//错误条码告诉PLC
    head3Code = "LSV";//车码头3位
    tightenNumber = 0;//PLC发来的拧紧位置号1：前轮 2：后轮
    stepInit();
    readPLCFlag = true;
    if(PLC_IP != "")
    {
        tcpSocket = new QTcpSocket;
        connect(tcpSocket, SIGNAL(readyRead()),this, SLOT(revPLCData()));
        connect(tcpSocket,SIGNAL(disconnected()),this,SLOT(disConnectDO()));
        plcConnects();
        QTimer::singleShot(2000,this,SLOT(timerFuncXS()));
    }
    else
    {
        qDebug()<<"PLC IP Addr NULL";
    }

    if(W500EnableFlag)
    {
        if(channel_Num == 1)
        {
            READW500 *W500_1 = new READW500;
            W500_1->W500IP = w500_IP;
            QTimer::singleShot(0,W500_1,SLOT(W500Start()));
            QObject::connect(W500_1,SIGNAL(sendTighteningResult1(int,int)),this,SLOT(revCmdFunc(int,int)),Qt::AutoConnection);
            QObject::connect(this,SIGNAL(startW500CMD(int,QString)),W500_1,SLOT(revW500StartCmd(int,QString)),Qt::AutoConnection);
        }
        else if(channel_Num == 2)
        {
            READW500 *W500_2 = new READW500;
            int intIP_Right=0;
            QStringList list = w500_IP.split(".",QString::SkipEmptyParts);
            if(list.size() > 1)
            {
                QString strIP_Right = list[list.size()-1];
                intIP_Right = list[list.size()-1].toInt()+2;
                w500_IP.remove(w500_IP.size()-strIP_Right.size(),strIP_Right.size());
            }
            else
            {
                w500_IP = "";
            }
            W500_2->W500IP = w500_IP.append(QString::number(intIP_Right));
            QTimer::singleShot(0,W500_2,SLOT(W500Start()));
            QObject::connect(W500_2,SIGNAL(sendTighteningResult2(int,int)),this,SLOT(revCmdFunc(int,int)),Qt::AutoConnection);
            QObject::connect(this,SIGNAL(startW500CMD(int,QString)),W500_2,SLOT(revW500StartCmd(int,QString)),Qt::AutoConnection);
        }
    }
}
/**************************************
 * step parameter init
 *************************************/
void ConnectPLC::stepInit()
{
    QVariantMap tempMap = parsejson->parseJson();
    if(tempMap["errorCode"] != 1)
    {
        int i=0;
        stepParaPLCBufInit(tempMap);
        PLC_IP      = tempMap["plcIP"].toString();
        stationAddr = tempMap["plcDB"].toInt();
        if(channel_Num == 2)
        {
            stationAddr += 1;
            qDebug()<<"stationAddr"<<stationAddr;
        }
        foreach (QVariant tempStep, PLCstepList)
        {
            PLCStepMap = tempStep.toMap();
//            qDebug()<<"this is tempstep"<<parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString());
            if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 2)//tightening type
            {
                tighten_Num = PLCStepMap["tightenTimes"].toInt();
                tighten_Num = readPackStatus(tighten_Num,100);//tighten_Num%100
                tightenTestFlag = true;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 9)//dutou type
            {
                dutou_Num = PLCStepMap["plugTimes"].toInt();
                dutou_Num = readPackStatus(dutou_Num,100);
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 26)//box type
            {
                box_Num = PLCStepMap["boxNums"].toInt();
                if(tightenTestFlag)
                {
                    box_Num = box_Num%100;
                }
                else
                {
                    box_Num = readPackStatus(box_Num,100);
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 16)//module type
            {
                module_Num = PLCStepMap["moduleNums"].toInt();
                moduleMessage = PLCStepMap["moduleNums"].toString();
                module_Num = readPackStatus(module_Num,10000);
                //module_typeNum = readPackStatus(module_Num,10);
                moduleBuf = QByteArray::number(module_Num);
                module_typeNum=moduleBuf.mid(3,1).toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 10)//request Record Data type
            {
                W500EnableFlag = true;
                w500_IP = PLCStepMap["w500IP"].toString();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 4)//request move tray
            {
                stationFinishAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 30)//request move tray
            {
                moveTrayStep = i;
                trayFinishAddr = PLCStepMap["writeAdd"].toInt();
                if(PLCWorkAllStep == 0)
                {
                    PLCWorkAllStep = i;
                }
                qDebug()<<"this is tray finish"<<trayFinishAddr;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 8)//mannul and auto mode
            {
                mannulNum = i; //mannul mode position number
                if(i > 0)
                {
                    PLCWorkAllStep = i;
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 50)//start work
            {
                startWorkNum = i;
                if(PLCWorkAllStep == 0)
                {
                    PLCWorkAllStep = i;
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 201)//glue start work
            {
                glueStartWorkNum = i;
                glueStartWorkAddr = PLCStepMap["writeAdd"].toInt();
                glueStartWorkFlag = true;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 202)//涂胶拧紧Pack1状态
            {
                glueTightPackStatusAddr = PLCStepMap["writeAdd"].toInt();
                glueTightPackStatusFlag = true;
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 203)//涂胶拧紧请求产品型号
            {
                glueTightProductTypeAddr = PLCStepMap["writeAdd"].toInt();
                glueTightProductTypeFlag = true;
            }

            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 204)//Pack数据
            {
                writeToPlcPackSNAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 205)//Pack1SN
            {
                writeToPlcPack1SNAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 206)//Pack2SN
            {
                writeToPlcPack2SNAddr = PLCStepMap["writeAdd"].toInt();
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 51)//pack1 status
            {
                pack1StatusNum = i;
                if(PLCWorkAllStep == 0)
                {
                    PLCWorkAllStep = i;
                }
            }
            else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 27) //scan regexp
            {
                regExpVm1 = PLCStepMap["regexpVM1"].toString();
                regExpVm2 = PLCStepMap["regexpVM2"].toString();
            }
            /*else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 100){
                emit sendSuspend();//挂起信号
            }
                        else if(parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString()) == 30)//getPLCMoveTrayFlag
            {
                trayFinishAddr = PLCStepMap["writeAdd"].toInt();
                if(PLCWorkAllStep == 0)
                {
                    PLCWorkAllStep = i;
                }
                qDebug()<<"this is tray finish"<<trayFinishAddr<<PLCWorkAllStep;
            }*/
            i++;
        }
    }
    else
    {
        qDebug()<<"PLC control parsejson in Null";
    }
    qDebug()<<"*****PLC***** stationAddr:"<<stationAddr<<"PLC_AllStep"<<PLC_AllStep<<PLCWorkAllStep<<W500EnableFlag<<PLC_IP;
    qDebug()<<"*****PLC***** tighten_Num:"<<tighten_Num<<box_Num<<dutou_Num<<module_Num<<moduleBuf;

    statusValidFlag = false;
//    tightenCount = 0;
    dutouCount = 0;
    boxCount = 0;
    moduleCount = 0;
    workSecondePackFlag = false;
}
/**************************************
 *
 *
 *************************************/
void ConnectPLC::stepParaPLCBufInit(QVariantMap Step)
{
    int num=0,n=0;
    QVariantMap stepMapTemp;
    PLCstepList  = Step["steps"].toList();
    num = PLCstepList.size();
    GStepListSize = PLCstepList.size();
    for(int i=0;i<num;i++)
    {
        stepMapTemp = PLCstepList[n].toMap();
        if(stepMapTemp["type"].toInt() == 0)
        {
            PLCstepList.removeAt(n);//remove manual step
        }
        else
        {
            n++;
        }
    }
    PLC_AllStep = PLCstepList.size();
}
/*******************************************************/
//nexo disconnect handle
/*******************************************************/
void ConnectPLC::disConnectDO()
{
    qDebug()<<"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCc";
    plcConnects();
}
/*******************************************************/
//nexo bulid connect
/*******************************************************/
void ConnectPLC::plcConnects()
{
    setUpCommFlag = 2;
    tcpSocket->abort(); //取消已有的连接
    tcpSocket->connectToHost(PLC_IP,102); //连接到主机，这里从界面获取主机地址和端口号
    tcpSocket->waitForConnected(2000); //waitting 2000ms
    qDebug() << "plc connect waiting>>>>>>>>"<< tcpSocket->state();
    if(tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "plc connect success!!!!!!!!!!!!!";
        ctlPLCHandle(startCmdBuf1,true);
//        plcLinkFlag = true;   //PLC connect   //20180620
//        emit sendPLCConnect(plcLinkFlag);
    }
    else
    {
        plcLinkFlag = false;   //PLC disconnect
        emit sendPLCConnect(plcLinkFlag);
        qDebug() << "plc connect fail?????????????????" ;
    }
}
/*******************************************************/
//500ms timer function DY:500ms一直循环处理
/*******************************************************/
void ConnectPLC::timerFuncXS()
{
    if(!plcRecvFlag)
    {
        qDebug()<<"plcRecvFlag==false";
    }
    if(plcLinkFlag && plcRecvFlag)
    {
        plcRecvFlag = false;
        if(plcAckFlag)
        {
//            qDebug()<<"testplcAckFlag";
//            writeDataToPLCFunc();
        }
        else
        {
            qDebug()<<"plcackflagerror";
        }
        writeDataToPLCFunc();
        if(writePLCFlag)
        {
//            clearPlcTrueFlag = true;
            if(plcAckFlag)
            {
                qDebug()<<"testplcAckFlagwritePLCFlag";
                plcAckFlag = false;
//                writePLCFunc(stationAddr,writeAddr,writeLen,strWriteBuf);//
            }
            else
            {
                qDebug()<<"there are not ff";
            }
            for(int i = 0;i<writeLen;i++){
                int sendTightenResultValue = strWriteBuf[i];
                qDebug()<<"sendTightenResultValue:"<<i<<sendTightenResultValue;
            }
            qDebug()<<"sendPLCTightenResult:"<<stationAddr<<writeAddr<<writeLen<<strWriteBuf[writeLen-1];
            writePLCFunc(stationAddr,writeAddr,writeLen,strWriteBuf);//
        }
        else if(readPLCFlag)
        {
            readPLCFunc(stationAddr,readAddr,50);//
        }
    }

    if(aliveCount > 50)
    {
        plcLinkFlag = false;
        aliveCount = 0;
        plcConnects();
        writePLCFlag = false;
    }
    aliveCount ++ ;
    if( (tellTrayReadyFlag)||(tellRollerBedReadyFlag) )
    {
        readRFIDCount++;
        if(readRFIDCount > 250)
        {
            readRFIDCount = 0;
            revRFIDAck("0002", "");//timeout
        }
    }
    if(getScanBarcodeVM010Flag && getRFIDCodeVM010Flag)
    {
        QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
        getScanBarcodeVM010Flag = false;
        getRFIDCodeVM010Flag = false;
        emit sendReqKeybarcode(strBoxCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
    }
    if(isStartStationHand)//拧紧枪到达拧紧位置
    {
//        if(TightIsArrive > 0)//拧紧枪到位提醒
        {
            isStartStationHand = false;
//            if(tightProtFlag)
//            {
//                strWriteBufTemp[0] = 0;
//                strWriteBufTemp[1] = 1;
//                writePLCFunc(stationAddr,108,2,strWriteBufTemp);//发给PLC拧紧状态拧紧中
//                TightNotArrive = true;
//                tightProtFlag = false;
//                if(tightenNumber == 2 && tightenNOKResult == 1)//后轮用后面一个配置 && 上一次的拧紧结果为合格
//                {
//                    whichChannel++;
//                }
//                qDebug()<<"VIN_PIN_SQL"<<VIN_PIN_SQL<<whichChannel;
//                emit sendCmdToMain(RFIDVin,0,whichChannel);//拧紧前界面主窗口的界面螺栓显示
//                QString aaatemp = "请求拧紧通道号：";
//                aaatemp.append(QString::number(whichChannel));
//                emit sendMesToWindow(aaatemp,true);
//            }
//            emit sendEnableToTighten(true);//控制拧紧机发使能
        }
    }
    timeSkipCount++;
    if(timeSkip)
    {
        if(timeSkipCount > 2)
        {
            timeSkipCount = 0;
            timeSkip = false;
            strWriteBuf111[0] = 0;
            strWriteBuf111[1] = 1;
            writePLCFunc(stationAddr,110,2,strWriteBuf111);
         }
    }
    else
    {
        if(timeSkipCount > 2)
        {
            timeSkipCount = 0;
            timeSkip = true;
            strWriteBuf111[0] = 0;
            strWriteBuf111[1] = 0;
            writePLCFunc(stationAddr,110,2,strWriteBuf111);
        }
    }
    QTimer::singleShot(500,this,SLOT(timerFuncXS()));
}
/*******************************************************/
//read plc cmd
/*******************************************************/
void ConnectPLC::readPLCFunc(int station,int addr,int len)
{
    QByteArray tempBuf ="\x03**\x1F\x02\xF0\x80\x32\x01***C*\x0E**\x04\x01\x12\x0A\x10\x02*\x0A*\xC6\x84**\x50";
    for(int i=0;i<tempBuf.size();i++)
    {
        if(tempBuf[i]=='*')
        {
            tempBuf[i] = 0;
        }
    }
    qDebug()<<"read";
    if(plcLinkFlag)
    {
        tempBuf[12] = sendCmd;    //SN
        tempBuf[24] = len;        //read data length
        tempBuf[25] = station>>8; //DB station high byte
        tempBuf[26] = station;    //DB station low byte
        tempBuf[27] = 132;        //wr area  DB(132) M(133) I(129) Q(130)
        tempBuf[29] = (addr<<3)>>8;//read high address
        tempBuf[30] = addr<<3;    //read low address
        readCmdBuf = tempBuf;
        //        qDebug() << "**** read  plc******" <<station<<addr;
        ctlPLCHandle(readCmdBuf,false);
        sendCmd ++;
    }
}

/*******************************************************/
//write plc cmd
/*******************************************************/
void ConnectPLC::writePLCFunc(int station,int addr,int len,char *buf)
{
    QByteArray tempBuf ="\x03**\x24\x02\xF0\x80\x32\x01***C*\x0E*\x08\x05\x01\x12\x0A\x10\x06*\x01*\xC6\x84**\x50*\x04*\x20";//writeCmdBuf;
    int i;
    for(i=0;i<tempBuf.size();i++)
    {
        if(tempBuf[i]=='*')
        {
            tempBuf[i] = 0;
        }
    }
    if(plcLinkFlag)
    {
        tempBuf[12] = sendCmd; //SN
        tempBuf[16] = len + 4; //write data length
        tempBuf[22] = 2;       //data type
        tempBuf[23] = 0;       //data len high byte
        tempBuf[24] = len;     //data len low byte
        tempBuf[25] = station>>8;  //DB station high byte
        tempBuf[26] = station;     //DB station low byte
        tempBuf[27] = 132;     //wr area  DB(132) M(133) I(129) Q(130)
        tempBuf[29] = (addr<<3)>>8;//write high address(addr*8)
        tempBuf[30] = addr<<3; //write low address(addr*8)
        tempBuf[33] = (len<<3)>>8;//data len high byte(len*8)
        tempBuf[34] = len<<3;  //data len low byte(len*8)
        for(i=0;i<len;i++)
        {
            tempBuf[35+i] = buf[i];
            if(channel_Num == 1)
            {
                if(addr == 110)
                {

                }
                else
                {
                    qDebug() << "*channel_Num == 1*write plc*" <<station<<addr<<len<<tempBuf.mid(35+i).toHex();
                }
            }
            else
            {
                qDebug() << "*channel_Num == 2*write plc*" <<station<<addr<<len<<tempBuf.mid(35+i).toHex();
            }
        }
        //        tempBuf.append(buf.toAscii());//data
        tempBuf[3]  = tempBuf.size(); //cmd length
        writeCmdBuf = tempBuf;
        ctlPLCHandle(writeCmdBuf,false);
        sendCmd ++;
    }
}

/***********************************/
//send data to PLC  type = true(* invalue)
/*************************************/
void ConnectPLC::ctlPLCHandle(QByteArray sendBuf,bool type)
{
    int Tx_len=0,i;
    Tx_len = sendBuf.size();
    if(type)
    {
        for(i=0;i<Tx_len;i++)
        {
            if(sendBuf[i]=='*')
            {
                sendBuf[i] = 0;
            }
        }
    }
    //qDebug()<<"sendBufmessage"<<sendBuf.toHex();
    tcpSocket->write(sendBuf,Tx_len);
    if(tcpSocket->waitForBytesWritten() == false)
    {}
//    if(channel_Num == 2)
//    qDebug() << "write ::: writeAddr:" << writeAddr << "data:" << sendBuf.toHex();
}
/***********************************/
//处理接受Nexo数据处理函数
//len:收到的网络包长度  Rx_Buffer[]:收到的网络包数据  Tx_len:发送网络包长度  Tx_Buffer[]:发送网络包数据
/*************************************/
void ConnectPLC::revPLCData()
{
    plcRecvFlag = true;
    int revLen,i=0;
    QByteArray recBuf;
    recBuf = tcpSocket->readAll();//接收PLC的传感器信号
    revLen = recBuf.size();
//    if(channel_Num == 2)
//    qDebug() << "&&&& read data:::" << recBuf.toHex();
    //qDebug() << "&&&& read data:::";
    aliveCount = 0;
    if(setUpCommFlag == 2)
    {
        qDebug()<<"this is second hand";
        setUpCommFlag = 1;
        ctlPLCHandle(startCmdBuf2,true);
    }
    else if(setUpCommFlag == 1)
    {
        qDebug()<<"this is send plcLinkFlag";
        setUpCommFlag = 0;
        plcLinkFlag = true;
        emit sendPLCConnect(plcLinkFlag);
//        QTimer::singleShot(200,this,SLOT(timerFuncXS()));
    }
    else
    {
        if(revLen == 22 || revLen == 44)//write ack
        {
            if(recBuf.size() == 44)
            {
                qDebug()<<"recBuffAdd"<<recBuf.toHex();
            }
            else
            {
                qDebug()<<"this is write ack"<<recBuf.mid((revLen-1),2).toHex();
            }
            plcAckFlag = true;
//            if(clearPLCFlag && clearPlcTrueFlag)
//            {
//                clearPLCFlag = false;
//                writePLCFlag = false;
//                clearPlcTrueFlag = false;
//            }
//            else if(writePLCFlag && clearPlcTrueFlag)
//            {
            if(writePLCFlag){
                writePLCFlag = false;
//                clearPlcTrueFlag = false;
                getTightenPositionFlag = false;
                getGluePositionFlag = false;
                getGlueBindSuccess = false;
//                getRobotTightenReadyFlag = false;
                getBoxPositionFlag = false;
                getBoxFinishFlag = false;
//                getTightenFinishFlag = false;
                getTightErrorFlag = false;
                getDistroyStatus = false;
//                getStationErrorFinishFlag = false;
                getBoundInforFlag = false;
                getDutoupositionFlag = false;
                getDutouRecordFinishFlag = false;
                getTrayReadyFlag = false;
                getModuleBoxNumFlag = false;
                getRfidUpDowmFlag = false;
                getModuleRecordFinishFlag = false;
                getRollerBedReadyFlag = false;
                getUpdownBoxPositionFlag = false;
                getScanBarcodeFlag = false;
                getUpdownBoxRecordDataFlag = false;
                getTrayType = false;
                tellModuleMoveFinishFlag = false;
                getModuleTypeFlag = false;
                getModuleMoveFinishFlag = false;
                getGlueProductTypeFlag = false;
                getProductTypeFlag = false;//收到回复关掉发送拧紧位置号状态
                if(pack1StatusFlag)
                {
                    pack1StatusFlag = false;
                    pack1StatusWriteFlag = false;
                }
//                else//请求产品型号时，要写入pack状态，再写入产品型号
//                {
//                    if(writeProductFinishFlag)
//                    {
//                        getProductTypeFlag = false;
//                    }
//                    getProductTypeFlag = false;
//                    getBindingSNFlag = false;
//                }
                if(startWorkFlag)
                {
                    qDebug()<<"this is #################### startWorkFlag"<<startWorkFlag;
                    startWorkFlag = false;
                    startWorkWriteFlag = true;
                }
                if(getStationFinishFlag)
                {
                    getStationFinishFlag = false;
                    statusValidFlag = false;
                }
                reqPLCMoveTrayFlag = false;
                getPLCMoveTrayFlag = false;
            }
        }
        else if(revLen > 22)//read data
        {
            qDebug()<<"len";
            if(recBuf.mid(21,1).toHex()=="ff" && recBuf.mid(22,1).toHex() == "04")//"ff" is success
            {
                qDebug()<<"recBufsize"<<recBuf.size();
                if(reqPLCMoveTrayFlag)
                {
                    return;
                }
                if(readPLCFlag)
                {
                    qDebug()<<"readPLCWriteData:"<<recBuf.toHex().mid(25*2,40*2);
                    if(statusValidFlagTemp != statusValidFlag)
                    {
                        statusValidFlagTemp = statusValidFlag;
                        qDebug()<<"statusValidFlag change:"<<statusValidFlag<<PLC_StepNum;
                    }
                    if(mannulNum != 99)
                    {
                        if((unsigned char)recBuf[26+(mannulNum<<1)] == 1)
                        {
                            mannulModeFlag = 1;//mannul mode
//                            qDebug()<<"mannule mode &&&&&";
                        }
                        else
                        {
                            mannulModeFlag = 0;//auto mode
//                            qDebug()<<"auto mode &&&&&";
                        }
                    }


                    int rfidReadOut = recBuf.toHex().mid(25*2+12*2,2*2).toInt(0,16);
                    if(rfidReadOutTemp != rfidReadOut){
                        rfidReadOutTemp = rfidReadOut;
                        if(1 == rfidReadOut){
                            emit sendMesToWindow("RFID读取超时,请使用扫码枪",true);
                        }
                    }

                      //encoder
                    GrecvStatusMsee = recBuf.toHex();
                    //                        qDebug()<<"recBuf"<<recBuf.toHex();
                    //                        qDebug()<<(unsigned char)recBuf[42]<<recBuf.toHex().mid(82,2).toInt();
                    trayIsAction = recBuf.toHex().mid(82,2).toInt();
                    //                        TightIsArrive = recBuf.toHex().mid(68,2).toInt();
                    QString RFIDCodeTtemp = recBuf.mid(25+20,20);//"782020720001053M    "
                    if(manualCodeFlag){
                        RFIDCodeTtemp.clear();
                    }

                    emit sendCurrentCode(RFIDCodeTtemp);
//                    qDebug()<<"TightIsArrive"<<RFIDCodeTtemp;
                    if(!RFIDCodeTtemp.isEmpty()){
                        QString headStr = RFIDCodeTtemp.left(3);
                        if(headStr == head3Code){//VIN
                            if(VINCheck(RFIDCodeTtemp.left(17))){
                                //读到个新码
//                                if(RFIDVin != RFIDCodeTtemp.mid(0,17))
                                {
                                    codeErrorFlag = false;
                                    RFIDVin = RFIDCodeTtemp.mid(0,17);
                                    emit sendStationFinishToMaindow(RFIDVin,"233");//发窗口做界面的操作
                                }
                            }else{//条码异常在PLC请求的时候发，并在界面显示方便用户查看
                                codeErrorFlag = true;
                                RFIDVin.clear();
                                emit sendMesToWindow("车码异常",true);
                            }
                        }else{//PIN
                            if(PINCheck(RFIDCodeTtemp.left(14))){
                                //读到个新码
//                                if(RFIDVin != RFIDCodeTtemp.mid(0,14))
                                {
                                    codeErrorFlag = false;
                                    RFIDVin = RFIDCodeTtemp.mid(0,14);//"78202072000105"
                                    RFIDG9 = RFIDCodeTtemp.mid(14,4);//"3M  "
                                    emit sendStationFinishToMaindow(RFIDVin,"233");//发窗口做界面的操作
                                }
                            }else{//条码异常在PLC请求的时候发，并在界面显示方便用户查看
                                codeErrorFlag = true;
                                RFIDVin.clear();
                                emit sendMesToWindow("车码异常",true);
                            }
                        }
                    }

                    RFIDCode = recBuf.mid(46,15);
                    if(!statusValidFlag)//read PLC work Flag
                    {
                        isOneCount = 0;
//                        qDebug()<<"recbuf"<<recBuf.toHex().mid(42,30);
                        for(i=0;i<PLC_AllStep;i++)
                        {
                            int val = recBuf.toHex().mid(25*2+2*(2*i),4).toInt(0,16);
                            if((unsigned char)recBuf[26+(i<<1)] >= 1)//need perform a task
                            {
                                PLC_StepNum = i+1;
                                isOneCount++;
                                qDebug()<<"@GTightResetOnline@"<<GTightResetOnline;
                                onlyRunFirst = true;
                                    if(i == 0){//第一步
                                        tightenNumber = val;
                                    }
                                    if(handlePLCStepTypeFunc(i))
                                        break;
                            }
                        }
                        if(isOneCount == 0)
                        {
                            if(onlyRunFirst)
                            {
                                qDebug()<<"^^^^^^^^^^^^^^^^@@@@@@@@";
//                                getDistroyStatus = true;
                                onlyRunFirst = false;
                            }
                        }
                        if(i == PLC_AllStep)
                        {
                            qDebug()<<"PLC_AllStep";
                            PLC_StepNum = 0;
                        }
                    }
                    else //statusValidFlag = true
                    {
                        if(PLC_StepNum>0)
                        {
                            if((unsigned char)recBuf[26+((PLC_StepNum-1)<<1)] == 0)
                            {
                                qDebug()<<"clearPLCFlag"<<PLC_StepNum;
                                statusValidFlag = false;
                                clearPLCFlag = true;
                            }
                        }
                    }
                }
            }
        }
        else
        {
            qDebug()<< "read and write error";
        }
    }
}

/*************************************************/
//receive cmd function 整合槽 对部分自定义类型做逻辑处理
/************************************************/
void ConnectPLC::revCmdFunc(int condition_type, int result)
{
    qDebug()<<"revCmdFunc:"<<condition_type<<result;
    switch (condition_type)
    {
    case 3://收到拧紧结果
        qDebug()<<"revCmdFunc=getTightenFinishFlag";
//        tightenResult = result;
//        tightenCount++;
        getTightenFinishFlag = true;
//        packStatusTrueFlag = true;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 0;
        writePLCFunc(stationAddr,108,2,strWriteBufTemp);//拧紧状态恢复初始状态
        break;
    case 4:
        qDebug()<<"this is dutouCount add"<<dutouCount;
        dutouCount ++;
        getDutouRecordFinishFlag = true;
        statusValidFlag = true;
        break;
    case 5:
        break;
    case 6:
        getTightErrorFlag = true;
        statusValidFlag = true;
        break;
    case 30: //mannul station funcation
        if(moveTrayStep != 99)
        {
            reqPLCMoveTrayFlag = true;
            PLCStepMap = PLCstepList[moveTrayStep].toMap();
            writeAddr = PLCStepMap["writeAdd"].toInt();
            statusValidFlag = false;
        }
        qDebug()<<"$$$$$$$$$$$$$ receive 30 $$$$$$$$$$$$$$"<<writeAddr<<moveTrayStep;
        break;
    case 99:
        statusValidFlag = false;
//        tightenCount = 0;
        dutouCount = 0;
        boxCount = 0;
        moduleCount = 0;
        workSecondePackFlag = false;
        break;
    default:
        break;
    }
}
/*************************************************/
//write data to PLC
/************************************************/
void ConnectPLC::writeDataToPLCFunc()
{
//    qDebug() << "plc 5000ms connects"<<"stationAddr:"<<stationAddr<<"readAddr:"<<readAddr<<"writeAddr:"<<writeAddr;
//    qDebug() << "%%%%%%%writ flag%%%%%%%"<<getProductTypeFlag<<getTightenFinishFlag;
    if(!startWorkWriteFlag) //start work flag
    {
        if(startWorkNum > 0)
        {
            startWorkFlag = true;
            PLCStepMap = PLCstepList[startWorkNum].toMap();
            writeAddr = PLCStepMap["writeAdd"].toInt();
        }
        else
        {
            startWorkWriteFlag = true;
        }
    }
    else if(pack1StatusWriteFlag)//pack1 and pack2 status
    {
        if(pack1StatusNum > 0)//first write pack status
        {
            qDebug()<<"*pack1StatusWriteFlag*";
            pack1StatusFlag = true;
            PLCStepMap = PLCstepList[pack1StatusNum].toMap();
            writeAddr = PLCStepMap["writeAdd"].toInt();
        }
        else
        {
            pack1StatusWriteFlag = false;
        }
    }
    if(pack1StatusFlag)
    {
        qDebug()<<"*packstatus*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = pack1Result;//PLCPack1Status.toInt();
        strWriteBuf[2] = 0;
        strWriteBuf[3] = pack2Result;//PLCPack2Status.toInt();
        writeLen = 4;
        if(glueTightPackStatusFlag)
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = pack1Result;//PLCPack1Status.toInt();
            strWriteBufTemp[2] = 0;
            strWriteBufTemp[3] = pack2Result;//PLCPack2Status.toInt();
            writePLCFunc(stationAddr,glueTightPackStatusAddr,4,strWriteBufTemp);
        }        
    }
    else if(getStationErrorFinishFlag)
    {
        qDebug()<<"getStationErrorFinishFlag"<<stationFinishAddr;
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
        writeAddr = stationFinishAddr;       
        getStationErrorFinishFlag = false;
//        getProductTypeFlag = true;
//        writeProductFinishFlag = false;
    }
    else if(getGlueProductTypeFlag)
    {
        qDebug()<<"*getGlueProductTypeFlag*"<<PLCPackType.toInt();
        writePLCFlag = true;
        strWriteBuf[0] = 0;
//        strWriteBuf[1] = 1;//ackCode;
        strWriteBuf[1] = PLCPackType.toInt();
        writeLen = 2;
        if(glueTightProductTypeFlag)
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = PLCPackType.toInt();
            writePLCFunc(stationAddr,glueTightProductTypeAddr,2,strWriteBufTemp);
        }
    }
    else if(getProductTypeFlag)//收到PLC请求轮胎位置
    {
        qDebug()<<"*getProductTypeFlag*"<<tightenNumber<<RFIDVin;
        int tightenStatus = 0;
        if(!codeErrorFlag){//条码检验无报错则匹配工艺
            VIN_PIN_SQL = RFIDVin;//保存车码到VIN_PIN_SQL，不清，只在有效的时候提取新码
            if(RFIDVin.left(3) == head3Code)//VIN
            {
                whichChannel = VINSearch(RFIDVin);
            }
            else//PIN
            {
                whichChannel = G9Search(RFIDG9);
            }
            if(-1 == whichChannel){
                PLCPackType = "161";
                emit sendMesToWindow("未匹配到车型",true);
            }else{
                if(tightenNumber == 1){
                    PLCPackType = "1";
                    emit sendMesToWindow("拧紧位置：前轮",true);
                }else if(tightenNumber == 2){
                    PLCPackType = "2";
                    emit sendMesToWindow("拧紧位置：后轮",true);
                }
            }
            if(tightenNumber == 2){
                whichChannel++;//后轮用后面一个配置
            }

            emit sendCmdToMain(RFIDVin,0,whichChannel);//拧紧前界面主窗口的界面螺栓显示
            QString aaatemp = "请求拧紧通道号：";
            aaatemp.append(QString::number(whichChannel));
            emit sendMesToWindow(aaatemp,true);

            tightenStatus = 1;
            emit sendEnableToTighten(true);//控制拧紧机发使能
        }else{
            PLCPackType = "162";//车码异常
        }

        qDebug()<<"VIN_PIN_SQL:"<<VIN_PIN_SQL<<whichChannel<<PLCPackType;

        strWriteBuf[0] = 0;
        strWriteBuf[1] = PLCPackType.toInt();//返回位置号并清除拧紧结果
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 0;
        strWriteBuf[4] = 0;
        strWriteBuf[5] = 0;
        strWriteBuf[6] = 0;
        strWriteBuf[7] = 0;
        strWriteBuf[8] = 0;
        strWriteBuf[9] = tightenStatus;//拧紧中
        writeLen = 10;
        writePLCFlag = true;
    }
    else if(getDistroyStatus)
    {
        qDebug()<<"*getDistroyStatus*";
        errorHandle();
        getDistroyStatus = false;
    }
    else if(getTightenPositionFlag)//get tightening position number
    {
//        emit sendStationFinishToMaindow("huhuhu","201");
//        qDebug()<<"*getTightenPositionFlag*";
//        writePLCFlag = true;
//        if(GPackType == "PHEV")
//        {
//            if(tightenCount >= (tighten_Num<<1))
//            {
//                tightenCount = 0;
//            }
//            if((workSecondePackFlag == true)||(pack12Num == 2))
//            {
//                countValue = tighten_Num;
//            }
//            else
//            {
//                countValue = 0;
//            }
//            strWriteBuf[0] = 0;
//            strWriteBuf[1] = tightenCount+1+countValue;
//        }
//        else
//        {
//            if(tightenCount >= tighten_Num)
//            {
//                tightenCount = 0;
//            }
////            strWriteBuf[0] = 0;
////            strWriteBuf[1] = tightenCount+1;
//        }
//        writeLen = 2;
    }
    else if(getGluePositionFlag)
    {
        qDebug()<<"*getGluePositionFlag*";
        QByteArray vinTemp = PLCPack1SN.toLatin1();

        for(int i=0;i<vinTemp.size();i++)
        {
            strWriteBufTemp[i] = (unsigned char)vinTemp[i];
        }
        writeLen = (vinTemp.size());
        qDebug()<<"this is send glue"<<strWriteBuf<<vinTemp.size();
        writePLCFunc(stationAddr,128,writeLen,strWriteBufTemp);
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(getGlueBindSuccess)
    {
        qDebug()<<"*getGlueBindSuccess*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 1;
        writeLen = 4;
    }
    else if(getTightErrorFlag)
    {
        qDebug()<<"*getTightErrorFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 2;
        writeLen = 2;
        writeAddr = 104;
    }
    else if(getTightenFinishFlag)//收到拧紧完成信号
    {
        getTightenFinishFlag = false;
        qDebug()<<"*getTightenFinishFlag*"<<workSecondePackFlag<<countValue<<GNokFlagTemp;
        qDebug()<<"point[900]:"<<GBoltOkNum<<GBoltAllNum<<currentProNum;
        QString aaatemp = "拧紧结果：";
        //合格发1，不合格不发
        if(GBoltOkNum < GBoltAllNum || currentProNum == 98 || currentProNum == 44)//5个螺栓没有全合格就不算合格，再回去重来 || 98号程序（反松）直接给不合格
        {
            GtightGlueresult = 2;
//            tightenNOKResult = 0;
//            tightenCount--;
//            tightProtFlag = true;
            aaatemp.append("不合格");

            if(currentProNum == 98 || currentProNum == 44){//反松完成发PLC
                strWriteBufTemp[0] = 0;
                strWriteBufTemp[1] = 1;
                writePLCFunc(stationAddr,102,2,strWriteBufTemp);
            }
        }
        else//合格
        {
            if(5 == GBoltOkNum && 5 == GBoltAllNum){
                GtightGlueresult = 1;
                //            tightenNOKResult = 1;
                qDebug()<<"point[901]";
                strWriteBufTemp[0] = 0;
                strWriteBufTemp[1] = 1;
                writePLCFunc(stationAddr,104,2,strWriteBufTemp);
                aaatemp.append("合格");


                //            if(tightenNumber == 2){//后轮拧紧合格便站完成(不需要了)2021年4月，PLC规则是放行时会发站完成信号
                //                stationFinishFunc();
                //            }
            }
        }
        GBoltOkNum = 0;
        GBoltAllNum = 0;
        currentProNum = 0;
        qDebug()<<"point[902]:"<<tighten_Num;
        qDebug()<<"tightenResultSendToPlcRedly:"<<strWriteBuf[1]<<GtightGlueresult<<writeLen<<GPackType;
        emit sendMesToWindow(aaatemp,true);

    }
    else if(getBoxFinishFlag)
    {
        qDebug()<<"overturn BoxFinishFlag"<<boxCount;
        writePLCFlag = true;
        boxCount++;
        if(boxCount < box_Num)
        {
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 1;
            writeLen = 2;
        }
        else
        {
            boxCount = 0;
            if((workSecondePackFlag == true)||(pack12Num != 3))
            {
                workSecondePackFlag = false;
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                strWriteBuf[2] = 0;
                strWriteBuf[3] = 1;
                writeLen = 4;
            }
            else
            {
                workSecondePackFlag = true;
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                writeLen = 2;
            }
        }
    }
    else if(getStationFinishFlag)
    {
        qDebug()<<"writeStationFinish"<<PLCWorkAllStep;
        writeLen = 10;
        for(int i=0;i<writeLen;i++) //clear valid step value
        {
            strWriteBuf[i] = 0;
        }
        strWriteBuf[106-100+1] = 1;//站完成，其他位清0，清除位给PLC发个1
        writeAddr = 100;
        writePLCFlag = true;

//        tightenCount = 0;
        boxCount = 0;
        dutouCount = 0;
        moduleCount = 0;
        workSecondePackFlag = false;
    }
    else if(getDutoupositionFlag)
    {
        emit sendStationFinishToMaindow("huhuhu","203");
        qDebug()<<"*getDutoupositionFlag*"<<dutouStatusPosition<<dotouPositionNum1<<dotouPositionNum2;
        QByteArray vinTemp = RFIDVin.toLatin1();
        if(vinTemp == "")
        {
            vinTemp = "Code__NULL";
        }
        writePLCFlag = true;
        if(dutouStatusPosition)
        {
            if(channel_Num == 1)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = dotouPositionNum1.mid(dutouCount,1).toInt();
            }
            else if(channel_Num == 2)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = dotouPositionNum2.mid(dutouCount,1).toInt();
            }
            else
            {
                qDebug()<<"this is channel_Num error";
            }
            if( dotouPositionNum2.mid(dutouCount,1).toInt() > 2)
            {
                vinTemp = PLCPack2SN.toLatin1();
            }
            else
            {
                vinTemp = PLCPack1SN.toLatin1();
            }
        }
        else
        {
            if( dutouCount >= (dutou_Num<<1))
            {
                dutouCount = 0;
            }
            if((workSecondePackFlag == true)||(pack12Num == 2))
            {
                countValue = dutou_Num;
            }
            else
            {
                countValue = 0;
            }
            strWriteBuf[0] = 0;
            strWriteBuf[1] = dutouCount+1+countValue;
            if(workSecondePackFlag)
            {
                vinTemp = PLCPack2SN.toLatin1();
            }
            else
            {
                 vinTemp = PLCPack1SN.toLatin1();
            }
        }
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 0;
        strWriteBuf[4] = 0;
        strWriteBuf[5] = 0;
        strWriteBuf[6] = 0;
        strWriteBuf[7] = 1;
        strWriteBuf[8] = 0;
        strWriteBuf[9] = pack1Result;
        strWriteBuf[10] = 0;
        strWriteBuf[11] = pack2Result;
        strWriteBuf[12] = 'S';
        strWriteBuf[13] = 'K';
        if(vinTemp.size() < 20)
        {
            for(int i = 0; i < vinTemp.size(); i++)
            {
                strWriteBuf[14+i] = (unsigned char)vinTemp[i];
            }
            writeLen = 14+vinTemp.size();
        }
        else
        {
            for(int i = 0; i < 20; i++)
            {
                strWriteBuf[14+i] = (unsigned char)vinTemp[i];
            }
            writeLen = 34;
        }
        qDebug()<<"this is vintemp"<<vinTemp.size()<<vinTemp;
        /*for(int i=0;i<10;i++)
        {
            strWriteBuf[14+i] = (unsigned char)vinTemp[i];
        }
        writeLen = 24;*/
    }
    else if(getDutouRecordFinishFlag)
    {
        qDebug()<<"*getDutouRecordFinishFlag*"<<dutouStatusPosition<<dutouCount<<dotouPositionNum1.size()<<dotouPositionNum2.size();
        writePLCFlag = true;
        if(dutouStatusPosition)
        {
            if(channel_Num == 1)
            {
                if(dutouCount < dotouPositionNum1.size())
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
                else
                {
                    dutouCount = 0;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
            }
            else if(channel_Num == 2)
            {
                if(dutouCount < dotouPositionNum2.size())
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
                else
                {
                    dutouCount = 0;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
            }
            else
            {
                qDebug()<<"this is channel error!!!";
            }
        }
        else
        {
            if(dutouCount < dutou_Num)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                writeLen = 2;
            }
            else
            {
                dutouCount = 0;
                if((workSecondePackFlag == true)||(pack12Num != 3))
                {
                    workSecondePackFlag = false;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
                else
                {
//                    vinCode = PLCPack2SN;
                    workSecondePackFlag = true;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
            }
        }

    }
    else if(getTrayReadyFlag)
    {
        qDebug()<<"send tray message to plc";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
//        strWriteBuf[1] = 1;//1;
        if(Station == "VM010-1" || Station == "VM010-2")
        {
            strWriteBuf[1] = GrfidStatusTemp;
        }
        else
        {
            strWriteBuf[1] = readRFIDResult;    //trayflag 1 160 161 162 otherstation
        }
//        strWriteBuf[1] = GrfidStatusTemp;   //traytype 1 2 3  vm010
//        strWriteBuf[1] = readRFIDResult;    //trayflag 1 160 161 162 otherstation
        writeLen = 2;
        /*if(Station == "A090-1" || Station == "A090-2")
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = 0;
            writePLCFunc(stationAddr,118,2,strWriteBufTemp);
        }*/
    }
    else if(getTrayType)
    {
        qDebug()<<"requset getTrayType rfid";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = GrfidStatusTemp;
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 1;
        writeLen = 4;
    }
    else if(getRfidUpDowmFlag)
    {
        qDebug()<<"*getRfidUpDowmFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = GrfidStatusTemp;//1;
        writeLen = 2;
    }
    else if(getModuleBoxNumFlag)
    {
        emit sendStationFinishToMaindow("请求模组入箱位置","204");

        qDebug()<<"*getModuleBoxNumFlag*"<<pack12Num;
        writePLCFlag = true;
        RFIDCodeTemp = RFIDCode;
        RFIDlist.append(RFIDCode);
        if(workSecondePackFlag == true || pack12Num == 2)
        {
            countValue = 2;
        }
        else
        {
            countValue = 0;
        }
        strWriteBuf[0] = 0;
        strWriteBuf[1] = moduleBuf.mid(moduleCount+countValue,1).toInt();//moduleCount + 1;
        writeLen = 2;
    }
    else if(getModuleRecordFinishFlag)
    {
        emit sendStationFinishToMaindow("车辆数据上传","204");
        qDebug()<<"*getModuleRecordFinishFlag*"<<moduleCount<<updataTrayTemp<<moduleBoundResult;
        writePLCFlag = true;
        /*strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,118,2,strWriteBufTemp);*/
        if(moduleBoundResult == 1)
        {
            trayfinishBoundCount++;
            if(updataTraySN.mid(0,1) == "P")
            {
                nextTrayflag = true;
            }
            else if(updataTraySN.mid(0,1) == "B")
            {
                if(Station == "A090-1")
                {
                    if(trayfinishBoundCount >= 4)
                    {
                        trayfinishBoundCount = 0;
                        nextTrayflag = true;
                    }
                }
                else if(Station == "A090-2")
                {
                    if(trayfinishBoundCount >= 2)
                    {
                        trayfinishBoundCount = 0;
                        nextTrayflag = true;
                    }
                }
            }
            else
            {
                nextTrayflag = true;
            }
            if(nextTrayflag)
            {
                emit sendModuleTrayClean(updataTraySN);
                nextTrayflag = false;
                updataTraySN = "";
                if(updataTrayTemp.isEmpty())
                {

                }
                else
                {
                    updataTraySN = updataTrayTemp;
                    updataTrayTemp = "";
                }
            }
        }
        if(GPackType == "PHEV")
        {
            int moduleTemp = 0;
            if(module_typeNum == 0)
            {
                moduleTemp = 1;
            }
            else
            {
                moduleTemp = 2;
            }
            qDebug()<<"this is moduletemp"<<moduleTemp<<moduleBoundResult;
            if(moduleTemp == 1)
            {
                if(!workSecondePackFlag)
                {

//                    workSecondePackFlag = true;
                    moduleCount = 0;
                    qDebug()<<"this is first send module ok"<<pack12Num;
                    if(pack12Num == 1)
                    {
                        if(moduleBoundResult == 1)
                        {
                            workSecondePackFlag = true;
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            strWriteBuf[2] = 0;
                            strWriteBuf[3] = 1;
                            writeLen = 4;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }

                    }
                    else if(pack12Num == 3)
                    {
                        /*workSecondePackFlag = true;
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        writeLen = 2;*/
                        if(moduleBoundResult == 1)
                        {
                            workSecondePackFlag = true;
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                    }
                    else if(pack12Num == 2)
                    {
                        if(moduleBoundResult == 1)
                        {
                            workSecondePackFlag = true;
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            strWriteBuf[2] = 0;
                            strWriteBuf[3] = 1;
                            writeLen = 4;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                    }
                }
                else
                {
                    qDebug()<<"this is second send module ok";
                    moduleCount = 0;
//                    workSecondePackFlag = false;
                    if(moduleBoundResult == 1)
                    {
                        workSecondePackFlag = false;
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        strWriteBuf[2] = 0;
                        strWriteBuf[3] = 1;
                        writeLen = 4;
                    }
                    else
                    {
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        writeLen = 2;
                    }
                }
            }
            else if(moduleTemp == 2)
            {
                if(moduleCount < 2)
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = moduleBoundResult;
                    writeLen = 2;
                    qDebug()<<"this is moduletemp1111111111"<<moduleTemp;//wz
                    qDebug()<<"workSecondePackFlag  1111111"<<workSecondePackFlag;

                }
                else
                {
                    moduleCount = 0;
                    qDebug()<<"workSecondePackFlag333222222222"<<workSecondePackFlag;
                    if((workSecondePackFlag == true)||(pack12Num != 3))
                    {
                        workSecondePackFlag = false;
                        if(moduleBoundResult == 1)
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            strWriteBuf[2] = 0;
                            strWriteBuf[3] = 1;
                            writeLen = 4;
                        }
                        else
                        {
                            strWriteBuf[0] = 0;
                            strWriteBuf[1] = moduleBoundResult;
                            writeLen = 2;
                        }
                        qDebug()<<"this is moduletemp222222222"<<moduleTemp;
                        qDebug()<<"this is pack12Num222222222"<<pack12Num;
                        qDebug()<<"workSecondePackFlag  222222222"<<workSecondePackFlag;
                    }
                    else
                    {
//                        vinCode = PLCPack2SN;
                        workSecondePackFlag = true;
                        strWriteBuf[0] = 0;
                        strWriteBuf[1] = moduleBoundResult;
                        writeLen = 2;
                        qDebug()<<"this is moduletemp333333333"<<moduleTemp;
                        qDebug()<<"workSecondePackFlag  33333333333"<<workSecondePackFlag;


                    }
                }
            }
        }
        else
        {
            if(moduleCount < (moduleMessage.size()-1))
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = moduleBoundResult;
                writeLen = 2;
            }
            else
            {
                moduleCount = 0;
                if(moduleBoundResult == 1)
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = moduleBoundResult;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
                else
                {
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = moduleBoundResult;
                    writeLen = 2;
                }
            }
        }
    }
    else if(getRollerBedReadyFlag)
    {
        qDebug()<<"*getRollerBedReadyFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = readRFIDResult;
        writeLen = 2;
    }
    else if(getUpdownBoxPositionFlag)//请求下箱体位置号
    {
        emit sendStationFinishToMaindow("huhuhu","205");
        qDebug()<<"*getUpdownBoxPositionFlag*"<<countValue<<boxCount<<pack12Num<<packUplodeTemp1<<packUplodeTemp2;
        writePLCFlag = true;
        if( boxCount >= (box_Num<<1))
        {
            boxCount = 0;
        }
        if(pack12Num == 3)
        {
            if(packUplodeTemp1 == "2")
            {
                if(packUplodeTemp2 == "1")
                {
                    pack12Num = 2;
                }
                else if(packUplodeTemp2 == "2")
                {
                    boxErrorFlag = true;
                }
            }
            else if(packUplodeTemp1 == "1")
            {
                if(packUplodeTemp2 == "1")
                {

                }
                else if(packUplodeTemp2 == "2")
                {
                    pack12Num = 1;
                }
            }
        }
        else if(pack12Num == 2)
        {
            if(packUplodeTemp2 == "2")
            {
                boxErrorFlag = true;
            }
        }
        else if(pack12Num == 1)
        {
            if(packUplodeTemp1 == "2")
            {
                boxErrorFlag = true;
            }
        }
        if((workSecondePackFlag == true)||(pack12Num == 2))
        {
            countValue = box_Num;
        }
        else
        {
            countValue = 0;
        }
        if(boxErrorFlag)
        {
            qDebug()<<"boxErrorFlag";
            boxErrorFlag = false;
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 0;
            strWriteBuf[2] = 0;
            strWriteBuf[3] = 0;
            strWriteBuf[4] = 0;
            strWriteBuf[5] = 1;
            writeLen = 6;
        }
        else
        {
            strWriteBuf[0] = 0;
            strWriteBuf[1] = boxCount+1+countValue;
            writeLen = 2;
        }
    }
    else if(getScanBarcodeFlag)
    {
        qDebug()<<"*getScanBarcodeFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = scanResult;
        writeLen = 2;
    }
    else if(getUpdownBoxRecordDataFlag)//请求条码数据绑定
    {
        qDebug()<<"*getUpdownBoxRecordDataFlag*"<<boxCount<<box_Num<<pack12Num<<workSecondePackFlag;
        writePLCFlag = true;
        if(GPackType == "PHEV")
        {
            boxCount++;
            if(boxCount < box_Num)
            {
                strWriteBuf[0] = 0;
                strWriteBuf[1] = 1;
                writeLen = 2;
            }
            else
            {
                boxCount = 0;
                if((workSecondePackFlag == true)||(pack12Num != 3))
                {
                    workSecondePackFlag = false;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    strWriteBuf[2] = 0;
                    strWriteBuf[3] = 1;
                    writeLen = 4;
                }
                else
                {
                    workSecondePackFlag = true;
                    strWriteBuf[0] = 0;
                    strWriteBuf[1] = 1;
                    writeLen = 2;
                }
            }
        }
        else
        {
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 1;
            strWriteBuf[2] = 0;
            strWriteBuf[3] = 1;
            writeLen = 4;
        }

    }
    else if(getModuleMoveFinishFlag)
    {
        qDebug()<<"*getModuleMoveFinishFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(getModuleTypeFlag)
    {
        qDebug()<<"*getModuleTypeFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(startWorkFlag)
    {
        qDebug()<<"*startWorkFlag*";
        if(glueStartWorkFlag)
        {
            strWriteBufTemp[0] = 0;
            strWriteBufTemp[1] = 1;
            writePLCFunc(stationAddr,glueStartWorkAddr,2,strWriteBufTemp);
        }
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 1;
        writeLen = 2;
    }
    else if(reqPLCMoveTrayFlag)
    {
        qDebug()<<"*reqPLCMoveTrayFlag*";
        writePLCFlag = true;
        strWriteBuf[0] = 0;
        strWriteBuf[1] = 0;
        strWriteBuf[2] = 0;
        strWriteBuf[3] = 1;
        writeLen = 4;
        writeAddr = 100;
    }
    else if(getPLCMoveTrayFlag)
    {
        if(Station == "VM010-1" || Station == "VM010-2")
        {
            writePLCFlag = true;
            strWriteBuf[0] = 0;
            strWriteBuf[1] = 1;
            writeLen = 2;
            writeAddr = trayFinishAddr;
        }
        else
        {
            qDebug()<<"*getPLCMoveTrayFlag*"<<PLCWorkAllStep;
            writePLCFlag = true;
            writeLen = ((trayFinishAddr%100)<<1);
            qDebug()<<"this is trayFinishAddr"<<writeLen;
    //        writeLen = PLCWorkAllStep<<1;
            for(int i=0;i<writeLen;i++) //clear valid step value
            {
                strWriteBuf[i] = 0;
            }
            writeAddr = 100;
        }
    }
}
/******************************************/
//handle plc step type parameter
/******************************************/
bool ConnectPLC::handlePLCStepTypeFunc(int num)
{
    //num 收到PLC步骤号，从0开始
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    GwhichStepNum = num;
    getDistroyStatus = false;
    PLCStepMap = PLCstepList[num].toMap();
    int type = parsejson->stepNameToTypeCode(PLCStepMap["singleStep"].toString());
    qDebug()<<"*handlePLCStepTypeFunc*"<<num<<type;
    QString aaatemp = "请求反转通道号：";
    switch (type)
    {
    case 1: //请求产品型号(拧紧位置号)
        reqProductTypeFlag = true;
        qDebug()<<"req"<<RFIDG9<<RFIDVin;
        tighten_Num = 2;
//        if(RFIDVin != vinTemp){//本次请求产品型号时的车型码与上一次不等(请求前轮)
//            tightenNumber = 1;//新码请求前轮
//            RFIDTemppp = RFIDVin;
//            vinTemp = RFIDVin;//保存本次车条码
//        }
        getProductTypeFlag = true;//处理发位置号给PLC的逻辑
//        ackProductTypeFlag = true;
        statusValidFlag = true;
//        tightProtFlag = true;
//        sendFisChangeFlag = true;
        reqProductTypeAddr = PLCStepMap["writeAdd"].toInt();
        break;
    case 2://请求机器人拧紧位置号
        reqTightenPositionFlag = true;
        getTightenPositionFlag = true;
        statusValidFlag = true;
        break;
    case 53://请求机器人涂胶位置号
        getGluePositionFlag = true;
        statusValidFlag = true;
        break;
    case 138://请求拧紧枪反转
        qDebug()<<"fanzhuan"<<VIN_PIN_SQL;
        emit sendChToWork(channel_Num,1,RFIDVin);//type=1 tighten2
//        TightNotArrive = true;
//        strWriteBufTemp[0] = 0;
//        strWriteBufTemp[1] = 1;
//        writePLCFunc(stationAddr,108,2,strWriteBufTemp);
//        if(PLCPackType == "2")
//        {
//            whichChannel++;
//        }
        emit sendCmdToMain(RFIDVin,0,whichChannel);
        emit sendEnableToTighten(false);
//        QString aaatemp = "请求反转通道号：";
        aaatemp.append(QString::number(whichChannel));
        emit sendMesToWindow(aaatemp,true);
        tighten_Num++;
        statusValidFlag = true;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,108,2,strWriteBufTemp);//拧紧过程中
        break;
    case 54://涂胶数据绑定完成
        getGlueBindSuccess = true;
        statusValidFlag = true;
        break;
    case 3: //拧紧枪到达拧紧位置
//        reqRobotTightenReadyFlag = true;
//        getRobotTightenReadyFlag = true;
//        GgetRobotTightenReadyFlag = true;
//        strWriteBufTemp[0] = 0;
//        strWriteBufTemp[1] = 1;
//        writePLCFunc(stationAddr,108,2,strWriteBufTemp);
        if(pack12Num == 1)
        {
            if(Station == "A400A" || Station == "A400B")
            {
                qDebug()<<"this is station pack A400";
//                vinCode =PLCPack2SN;
            }
            else
            {
//                vinCode =  PLCPack1SN;
            }
//            vinCode =  PLCPack1SN;
//            emit sendCmdToMain(PLCPack1SN,0,tightenCount+1);
//            emit sendChToWork(channel_Num,1,PLCPack1SN);//type=1 tighten2  tell work vin
        }
        else if(pack12Num == 2)
        {
//            vinCode =  PLCPack2SN;
//            emit sendCmdToMain(PLCPack2SN,0,tightenCount+1);
//            emit sendChToWork(channel_Num,1,PLCPack2SN);//type=1 tighten2
        }
        else//pack12Num = 3
        {
//            if(!workSecondePackFlag)
//            {
//                vinCode =  PLCPack1SN;
//            }
//            else
//            {
//                vinCode =  PLCPack2SN;
//            }
        }
//        vinCode = fisVINCode;//空的，啥都没有
            isStartStationHand = true;

//        emit sendChToWork(channel_Num,1,vinCode);//type=1 tighten2
        statusValidFlag = true;
        break;
    case 4://已收到站完成信号(data状态清除) 2021.3.26现在PLC不给这个信号了，后轮完成直接到站完成状态 (4月后面又启用了)
        stationFinishFunc();
        break;
    case 5:
        reqBoxPositionFlag = true;
        getBoxPositionFlag = true;
        statusValidFlag = true;
        break;
    case 6://机器人翻转下箱体完成
        reqBoxFinishFlag = true;
        getBoxFinishFlag = true;
        statusValidFlag = true;
        break;
    case 8://手动/自动模式
        statusValidFlag = false;
//        if(Station == "A050new")
//        if(GTightResetOnline == "Yes")
//        {
//            qDebug()<<"GTightResetOnline == 8";
//        }
//        else
        {
            PLC_StepNum = 0;
        }
        return false;
        break;
    case 9://请求堵头安装位置号
        reqDutouPositionFlag = true;
        getDutoupositionFlag = true;
//        emit startW500CMD(channel_Num,vinCode);
        statusValidFlag = true;
        break;
    case 10://请求记录堵头安装数据
        reqDutouRecordDataFlag = true;
        getDutouRecordDataFlag = true;
        statusValidFlag = true;
        emit startW500CMD(channel_Num,"read");
        break;
    case 15://辊道托盘到位
        qDebug()<<"××××××××××托盘到位××××××××××××××××";
        tellTrayReadyFlag = true;
//        getTrayReadyFlag = true;
        statusValidFlag = true;
        isReadRfidFlag = true;
        GEKSReRFID = true;
//        emit sendCmdToStep(6,"ready",15);
        emit sendReadCmdToRFID("0000","0015");
        break;
    case 22://下线托盘型号
        getRfidUpDowmFlag = true;
        statusValidFlag = true;
        break;
    case 16://请求模组入箱位置号
        reqModuleBoxNumFlag = true;
        getModuleBoxNumFlag = true;
        statusValidFlag = true;
        break;
    case 17://模组入箱完成
        reqModuleRecordDataFlag = true;
        getModuleRecordDataFlag = true;
        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%";
        emit sendShowPush();//显示当前模组绑定bypass图标
//        if(moduleCount == 0)
//        {
//        if(!RFIDlist.isEmpty())
//        {
            if(!workSecondePackFlag)
            {
                if(pack12Num == 2)
                {
                    qDebug()<<"this is send moudle pack to http"<<updataTraySN<<PLCPack2SN;
                    emit sendReqModulePack(updataTraySN,PLCPack2SN);
                }
                else
                {
                    qDebug()<<"this is send moudle pack to http first"<<updataTraySN<<PLCPack1SN;
                    emit sendReqModulePack(updataTraySN,PLCPack1SN);
                }
            }
            else
            {
                qDebug()<<"this is send moudle pack to http"<<updataTraySN<<PLCPack2SN;
                emit sendReqModulePack(updataTraySN,PLCPack2SN);
            }
            /*RFIDlist.removeFirst();
        }
        else
        {
            qDebug()<<"rfidlist is empty";
        }
        }
        else
        {
            moduleCount ++;
            getModuleRecordFinishFlag = true;
        }*/
        statusValidFlag = true;
        break;
    case 18://托盘到位匹配产品型号
        tellRollerBedReadyFlag = true;
//        getRollerBedReadyFlag = true;
        statusValidFlag = true;
        emit sendReadCmdToRFID("0000","0015");//read RFID addr 0000 len 15
        break;
    case 20://本次模组搬运完成
        tellModuleMoveFinishFlag = true;
        emit sendReqModuleCount();
        getModuleMoveFinishFlag = true;
        statusValidFlag = true;
        break;
    case 21://请求模组产品型号
        reqModuleTypeFlag = true;
        emit sendReqModule();
        getModuleTypeFlag = true;
        statusValidFlag = true;
        break;
    case 25://请求捆绑流水号、料车号（AGV到位）
        reqBindingSNFlag = true;
        emit sendBoundPack(); //A010
        ackBoundPackFlag = true;
        statusValidFlag = true;
        reqBoundAddr = PLCStepMap["writeAdd"].toInt();
        break;
    case 26://请求下箱体位置号
        reqUpdownBoxPositionFLag = true;
        getUpdownBoxPositionFlag = true;
        statusValidFlag = true;
        break;
    case 27://请求下箱体条码扫描
        statusValidFlag = true;
        reqScanBarcodeFlag = true;
        //sao ma
        emit sendStartBarcode() ;
        curStepNum = num;
        //        getScanBarcodeFlag = true;
        //        statusValidFlag = true;
        break;
    case 29://请求辊道托盘类型
        getTrayType = true;
    case 28://请求条码数据绑定
        //while(!GIsInterfaceUpSuccess)
        {
            reqUpdownBoxRecordDataFlag = true;
            getUpdownBoxRecordDataFlag = true;
            if(pack12Num == 1)
            {
                emit sendReqKeybarcode(strBoxCode,PLCPack1SN,time.toString("yyyy-MM-dd hh:mm:ss"),1);
            }
            else if(pack12Num == 2)
            {
                emit sendReqKeybarcode(strBoxCode,PLCPack2SN,time.toString("yyyy-MM-dd hh:mm:ss"),2);
            }
            else//pack12Num = 3
            {
                if(!workSecondePackFlag)
                {
                    emit sendReqKeybarcode(strBoxCode,PLCPack1SN,time.toString("yyyy-MM-dd hh:mm:ss"),1);
                }
                else
                {
                    emit sendReqKeybarcode(strBoxCode,PLCPack2SN,time.toString("yyyy-MM-dd hh:mm:ss"),2);
                }
            }
            statusValidFlag = true;
            //sleep(1);
            //continue;
        }
        //GIsInterfaceUpSuccess = false;
        break;
    case 30://站完成请求PCL放行托盘
        getPLCMoveTrayFlag = true;
        trayProtectFlag = false;
//        qDebug()<<"this is rfidCode"<<RFIDCode<<GTrayStatus;
        if(Station == "VM010-1" || Station == "VM010-2")
        {
            statusValidFlag = true;
            QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            emit sendReqStationState(RFIDCode,GTrayStatus,"","",DataTime);
        }
        if(Station == "OP80")
        {
            qDebug()<<"this is bmce sendModuleTrayClean"<<RFIDCode;
//            emit sendModuleTrayClean(RFIDCode);
            emit sendBMCETrayClean(RFIDCode,GPackSN);
        }
        if(RFIDCode == "P-xxx")
        {
            qDebug()<<"this is rfidcode"<<RFIDCode;
        }
        else
        {
            QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
            if(Station.mid(0,2) == "VM")
            {
                int Type = 2;//出站
                emit sendUpInAndOutStation(Type,RFIDCode,"",Station,DataTime);//小车出站发到interface上传接口
//                emit sendReqStationState(RFIDCode,GTrayStatus,"","",DataTime);
            }
        }
        //        statusValidFlag = true;
    case 50://开班信号
        break;
    default:
        statusValidFlag = false;
        return false;
        break;
    }
    writeAddr = PLCStepMap["writeAdd"].toInt();
    return true;
}


void ConnectPLC::recvMasterReadRfid()
{
    if(!isReadRfidFlag)
    {
        qDebug()<<"master read RFID";
        emit sendReadCmdToRFID("0000","0015");
    }
}

void ConnectPLC::reSendReqModulePack()
{
    if(!workSecondePackFlag)
    {
        qDebug()<<"this is send moudle pack to http first"<<RFIDCode<<PLCPack1SN;
        emit sendReqModulePack(RFIDCode,PLCPack1SN);
    }
    else
    {
        qDebug()<<"this is send moudle pack to http"<<RFIDCode<<PLCPack2SN;
        emit sendReqModulePack(RFIDCode,PLCPack2SN);
    }
}

void ConnectPLC::revGluePackStatus(QString gluePackStatus)
{
    if(gluePackStatus == "2")
    {
        qDebug()<<"gluePackStatusErrorFinishFlag"<<stationFinishAddr;
//        writePLCFlag = true;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,106,2,strWriteBufTemp);
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 1;
        writePLCFunc(stationAddr,stationFinishAddr,2,strWriteBufTemp);
    }
}

void ConnectPLC::recDutouTightStatus1(QString recMessage)
{
   // QString dotouPositionNum = "";
    if(recMessage.size() == 8)
    {
        dutouStatusPosition = true;
        dotouPositionNum1 = "";
        if(recMessage.mid(0,1) == "1" && recMessage.mid(1,1) == "1")
        {
            dotouPositionNum1.append("1");
        }
        if(recMessage.mid(2,1) == "1" && recMessage.mid(3,1) == "1")
        {
            dotouPositionNum1.append("2");
        }
        if(recMessage.mid(4,1) == "1" && recMessage.mid(5,1) == "1")
        {
            dotouPositionNum1.append("3");
        }
        if(recMessage.mid(6,1) == "1" && recMessage.mid(7,1) == "1")
        {
            dotouPositionNum1.append("4");
        }

        if(recMessage == "00000000" || dotouPositionNum1.isEmpty())
        {
            getStationErrorFinishFlag = true;
        }
    }
    else
    {
        qDebug()<<"this is recMessage is error";
    }
    qDebug()<<"dotouPositionNum1"<<dotouPositionNum1<<recMessage;
}

void ConnectPLC::recDutouTightStatus2(QString recMessage)
{
    if(recMessage.size() == 8)
    {
        dutouStatusPosition = true;
        dotouPositionNum2 = "";
        if(recMessage.mid(0,1) == "1" && recMessage.mid(1,1) == "1")
        {
            dotouPositionNum2.append("1");
        }
        if(recMessage.mid(2,1) == "1" && recMessage.mid(3,1) == "1")
        {
            dotouPositionNum2.append("2");
        }if(recMessage.mid(4,1) == "1" && recMessage.mid(5,1) == "1")
        {
            dotouPositionNum2.append("3");
        }if(recMessage.mid(6,1) == "1" && recMessage.mid(7,1) == "1")
        {
            dotouPositionNum2.append("4");
        }
        if(recMessage == "00000000" || dotouPositionNum2.isEmpty())
        {
            getStationErrorFinishFlag = true;
        }
    }
    else
    {
        qDebug()<<"this is recMessage is error";
    }
    qDebug()<<"dotouPositionNum2"<<dotouPositionNum2<<recMessage;
}

void ConnectPLC::recReadyClicked()
{
    if(Station.mid(0,5) == "VM444")
    {
        qDebug()<<"onClickedReadyPlcMessage"<<GrecvStatusMsee;
        strWriteBufTemp[0] = 0;
        strWriteBufTemp[1] = 0;
        strWriteBufTemp[2] = 0;
        strWriteBufTemp[3] = 1;
        writePLCFunc(stationAddr,100,4,strWriteBufTemp);
    }

}
//void ConnectPLC::revFisPackType(QString G9)
//{
//    qDebug()<<"thisIsVinCode"<<G9;
//    handFisVinCode(G9);
//    tighten_Num = 2;

//}
//void ConnectPLC::handFisVinCode(QString G9)
//{
//    if(G9.isEmpty())
//    {
//        qDebug()<<"G9isempty";
//        PLCPackType = "161";
//    }
//    else
//    {
//        vinCode = RFIDVin;
//        if(G9 == "RM")
//        {
//            PLCPackType = "1";
//        }
//        else
//        {
//            PLCPackType = "2";
//        }
//    }
////    PLCPackType = "1";
//    getProductTypeFlag = true;

//}

//void ConnectPLC::revPackStatus(QVariant packStatus)
//{
//    PACK_STATUS statusStruct;
//    statusStruct = packStatus.value<PACK_STATUS>();
//    qDebug()<<"connectplc statusStruct"<<statusStruct.Result<<statusStruct.Errorcode;
//    if( (reqProductTypeFlag)||(reqBindingSNFlag) )
//    {
//        reqProductTypeFlag = false;
//        reqBindingSNFlag = false;
//        tmpPackNum = 0;
//        if(statusStruct.Result)
//        {
//            PLCPackType = statusStruct.PackType;//product type
//            QString stepPackTypeTemp;
//            if(PLCPackType == "1")
//            {
//                stepPackTypeTemp = "PHEV";
//            }
//            else
//            {
//                stepPackTypeTemp = "BEV";
//            }

//            if(GPackType == stepPackTypeTemp)
//            {

//            }
//            else
//            {
//                qDebug()<<"this is PLCPackType"<<stepPackTypeTemp;
//                GPackType = stepPackTypeTemp;
//                stepInit();
//                statusValidFlag = true;
//            }
//            PLCPackAGVSN = statusStruct.AGVSN;
//            PLCPack1SN = statusStruct.Pack1SN;
//            PLCPack2SN = statusStruct.Pack2SN;
//            PLCPack1Status = statusStruct.Pack1Status;
//            PLCPack2Status = statusStruct.Pack2Status;
//            if(writeToPlcPack1SNAddr == 0)
//            {
//                qDebug()<<"this is error write packSN to plc";
//            }
//            else
//            {
//                 writePackSNToPlc(PLCPack1SN,PLCPack2SN);
//            }
//            if(Station == "A160")
//            {
//                if(channel_Num == 1)
//                {
//                    emit sendReqDutouTightStatus(PLCPackAGVSN,"A140A");
//                }
//                else if(channel_Num == 2)
//                {
//                    emit sendReqDutouTightStatus(PLCPackAGVSN,"A150");
//                }
//                else
//                {
//                    qDebug()<<"channel_Num@@@@@@@@"<<channel_Num;
//                }
//            }
//            if(Station == "A350-2")
//            {
//                if(PLCPack1SN.isEmpty())
//                {
//                    qDebug()<<"skipEmpty";
//                }
//                else
//                {
//                    emit sendGlueWorkFlag(PLCPack1SN);
//                }
//            }
//            vinCode = PLCPack1SN;
//            if(Station == "A400A" || Station == "A400B")
//            {
//                vinCode = PLCPack2SN;
//                emit sendStationFinishToMaindow(vinCode,"22");
//            }
//            else if(Station == "A390A" || Station == "A390B")
//            {
//                vinCode = PLCPack1SN;
//                emit sendStationFinishToMaindow(vinCode,"21");
//            }
////            vinCode = PLCPack1SN;
////            emit sendStationFinishToMaindow(vinCode,"21");
//            //*************************只做pack包是ok的************************
//            if(!PLCPack1SN.isEmpty() && PLCPack1Status == "1")
//            {
//                tmpPackNum += 1;
//            }
//            if(!PLCPack2SN.isEmpty() && PLCPack2Status == "1")
//            {
//                tmpPackNum += 2;
//            }
////            if((Station == "A390A" || Station == "A390B") &&(!PLCPack1SN.isEmpty()) && PLCPack1Status == "1")
//            if((Station == "A390A" || Station == "A390B"))
//            {
//                if((!PLCPack1SN.isEmpty()) && PLCPack1Status == "1")
//                {
//                    pack12Num = 1;
//                }
//                else
//                {
//                    pack12Num = 0;
//                }
//                qDebug()<<"this is A390 station"<<pack12Num;
////                pack12Num = 1;
////                qDebug()<<"this is A390 station"<<pack12Num;
//            }
////            else if((Station == "A400A" || Station == "A400B") &&(!PLCPack2SN.isEmpty()) && PLCPack2Status == "1")
//            else if((Station == "A400A" || Station == "A400B"))
//            {
//                if((!PLCPack2SN.isEmpty()) && PLCPack2Status == "1")
//                {
//                    pack12Num = 1;
//                }
//                else
//                {
//                    pack12Num = 0;
//                }
//                qDebug()<<"this is A400 station"<<pack12Num;
////                pack12Num = 1;
////                qDebug()<<"this is A400 station"<<pack12Num;
//            }
//            else
//            {
//                pack12Num = tmpPackNum;
//                qDebug()<<"this is other station"<<pack12Num;
//            }
//            qDebug() << "$$$$$$$$$$ PLC product type::"<<PLCPackType<<PLCPack1SN<<PLCPack1Status<<PLCPack2SN<<PLCPack2Status<<tmpPackNum<<pack12Num;
//            changePackResult();
//            pack1StatusWriteFlag = true;
//            if(writeProductTypeFlag)
//            {
//                qDebug()<<"this is ackCodeHandle";
//                ackCodeHandle("0");
//            }
//        }
//        else
//        {
//            qDebug() << "request PackStatus fail ErrorCode:" <<statusStruct.Errorcode;
////            pack1Result = 3;
////            pack2Result = 3;
////            pack1StatusWriteFlag = true;
////            getStationErrorFinishFlag = true;
////            emit sendStationFinishToMaindow("AGV强制放行","204");
//            if(statusStruct.Errorcode.isEmpty())
//            {
//                PLCPackType = "101";
//            }
//            else
//            {
//                PLCPackType = statusStruct.Errorcode;
//            }
//            getGlueProductTypeFlag = true;
//            statusValidFlag = true;
//        }
//    }
//}


void ConnectPLC::revModulePack(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"post ModulePack success";
        moduleBoundResult = 1;
        moduleCount ++;
    }
    else
    {
        qDebug()<<"post ModulePack fail";
        moduleBoundResult = 101;
    }
    getModuleRecordFinishFlag = true;
}

void ConnectPLC::revKeybarcode(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"post Keybarcode success";
    }
    else
        qDebug()<<"post Keybarcode fail";
}

void ConnectPLC::revModule(bool isSuccess, QString module)
{
    if(isSuccess)
    {
        qDebug()<<"request module success"<<module;
    }
    else
        qDebug()<<"request Module fail";
}

void ConnectPLC::revStationState(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"auto post StationState success";
    }
    else
        qDebug()<<"auto post StationState fail";
}

void ConnectPLC::revModuleCount(bool isSuccess)
{
    if(isSuccess)
    {
        qDebug()<<"post ModuleCount success";
    }
    else
        qDebug()<<"post ModuleCount fail";
}
/*********************************************/
//receive box barcode
/*********************************************/
void ConnectPLC::revBoxCodeFunc(QString buf)
{
    QDateTime time = QDateTime::currentDateTime();//获取系统现在的时间
    QString regexpVM1 = "12121212";
    QString regexpVM2 = "1212";
    strBoxCode = buf;
    if(buf == "Scan Error")
    {
        scanResult = 171;
    }
    else
    {
        scanResult = 1;
    }
    if(strBoxCode != strBoxCodeTemp)
    {
        strBoxCodeTemp = strBoxCode;
        strBoxCodeFlag = true;
    }
    else
    {
        qDebug()<<"this is equ scanBuff";
    }
    if(strBoxCodeFlag)
    {
        if(Station == "VM010-1")
        {
            if(regexpVM1.mid(codeNum,1) == "1")
            {
                if(strBoxCode.contains(regExpVm1))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm1 scan regexp1 error"<<strBoxCode<<regExpVm1;
                    scanResult = 172;
                }
            }
            else if(regexpVM1.mid(codeNum,1) == "2")
            {
                if(strBoxCode.contains(regExpVm2))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm1 scan regexp2 error"<<strBoxCode<<regExpVm2;
                    scanResult = 172;
                }
            }
        }
        if(Station == "VM010-2")
        {
            if(regexpVM2.mid(codeNum,1) == "1")
            {
                if(strBoxCode.contains(regExpVm1))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm2 scan regexp1 error"<<strBoxCode<<regExpVm1;
                    scanResult = 172;
                }
            }
            else if(regexpVM2.mid(codeNum,1) == "2")
            {
                if(strBoxCode.contains(regExpVm2))
                {
                    strBoxCoderegExpFlag = true;
                }
                else
                {
                    qDebug()<<"this is vm2 scan regexp2 error"<<strBoxCode<<regExpVm2;
                    scanResult = 172;
                }
            }
        }
//        if(Station == "VM010-1" || Station == "VM010-2")
//        {
//            if(regexpVM1.mid(codeNum,1) == "1")
//            {
//                if(strBoxCode.contains(regExpVm1))
//                {
//                    strBoxCoderegExpFlag = true;
//                }
//                else
//                {
//                    qDebug()<<"this is vm scan regexp1 error"<<strBoxCode<<regExpVm1;
//                    scanResult = 172;
//                }
//            }
//            else if(regexpVM2.mid(codeNum,1) == "2")
//            {
//                if(strBoxCode.contains(regExpVm2))
//                {
//                    strBoxCoderegExpFlag = true;
//                }
//                else
//                {
//                    qDebug()<<"this is vm scan regexp2 error"<<strBoxCode<<regExpVm2;
//                    scanResult = 172;
//                }
//            }
//        }
    }
    getScanBarcodeFlag = true;
    statusValidFlag = true;
    PLCStepMap = PLCstepList[curStepNum].toMap();
    writeAddr = PLCStepMap["writeAdd"].toInt();
    if(Station == "VM010-1" && strBoxCodeFlag)
    {
        strBoxCodeFlag = false;
        if(codeNum >=8)
        {
            codeNum = 0;
        }
        if(codeNum != 0)
        {
            emit sendReqKeybarcode(strBoxCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
        }
        else
        {
            getScanBarcodeVM010Flag = true;
        }
        codeBuf[codeNum] = strBoxCode;
        codeNum++;
        if(codeNum == 8)
        {
            qDebug()<<"this is send Vm010-1 bind";
            emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7]);
        }
    }
    if(Station == "VM010-2" && strBoxCodeFlag)\
    {
        strBoxCodeFlag = false;
        if(codeNum >= 4)
        {
            codeNum = 0;
        }
        if(codeNum != 0)
        {
            emit sendReqKeybarcode(strBoxCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
        }
        else
        {
            getScanBarcodeVM010Flag = true;
        }
        codeBuf[codeNum] = strBoxCode;
        codeNum ++;
        if(codeNum == 4)
        {
            qDebug()<<"this is send Vm010-2 bind";
            emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3]);
        }
    }
//    if(Station == "VM010-1" || Station == "VM010-2")
//    {
//        emit sendReqKeybarcode(strBoxCode,RFIDCode,time.toString("yyyy-MM-dd hh:mm:ss"),1);
//        if(codeNum >= 8)
//        {
//            codeNum = 0;
//        }
//        codeBuf[codeNum] = strBoxCode;
//        codeNum ++;
//        if(codeNum == 8)
//        {
//            qDebug()<<"this is send Vm010 bind";
//            emit sendReqModuleTray(RFIDCode,codeBuf[0],codeBuf[1],codeBuf[2],codeBuf[3],codeBuf[4],codeBuf[5],codeBuf[6],codeBuf[7]);
//        }
//    }
}
/*********************************************/
//server ack error code handel
/*********************************************/
//void ConnectPLC::ackCodeHandle(QString type)
//{
//    if(ackBoundPackFlag)
//    {
//        ackBoundPackFlag = false;
//        getBindingSNFlag = true;
//        if(type == "0")
//        {
//            ackCode = 1;
//        }
//        else
//        {
//            ackCode = 101;
//        }
//    }
//    else if(ackProductTypeFlag)
//    {
//        qDebug()<<"%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";
//        ackProductTypeFlag = false;
//        getProductTypeFlag = true;
////        writeProductFinishFlag = false;
//        if(type == "0")
//        {
//            ackCode = 1;
//        }
//        else
//        {
//            ackCode = 101;
//        }
//    }
//    qDebug()<<"**********123456**********"<<type<<ackCode;
//}


void ConnectPLC::recvTrayCleanResult()
{
    qDebug()<<"send type == 1";
    QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    if(Station.mid(0,2) == "VM")
    {
        int Type = 1;//进站
        emit sendUpInAndOutStation(Type,RFIDCode,"",Station,DataTime);//小车进站发到interface上传接口
    }
}

/*********************************************/
//receive RFID ACK
/*********************************************/
void ConnectPLC::revRFIDAck(QString errorCode, QByteArray code)
{
    qDebug()<<"read RFID ACK"<<errorCode<<code<<reqProductTrayFlag;
    isReadRfidFlag = false;
    if(errorCode == "0000")
    {
        if(reqProductTrayFlag)
        {
            updataTraySN = code;
            reqProductTrayFlag = false;
        }
    }
    if(tellTrayReadyFlag)
    {
        tellTrayReadyFlag = false;
        if(errorCode == "0000")//RFID read OK
        {
            RFIDCode = code;
            qDebug()<<"$$$$$updataTraySN"<<updataTraySN<<RFIDCode;
            if(updataTraySN.isEmpty())
            {
                updataTraySN = RFIDCode;
            }
            else
            {
                if(updataTraySN == RFIDCode)
                {

                }
                else
                {
                    updataTrayTemp = RFIDCode;
                }
            }
            if(Station == "VM010-1" || Station == "VM010-2"){
                qDebug()<<"this is sendModuleTrayClean"<<RFIDCode;
                emit sendModuleTrayClean(RFIDCode);
            }
            if(RFIDCodeTemp1 != RFIDCode)
            {
                getRFIDCodeVM010Flag = true;
                RFIDCodeTemp1 = RFIDCode;
                trayProtectFlag = true;
                qDebug()<<"this is new Tray";
                QString DataTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
                if(Station.mid(0,2) == "VM")
                {
                    int Type = 1;//进站
                    emit sendUpInAndOutStation(Type,RFIDCode,"",Station,DataTime);//小车进站发到interface上传接口
                }
            }
            if(Station.mid(0,2) == "OP")
            {
                emit sendBMCEFirstStation(RFIDCode);
            }
            if(Station == "OP80")
            {
                QByteArray vinTemp = RFIDCode.toLatin1();
                for(int i = 0; i < vinTemp.size(); i++)
                {
                    strWriteBufTemp[i] = (unsigned char)vinTemp[i];
                }
                writeLen = vinTemp.size();
                writePLCFunc(stationAddr,150,writeLen,strWriteBufTemp);
            }
            GRFIDMessage = code;
            readRFIDResult = 1;
            if(RFIDCode.mid(0,1) == "P")
            {
                GrfidStatusTemp = 1;
            }
            else if(RFIDCode.mid(0,1) == "B")
            {
                GrfidStatusTemp = 2;
            }
            else
            {
                qDebug()<<"this is rfid error";
                GrfidStatusTemp = 3;
            }
            emit sendCmdToStep(6,RFIDCode,15);
        }
        else if(errorCode == "0001")//RFID no connect
        {
            readRFIDResult = 160;
            GrfidStatusTemp = 160;
//            RFIDCode = "disconnect";
            GEKSReRFID = true;
            tellTrayReadyFlag = true;
            if(!trayProtectFlag)
            {
                emit sendStationFinishToMaindow("RFID未连接","204");
            }
            emit sendReadCmdToRFID("0000","0015");//请求 RFID 数据
            //emit sendStates("RFID未连接");
        }
        else if(errorCode == "0002")//RFID no read data / over 3S to timeout
        {
            readRFIDResult = 161;
            GrfidStatusTemp =161;
//            RFIDCode = "timeOut";
            GEKSReRFID = true;
            tellTrayReadyFlag = true;
            if(!trayProtectFlag)
            {
                emit sendStationFinishToMaindow("RFID读取超时","204");
            }
            emit sendReadCmdToRFID("0000","0015");//请求rfid数据
           // emit sendStates("RFID读取超时");
        }
        else//RFID read error data
        {
            readRFIDResult = 162;
            GrfidStatusTemp = 162;
//            RFIDCode = "otherError";
            GEKSReRFID = true;
            tellTrayReadyFlag = true;
            if(!trayProtectFlag)
            {
                emit sendStationFinishToMaindow("RFID连接未知错误","204");
            }
            emit sendReadCmdToRFID("0000","0015");//请求rfid数据
            //emit sendStates("RFID连接未知错误");
        }
//        GrfidStatusTemp = readRFIDResult;
        getTrayReadyFlag = true;
//        emit sendCmdToStep(6,"P-002",15);
//        emit sendCmdToStep(6,RFIDCode,15);
    }
    else if(tellRollerBedReadyFlag)
    {
        tellRollerBedReadyFlag = false;
        if(errorCode == "0000")//read OK
        {
            RFIDCode = code;
            /*if( ((PLCPackType == "1")&&(RFIDCode.mid(0,1) == "P")) ||
                ((PLCPackType == "2")&&(RFIDCode.mid(0,1) == "B")) )
            {
                readRFIDResult = 1;
            }
            else
            {
                readRFIDResult = 114;
            }*/
            if(RFIDCode.mid(0,1) == "P")
            {
                readRFIDResult = 1;
            }
            else if(RFIDCode.mid(0,1) == "B")
            {
                readRFIDResult = 2;
            }
            else
            {
                qDebug()<<"this is rfid error";
                readRFIDResult = 3;
            }
        }
        else if(errorCode == "0001")//RFID no connect
        {
            readRFIDResult = 160;
        }
        else if(errorCode == "0002")//RFID no read data / over 3S to timeout
        {
            readRFIDResult = 161;
        }
        else//RFID read error data
        {
            readRFIDResult = 162;
        }
        getRollerBedReadyFlag = true;
    }
}

void ConnectPLC::revCS351Status(bool)
{

}

//重新读取rfid
void ConnectPLC::revRFIDRead()
{
    qDebug()<<"this is maindow send RFID Read";
    //statusValidFlag = false;
}

/**************************************
 * read pack status
***************************************/
int ConnectPLC::readPackStatus(int numValue,int size)
{
    pack12Num = numValue/size; //pack number
    numValue = numValue%size;//work times per pack
    if(pack12Num == 1)//first pack
    {
        pack1Result = 4;
        pack2Result = 3;
    }
    else if(pack12Num == 2)//second pack
    {
        pack1Result = 3;
        pack2Result = 4;
    }
    else if(pack12Num == 3)//two pack
    {
        pack1Result = 4;
        pack2Result = 4;
    }
    else if(pack12Num == 0)
    {
        pack1Result = 3;
        pack2Result = 3;
    }
    qDebug()<<":pack1Result::"<<pack1Result<<":pack2Result::"<<pack2Result<<":pack12Num::"<<pack12Num;
    return numValue;
}

void ConnectPLC::changePackResult()
{
    qDebug()<<"this is GPackType"<<GPackType<<"GBevPass"<<GBevPass;
    if(GPackType == GBevPass)
    {
        pack1Result = 3;
        pack2Result = 3;
        packUplodeTemp1 = "3";
        packUplodeTemp2 = "3";
        getStationErrorFinishFlag = true;
        writeProductTypeFlag = false;
        emit sendStationFinishToMaindow("AGV强制放行","204");
        qDebug()<<"this is bev getStationErrorFinishFlag";
    }
    else
    {
        if(pack12Num == 1)//first pack
        {
            pack1Result = 4;
            pack2Result = 3;
            writeProductTypeFlag = true;
        }
        else if(pack12Num == 2)//second pack
        {
            pack1Result = 3;
            pack2Result = 4;
            writeProductTypeFlag = true;
        }
        else if(pack12Num == 3)//two pack
        {
            pack1Result = 4;
            pack2Result = 4;
            writeProductTypeFlag = true;
        }
        else if(pack12Num == 0 || (GPackType == "BEV" && GBevPass == "Yes"))
        {
            if(Station == "A350-1" || Station == "A350-2")
            {
                pack1Result = 4;
                pack2Result = 4;
                writeProductTypeFlag = true;
            }
            else
            {
                pack1Result = 3;
                pack2Result = 3;
                getStationErrorFinishFlag = true;
                writeProductTypeFlag = false;
                emit sendStationFinishToMaindow("AGV不合格放行","220");
                qDebug()<<"this is getStationErrorFinishFlag";
            }
        }}
    qDebug()<<"this is pack1result"<<pack1Result<<pack2Result<<GBevPass;
}

void ConnectPLC::errorHandle()
{
    if(glueStartWorkFlag)
    {
        for(int i=0;i<8;i++) //clear valid step value
        {
            strWriteBufTemp[i] = 0;
        }
        writePLCFunc(stationAddr,100,6,strWriteBufTemp);
        writePLCFunc(stationAddr,114,6,strWriteBufTemp);
    }
    else
    {
        //qDebug()<<"glueStartWorkFlag"<<glueStartWorkFlag;
        writePLCFlag = true;
        writeLen = (PLCWorkAllStep<<1)-2;
        for(int i=0;i<writeLen;i++) //clear valid step value
        {
            strWriteBuf[i] = 0;
        }
        writeAddr = 100;
    }
}

void ConnectPLC::writePackSNToPlc(QString pack1sn, QString pack2sn)
{
    if(pack1sn.isEmpty())
    {
        pack1sn = "pack1SN=NULL";
    }
    if(pack2sn.isEmpty())
    {
        pack2sn = "pack2SN=NULL";
    }
//    QString packSN = "=="+pack1sn+"<>"+pack2sn;
//    QByteArray vinTemp = packSN.toLatin1();
//    for(int i = 0; i < vinTemp.size(); i++)
//    {
//        strWriteBufTemp[i] = (unsigned char)vinTemp[i];
//    }
//    writeLen = vinTemp.size();
//    writePLCFunc(stationAddr,writeToPlcPackSNAddr,writeLen,strWriteBufTemp);
    pack1sn = "=="+pack1sn;
    QByteArray vinTemp = pack1sn.toLatin1();
    for(int i = 0; i < vinTemp.size(); i++)
    {
        strWriteBufTemp[i] = (unsigned char)vinTemp[i];
    }
    writeLen = vinTemp.size();
    writePLCFunc(stationAddr,writeToPlcPack1SNAddr,writeLen,strWriteBufTemp);
    pack2sn = "=="+pack2sn;
    vinTemp = pack2sn.toLatin1();
    for(int i = 0; i < vinTemp.size(); i++)
    {
        strWriteBufTemp[i] = (unsigned char)vinTemp[i];
    }
    writeLen = vinTemp.size();
    writePLCFunc(stationAddr,writeToPlcPack2SNAddr,writeLen,strWriteBufTemp);
}

//void ConnectPLC::reModuleClean( )
//{
//}


//匹配VIN码
int ConnectPLC::VINSearch(QString vin)
{
    //匹配VIN码
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    bool flags = true;
    QString vinTemp = vin.mid(3,5);
    int i = 1;

    for(i = 1;i < 21;i++)
    {
        QString temp = config->value(QString("carinfo").append(QString::number(i)).append("/VIN")).toString();
        if(temp.size() > 0){
            flags = true;
            for(int k = 0;k < temp.size();k++)//校验
            {
                if(temp.at(k) == '?')//设置为?则不匹配，跳过
                {
                    continue;
                }
                else
                {
                    if(vinTemp.at(k) == temp.at(k))
                    {
                        continue;
                    }
                    else
                    {
                        flags = false;
                        break;
                    }
                }
            }
        }else{
            flags = false;
            break;

        }
        if(flags){//匹配到直接出来
            break;
        }
    }

    if(flags)
    {
        delete config;
        return i;
    }else{
        delete config;
        return -1;
    }

}

//匹配G9
int ConnectPLC::G9Search(QString G9)
{
    //匹配G9
    int i = 0;
    QString temp;
    bool flagss = true;
    QSettings *config = new QSettings("/config.ini", QSettings::IniFormat);
    for(i = 1;i < 21;i++)
    {
        temp = config->value(QString("carinfo").append(QString::number(i)).append("/G9")).toString();
        if(temp.size() > 0){
            flagss = true;
            for(int j=0;j<temp.size();j++)    //change 2018-0427
            {
                if(temp.at(j) == '?')
                    continue;
                else
                {
                    if(temp.at(j) == G9.at(j))
                    {
                        continue;
                    }
                    else
                    {
                        flagss = false;
                        break;
                    }
                }
            }
        }else{
            flagss = false;
            break;
        }
        if(flagss)
        {
            break;
        }

    }
    if(flagss)
    {
        delete config;
        return i;
    }else{
        delete config;
        return -1;
    }
}

//QString ConnectPLC::FISHandType()
//{
//    qDebug()<<"fishand1";
//    QString Temp1 = "3";
//    QString RFIDTemp;
//    QString VINTemp;
//    QSqlDatabase db2=QSqlDatabase::addDatabase("QMYSQL","mysqlconnect");
//    db2.setHostName("localhost");
//    db2.setDatabaseName("Tighten");
//    db2.setUserName("root");
//    db2.setPassword("123456");
//    query2 = QSqlQuery(db2);
//    if(!db2.open())
//    {
//        if(!db2.open())
//        {
//            qDebug()<< "fisupdate localmysql "<< db2.lastError().text();
//        }else
//        {
//            qDebug()<< "fisupdate localmysql open ok 2";
//        }
//    }
//    else
//    {
//        qDebug()<< "fisupdate localmysql open ok 1";
//    }
//    if(db2.isOpen()||QSqlDatabase::contains("mysqlconnect"))
//    {
//        QString  tmpserial;
//        //        if(serialNums.length()==14)
//        //        {

//        //            selectVin = "select FisMatch from "+tablePreview+" where KNR ='";
//        //            tmpserial = serialNums.right(8);
//        //        }
//        //        else if(serialNums.length()==17)
//        //        {

//        //            selectVin = "select FisMatch from "+tablePreview+" where VIN ='";

//        //            tmpserial = serialNums;
//        //        }
//        if(RFIDVin.size() > 15)
//        {
//            RFIDTemp = "VIN";
//            VINTemp = RFIDVin;
//            selectVin = "select FisMatch from "+tablePreview+" where VIN ='";
//            tmpserial = RFIDVin;
//        }
//        else
//        {
//            RFIDTemp = "KNR";
//            VINTemp = RFIDVin.right(8);
//            selectVin = "select FisMatch from "+tablePreview+" where KNR ='";
//            tmpserial = RFIDVin.right(8);
//        }
//        QString all = selectVin+tmpserial.append("'");
//        qDebug()<<"sqlall"<<all;
//        //        if(!query2.exec("select FisMatch from "+tablePreview+" where "+RFIDTemp+" = "+VINTemp))
//        query2.exec(all);
//        if(query2.next())
//        {
//            QByteArray jsonData;
//            QJson::Parser parser;
//            bool ok = false;
//            QVariantMap FisMatch;
//            jsonData = query2.value(0).toByteArray();
//            qDebug()<< "jsonData" <<jsonData<<query2.value(0);
//            FisMatch = parser.parse(jsonData, &ok).toMap();
//            if (!ok)
//            {
//                qDebug()<<"An error occurred during parsing"<<jsonData;
//            }
//            if(FisMatch["KRS"].toString() == "0F0")
//            {
//                qDebug()<<"fisISTrue";
//                Temp1 = "2";
//            }
//        }
//    }
//    else
//        qDebug() << "fis open fail";
//    if(db2.isOpen())
//    {
//        db2.close();
//    }
//    return Temp1;
//}

void ConnectPLC::fromsecondthreaddataSB356(QString proNum)
{
    currentProNum = proNum.toInt();
}

//VIN码检验
bool ConnectPLC::VINCheck(QString code)
{
    QByteArray buf = code.toLocal8Bit().data();
    QByteArray VinTempBuf;
    QByteArray VINhead = head3Code.toLocal8Bit();
    int Sum_Num = 0;
    for(int i=0;i<3;i++)
    {
        if(buf[i]!=VINhead[i])
            return false;
    }

    for(int i=0;i<6;i++)//right 6 bit range(0~9)
    {
        if((buf[i+11]>'9')||(buf[i+11]<'0'))
        {
            return false;
        }
    }

    for(int i=0;i<17;i++)
    {
        if(i == 8)
        {
            if(buf[i]=='X') //校验位不需要
            {
                VinTempBuf[i] = 'X';
                continue;
            }
        }
        switch(buf[i])
        {
        case '0':
            VinTempBuf[i] = 0;
            break;
        case '1':
        case 'A':
        case 'J':
            VinTempBuf[i] = 1;
            break;
        case '2':
        case 'B':
        case 'K':
        case 'S':
            VinTempBuf[i] = 2;
            break;
        case '3':
        case 'C':
        case 'L':
        case 'T':
            VinTempBuf[i] = 3;
            break;
        case '4':
        case 'D':
        case 'M':
        case 'U':
            VinTempBuf[i] = 4;
            break;
        case '5':
        case 'E':
        case 'N':
        case 'V':
            VinTempBuf[i] = 5;
            break;
        case '6':
        case 'F':
        case 'W':
            VinTempBuf[i] = 6;
            break;
        case '7':
        case 'G':
        case 'P':
        case 'X':
            VinTempBuf[i] = 7;
            break;
        case '8':
        case 'H':
        case 'Y':
            VinTempBuf[i] = 8;
            break;
        case '9':
        case 'R':
        case 'Z':
            VinTempBuf[i] = 9;
            break;
        default:
            return false;//Vin code valid
            //				break;
        }
    }
    Sum_Num  = VinTempBuf[7]*10;
    Sum_Num += VinTempBuf[9]*9;
    Sum_Num += (VinTempBuf[0]+VinTempBuf[10])*8;
    Sum_Num += (VinTempBuf[1]+VinTempBuf[11])*7;
    Sum_Num += (VinTempBuf[2]+VinTempBuf[12])*6;
    Sum_Num += (VinTempBuf[3]+VinTempBuf[13])*5;
    Sum_Num += (VinTempBuf[4]+VinTempBuf[14])*4;
    Sum_Num += (VinTempBuf[5]+VinTempBuf[15])*3;
    Sum_Num += (VinTempBuf[6]+VinTempBuf[16])*2;
    Sum_Num %=11; //求余数
    qDebug()<<"***********sum"<<Sum_Num<<VinTempBuf[8];
    if(Sum_Num == 10)
    {
        if(VinTempBuf[8] == 'X')
            return 1;//Vin code valid
        else
            return 0;//Vin code invalid
    }
    else
    {
        if((int)VinTempBuf[8] == Sum_Num)
            return 1;//Vin code valid
        else
            return 0;//Vin code invalid
    }
}

//PIN码检验
bool ConnectPLC::PINCheck(QString code)
{
    QByteArray pinBuf = code.toLocal8Bit().data();
    int sum,sumOdd,sumEven,result;
    int len = 14;
    QByteArray PinTempBuf;
    sum = 0;
    sumOdd = 0;
    sumEven = 0;
    for(int i=0;i<len;i++)
    {
        switch(pinBuf[i])
        {
        case '0':
            PinTempBuf[i] = 0;
            break;
        case '1':
        case 'A':
        case 'J':
        case 'S':
            PinTempBuf[i] = 1;
            break;
        case '2':
        case 'B':
        case 'K':
        case 'T':
            PinTempBuf[i] = 2;
            break;
        case '3':
        case 'C':
        case 'L':
        case 'U':
            PinTempBuf[i] = 3;
            break;
        case '4':
        case 'D':
        case 'M':
        case 'V':
            PinTempBuf[i] = 4;
            break;
        case '5':
        case 'E':
        case 'N':
        case 'W':
            PinTempBuf[i] = 5;
            break;
        case '6':
        case 'F':
        case 'O':
        case 'X':
            PinTempBuf[i] = 6;
            break;
        case '7':
        case 'G':
        case 'P':
        case 'Y':
            PinTempBuf[i] = 7;
            break;
        case '8':
        case 'H':
        case 'Q':
        case 'Z':
            PinTempBuf[i] = 8;
            break;
        case '9':
        case 'R':
        case 'I':
            PinTempBuf[i] = 9;
            break;
        default:
            return false ;
            break;
        }
    }
    for(int i=0;i<len-1;i+=2)
    {
        sumOdd += PinTempBuf[i];
    }
    for(int i=1;i<len-1;i+=2)
    {
        sumEven += PinTempBuf[i];
    }
    sum = sumOdd*3 + sumEven;
    result = (10-sum%10)%10;
    if(result == PinTempBuf[len-1])
    {
        return true;
    }
    else
    {
        return false;
    }
}


void ConnectPLC::receiveManualVIN(QString code)
{
    if(RFIDVin.isEmpty()){
        manualCodeFlag = true;
        QString headStr = code.left(3);
        if(headStr == head3Code){//VIN
            if(VINCheck(code.left(17))){
                //读到个新码
//                if(RFIDVin != code.mid(0,17))
                {
                    codeErrorFlag = false;
                    RFIDVin = code.mid(0,17);
                    strWriteBufTemp[0] = 0;
                    strWriteBufTemp[1] = 1;
                    writePLCFunc(stationAddr,112,2,strWriteBufTemp);
                    emit sendStationFinishToMaindow(RFIDVin,"233");//发窗口做界面的操作
                }
            }else{//条码异常在PLC请求的时候发，并在界面显示方便用户查看
                codeErrorFlag = true;
                RFIDVin.clear();
                emit sendMesToWindow("手输车码异常",true);
            }
        }else{//PIN
            if(PINCheck(code.left(14))){
                //读到个新码
//                if(RFIDVin != code.mid(0,14))
                {
                    codeErrorFlag = false;
                    RFIDVin = code.mid(0,14);//"78202072000105"
                    RFIDG9 = code.mid(14,4);//"3M  "
                    strWriteBufTemp[0] = 0;
                    strWriteBufTemp[1] = 1;
                    writePLCFunc(stationAddr,112,2,strWriteBufTemp);
                    emit sendStationFinishToMaindow(RFIDVin,"233");//发窗口做界面的操作
                }
            }else{//条码异常在PLC请求的时候发，并在界面显示方便用户查看
                codeErrorFlag = true;
                RFIDVin.clear();
                emit sendMesToWindow("手输车码异常",true);
            }
        }
    }else{
        emit sendMesToWindow("有车，请勿手动输入条码!",true);
    }
}

void ConnectPLC::stationFinishFunc()
{
    getStationFinishFlag = true;//站完成标志
    emit sendMesToWindow("等待小车进站!",true);
    manualCodeFlag = false;//手动输入条码状态取消
    statusValidFlag = true;
    RFIDG9.clear();
    RFIDVin.clear();
    emit sendStationFinishToMaindow(RFIDVin,"233");//站完成发窗口做界面的操作
    workSecondePackFlag = false;
}
