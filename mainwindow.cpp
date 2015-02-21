#include "mainwindow.h"
#include "ui_mainwindow.h"


//暂时只能这样咯，置底~
#define WINDOW_FLAG_TOPHINT Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::SubWindow
#define WINDOW_FLAG_NORMAL Qt::FramelessWindowHint|Qt::WindowStaysOnBottomHint|Qt::SubWindow


MainWindow::MainWindow(QWidget *parent) :QMainWindow(parent,WINDOW_FLAG_TOPHINT),ui(new Ui::MainWindow),sharedMem("ThisisSicily"){

    this->setAttribute(Qt::WA_TranslucentBackground);

    //this->setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::Tool|Qt::SubWindow);
    //this->setWindowTitle("SicilyChan");

    QDesktopWidget *desktopWidgt = QApplication::desktop();
    QRect deskRect = desktopWidgt->availableGeometry();
    width = deskRect.width();
    height = deskRect.height();

    //这样分配窗口吧，先分配Sicily酱的位置 450*300
    //剩余的是chatBox大小
    //qDebug("%d %d xx",height,width);
    //int panelY = 40;
    this->setMinimumSize(300,height);
    this->setMaximumSize(300,height);
    this->resize(300,height);
    this->move(width - 300,0);


    sicilyPosY = height - 450 + 6;
    //ui->sicily->resize(300,100);

    //this->move(width-300,height-440-350);


    ui->setupUi(this);

    QPixmap pix = QPixmap(GetFileDir("sicily.png"));

    chatBoxPic[0] = QPixmap(GetFileDir("chat0.png"));
    chatBoxPic[1] = QPixmap(GetFileDir("chat1.png"));
    chatBoxPic[2] = QPixmap(GetFileDir("chat2.png"));

    sbody = pix.copy(0,0,300,450);
    for (int i = 0; i < 3; i++)
        sface[i] = pix.copy(300 * (i + 1), 0, 300, 450);

    wings[0] = QPixmap(GetFileDir("leftWing.png"));
    wings[1] = QPixmap(GetFileDir("rightWing.png"));

    ui->sicily->move(0, sicilyPosY);
    ui->eye->move(0, sicilyPosY);

    ui->sicily->setPixmap(sbody);
    ui->eye->setPixmap(sface[0]);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    //connect(ui->sicily, SIGNAL(click()), this, SLOT(ChangeChatBox()));
    timer->start(25);//FPS 40

    if(sharedMem.create(sizeof(sicilyConnect))){
        qDebug("shared memory created");
        sharedMem.lock();
        sicilyConnect = (SicilyConnect*)(sharedMem.data());
        sharedMem.unlock();
    }
    else if (sharedMem.attach()){
        //exit(-1);
        sharedMem.lock();
        sicilyConnect = (SicilyConnect*)(sharedMem.data());
        sharedMem.unlock();
    }

    char text[] = " Hello Sicily >.<";
    memcpy(sicilyConnect->text,text,sizeof(text));
    sicilyConnect->boxLife = 5;


    //Python

    Py_Initialize();

    PyRun_SimpleString("import sys");
    PyRun_SimpleString("reload(sys)");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("sys.setdefaultencoding('utf-8')");


    haveError = false;
    pDragModule = PyImport_ImportModule("drag");
    //有点讨厌，有目录问题，放错还没提示！
    //检测0就好了
    if(pDragModule == 0){
        ErrorSend("打开Python失败啦");
    }
    else{


        Drag = PyObject_GetAttrString(pDragModule, "Drag");
        //ADrag = PyObject_GetAttrString(pTransModule, "ADrag");
        if(!Drag){
            ErrorSend("拖拽模块出问题了");
        }
    }

    lastTime = time(0);
    playTime = 0;
    LoadData();

    comboLeftButton = 0;
    comboRightButton = 0;
    comboMiddleButton = 0;

    ox = (this->pos()).x();
    oy = (this->pos()).y();
    dragTime = clock();

    wframe = 0;
    wingID = 0;

    mouseCount = 0;
    mouseTime = clock();
    sleeped = false;
    tabDesktop = false;
    tabDesktopTime = false;


    this->update();

}

void MainWindow::ErrorSend(string msg){
    haveError = true;
    errorMsg = msg;
}

