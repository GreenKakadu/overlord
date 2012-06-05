/****************************************************************************
                         ConnectionController.cpp

        Manages Connection with Server.
    Implements specific Client-Server protocol part
    and interprets messages from server.
                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <iostream>//
#include "ConnectionController.h"
#include "../PbmCommon/PbmMessage.h"
#include <fstream>
using namespace std;



ConnectionController::ConnectionController()
{
    currentTurn = 0;
    tcpSocket = new QTcpSocket();
    state = UNKNOWN;
    msg = new PbmMessage();
    connect(tcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(tcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT (slotError (QAbstractSocket::SocketError))  );

}

int ConnectionController::init(QString gameName, QString userId,
                               QString password, int turn)
{
    gameName_ = gameName;
    userId_   = userId;
    password_ = password;
    currentTurn = turn;
    return 0;

}



int ConnectionController::start(QString ipAdr, int port)
{
    ipAdr_ = ipAdr;
    port_ = port;
    tcpSocket->connectToHost(ipAdr_, port_);
    std::cout<< "Connecting to "<< ipAdr_.toStdString()<<":"<<port_ <<std::endl;
    setState(CONNECTING);
    return 0;
}



ConnectionController::~ConnectionController()
{
}



void ConnectionController::setState(ConnectionState a_state)
{
    state = a_state;
    emit stateChaged(state);
}



void ConnectionController::checkNewTurn(int turn)
{
    PbmMessage answer = PbmMessage::makeControlMessage
                        (IS_REPORT_AVAILABLE_MESSAGE,gameName_,turn);
    answer.send(this->tcpSocket);
}


void ConnectionController::sendAuthMessage()
{
    emit printMessage("Received the connected signal \n");
    emit printStatus("Authorising");
    setState(AUTHORIZING);
    PbmMessage msg = PbmMessage::makeAuthMessage(gameName_,userId_,password_);
    msg.send(this->tcpSocket);
}



void ConnectionController::slotConnected()
{
    sendAuthMessage();
}



void ConnectionController::slotError(QAbstractSocket::SocketError err)
{
    QString strError =
            "Error: " + (err == QAbstractSocket::HostNotFoundError ?
                         "The host was not found." :
                         err == QAbstractSocket::RemoteHostClosedError ?
                         "The remote host is closed." :
                         err == QAbstractSocket::ConnectionRefusedError ?
                         "The connection was refused." :
                         QString(tcpSocket->errorString()) );
    emit printMessage(strError + "\n");
    setState(CONNECTION_FAILED);
    emit printStatus("Server offline");
}


void ConnectionController::slotReadyRead()
{
    for(;;)
    {
        bool result = msg->receive(tcpSocket);
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
        //processMessage(msg);
    }
}



void ConnectionController::processMessage(PbmMessage * msg)
{
    QString str = msg->nameOut();
    emit writeLogMessage(str + "\n");
    int turn  = msg->getTurnNum();
    // Process received message
    switch(msg->getMsgType())
    {
    case AUTENTIFICATION_MESSAGE:
    case IS_REPORT_AVAILABLE_MESSAGE:
    case SEND_ORDERS_MESSAGE:
    case GET_REPORT_MESSAGE:
        {
            break;
        }
    case AUTENTIFICATION_ACKNOWLEDGE_MESSAGE:
        {
            currentTurn = turn;
            emit printStatus("Connected");
            emit setTurnNumSignal(currentTurn);
            setState(CONNECTED);
            checkNewTurn(currentTurn);
            break;
        }
    case AUTENTIFICATION_FAILURE_MESSAGE:
        {
            emit printStatus("Authorisation Failed");
            setState(AUTHORIZATION_FAILED);
            break;
        }
    case REPORT_AVAILABLE_MESSAGE:
        {
            emit receiveLedSignal(Qt::green,true);
            emit printStatus("Report for turn "
                             + QString::number(turn) + " is available");
            emit setReceiveButtonSignal(true);
            break;
        }
    case NO_REPORT_MESSAGE:
        {
            emit receiveLedSignal(Qt::gray,false);
            emit setReceiveButtonSignal(false);
            emit printStatus("No report for turn "
                             + QString::number(turn));
            break;
        }
    case NO_NEW_REPORT_MESSAGE:
        {
            emit receiveLedSignal(Qt::gray,false);
            emit setReceiveButtonSignal(false);
            emit printStatus("No report for turn "
                             + QString::number(turn));
             if(currentTurn > turn)
            {
                currentTurn = turn;
            emit setTurnNumSignal(currentTurn);
            }
           break;
        }
    case THE_TURN_IS_RUNING_MESSAGE:
        {
            emit receiveLedSignal(Qt::yellow,true);
            emit printStatus("Turn "
                          + QString::number(turn) + " is runing now");

             if(currentTurn > turn)
            {
                currentTurn = turn;
            emit setTurnNumSignal(currentTurn);
            }
            break;
        }
    case GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE:
        {
            if((msg->getGameId() == this->gameName_) &&
               (turn == this->currentTurn))
            {
                emit sendLedSignal(Qt::green,false);
            emit printStatus("Server confirms receiving orders for turn "
                          + QString::number(turn));
            }
            else
            {
                emit writeLogMessage(" Wrong Acknowlegement game= "
                        + msg->getGameId() + " "
                        + QString::number(turn) + "\n");
            }
            break;
        }
    case SEND_REPORT_MESSAGE:
        {
            QString fileName = /*conf_->playersPath +*/ "report."
                                                    + QString::number(turn);

            ofstream outfile(fileName.toStdString().c_str());
            outfile<<msg->getMsgBody().toStdString();
            outfile.close();
            emit printMessage(" Report for turn "
                 + QString::number(turn) + " received to " + fileName + "\n");
             emit gotNewReport(turn);
             if(currentTurn == turn)
            {
                currentTurn++;
                emit setTurnNumSignal(currentTurn);
            }
            break;
        }
    default:
        {
            cerr<<"Unknown message type "<<msg->getMsgType()<<endl;
            //return false;
        }
    }
}



int ConnectionController::close()
{
    tcpSocket->disconnectFromHost();
    setState(UNKNOWN);
    return 0;
}



bool ConnectionController::sendDataMessage(QString fileName)
{
    PbmMessage msg = PbmMessage::makeSendFileMessage(
            SEND_ORDERS_MESSAGE,gameName_,currentTurn,fileName);
    if(msg.isValid())
    {
        msg.send(this->tcpSocket);
        return true;
    }
    else
    {
        return false;
    }

}




bool ConnectionController::sendControlMessage(MsgType type, int turn)
{
    PbmMessage msg = PbmMessage::makeControlMessage
                     (type,gameName_,turn);
    msg.send(tcpSocket);
    //emit writeLogMessage(" Request for turn sent\n");
    return true;
}
