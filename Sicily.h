#ifndef SICILY_H
#define SICILY_H

#include "Defines.h"
//#include "trans.h"
class Trans : public QThread
{
    Q_OBJECT

public:
    Trans();
    ~Trans();
private:
    string res;
    string ori;
    string cstr;

    PyObject *pTransModule;
    PyObject *Translate;
    //之前用py出现乱码的原因，没有将字符串转str!还要encodeutf8!

    void PyTrans(string text,bool eng);
protected:
    void run();

public:

    void Set(const string &text);
    string Get();
    string GerOrigin();
    volatile bool ing;
    volatile bool finished;
};


namespace Ui {
class Sicily;

}

class Sicily : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sicily(QWidget *parent = 0);
    ~Sicily();

private slots:
    void timerUpDate();

private:
    Ui::Sicily *ui;
    QPixmap sbody;
    QPixmap sface[3];
    QPixmap wings[2];
    QPixmap chatBoxPic[3];

    int deskWidth,deskHeight;

    int boxLife;
    string sText;

    int sicilyPosY;

    int lastTime;
    int playTime;

    int freezeTime;

    int comboRightButton;
    int comboLeftButton;
    int comboMiddleButton;

    float wingID;

    //QSharedMemory sharedMem;
    //SicilyConnect *sicilyConnect;


    QPoint dragPosition;
    int ox,oy;
    int dragTime;

    //true表示左键,false表示右键
    bool mouseList[64];
    int mouseCount;
    int mouseTime;
    bool sleeped;
    int sleepTime;
    bool tabDesktop;
    int tabDesktopTime;

    Trans trans;


    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void Restart();
    void SaveData();
    void LoadData();
    void SwitchHint(bool top);
    void ErrorSend(string msg);

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
    PyObject *pDragModule;
    PyObject *Drag;

    bool haveError;
    string errorMsg;
private:
    void SicilySay(string text,int time,int freeze = 0);
};
#endif // MAINWINDOW_H
//#include "trans.moc"