void MainWindow::timerUpDate(){

    //FPS:40

    static int frame = 0;
    frame++;
    int sid = 0;
    //frame -= rand()%20/15;
    const int eyeBasicTime = 40 * (60.0 / 16);
    static int eyeTime = eyeBasicTime;

    if(frame > eyeTime)sid=1;
    if(frame > eyeTime + 3)sid=2;
    if(frame > eyeTime + 6){
        frame=0;
        eyeTime = eyeBasicTime + rand()%60;
    }
    if(sleeped)sid = 2;

    ui->eye->setPixmap(sface[sid]);

    /*
    wframe ++;
    if(wframe >= 3){
        wframe = 0;
        wingID ++;
        if(wingID >= 720)
            wingID = 0;
    }
    */
    wingID += 1.0f/6;
    if(wingID >= 360)
        wingID = 0;

    //Combo处理
    //睡眠状态切换
    //qDebug("mouse %d",mouseCount);

    //sleepTime 是模拟关机、睡眠等效果的

    static int lastSleepTime = 0,lastWakeTime = 0;
    if(!sleeped && mouseCount == 3){
        //左右右
        if(mouseList[0] && !mouseList[1] && !mouseList[2]){
            sleeped = true;
            sleepTime = time(0);
            //发现未满15分钟的处理方法,两分钟之内才可挽救
            if(sleepTime - lastWakeTime <= 2 * 60){
                sleepTime = lastSleepTime;
            }
        }
        mouseCount = 0;//清除状态,一定要用的
    }

    if(sleeped && mouseCount > 0){
        //醒来了
        //警示:startTime = lastWakeTime - playTime;//伪造时间[=]很好看嘛 (一个错误的做法)
        //违背真理只能适得其反
        lastWakeTime = time(0);
        lastSleepTime = sleepTime;
        lastTime = sleepTime;
        SicilySay("什么都没有嘛( ◕ω ◕ )",5);
        sleeped = false;
        mouseCount = 0;
    }

    //触摸板三次切换延时在300以内，鼠标200
    if(clock() - mouseTime >= 400){
        mouseCount = 0;
    }


    int escapeTime = time(0) - lastTime;
    if(escapeTime>=1){

        lastTime = time(0);
        if(!sleeped)
            playTime += escapeTime;

        sicilyConnect->boxLife --;

        if(sicilyConnect->boxLife<0){
            sicilyConnect->boxLife = 0;
            update();
        }
        static int nextSaveTime = 10 * 60;
        if(escapeTime > 15*60){
            //超过15分钟没有刷新，说明关机了！
            playTime = 0;
            nextSaveTime = 10 * 60;
            SaveData();
            sicilyConnect->boxLife = 0;
            update();
        }

        if(!sleeped){
            if(playTime > nextSaveTime){
                SaveData();
                nextSaveTime += 10 * 60;
            }
            if(playTime >= 40*60){
                SwitchHint(true);
                SicilySay("你已经玩很久了呢！\n休息一下吧",8);
            }
        }else{
            SicilySay("呼呼 ●ω●",8);
        }

        //TabDesktop
        if(tabDesktop){
            tabDesktopTime ++;
            if(tabDesktopTime >= 5){
                Restart();
            }
        }

        //互动

        if(haveError){
            SicilySay(errorMsg,5);
        }

        if(true){
            //Sicily教你翻译！
            QClipboard *clipboard = QApplication::clipboard();
            string cstr = clipboard->text().toStdString();
            if(!trans.ing)trans.Set(cstr);
        }

        comboLeftButton = 0;
        comboRightButton = 0;
        comboMiddleButton = 0;
    }

    if(trans.finished){
        string res = trans.Get();
        string transStr = trans.GerOrigin();
        if(res != transStr){
            if(res.size()>0){
                string mes = transStr;
                mes += "\n   意思是： ≥▽≤\n";
                mes += res;
                SicilySay(mes,6);
            }else{
                SicilySay("不知道啊 (。_。)",3);
            }
        }else{
            SicilySay("不知道 ≥﹏≤",3);
        }
    }

    boxLife = sicilyConnect->boxLife;

    if(sicilyConnect->update){
        this->update();
        sicilyConnect->update=false;
    }
    //qDebug("%d",boxLife);
}

MainWindow::~MainWindow()
{
    delete ui;
    sharedMem.detach();
}

void MainWindow::SicilySay(string text, int time){
    static string viewtext;
    if(text.length()>0){
        viewtext = text;
    }
    sicilyConnect->boxLife = time;
    strcpy(sicilyConnect->text,viewtext.c_str());
    sicilyConnect->update = true;
}

void MainWindow::SaveData(){
    ofstream fout("data.tmp");
    fout<<playTime<<" "<<lastTime;
    fout.close();
}

void MainWindow::LoadData(){
    ifstream fin("data.tmp");
    if(fin){
        fin>>playTime>>lastTime;
        //qDebug("LastTime:%d StartTime:%d NowTime:%d",lastTime,startTime,time(0));
    }
    fin.close();
}

