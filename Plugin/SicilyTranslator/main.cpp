#include <QApplication>
#include <QtNetwork>
#include <QClipboard>

//windows 的编码问题
#if defined(WIN32)
#pragma execution_character_set("utf-8")
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "Trans.h"

const QHostAddress addr = QHostAddress("127.0.0.1");
const int port = 3939;

inline void Send(string &mes,QUdpSocket *sender){
    sender->writeDatagram(mes.c_str(),mes.size(),addr,port);
}

int main(int argc, char *argv[]){
    QApplication a(argc, argv);//使用这个才能使用剪贴板！

    //那个TCP有缓存= =
    QUdpSocket *sender;
    sender = new QUdpSocket();

    Trans trans;
    /* sleep直接调用内核的指令,所在线程挂起,CPU搜索执行队列的重排序Timer每次会在线程池中取新的线程来执行,存在多次访问线程池的损耗 */

    while(true){

        Sleep(1000);//ms

        //Sicily教你翻译！

        QClipboard *clipboard = QApplication::clipboard();
        string cstr = clipboard->text().toStdString();

        if (cstr.size() > 0){
            trans.Set(cstr);
        }

        if(trans.finished){
            string mes;
            string res = trans.Get();
            string transStr = trans.GerOrigin();
            if(res != transStr){
                if(res.size()>0){
                    mes = transStr;
                    mes += "\n   意思是： ≥▽≤\n";
                    mes += res;
                }else{
                    mes = "不知道啊 (。_。)";
                }
            }else{
                mes = "不知道 ≥﹏≤";
            }
            Send(mes,sender);
        }
    }

    return a.exec();
}
