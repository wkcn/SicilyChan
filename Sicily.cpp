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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    timer->start(25);//FPS 40

    ReadModules();

    SwitchHint(true);
    this->update();

}

Sicily::~Sicily(){
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
    //提前计算
    for (int i = 0;i < 360 * 6;++i){
        wingCos[i] = cos(i / 6.0) * 12 + 8;
    }

    sleeped = false;
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
    static int osid = 0;
    frame++;
    int sid = 0;
    const int eyeBasicTime = 40 * (60.0 / 16);
    static int eyeTime = eyeBasicTime;

    if(sleeped)sid = 2;
    else{
        if(frame > eyeTime + 6){
            frame = 0;
            eyeTime = eyeBasicTime + rand()%60;
        }else{
            if(frame > eyeTime + 3)sid=2;
            else if(frame > eyeTime)sid=1;
        }
    }

    if (osid != sid){
        ui->eye->setPixmap(sface[sid]);
        osid = sid;
    }

    wingID += 1;
    if(wingID >= 360 * 6)
        wingID = 0;

    this->update();
}

void Sicily::UpdateButton(){
    static int lastSleepTime = 0,lastWakeTime = 0;
    if(!sleeped && actionList.match("122")){
		//左右右
		sleeped = true;
		//cancover = true;
		sleepTime = time(0);
		//发现未满15分钟的处理方法,两分钟之内才可挽救
		if(sleepTime - lastWakeTime <= 2 * 60){
			sleepTime = lastSleepTime;
		}
		actionList.clear();
	}

    //top or bottom
    if (actionList.match("22")){
        static bool topHint = true;
        topHint = !topHint;
        SwitchHint(topHint);
		actionList.clear();
    }

    if(sleeped && actionList.size() > 0){
        //醒来了
        //警示:startTime = lastWakeTime - playTime;//伪造时间[=]很好看嘛 (一个错误的做法)
        //违背真理只能适得其反
        lastWakeTime = time(0);
        lastSleepTime = sleepTime;
        lastTime = sleepTime;
        int es = (15 * 60 - (lastWakeTime - lastTime))/60;
        if (es < 0){
            SicilySay("什么都没有嘛( ◕ω ◕ )",3);
        }else{
            char temp[128];
            sprintf(temp,"还有%d分钟哦 ● ﹏ ●",es);
            SicilySay(temp,3);
        }
        sleeped = false;
		actionList.clear();
    }

    //触摸板三次切换延时在300以内，鼠标200
    //notice: clock() is different between Ubuntu and Windows!
    //clock()/ CLOCKS_PER_SEC
    actionList.update(GetClock());

    int escapeTime = time(0) - lastTime;

    const int step = 30;
    if(escapeTime >= step){
        /* 注明各个变量的意思
         * LastTime指上次更新的时间
         * escapeTime指与上次更新的间隔时间
         * 这意味着睡眠状态是LastTime会很大
         */
        lastTime = time(0);
        if(!sleeped)
            playTime += step;//1

        static int nextSaveTime = 10 * 60;
        if(escapeTime > 15 * 60){
            //超过15分钟没有刷新，说明关机了！
            playTime = 0;
            nextSaveTime = 10 * 60;
            SaveData();
            this->update();
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

        //互动

        //取消出错显示
        /*
        if(haveError){
            SicilySay(errorMsg,5);
        }
        */

    }

}

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
    this->hide();
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
        //chatbox->setWindowFlags(WINDOW_FLAG_TOPHINT);
        //chatbox->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    }else{
        this->setWindowFlags(WINDOW_FLAG_NORMAL);
        //chatbox->setWindowFlags(WINDOW_FLAG_NORMAL);
        //chatbox->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnBottomHint);
    }
    //chatbox->show();
    this->show();
}

void Sicily::mousePressEvent(QMouseEvent *event){
    dragPosition=event->globalPos() -frameGeometry().topLeft();

    ox = (this->pos()).x();
    oy = (this->pos()).y();
	int nowTime = GetClock();
    dragTime = nowTime;

    if(event->button()==Qt::LeftButton){
		actionList.push('1',nowTime);
        event->accept();
    }
    else if(event->button()==Qt::RightButton || event->button()==Qt::MiddleButton){
		actionList.push('2',nowTime);
        char str[256];
        sprintf(str,"已经玩%d分钟了哦 >.<",playTime / 60);
        SicilySay(str,1);
        event->accept();
    }
}

void Sicily::mouseReleaseEvent(QMouseEvent * /*event*/){
    /*
    if(event->button() == Qt::LeftButton){
        event->accept();
    }
    */
}

void Sicily::mouseMoveEvent(QMouseEvent *event){
    move(event->globalPos()-dragPosition);
    event->accept();
}

void Sicily::paintEvent(QPaintEvent *){
    QPainter painter;

    painter.begin(this);

    float leftAngle,rightAngle;

    //原来是ID/2
    leftAngle = wingCos[wingID];//cos(wingID / 6.0) * 12 + 8;
    rightAngle = -leftAngle;

    //left
    painter.translate(167+sicilyPosX,186+sicilyPosY);
    painter.rotate(leftAngle);
    painter.translate(-167-sicilyPosX,-186-sicilyPosY);

    painter.drawPixmap(-3+sicilyPosX,sicilyPosY,wings[0]);

    painter.resetMatrix();

    painter.translate(113+sicilyPosX,183+sicilyPosY);
    painter.rotate(rightAngle);
    painter.translate(-113-sicilyPosX,-183-sicilyPosY);
    painter.drawPixmap(3+sicilyPosX,sicilyPosY,wings[1]);

    painter.end();
}