void MainWindow::Restart(){
    qDebug("See you");

    SaveData();
    //hide();
    //show();
    QProcess::startDetached(qApp->applicationFilePath(), QStringList());
    exit('r'+'e'+'s'+'t'+'a'+'r'+'t');
}


void MainWindow::SwitchHint(bool top){
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

void MainWindow::mousePressEvent(QMouseEvent *event){
    dragPosition=event->globalPos() -frameGeometry().topLeft();

    ox = (this->pos()).x();
    oy = (this->pos()).y();
    dragTime = clock();

    if(event->button()==Qt::LeftButton){
        //qDebug("Left %d",clock()-mouseTime);
        comboLeftButton ++;
        mouseList[mouseCount++] = true;
        mouseTime = clock();
        if(comboLeftButton >= 2){
            SicilySay("",5);
        }
        event->accept();
    }
    else if(event->button()==Qt::RightButton){
        //qDebug("Right %d",clock()-mouseTime);
        comboRightButton ++;
        mouseList[mouseCount++] = false;
        mouseTime = clock();
        //this->update();
        //this->updateGeometry();
        //qDebug("Right%d",time(0));
        if(comboRightButton >= 2){
            static bool topHint = true;
            topHint = !topHint;
            SwitchHint(topHint);
        }
        char str[256];
        //int totalTime = (time(0)-startTime)/60;
        sprintf(str,"已经玩%d分钟了哦 >.<",playTime / 60);


        SicilySay(str,3);

        event->accept();
    }
    else if(event->button()==Qt::MiddleButton){
        comboMiddleButton ++;
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    //if(event->button()==Qt::LeftButton){
        int ax = (this->pos()).x() - ox;
        int ay = (this->pos()).y() - oy;
        string res = PyString_AS_STRING(PyObject_CallFunction(Drag,"iii",ax,ay,clock()-dragTime));
        if(res != ""){
            SicilySay(res,3);
        }
        event->accept();
    //}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){

    QPoint p = event->globalPos()-dragPosition;
    int x = p.x();
    int y = p.y();

    qDebug("%d %d",x,y);
    if(y<-600){
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

void MainWindow::paintEvent(QPaintEvent *){

    QPainter painter;

    painter.begin(this);

    if (boxLife>0&&sicilyConnect->text[0]!='\0'){
        //QPainter painter;
        int boxX = 28;
        int boxY = sicilyPosY - 70;
        const int lenPerLine = 30;//23

        char *tadr = sicilyConnect->text;//"Hello Sicily";
        int lines = 1;
        float count = 0;
        string strlist[12];

        const int  maxLine = 10;

        for(char *u = tadr;*u!='\0';){
            if(*u=='\n'){
                if(count > 0)
                    lines += 1;
                count = 0;
                u++;
                if(lines > maxLine)break;
                continue;
            }
            strlist[lines-1] += *u;
            if(*u<0){strlist[lines-1] += *(++u);count+=0.5;}
            if(*u<0){strlist[lines-1] += *(++u);count+=0.5;}
            count+=1;
            if(count >= lenPerLine){
                lines += 1;
                count = 0;
            }
            if(lines > maxLine)break;
            if(*u!='\0')u++;
        }

        //预先处理一下，暂时采取这个方案评估
        while(lines > 0 && (strlist[lines-1].length()==0 || strlist[lines-1][0] == '\n')){
            lines --;
        }

        //int lines = int((boxTest.length()+lenPerLine-1)/lenPerLine);
         const int font_size = 28;
        boxY -= (lines - 1) * font_size;

        //17,13,31

        //painter.begin(this);
        painter.setPen(Qt::black);
        painter.drawPixmap(boxX, -4 + boxY, 219, 17, chatBoxPic[0]);
        painter.drawPixmap(boxX, 13 + font_size * lines + boxY, 219, 31, chatBoxPic[2]);
        string boxTest;
        for(int i=0;i<lines;i++){
            painter.drawPixmap(boxX, 13 + font_size * i + boxY, 219, font_size, chatBoxPic[1]);
            int tbegin = lenPerLine * i;
            int slen = int(boxTest.length()) - tbegin;
            if(slen > lenPerLine)slen = lenPerLine;
            //QString qstr = "中文难道不行啊没";
            //
            painter.drawText(15 + boxX, 8 + font_size * i + boxY, 200, font_size, Qt::AlignBottom, strlist[i].c_str());
        }

        //painter.end();
    }


    //抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

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


