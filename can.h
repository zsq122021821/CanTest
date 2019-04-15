#ifndef CAN_H
#define CAN_H
/*
下面是具体的调用动态库的做法，采用的是动态加载的方法，主要就是在程序初始化的
时候从动态库中取得各个函数的地址并保存起来，然后在需要的时候调用就可以了，最后在
程序退出的时候释放掉打开的动态库句柄就行。
*/

/*---------------------------兼容ZLG的及数据类型------------------------------*/
//首先定义需要用到的数据结构
//USB-CAN总线适配器板卡信息的数据类型。
typedef  struct  _VCI_BOARD_INFO{
        unsigned short	hw_Version;
        unsigned short	fw_Version;
        unsigned short	dr_Version;
        unsigned short	in_Version;
        unsigned short	irq_Num;
        unsigned char	can_Num;
        char	str_Serial_Num[20];
        char	str_hw_Type[40];
        unsigned short	Reserved[4];
} VCI_BOARD_INFO,*PVCI_BOARD_INFO;

//定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJ{
    unsigned int	ID;
    unsigned int	TimeStamp;	 		 //时间标识
    unsigned char	TimeFlag;			 //是否使用时间标识
    unsigned char	SendType;			 //发送标志。保留，未用
    unsigned char	RemoteFlag;			 //是否是远程帧
    unsigned char	ExternFlag;			 //是否是扩展帧
    unsigned char	DataLen;
    unsigned char	Data[8];
    unsigned char	Reserved[3];			//保留
}VCI_CAN_OBJ,*PVCI_CAN_OBJ;

//定义CAN控制器状态的数据类型。
typedef struct _VCI_CAN_STATUS{
    unsigned char	ErrInterrupt;
    unsigned char	regMode;
    unsigned char	regStatus;
    unsigned char	regALCapture;
    unsigned char	regECCapture;
    unsigned char	regEWLimit;
    unsigned char	regRECounter;
    unsigned char	regTECounter;
    unsigned int	Reserved;
}VCI_CAN_STATUS,*PVCI_CAN_STATUS;

//定义错误信息的数据类型。
typedef struct _ERR_INFO{
    unsigned int	ErrCode;
    unsigned char	Passive_ErrData[3];
    unsigned char	ArLost_ErrData;
} VCI_ERR_INFO,*PVCI_ERR_INFO;

//定义初始化CAN的数据类型
typedef struct _INIT_CONFIG{
    unsigned int	AccCode;
    unsigned int	AccMask;
    unsigned int	InitFlag;
    unsigned char	Filter;   //0,1接收所有帧。2标准帧滤波，3是扩展帧滤波。
    unsigned char	Timing0;
    unsigned char	Timing1;
    unsigned char	Mode;     //模式，0表示正常模式，1表示只听模式,2自测模式
}VCI_INIT_CONFIG,*PVCI_INIT_CONFIG;

//接下来定义要导入的函数类型
//根据ControlCAN.h中的函数声明定义函数指针类型
//////////////////////////////////////////////////////////////////////////
//兼容ZLG的函数
typedef unsigned int (__stdcall*LPVCI_OpenDevice)(unsigned int,unsigned int,unsigned int);
typedef unsigned int (__stdcall*LPVCI_CloseDevice)(unsigned int,unsigned int);
typedef unsigned int (__stdcall*LPVCI_InitCan)(unsigned int,unsigned int,unsigned int,PVCI_INIT_CONFIG);

typedef unsigned int (__stdcall*LPVCI_ReadBoardInfo)(unsigned int,unsigned int,PVCI_BOARD_INFO);
typedef unsigned int (__stdcall*LPVCI_ReadErrInfo)(unsigned int,unsigned int,unsigned int,PVCI_ERR_INFO);
typedef unsigned int (__stdcall*LPVCI_ReadCanStatus)(unsigned int,unsigned int,unsigned int,PVCI_CAN_STATUS);

typedef unsigned int (__stdcall*  LPVCI_GetReference)(unsigned int,unsigned int,unsigned int,unsigned int,void *);
typedef unsigned int (__stdcall*  LPVCI_SetReference)(unsigned int,unsigned int,unsigned int,unsigned int,void *);

typedef unsigned long (__stdcall*  LPVCI_GetReceiveNum)(unsigned int,unsigned int,unsigned int);
typedef unsigned int (__stdcall*  LPVCI_ClearBuffer)(unsigned int,unsigned int,unsigned int);

typedef unsigned int (__stdcall*  LPVCI_StartCAN)(unsigned int,unsigned int,unsigned int);
typedef unsigned int (__stdcall*  LPVCI_ResetCAN)(unsigned int,unsigned int,unsigned int);

typedef unsigned long (__stdcall*  LPVCI_Transmit)(unsigned int,unsigned int,unsigned int,PVCI_CAN_OBJ,unsigned long);
typedef unsigned long (__stdcall*  LPVCI_Receive)(unsigned int,unsigned int,unsigned int,PVCI_CAN_OBJ,unsigned long,int);

