#include "TCPServer.h"

TCPServer::TCPServer(QObject *parent, int port):QTcpServer(parent){
    this -> listen(QHostAddress::Any,port);
    //当有客户端连接时，调用acceptConnection()
    connect(this,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}

void TCPServer::acceptConnection(){
    qDebug("newac");
    TCPSocket *socket = new TCPSocket(this);
    connect(socket,SIGNAL(updateClients(const char*,int)),this,SLOT(slotReceive(const char*,int)));
    connect(socket,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected(int)));//断开连接
    socket->setSocketDescriptor(this->nextPendingConnection()->socketDescriptor());
    tcpList.push_back(socket);
}

void TCPServer::slotReceive(const char *cstr, int len){
    emit Read(cstr,len);
}

void TCPServer::Write(char *cstr, int len){
    //向客户端发送信息
    for(list<TCPSocket*>::iterator iter = tcpList.begin();iter != tcpList.end();++iter){
        dynamic_cast<QTcpSocket*>(*iter)->write(cstr,len);
    }
}

void TCPServer::slotDisconnected(int descriptor){
    for(list<TCPSocket*>::iterator iter = tcpList.begin();iter != tcpList.end();++iter){
        if (dynamic_cast<QTcpSocket*>(*iter)->socketDescriptor() == descriptor){
            tcpList.erase(iter);
            return;
        }
    }
}
