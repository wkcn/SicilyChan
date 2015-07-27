#include "Sicily.h"
#include "ui_Sicily.h"


//暂时只能这样咯，置底~
#define WINDOW_FLAG_TOPHINT Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::SubWindow
#define WINDOW_FLAG_NORMAL Qt::FramelessWindowHint|Qt::WindowStaysOnBottomHint|Qt::SubWindow

Sicily::Sicily(QWidget *parent):QMainWindow(parent,WINDOW_FLAG_TOPHINT),ui(new Ui::Sicily){//,sharedMem("ThisisSicily"){

    this->setAttribute(Qt::WA_TranslucentBackground);

    InitData();

    ui->setupUi(this);

    ReadResource();

    chatbox = new ChatBox(this);
    chatbox->show();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    timer->start(25);//FPS 40

    ReadModules();

    //SwitchHint(true);
    this->update();

}

Sicily::~Sicily(){
    /*
    for(size_t i = 0;i < cstrList.size();++i){
        delete [] cstrList[i];
    }
    */
    //chatbox和timer在关闭父窗体时会被析构
    delete ui;
}

//初始化
void Sicily::ReadResource(){

    QPixmap pix = QPixmap(GetFileDir("Pic\\sicily.png"));

    sbody = pix.copy(0,0,300,450);

    for (int i = 0; i < 3; i++)
        sface[i] = pix.copy(300 * (i + 1), 0, 300, 450);

    wings[0] = QPixmap(GetFileDir("Pic\\leftWing.png"));
    wings[1] = QPixmap(GetFileDir("Pic\\rightWing.png"));

    ui->sicily->setPixmap(sbody);
    ui->sicily->move(sicilyPosX,sicilyPosY);
    ui->eye->setPixmap(sface[0]);
    ui->eye->move(sicilyPosX,sicilyPosY);

    this->setMinimumSize(217,435);
    this->setMaximumSize(217,435);
}

void Sicily::ReadModules(){
    haveError = false;
    receiver = new QUdpSocket(this);
    receiver->bind(QHostAddress::LocalHost,SICILY_PORT);
    connect(receiver,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));
}

void Sicily::InitData(){
    QDesktopWidget *desktopWidgt = QApplication::desktop();
    QRect deskRect = desktopWidgt->availableGeometry();
    deskWidth = deskRect.width();
    deskHeight = deskRect.height();

    //这样分配窗口吧，先分配Sicily酱的位置 450*300
    this->move(deskWidth - 300 - sicilyPosX,deskHeight - 450 - sicilyPosY);

    lastTime = time(0);
    playTime = 0;
    LoadData();

    ox = (this->pos()).x();
    oy = (this->pos()).y();
    dragTime = GetClock();

    wingID = 0;

    mouseCount = 0;
    mouseTime = GetClock();
    sleeped = false;
    //tabDesktop = false;
    //tabDesktopTime = 0;
}

void Sicily::readPendingDatagrams(){
    //格式：文本 + Life(1 byte) + '\0'
    //好像不会接收到\0
    while (receiver->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(datagram.data(),datagram.size());

        int life = int(datagram.data()[datagram.size() - 1]);

        datagram[datagram.size() - 1] = '\0';
        SicilySay(datagram.data(),life);
    }
}

void Sicily::UpdateAnimation(){
    //FPS:40
    static int frame = 0;
    frame++;
    int sid = 0;
    const int eyeBasicTime = 40 * (60.0 / 16);
    static int eyeTime = eyeBasicTime;

    if(frame > eyeTime)sid=1;
    if(frame > eyeTime + 3)sid=2;
    if(frame > eyeTime + 6){
        frame = 0;
        eyeTime = eyeBasicTime + rand()%60;
    }
    if(sleeped)sid = 2;

    ui->eye->setPixmap(sface[sid]);

    wingID += 1.0f/6;
    if(wingID >= 360)
        wingID = 0;
}

