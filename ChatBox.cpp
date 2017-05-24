#include "ChatBox.h"
#include "ui_ChatBox.h"

ChatMsg::ChatMsg(){}
ChatMsg::ChatMsg(string message, int msgLife):life(msgLife),msg(message){
}

ChatBox::ChatBox(QWidget *parent) :QDialog(parent,Qt::FramelessWindowHint),ui(new Ui::ChatBox){
    ui->setupUi(this);

    this->setAttribute(Qt::WA_TranslucentBackground);
    //Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint|Qt::SubWindow

    //资源读取
    chatBoxPic[0] = QPixmap(GetFileDir("Pic\\chat0.png")); //top
    chatBoxPic[1] = QPixmap(GetFileDir("Pic\\chat1.png"));
    chatBoxPic[2] = QPixmap(GetFileDir("Pic\\chat2.png"));

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpDate()));
    timer->start(1000);//1s

    boxLife = 0;
    lines = 0;
    cancover = true;

    this->hide();

    this->setMinimumWidth(219);
    this->setMaximumWidth(219);
}

int ChatBox::GetStrWidth(const string& str){
    return this->fontMetrics().width(QString().fromStdString(str));
}

void ChatBox::Say(string &text, int time){
    //简单显示
    if (time == 0){
        if (boxLife > 0 && !cancover)return;
        boxText = text;
        boxLife = 3;
        cancover = true;
        UpdateChatBoxDis();
        this->update();
    }else{
        //复杂显示,加入消息队列
        const int maxLen = 5;
        if (!msgQueue.empty() && msgQueue.back().msg == text){
            return; // 去重
        }
        while(msgQueue.size() >= maxLen){
            msgQueue.pop();
        }
        msgQueue.push(ChatMsg(text,time));
    }
}

void ChatBox::UpdateChatBoxDis(){

    lines = 0;
    bool nextline = true;

    //vector<string> strList;

    strList.clear();

    /*
    for(size_t i = 0;i < cstrList.size();++i){
        delete [] cstrList[i];
    }
    */

    //static string punc = " !@#$%^&*()-_+={}[]|:;'<>?,./\"";
    size_t slen = boxText.size();

    //a chinese equal two english
    for(size_t i = 0;i < slen;){
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

    //cstrList.clear();
    /*
    for(size_t i = 0;i < strList.size();++i){
        const char *cs = strList[i].c_str();
        size_t cl = strList[i].size();//注意,strlen("ab") = 2,还要加上\0
        char *newstr = new char[cl + 1];
        memcpy(newstr,cs,cl);
        newstr[cl] = '\0';
        cstrList.push_back(newstr);
    }
    */

    boxH = 13 + font_size * lines - (-4)+31;//(lines - 1) * font_size + 70;//(lines - 1) * font_size;
    this->setMinimumHeight(boxH);
    this->setMaximumHeight(boxH);
}

void ChatBox::timerUpDate(){
    if(boxLife <= 0){
        boxLife = 0;
        this->hide();
        //update();
    }else{
        boxLife --;
    }
    UpdateChatBox();
}
void ChatBox::UpdateChatBox(){
    if (cancover || boxLife <= 0){
        if (!msgQueue.empty()){
            const ChatMsg &cm = msgQueue.front();
            boxText = cm.msg;
            boxLife = max(cm.life,10);
            cancover = false;
            msgQueue.pop();
            UpdateChatBoxDis();
            Update();
        }
    }
}

void ChatBox::Update(){
    if(boxLife > 0){
        this->show();
        this->update();//show后才会执行paintEvent()
    }
}

void ChatBox::paintEvent(QPaintEvent *){
    if (boxLife > 0){
        QRect pg = this->parentWidget()->geometry();
        this->move(pg.x()-20,pg.y() - boxH);

        QPainter painter;
        painter.begin(this);

        //这里要进行优化，只在设定信息时更新行距
        //17,13,31
        painter.setPen(Qt::black);
        painter.drawPixmap(0,0, 219, 17, chatBoxPic[0]);
        painter.drawPixmap(0, 17 + font_size * lines , 219, 31, chatBoxPic[2]);
        for(int i=0;i < lines;i++){
            painter.drawPixmap(0, 17 + font_size * i , 219, font_size, chatBoxPic[1]);
            painter.drawText(16, 12 + font_size * i, 200, font_size, Qt::AlignBottom, strList[i].c_str());
        }
        painter.end();
    }
}

ChatBox::~ChatBox(){
    delete ui;
}
