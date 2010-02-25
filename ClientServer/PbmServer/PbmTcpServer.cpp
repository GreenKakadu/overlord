/****************************************************************************
                         PbmTcpServer.cpp
    TCP Server, that opens PbmConnections.
                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <QtGui>
#include "PbmTcpServer.h"
#include "PbmConnection.h"
#include "ServerWindow.h"
PbmTcpServer::PbmTcpServer(int port, ServerConfiguration * conf)
{
    conf_ = conf;
    port_ = port;
}


int PbmTcpServer::start()
{
    if (!this->listen(QHostAddress::Any, port_))
    {
        QMessageBox::critical(0, tr("Server Error"),
                              tr("Unable to start the server:")
                              + this->errorString() );
        this->close() ;
        return 1;
    }
    connect(this, SIGNAL(newConnection()), this, SLOT(slotNewConnection()) );
    QString strMessage = "Opening Server on port : " + QString::number(port_);
    emit signalPrintMessage(strMessage);
    return 0;
}

void PbmTcpServer::slotNewConnection()
{
  //Connect PbmConnection  to pClientSocket  slots
  QTcpSocket* pClientSocket = this->nextPendingConnection();
  PbmConnection * connection = new PbmConnection(pClientSocket, conf_);
  connect(connection,SIGNAL(signalPrintMessage(QString)),
          this,SLOT(slotPrintMessage(QString)));
  connect(connection,SIGNAL(signalLogMessage(QString)),
          this,SLOT(slotLogMessage(QString)));
  QString strMessage = "New connection from : " + pClientSocket->peerName() +
                       " (" + pClientSocket->peerAddress().toString() + ":" +
                       QString::number(pClientSocket->peerPort()) + ")";
  emit signalLogMessage(strMessage);
}


void PbmTcpServer::slotPrintMessage (QString message)
{
    emit signalPrintMessage(message);
}
void PbmTcpServer::slotLogMessage (QString message)
{
    emit signalLogMessage(message);
}
