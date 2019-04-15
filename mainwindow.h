#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>


//定义CAN信息帧的数据类型。
typedef  struct  _VCI_CAN_OBJj{
    unsigned int	ID;
    unsigned int	TimeStamp;	 		 //时间标识
    unsigned char	TimeFlag;			 //是否使用时间标识
    unsigned char	SendType;			 //发送标志。保留，未用
    unsigned char	RemoteFlag;			 //是否是远程帧
    unsigned char	ExternFlag;			 //是否是扩展帧
    unsigned char	DataLen;
    unsigned char	Data[8];
    unsigned char	Reserved[3];			//保留
}VCI_CAN_OBJj,*PVCI_CAN_OBJj;



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTimer *m_CanReadTimer;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void CanSendData();
    void DisplayString(QString cStr);
private:
    void DisplayData(VCI_CAN_OBJj *dispdata);
private slots:
    void on_CanConnect_clicked();
    void on_CanReadTimerOut();
    void on_CanStart_clicked();
    void on_SendOne_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
