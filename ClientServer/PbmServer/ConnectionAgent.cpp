/****************************************************************************
                         ConnectionAgent.cpp

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <QFile>
#include "ConnectionAgent.h"
#include "../PbmCommon/PbmMessage.h"
#include "ServerWindow.h"
#include <iostream>//
#include <fstream>
using namespace std;

ConnectionAgent::ConnectionAgent(QTcpSocket* pClientSocket,
                                 ServerConfiguration * conf, QString faction)
{
    conf_ = conf;
    sock_ = pClientSocket;
    fId_ = faction;
    QString nameBase =conf_->gameDir;
    if(nameBase != "")
    {
        nameBase = nameBase + "/";
    }
    if(fId_ != "")
    {
        nameBase = nameBase + fId_ + "/";
    }
    if(conf_->ordersDir != "")
    {
        ordersNameBase = nameBase + conf_->ordersDir + "/orders.";
    }
    else
    {
        ordersNameBase = nameBase + "orders.";
    }
    if(conf_->reportsDir != "")
    {
        reportsNameBase = nameBase + conf_->reportsDir + "/report.";
    }
    else
    {
        reportsNameBase = nameBase + "report.";
    }
}



int ConnectionAgent::processMessage(PbmMessage * msg)
{
    QString fileName;
    int result = 0;
    switch(msg->getMsgType())
    {
    case AUTENTIFICATION_MESSAGE:
    case AUTENTIFICATION_ACKNOWLEDGE_MESSAGE:
    case AUTENTIFICATION_FAILURE_MESSAGE:
    case REPORT_AVAILABLE_MESSAGE:
    case NO_NEW_REPORT_MESSAGE:
    case NO_REPORT_MESSAGE:
    case THE_TURN_IS_RUNING_MESSAGE:
    case GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE:
    case SEND_REPORT_MESSAGE:
        {
            cerr<<"Irrelevant message type "<<msg->nameOut().toStdString();
            break;
        }
    case GET_REPORT_MESSAGE:
        {
            int turn = msg->getTurnNum();
            if(turn <= conf_->turn)
            {
                if((turn == conf_->turn)&&(QFile::exists(conf_->lockFileName)))
                {
                  PbmMessage answer = PbmMessage::makeControlMessage(
                          THE_TURN_IS_RUNING_MESSAGE,
                          conf_->gameName, conf_->turn);
                  answer.send(sock_);
                }
                else
                {
                    fileName = reportsNameBase + QString::number(turn);
                    PbmMessage answer = PbmMessage::makeSendFileMessage(
                          SEND_REPORT_MESSAGE, conf_->gameName, turn, fileName);
                    if(answer.isValid())
                    {
                        answer.send(sock_);
                        cout<< "Report for turn "<<turn<< " sent"<<endl;
                    }
                    else
                    {
                        cout<< "No report for turn "<<turn
                                <<" |"<<fileName.toStdString()<<"| "<<endl;
                        PbmMessage answer = PbmMessage::makeControlMessage(
                                NO_REPORT_MESSAGE,
                                conf_->gameName, turn);
                        answer.send(sock_);
                    }
                }
            }
            else
            {
                cout<< "Report for turn "<<turn
                        << " is not ready. Current turn "<<conf_->turn<<endl;
                // wrong request
            }
            break;
        }
    case IS_REPORT_AVAILABLE_MESSAGE:
        {
            int turn = msg->getTurnNum();
                if((turn == conf_->turn)&&(QFile::exists(conf_->lockFileName)))
                {
                  PbmMessage answer = PbmMessage::makeControlMessage(
                          THE_TURN_IS_RUNING_MESSAGE,
                          conf_->gameName, conf_->turn);
                  answer.send(sock_);
                }
            if(turn > conf_->turn)
            {
                PbmMessage answer = PbmMessage::makeControlMessage(
                        NO_NEW_REPORT_MESSAGE,conf_->gameName, conf_->turn);
                answer.send(sock_);
            }
            else
            {
                 fileName = reportsNameBase + QString::number(turn);
                 //cout << "Looking for report: " << fileName.toStdString()<<endl;
                 if(!QFile::exists(fileName))
                {
                 //cout << " Not found. " <<endl;
                    PbmMessage answer = PbmMessage::makeControlMessage(
                            NO_NEW_REPORT_MESSAGE,conf_->gameName, conf_->turn);
                    answer.send(sock_);
                }
                else
                {
                     PbmMessage answer = PbmMessage::makeControlMessage(
                            REPORT_AVAILABLE_MESSAGE,conf_->gameName, turn);
                    answer.send(sock_);
                 //cout << " Found. " <<endl;
                }

            }
            break;
        }
    case SEND_ORDERS_MESSAGE:
        {
            if(QFile::exists(conf_->lockFileName))
            {
                PbmMessage answer = PbmMessage::makeControlMessage(
                        THE_TURN_IS_RUNING_MESSAGE,  conf_->gameName,
                        conf_->turn);
                answer.send(sock_);
            }
            else
            {
                int turn = msg->getTurnNum();
                if(turn == conf_->turn)
                {
                fileName = ordersNameBase + QString::number(conf_->turn);
                if(!QFile::exists(fileName))
                {
                  result = 1;
                }
                ofstream outfile (fileName.toAscii());
                outfile<<msg->getMsgBody().toStdString();
                outfile.close();
                PbmMessage answer = PbmMessage::makeControlMessage(
                        GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE,conf_->gameName,
                        conf_->turn);
                answer.send(sock_);
            }
                else //?
                {
                }
            }


            break;
        }
    default:
        {
            cerr<<"Unknown message type " <<msg->printOut().toStdString()<<endl;
        }
    }
    return result; // 1 if new report was received
}
