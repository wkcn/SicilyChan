#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QObject>
#include "TCPSocket.h"

#include <vector>
using namespace std;

//用于与后端通信
class TCPServer : public QTcpServer{
    Q_OBJECT
public:
    TCPServer(QObject *parent,int port);
    void Write(char *cstr,int len);
signals:
    void Read(const char *cstr,int len);
private:
    list<TCPSocket *> tcpList;
public slots:
    void slotReceive(const char *cstr,int len);
    void slotDisconnected(int descriptor);
    void acceptConnection();
};

#endif // TCPSERVER_H
