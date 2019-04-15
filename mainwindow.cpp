#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "can.h"
#include <QTimer>
#include <QLibrary>
unsigned long  m_devtype=4;//USBCAN2类型号
unsigned long m_devind=0;
unsigned long m_cannum=0;
int m_connect=0;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_CanReadTimer = new QTimer(this);
    connect(m_CanReadTimer,SIGNAL(timeout()),this,SLOT(on_CanReadTimerOut()));  //绑定定时器的信号与槽

    QLibrary CanLib("ControlCAN.dll");
    if(CanLib.load())
    {
        //取得函数地址
        VCI_OpenDevice=(LPVCI_OpenDevice)CanLib.resolve("VCI_OpenDevice");
        VCI_CloseDevice=(LPVCI_CloseDevice)CanLib.resolve("VCI_CloseDevice");
        VCI_InitCAN=(LPVCI_InitCan)CanLib.resolve("VCI_InitCAN");
        VCI_ReadBoardInfo=(LPVCI_ReadBoardInfo)CanLib.resolve("VCI_ReadBoardInfo");
        VCI_ReadErrInfo=(LPVCI_ReadErrInfo)CanLib.resolve("VCI_ReadErrInfo");
        VCI_ReadCanStatus=(LPVCI_ReadCanStatus)CanLib.resolve("VCI_ReadCANStatus");
        VCI_GetReference=(LPVCI_GetReference)CanLib.resolve("VCI_GetReference");
        VCI_SetReference=(LPVCI_SetReference)CanLib.resolve("VCI_SetReference");
        VCI_GetReceiveNum=(LPVCI_GetReceiveNum)CanLib.resolve("VCI_GetReceiveNum");
        VCI_ClearBuffer=(LPVCI_ClearBuffer)CanLib.resolve("VCI_ClearBuffer");
        VCI_StartCAN=(LPVCI_StartCAN)CanLib.resolve("VCI_StartCAN");
        VCI_ResetCAN=(LPVCI_ResetCAN)CanLib.resolve("VCI_ResetCAN");
        VCI_Transmit=(LPVCI_Transmit)CanLib.resolve("VCI_Transmit");
        VCI_Receive=(LPVCI_Receive)CanLib.resolve("VCI_Receive");
        VCI_GetReference2=(LPVCI_GetReference2)CanLib.resolve("VCI_GetReference2");
        VCI_SetReference2=(LPVCI_SetReference2)CanLib.resolve("VCI_SetReference2");
        VCI_ResumeConfig=(LPVCI_ResumeConfig)CanLib.resolve("VCI_ResumeConfig");
        VCI_ConnectDevice=(LPVCI_ConnectDevice)CanLib.resolve("VCI_ConnectDevice");
        VCI_UsbDeviceReset=(LPVCI_UsbDeviceReset)CanLib.resolve("VCI_UsbDeviceReset");
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    m_CanReadTimer->stop();  //停止定时
}


void MainWindow::on_CanConnect_clicked()
{

    if(m_connect==1)
   {
           ui->CanConnect->setText("连接");
           m_connect=0;
           //WaitForSingleObject(m_readhandle,2000);
           //m_readhandle=NULL;
           VCI_CloseDevice(m_devtype,m_devind);
           //EnableUI(TRUE);
           return;
   }
   int index = 0;
   int cannum = ui->CanRoute->currentIndex();
   VCI_INIT_CONFIG initconfig;
   //initconfig.AccCode=ui->AccCode->text();
   initconfig.AccCode = 0;
   //initconfig.AccMask=ui->AccMask->text();
   initconfig.AccMask= 0xFFFFFFFF;

   //initconfig.Timing0=ui->Timing0->text();
   initconfig.Timing0 = 0;
   //initconfig.Timing1=ui->Timing0->text();
   initconfig.Timing1 = 0x14;

   initconfig.Filter=ui->FilterType->currentIndex();
   initconfig.Mode=ui->WorkMode->currentIndex();

   if(index>=0&&cannum>=0)
   {

       if(VCI_OpenDevice(m_devtype,index,0)==1)
           {

               if(VCI_InitCAN(m_devtype,index,cannum,&initconfig)==1)
                   {

                           m_connect=1;
                           m_devind=0;
                           m_cannum=cannum;
                            ui->CanConnect->setText("断开");
                   }

                   else
                   {

                   }
           }
           else
           {

          }

   }

   //EnableUI(FALSE);
}