void Sicily::UpdateButton(){
    static int lastSleepTime = 0,lastWakeTime = 0;
    if(!sleeped && mouseCount == 3){
        //左右右
        if (mouseList[0] && !mouseList[1] && !mouseList[2]){
            sleeped = true;
            //cancover = true;
            sleepTime = time(0);
            //发现未满15分钟的处理方法,两分钟之内才可挽救
            if(sleepTime - lastWakeTime <= 2 * 60){
                sleepTime = lastSleepTime;
            }
        }
        mouseCount = 0;//清除状态,一定要用的
    }

    //top or bottom
    if (mouseCount == 2 && !mouseList[0] && !mouseList[1]){
        static bool topHint = true;
        topHint = !topHint;
        SwitchHint(topHint);
        mouseCount = 0;
    }

    //REPEAT SAY
    /*
    if (mouseCount == 2 && mouseList[0] && mouseList[1]){
        SicilySay("",5);
        mouseCount = 0;
    }
    */


    if(sleeped && mouseCount > 0){
        //醒来了
        //警示:startTime = lastWakeTime - playTime;//伪造时间[=]很好看嘛 (一个错误的做法)
        //违背真理只能适得其反
        lastWakeTime = time(0);
        lastSleepTime = sleepTime;
        lastTime = sleepTime;
        int es = (15 * 60 - (lastWakeTime - lastTime))/60;
        if (es<0){
            SicilySay("什么都没有嘛( ◕ω ◕ )",3);
        }else{
            char temp[128];
            sprintf(temp,"还有%d分钟哦 ● ﹏ ●",es);
            SicilySay(temp,3);
        }
        sleeped = false;
        mouseCount = 0;
    }

    //触摸板三次切换延时在300以内，鼠标200
    //notice: clock() is different between Ubuntu and Windows!
    //clock()/ CLOCKS_PER_SEC

    //qDebug("hha%d",GetClock() - mouseTime);
    if(GetClock() - mouseTime >= 400){
        mouseCount = 0;
    }

    int escapeTime = time(0) - lastTime;
    if(escapeTime >= 1){
        /* 注明各个变量的意思
         * LastTime指上次更新的时间
         * escapeTime指与上次更新的间隔时间
         */
        lastTime = time(0);
        if(!sleeped)
            playTime += 1;//escapeTime;

        static int nextSaveTime = 10 * 60;
        if(escapeTime > 15*60){
            //超过15分钟没有刷新，说明关机了！
            playTime = 0;
            nextSaveTime = 10 * 60;
            SaveData();
            //boxLife = 0;
            update();
        }

        if(!sleeped){
            if(playTime > nextSaveTime){
                SaveData();
                nextSaveTime += 10 * 60;
            }
            if(playTime >= 40*60){
                SwitchHint(true);
                SicilySay("你已经玩很久了呢！\n休息一下吧");
            }
        }else{
            SicilySay("呼呼 ●ω●");
        }

        //TabDesktop
        /*
        if(tabDesktop){
            tabDesktopTime ++;
            if(tabDesktopTime >= 5){
                Restart();
            }
        }
        */

        //互动

        //取消出错显示
        /*
        if(haveError){
            SicilySay(errorMsg,5);
        }
        */

    }

}
/*
void Sicily::FixPos(int h){
    static int ow = 300;
    static int oh = 450;
    if (h == oh)return;
    int ox = (this->pos()).x();
    int oy = (this->pos()).y();
    //sicilyPosY指现在sicily在窗口的位置
    sicilyPosY = h - 450;
    //this->setMaximumSize(ow,h);
    this->setHidden(true);

    this->setMinimumSize(ow,h);
    this->setMaximumSize(ow,h);
    //this->resize(ow,h);
    ui->sicily->move(0, sicilyPosY);
    ui->eye->move(0, sicilyPosY);
    this->move(ox,oy+oh-h);

    this->setHidden(false);
    oh = h;
}*/
void Sicily::timerUpDate(){
    //UpdateChatBox();
    UpdateAnimation();
    UpdateButton();
    chatbox->Update();
}


void Sicily::SicilySay(string text, int time){
    chatbox->Say(text,time);
}

