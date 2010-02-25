/****************************************************************************
                         PbmConnection.cpp

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <QtNetwork>
#include "PbmConnection.h"
#include "ConnectionAgent.h"
#include "ServerWindow.h"
#include <iostream>//
#include "../PbmCommon/PbmMessage.h"
int PbmConnection::connectionCounter=0;

PbmConnection::PbmConnection(QTcpSocket* pClientSocket,
                             ServerConfiguration * conf)
{
    num = PbmConnection::connectionCounter++;
    pClientSocket_ = pClientSocket;
    agent = 0;
    msg = new PbmMessage();
    conf_ = conf;
    isAuthorized = false;
    connect(pClientSocket_, SIGNAL(disconnected()),
            this, SLOT(deleteConection()) );
    connect(pClientSocket_, SIGNAL(readyRead()),
            this, SLOT(slotReadClient()) );
    timeoutTimer = new QTimer(this);
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(deleteConection()));
    timeoutTimer->start(2000);
}



PbmConnection::~PbmConnection()
{
    //std::cout<<"PbmConnection "<< num << " destroyed"<<std::endl;
}



void PbmConnection::deleteConection()
{
    //std::cout<<"PbmConnection "<< num << " should be destroyed now"<<std::endl;
    delete agent;
    pClientSocket_->deleteLater();
    this->deleteLater();
}

void PbmConnection::authenticationFailed()
{
    PbmMessage answer = PbmMessage::makeControlMessage(
            AUTENTIFICATION_FAILURE_MESSAGE,conf_->gameName, conf_->turn);
    answer.send(pClientSocket_);
    pClientSocket_->disconnectFromHost();
    emit signalLogMessage("Connection rejected");
}




void PbmConnection::processMessage(PbmMessage * msg)
{
    emit signalLogMessage(msg->nameOut());
    if(!isAuthorized)
    {
        switch(msg->getMsgType())
        {
        case AUTENTIFICATION_MESSAGE:
            {
                if(msg->getGameId() != conf_->gameName )
                {
                    authenticationFailed();
                    return;
                }
                // Find record for User Id
                QMap<QString, QString>::const_iterator s
                        = conf_->users.find(msg->getFactionId());
                if(s== conf_->users.end())
                {
                    authenticationFailed();
                    return;
                }
                if(s.value() != msg->getPassword())
                {
                    authenticationFailed();
                    return;
                }
                agent = new ConnectionAgent(pClientSocket_,
                                            conf_, msg->getFactionId());
                PbmMessage answer = PbmMessage::makeControlMessage(
                        AUTENTIFICATION_ACKNOWLEDGE_MESSAGE,
                        conf_->gameName, conf_->turn);
                answer.send(pClientSocket_);
                emit signalLogMessage(answer.printOut());
                isAuthorized = true;
                timeoutTimer->stop();
                break;
            }
        case GET_REPORT_MESSAGE:
        case IS_REPORT_AVAILABLE_MESSAGE:
        case AUTENTIFICATION_ACKNOWLEDGE_MESSAGE:
        case AUTENTIFICATION_FAILURE_MESSAGE:
        case REPORT_AVAILABLE_MESSAGE:
        case NO_NEW_REPORT_MESSAGE:
        case NO_REPORT_MESSAGE:
        case THE_TURN_IS_RUNING_MESSAGE:
        case GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE:
        case SEND_ORDERS_MESSAGE:
        case SEND_REPORT_MESSAGE:
            {
                std::cerr<<"Unappropriate message type "
                        <<msg->nameOut().toStdString()
                        << " Authorization expected"<<std::endl;
                break;
            }

        default:
            {
                std::cerr<<"Unknown message type "
                        <<msg->getMsgType()<<std::endl;
            }
        }
    }
    else // Process received message
    {
        if(agent)
        {
            if(agent->processMessage(msg))
            {
                 emit signalLogMessage("New orders received.");
            }
        }

    }
}
void PbmConnection::slotReadClient()
{
    for(;;)
    {
        bool result = msg->receive(pClientSocket_);
        if(result == false)// Mo more data to read
        {
            return;
        }
        if(msg->isClosed())
        {
            processMessage(msg);
            delete msg;
            msg = new PbmMessage();
        }
    }
}


