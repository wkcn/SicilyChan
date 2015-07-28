#ifndef CHATBOX_H
#define CHATBOX_H

#include "Defines.h"

struct ChatMsg{
    int life;
    string msg;
    ChatMsg();
    ChatMsg(string message,int msgLife);
};

namespace Ui {
class ChatBox;
}

class ChatBox : public QDialog
{
    Q_OBJECT

public:
    explicit ChatBox(QWidget *parent = 0);
    ~ChatBox();
public:
    void Say(string &text,int time);
    void Update();
private:
    void UpdateChatBox();
    void UpdateChatBoxDis();
    int GetStrWidth(const string& str);
    void paintEvent(QPaintEvent *);
private slots:
    void timerUpDate();
private:
    Ui::ChatBox *ui;
    QPixmap chatBoxPic[3];

    //聊天窗口
    int boxLife;
    string boxText;
    bool cancover;//可覆盖
    queue<ChatMsg> msgQueue;//消息队列
    const int widthPerLine = 172;
    const int font_size = 28;
    int boxH;
    int lines;
    //vector<char*> cstrList;
    vector<string> strList;
};

#endif // CHATBOX_H