void Sicily::SaveData(){
    ofstream fout(GetStdFileDir("Data\\data.tmp").c_str());
    fout<<playTime<<" "<<lastTime;
    fout.close();
}

void Sicily::LoadData(){
    ifstream fin(GetStdFileDir("Data\\data.tmp").c_str());
    if(fin){
        fin>>playTime>>lastTime;
    }
    fin.close();
}

void Sicily::Restart(){
    SaveData();
    //();
    //show();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    exit('r'+'e'+'s'+'t'+'a'+'r'+'t');
}


void Sicily::SwitchHint(bool top){
    static bool last = true;
    if(last == top)return;
    last = top;
    this->hide();
    //chatbox->hide();
    if(top){
        this->setWindowFlags(WINDOW_FLAG_TOPHINT);
        //chatbox->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    }else{
        this->setWindowFlags(WINDOW_FLAG_NORMAL);
        //chatbox->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnBottomHint);
    }
    //chatbox->show();
    this->show();
}

bool Sicily::mouseSeq(int seq){
    int w = 1;
    int js = 10;
    while(seq > js){
        w ++;
        js *= 10;
    }
    //if seq = 112
    // here,w=3,js=1000
    //qDebug("%d %d %d",seq,w,mouseCount);
    if(mouseCount == w){
        //qDebug("()%d %d",w,seq);
        for(int i=0;i<w;i++){
            js /= 10;
            int m = seq / js;
            if(m == 1 && !mouseList[i])return false;
            if(m == 2 && mouseList[i])return false;
        }
        mouseCount = 0;
        return true;
    }
    return false;
}

void Sicily::mousePressEvent(QMouseEvent *event){
    dragPosition=event->globalPos() -frameGeometry().topLeft();

    ox = (this->pos()).x();
    oy = (this->pos()).y();
    dragTime = GetClock();

    if(event->button()==Qt::LeftButton){
        mouseList[mouseCount++] = true;
        mouseTime = GetClock();
        event->accept();
    }
    else if(event->button()==Qt::RightButton){
        mouseList[mouseCount++] = false;
        mouseTime = GetClock();
        char str[256];
        //int totalTime = (time(0)-startTime)/60;
        sprintf(str,"已经玩%d分钟了哦 >.<",playTime / 60);
        SicilySay(str,3);
        event->accept();
    }
    else if(event->button() == Qt::MiddleButton){

        //event->accept();
    }
}

void Sicily::mouseReleaseEvent(QMouseEvent */*event*/){
    /*
    if(event->button() == Qt::LeftButton){
        event->accept();
    }
    */
}

void Sicily::mouseMoveEvent(QMouseEvent *event){

    //QPoint p = event->globalPos()-dragPosition;
    //int x = p.x();
    //int y = p.y();

    //qDebug("%d %d",x,y);
    /*
    if(y<50 - this->height()){
        if(x<300){
            Restart();
        }else{
            this->hide();
            tabDesktopTime = 0;
            tabDesktop = true;
        }
    }
  */

    move(event->globalPos()-dragPosition);
    event->accept();
}

void Sicily::paintEvent(QPaintEvent *){
    QPainter painter;

    painter.begin(this);

    //抗锯齿
    //painter.setRenderHint(QPainter::Antialiasing, true);

    //QPainter wpainter;
    //painter.begin(this);
    float leftAngle,rightAngle;

    //原来是ID/2
    leftAngle = cos(wingID) * 12 + 8;
    rightAngle = -leftAngle;

    //left
    painter.translate(167+sicilyPosX,186+sicilyPosY);
    painter.rotate(leftAngle);
    painter.translate(-167-sicilyPosX,-186-sicilyPosY);
    //painter.shear(0,leftAngle*0.1);
    painter.drawPixmap(-3+sicilyPosX,sicilyPosY,wings[0]);

    painter.resetMatrix();

    painter.translate(113+sicilyPosX,183+sicilyPosY);
    painter.rotate(rightAngle);
    painter.translate(-113-sicilyPosX,-183-sicilyPosY);
    painter.drawPixmap(3+sicilyPosX,sicilyPosY,wings[1]);

    painter.end();
}
