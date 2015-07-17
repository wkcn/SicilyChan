#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include <QObject>

class TCPSocket : public QTcpSocket{
    Q_OBJECT
public:
    TCPSocket(QObject *parent = 0);
signals:
    void updateClients(const char *cstr,int len);
    void disconnected(int descriptor);
public slots:
    void dataReceived();
    void slotDisconnected();
};

#endif // TCPSOCKET_H
