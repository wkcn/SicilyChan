#ifndef SICILY_H
#define SICILY_H

#include "Defines.h"

namespace Ui {
class Sicily;

}

#define SICILY_PORT 3939

struct ChatMsg{
    int life;
    string msg;
    ChatMsg();
    ChatMsg(string message,int msgLife);
};

class Sicily : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sicily(QWidget *parent = 0);
    ~Sicily();

private slots:
    void timerUpDate();
    //void Receive(const char *cstr,int len);
    void readPendingDatagrams();

private:
    Ui::Sicily *ui;
    QPixmap sbody;
    QPixmap sface[3];
    QPixmap wings[2];
    QPixmap chatBoxPic[3];

    int deskWidth,deskHeight;

    //聊天窗口
    int boxLife;
    string boxText;
    bool cancover;//可覆盖
    queue<ChatMsg> msgQueue;//消息队列
    const int widthPerLine = 172;
    const int font_size = 28;
    const int boxX = 30;
    const int boxY = 0;
    int boxH;
    int lines;
    //vector<char*> cstrList;
    vector<string> strList;

    int sicilyPosY;

    int lastTime;
    int playTime;

    float wingID;

    //TCPServer *server;
    QUdpSocket *receiver;

    QPoint dragPosition;
    int ox,oy;
    int dragTime;

    //true表示左键,false表示右键
    bool mouseList[64];
    int mouseCount;
    int mouseTime;
    bool sleeped;
    int sleepTime;
    //bool tabDesktop;
    //int tabDesktopTime;

    void UpdateChatBox();
    void UpdateChatBoxDis();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    bool mouseSeq(int seq);//鼠标序列

    void Restart();
    void SaveData();
    void LoadData();
    void SwitchHint(bool top);

    //封装一些功能
    void ReadResource();
    void ReadModules();
    void InitData();
    void FixPos(int h);
    int GetStrWidth(const string& str);
    //Update
    //更新动画
    void UpdateAnimation();
    void UpdateButton();
private:
    bool haveError;
    string errorMsg;
private:
    void SicilySay(string text,int time = 0);//time = 0的为优先级最低的语句，可以覆盖
};
#endif // MAINWINDOW_H