/*-------------------------其他函数及数据类型描述-----------------------------*/
//首先定义需要用到的数据结构
//USB-CAN总线适配器板卡信息的数据类型，该类型在VCI_FindUsbDevice函数中引用。
typedef  struct  _VCI_BOARD_INFO1{
        unsigned short	hw_Version;
        unsigned short	fw_Version;
        unsigned short	dr_Version;
        unsigned short	in_Version;
        unsigned short	irq_Num;
        unsigned char	can_Num;
                unsigned char    reserved;
        char	str_Serial_Num[8];
        char	str_hw_Type[16];
        char	str_USB_Serial[4][4];
} VCI_BOARD_INFO1,*PVCI_BOARD_INFO1;

//定义常规参数类型
typedef struct _REF_NORMAL{
    unsigned char Mode;		 //工作模式
    unsigned char Filter;		 //滤波方式
    unsigned int AccCode;		//接收滤波验收码
    unsigned int AccMask;		//接收滤波屏蔽码
    unsigned char kBaudRate;		//波特率索引号，0-SelfDefine,1-5Kbps(未用),2-18依次为：10kbps,20kbps,40kbps,50kbps,80kbps,100kbps,125kbps,200kbps,250kbps,400kbps,500kbps,666kbps,800kbps,1000kbps,33.33kbps,66.66kbps,83.33kbps
    unsigned char Timing0;
    unsigned char Timing1;
    unsigned char CANRX_EN;		//保留，未用
    unsigned char UARTBAUD;		//保留，未用
}VCI_REF_NORMAL,*PVCI_REF_NORMAL;

//定义波特率设置参数类型
typedef struct _BAUD_TYPE{
    unsigned int Baud;		//存储波特率实际值
    unsigned char SJW;		//同步跳转宽度，取值1-4
    unsigned char BRP;		//预分频值，取值1-64
    unsigned char SAM;		//采样点，取值0=采样一次，1=采样三次
    unsigned char PHSEG2_SEL;	//相位缓冲段2选择位，取值0=由相位缓冲段1时间决定,1=可编程
    unsigned char PRSEG;		//传播时间段，取值1-8
    unsigned char PHSEG1;		//相位缓冲段1，取值1-8
    unsigned char PHSEG2;		//相位缓冲段2，取值1-8
}VCI_BAUD_TYPE,*PVCI_BAUD_TYPE;

//定义Reference参数类型
typedef struct _REF_STRUCT{
    VCI_REF_NORMAL RefNormal;
    unsigned char Reserved;
    VCI_BAUD_TYPE BaudType;
}VCI_REF_STRUCT,*PVCI_REF_STRUCT;


//其他函数
typedef unsigned int (__stdcall*  LPVCI_GetReference2)(unsigned int,unsigned int,unsigned int,unsigned int,PVCI_REF_STRUCT);
typedef unsigned int (__stdcall*  LPVCI_SetReference2)(unsigned int,unsigned int,unsigned int,unsigned int,void *);
typedef unsigned int (__stdcall*  LPVCI_ResumeConfig)(unsigned int,unsigned int,unsigned int);

typedef unsigned int (__stdcall*  LPVCI_ConnectDevice)(unsigned int,unsigned int);
typedef unsigned int (__stdcall*  LPVCI_UsbDeviceReset)(unsigned int,unsigned int,unsigned int);
typedef unsigned int (__stdcall*  LPVCI_FindUsbDevice)(PVCI_BOARD_INFO1);

//接口函数指针
LPVCI_OpenDevice VCI_OpenDevice;
LPVCI_CloseDevice VCI_CloseDevice;
LPVCI_InitCan VCI_InitCAN;
LPVCI_ReadBoardInfo VCI_ReadBoardInfo;
LPVCI_ReadErrInfo VCI_ReadErrInfo;
LPVCI_ReadCanStatus VCI_ReadCanStatus;
LPVCI_GetReference VCI_GetReference;
LPVCI_SetReference VCI_SetReference;
LPVCI_GetReceiveNum VCI_GetReceiveNum;
LPVCI_ClearBuffer VCI_ClearBuffer;
LPVCI_StartCAN VCI_StartCAN;
LPVCI_ResetCAN VCI_ResetCAN;
LPVCI_Transmit VCI_Transmit;
LPVCI_Receive VCI_Receive;
LPVCI_GetReference2 VCI_GetReference2;
LPVCI_SetReference2 VCI_SetReference2;
LPVCI_ResumeConfig VCI_ResumeConfig;
LPVCI_ConnectDevice VCI_ConnectDevice;
LPVCI_UsbDeviceReset VCI_UsbDeviceReset;
LPVCI_FindUsbDevice VCI_FindUsbDevice;
#endif // CAN_H
