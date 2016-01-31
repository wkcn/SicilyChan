#ifndef SICILY_H
#define SICILY_H

#include "Defines.h"
#include "ChatBox.h"
#include "ActionList.h"

namespace Ui {
class Sicily;

}

#define SICILY_PORT 3939

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
    float wingCos[360 * 6];

    ChatBox *chatbox;

    int deskWidth,deskHeight;
    const int sicilyPosX = -66;
    const int sicilyPosY = -8;

    int lastTime;
    int playTime;

    int wingID;

    //TCPServer *server;
    QUdpSocket *receiver;

    QPoint dragPosition;
    int ox,oy;
    int dragTime;

	ActionList actionList;

    bool sleeped;
    int sleepTime;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    void Restart();
    void SaveData();
    void LoadData();
    void SwitchHint(bool top);

    //封装一些功能
    void ReadResource();
    void ReadModules();
    void InitData();
    void FixPos(int h);

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
