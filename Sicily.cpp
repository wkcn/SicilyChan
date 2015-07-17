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

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    timer->start(25);//FPS 40

    ReadModules();

    //SwitchHint(true);
    this->update();

}

void Sicily::ReadResource(){

    QPixmap pix = QPixmap(GetQDir("Pic\\sicily.png"));

    chatBoxPic[0] = QPixmap(GetQDir("Pic\\chat0.png")); //top
    chatBoxPic[1] = QPixmap(GetQDir("Pic\\chat1.png"));
    chatBoxPic[2] = QPixmap(GetQDir("Pic\\chat2.png"));

    sbody = pix.copy(0,0,300,450);
    //this->setMask(sbody.mask());

    for (int i = 0; i < 3; i++)
        sface[i] = pix.copy(300 * (i + 1), 0, 300, 450);

    wings[0] = QPixmap(GetQDir("Pic\\leftWing.png"));
    wings[1] = QPixmap(GetQDir("Pic\\rightWing.png"));

    ui->sicily->setPixmap(sbody);
    ui->eye->setPixmap(sface[0]);
}
/*
void Sicily::Receive(const char *cstr,int len){
    len;
    SicilySay(cstr,5);
}*/
void Sicily::readPendingDatagrams(){
    while (receiver->hasPendingDatagrams()){
        QByteArray datagram;
        datagram.resize(receiver->pendingDatagramSize());
        receiver->readDatagram(datagram.data(),datagram.size());
        SicilySay(datagram.data(),5);
    }
}

void Sicily::ReadModules(){
    haveError = false;
    //server = new TCPServer(this,SICILY_PORT);
    //connect(this->server,SIGNAL(Read(const char*,int)),this,SLOT(Receive(const char*,int)));
    receiver = new QUdpSocket(this);
    receiver->bind(QHostAddress::LocalHost,SICILY_PORT);
    connect(receiver,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));
}

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
    //this->resize(ow,h);
    ui->sicily->move(0, sicilyPosY);
    ui->eye->move(0, sicilyPosY);
    this->move(ox,oy+oh-h);
    this->setHidden(false);
    oh = h;
}

void Sicily::InitData(){
    QDesktopWidget *desktopWidgt = QApplication::desktop();
    QRect deskRect = desktopWidgt->availableGeometry();
    deskWidth = deskRect.width();
    deskHeight = deskRect.height();

    //这样分配窗口吧，先分配Sicily酱的位置 450*300
    //剩余的是chatBox大小
    this->move(deskWidth - 300,deskHeight-450);
    //FixPos(300,450);

    //sicilyPosY = height - 450 + 6;
    sicilyPosY = 0;

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
    tabDesktop = false;
    tabDesktopTime = 0;

    cancover = true;
}

int Sicily::GetStrWidth(const string& str){
    return this->fontMetrics().width(QString().fromStdString(str));
}

