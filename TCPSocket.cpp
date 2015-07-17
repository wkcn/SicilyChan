#include "TCPSocket.h"

TCPSocket::TCPSocket(QObject *parent):QTcpSocket(parent){
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(disconnected(int)),this,SLOT(slotDisconnected()));
}

void TCPSocket::dataReceived(){
    while(this->bytesAvailable() > 0){
        QString msg = this->readAll();
        emit updateClients(msg.toStdString().c_str(),msg.length());
    }
}

void TCPSocket::slotDisconnected(){
    emit disconnected(this->socketDescriptor());
}
