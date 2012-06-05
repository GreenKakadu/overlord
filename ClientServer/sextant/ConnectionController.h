/****************************************************************************
                         ConnectionController.h

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#ifndef CONNECTIONCONTROLLER_H
#define CONNECTIONCONTROLLER_H
#include <QObject>
#include <QTcpSocket>
#include <QColor>
#include "../PbmCommon/PbmMessage.h"
enum ConnectionState {
    CONNECTED = 1,
    CONNECTING = 2,
    AUTHORIZING = 3,
    AUTHORIZATION_FAILED = 4,
    CONNECTION_FAILED = 5,
    UNKNOWN = 99
};
class ConnectionController: public  QObject
{
    Q_OBJECT
public:
    ConnectionController();
    ~ConnectionController();

    ConnectionState getState(){return state;}
    int init(QString gameName, QString userId, QString password, int turn);
    int start(QString ipAdr, int port);
    int close();
    void sendAuthMessage();
    bool sendDataMessage(QString fileName);
    bool sendControlMessage(MsgType type, int turn);
    void processMessage(PbmMessage * msg) ;
    void checkNewTurn(int turn);

private slots:
    void slotConnected();
    void slotReadyRead();
    void slotError (QAbstractSocket::SocketError);
signals:
    void stateChaged(ConnectionState state);
    void writeLogMessage(QString strMessage);
    void printMessage(QString strMessage);
    void printStatus(QString strMessage);
    void sendLedSignal(QColor,bool);
    void receiveLedSignal(QColor,bool);
    void setReceiveButtonSignal(bool);
    void gotNewReport(int turn);
    void setTurnNumSignal(int turn);

private:
    void setState(ConnectionState state);
    ConnectionState state;
    QTcpSocket *tcpSocket;
    quint16 nextBlockSize;
    PbmMessage * msg;
    QString ipAdr_;
    int port_;
    QString gameName_;
    QString userId_;
    QString password_;
    int currentTurn;
};

#endif // CONNECTIONCONTROLLER_H
