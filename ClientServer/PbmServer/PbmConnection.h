/****************************************************************************
                         PbmConnection.h

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#ifndef PBMCONNECTION_H
#define PBMCONNECTION_H
#include <QWidget>
class QTcpSocket;
class ConnectionAgent;
struct ServerConfiguration;
class PbmMessage;
class QTimer;
class PbmConnection: public QWidget
{
    Q_OBJECT

public:
    PbmConnection(QTcpSocket* pClientSocket, ServerConfiguration * conf);
    ~PbmConnection();
public slots:
    void slotReadClient ();
    void deleteConection();
signals:
    void signalPrintMessage(QString strMessage);
    void signalLogMessage(QString strMessage);
public:
    static int connectionCounter;
    void processMessage(PbmMessage * msg);
    void authenticationFailed();

protected:
    QTcpSocket* pClientSocket_;
    quint16 nextBlockSize;
    QTimer * timeoutTimer;
    ServerConfiguration * conf_;
    int num;
    bool isAuthorized;
    ConnectionAgent * agent;
    PbmMessage * msg;
};

#endif // PBMCONNECTION_H