VCI_CAN_OBJ s_CanData[1000];
int iCanDataNum = 0;
void MainWindow::on_CanReadTimerOut()
{
    uint32_t CanDataID = 0;
    QString cStr;
    iCanDataNum = VCI_Receive(m_devtype, m_devind, m_cannum,s_CanData,1000, 100);
    printf("%d\n",iCanDataNum);
    for(uint32_t i = 0;i < iCanDataNum; i++)
    {
        CanDataID = s_CanData[i].ID;

        DisplayData((VCI_CAN_OBJj *)&s_CanData[i].ID);
    }
}

void MainWindow::on_CanStart_clicked()
{
    VCI_StartCAN(m_devtype,m_devind,m_cannum);
    m_CanReadTimer->start(100);  //启动CAN数据读取定时
}


void MainWindow::CanSendData()
{
    VCI_CAN_OBJ SendCanData;
    SendCanData.ExternFlag = ui->FilterType->currentIndex();
    SendCanData.RemoteFlag = ui->FreamFormat->currentIndex();
    SendCanData.DataLen = 8;
    SendCanData.ID = ui->FreamID->text().toInt();
    SendCanData.Data[0] = 0x01;
    SendCanData.Data[1] = 0x02;
    SendCanData.Data[2] = 0x03;
    SendCanData.Data[3] = 0x04;
    SendCanData.Data[4] = 0x05;
    SendCanData.Data[5] = 0x06;
    SendCanData.Data[6] = 0x07;
    SendCanData.Data[7] = 0x08;
    if(VCI_Transmit(m_devtype,m_devind,m_cannum,(VCI_CAN_OBJ *)&SendCanData.ID,1)==0)
    {

    }
    DisplayData((VCI_CAN_OBJj *)&SendCanData.ID);
}

void MainWindow::DisplayData(VCI_CAN_OBJj *dispdata)
{
      uint32_t   CanDataID;
    CanDataID = dispdata->ID;
QString cStr;
            cStr = "帧ID:";
            cStr.append(QString("%1").arg(CanDataID,8,16,QLatin1Char('0')));
            //cStr =
            //sprintf((char*)cStr,"%d",CanDataID);
            cStr.append(" 发送板:");

            if (((CanDataID >> 24)&0x000F) == 0x00)
            {
                cStr.append("控制板A");
            }
            else if(((CanDataID >> 24)&0x000F) == 0x01)
            {
                cStr.append("控制板B");
            }
            else if(((CanDataID >> 24)&0x000F) == 0x02)
            {
                cStr.append("记录板");
            }
            else if(((CanDataID >> 24)&0x000F) == 0x03)
            {
                cStr.append("轴温A");
            }
            else if(((CanDataID >> 24)&0x000F) == 0x04)
            {
                cStr.append("轴温B");
            }

            else if(((CanDataID >> 24)&0x000F) == 0x05)
            {
                cStr.append("平稳");
            }
            else if(((CanDataID >> 24)&0x000F) == 0x06)
            {
                cStr.append("失稳");
            }

            cStr.append(" 接收板：");
            if (((CanDataID >> 14) & 0x03FF) == 0x1)
            {
                cStr.append("控制板A");
            }
            else if (((CanDataID >> 14) & 0x03FF) == 0x2)
            {
                cStr.append("控制板B");
            }
            else if (((CanDataID >> 14) & 0x03FF) == 0x4)
            {
                cStr.append("记录板");
            }
            else if (((CanDataID >> 14) & 0x03FF) == 0x8)
            {
                cStr.append("轴温A");
            }
            else if (((CanDataID >> 14) & 0x03FF) == 0x10)
            {
                cStr.append("轴温B");
            }

            else if (((CanDataID >> 14) & 0x03FF) == 0x20)
            {
                cStr.append("平稳");
            }
            else if (((CanDataID >> 14) & 0x03FF) == 0x40)
            {
                cStr.append("失稳");
            }
            cStr.append(" 生命信号：");

            uint32_t Temp = 0;
            Temp = ((CanDataID >> 12) & 0x0003);
            cStr.append(QString::number(Temp,16));

            cStr.append(" 帧ID总数：");
            Temp = ((CanDataID >> 6) & 0x003F);
            cStr.append(QString::number(Temp,16));

            cStr.append(" 帧序号：");
            Temp = (CanDataID & 0x003F);
            cStr.append(QString::number(Temp,16));

            cStr.append(" 数据：");
            if (dispdata->RemoteFlag == 0)
            {
                for(uint8_t j = 0; j < 8;j++)
                {

                   cStr.append(QString("%1").arg(dispdata->Data[j],2,16,QLatin1Char('0')));
                   cStr.append(" ");
                }
            }


            ui->plainTextEdit->appendPlainText(cStr);

}

void MainWindow::on_SendOne_clicked()
{
    CanSendData();
}