void Sicily::ErrorSend(string msg){
    haveError = true;
    errorMsg = msg;
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
            cancover = true;
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
    if (mouseCount == 2 && mouseList[0] && mouseList[1]){
        SicilySay("",5);
        mouseCount = 0;
    }

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

        if(boxLife <= 0){
            boxLife = 0;
            update();
        }else{
            boxLife --;
        }

        static int nextSaveTime = 10 * 60;
        if(escapeTime > 15*60){
            //超过15分钟没有刷新，说明关机了！
            playTime = 0;
            nextSaveTime = 10 * 60;
            SaveData();
            boxLife = 0;
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
        if(tabDesktop){
            tabDesktopTime ++;
            if(tabDesktopTime >= 5){
                Restart();
            }
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

void Sicily::timerUpDate(){
    UpdateAnimation();
    UpdateButton();
}

Sicily::~Sicily(){
    delete ui;
}

void Sicily::SicilySay(string text, int time){

    if (boxLife > 0 && !cancover)return;

    static string viewtext;

    if(text.length()>0){
        viewtext = text;
    }
    if (time == 0){
        cancover = true;
        boxLife = 3;
    }else{
        cancover = false;
        boxLife = time;
    }

    boxText = viewtext;
    this->update();
}

void Sicily::SaveData(){
    ofstream fout(GetDataDir("data.tmp").c_str());
    fout<<playTime<<" "<<lastTime;
    fout.close();
}

void Sicily::LoadData(){
    ifstream fin(GetDataDir("data.tmp").c_str());
    if(fin){
        fin>>playTime>>lastTime;
    }
    fin.close();
}

void Sicily::Restart(){
    SaveData();
    //hide();
    //show();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    exit('r'+'e'+'s'+'t'+'a'+'r'+'t');
}


void Sicily::SwitchHint(bool top){
    static bool last = true;
    if(last == top)return;
    last = top;
    this->hide();
    if(top){
        this->setWindowFlags(WINDOW_FLAG_TOPHINT);
    }else{
        this->setWindowFlags(WINDOW_FLAG_NORMAL);
    }
    this->show();
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

        event->accept();
    }
}

void Sicily::mouseReleaseEvent(QMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        event->accept();
    }
}

void Sicily::mouseMoveEvent(QMouseEvent *event){

    QPoint p = event->globalPos()-dragPosition;
    int x = p.x();
    int y = p.y();

    //qDebug("%d %d",x,y);
    if(y<50 - this->height()){
        if(x<300){
            Restart();
        }else{
            this->hide();
            tabDesktopTime = 0;
            tabDesktop = true;
        }
    }


    move(event->globalPos()-dragPosition);
    event->accept();
}

void Sicily::paintEvent(QPaintEvent *){

    QPainter painter;

    painter.begin(this);

    if (boxLife > 0 && boxText.size() > 0){
        const int widthPerLine = 172;

        int lines = 0;
        bool nextline = true;
        vector<string> strList;
        //static string punc = " !@#$%^&*()-_+={}[]|:;'<>?,./\"";
        size_t slen = boxText.size();

        //a chinese equal two english
        for(size_t i=0;i<slen;){
            if(boxText[i] == '\n'){
                nextline = true;
                i ++;
                continue;
            }
            if(nextline){
                nextline = false;
                lines ++;
                strList.push_back("");
            }

            bool zh = boxText[i] < 0;

            strList[lines - 1] += boxText[i++];

            if(zh){
                //UTF - 8 可变长字符
                while((boxText[i]&0xC0) == 0x80)
                    strList[lines - 1] += boxText[i++];
            }
            if(GetStrWidth(strList[lines - 1]) >= widthPerLine){
                nextline = true;
            }
        }

         const int font_size = 28;

         int boxX = 30;

         int boxH = 13 + font_size * lines - (-4)+31;//(lines - 1) * font_size + 70;//(lines - 1) * font_size;
         int boxY = 0;

         FixPos(450+boxH);

        //17,13,31
        painter.setPen(Qt::black);
        painter.drawPixmap(boxX, boxY, 219, 17, chatBoxPic[0]);
        painter.drawPixmap(boxX, 17 + font_size * lines + boxY, 219, 31, chatBoxPic[2]);
        for(int i=0;i<lines;i++){
            painter.drawPixmap(boxX, 17 + font_size * i + boxY, 219, font_size, chatBoxPic[1]);
            painter.drawText(16 + boxX, 12 + font_size * i + boxY, 200, font_size, Qt::AlignBottom, strList[i].c_str());
        }
    }else{
        FixPos(450);
    }


    //抗锯齿
    //painter.setRenderHint(QPainter::Antialiasing, true);

    //QPainter wpainter;
    //painter.begin(this);
    float leftAngle,rightAngle;

    //原来是ID/2
    leftAngle = cos(wingID) * 12 + 8;
    rightAngle = -leftAngle;

    //left
    painter.translate(167,186+sicilyPosY);
    painter.rotate(leftAngle);
    painter.translate(-167,-186-sicilyPosY);
    //painter.shear(0,leftAngle*0.1);
    painter.drawPixmap(-3,sicilyPosY,wings[0]);

    painter.resetMatrix();

    painter.translate(113,183+sicilyPosY);
    painter.rotate(rightAngle);
    painter.translate(-113,-183-sicilyPosY);
    painter.drawPixmap(3,sicilyPosY,wings[1]);
    painter.end();
}
