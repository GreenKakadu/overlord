/****************************************************************************
                         ConnectionAgent.h

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#ifndef CONNECTIONAGENT_H
#define CONNECTIONAGENT_H
#include <QString>
class PbmMessage;
class QTcpSocket;
struct ServerConfiguration;
class ConnectionAgent
{
public:
    ConnectionAgent(QTcpSocket* pClientSocket, ServerConfiguration * conf,
                    QString faction);
    int processMessage(PbmMessage * msg);

protected:
    ServerConfiguration * conf_;
    QTcpSocket* sock_;
    QString fId_;
     QString reportsNameBase;
     QString ordersNameBase;
};

#endif // CONNECTIONAGENT_H
