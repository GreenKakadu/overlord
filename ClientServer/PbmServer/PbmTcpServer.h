/****************************************************************************
                         PbmTcpServer.h

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#ifndef PBMTCPSERVER_H
#define PBMTCPSERVER_H
#include <QtNetwork>
#include <QTcpServer>
#include <QMap>
struct ServerConfiguration;
class PbmTcpServer : public QTcpServer
{
    Q_OBJECT

public:
    PbmTcpServer(int port, ServerConfiguration * conf);

    int start();
public slots:
    virtual void slotNewConnection();
    void slotPrintMessage (QString message);
    void slotLogMessage (QString message);
signals:
    void signalPrintMessage(QString strMessage);
    void signalLogMessage(QString strMessage);
 protected:
    int port_;
    ServerConfiguration * conf_;
};

#endif // PBMTCPSERVER_H
