#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    void PyTrans(string text,string to_t,string from_t);
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
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void timerUpDate();

private:
    Ui::MainWindow *ui;
    QPixmap sbody;
    QPixmap sface[3];
    QPixmap wings[2];
    QPixmap chatBoxPic[3];

    int width,height;

    int boxLife;
    int sicilyPosY;

    int lastTime;
    int playTime;

    int comboRightButton;
    int comboLeftButton;
    int comboMiddleButton;

    int wframe;
    float wingID;

    QSharedMemory sharedMem;
    SicilyConnect *sicilyConnect;


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
private:
    PyObject *pDragModule;
    PyObject *Drag;

    bool haveError;
    string errorMsg;
private:
    void SicilySay(string text,int time);
};
#endif // MAINWINDOW_H
//#include "trans.moc"
