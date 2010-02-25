/****************************************************************************
                         .cpp

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <QTcpSocket>
#include <iostream>
#include <fstream>
#include "PbmMessage.h"

using namespace std;//
PbmMessage::PbmMessage()
{
    type_ = LAST_MESSAGE_TYPE;
    turnNum_ =0;

    isClosed_ = false; // This flag is set when the last fragment
                       // of fragmented message was received

    lastFragNum_ = 0;
    isPartialyRead_ = false;
}



PbmMessage  PbmMessage::makeControlMessage(MsgType type, QString gameId,
                                                   quint16 turnNum)
 {
     PbmMessage msg;
     msg.setMsgType(type);
     msg.setGameId(gameId);
     msg.setTurnNum(turnNum);
    return msg;
 }



PbmMessage  PbmMessage::makeAuthMessage(QString gameId,
                                                QString factionId,
                                                QString password)
 {
     PbmMessage msg;
     msg.setGameId(gameId);
     msg.setFactionId(factionId);
     msg.setPassword(password);
     msg.setMsgType(AUTENTIFICATION_MESSAGE);
     return msg;
 }



PbmMessage  PbmMessage::makeDataMessage(MsgType type, QString gameId,
                                                quint16 turnNum,
                                                QString body)
 {
     PbmMessage msg;
     msg.setMsgType(type);
     msg.setGameId(gameId);
     msg.setTurnNum(turnNum);
     msg.setMsgBody(body);
     return msg;
 }


 PbmMessage  PbmMessage::makeSendFileMessage(MsgType type, QString gameId,
                                                quint16 turnNum,
                                                QString fileName)
{
    // Check that string is not empty and file exist
    // If not return some indication
        ifstream is;
        is.open (fileName.toStdString().c_str());
        if(is.fail())
        {
            PbmMessage msg = PbmMessage::makeDataMessage(ERROR, "", 0, "");
            return msg;
        }
          int length;
          char * buffer;

      // get length of file:
      is.seekg (0, ios::end);
      length = is.tellg();
      is.seekg (0, ios::beg);
      // allocate memory:
      buffer = new char [length+1];
      buffer[length]='\0';

      // read data as a block:
      is.read (buffer,length);
      is.close();
      QString data = buffer;
      PbmMessage msg = PbmMessage::makeDataMessage(type, gameId, turnNum, data);
      return msg;
}


bool PbmMessage::isValid()
{
    if(this->type_ != ERROR)
    {
        return true;
    }
    else
    {
        return false;
    }
}



bool  PbmMessage::send(QTcpSocket *tcpSocket)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);

    out << quint16(0) <<getMsgType();

    switch(getMsgType())
    {
    case AUTENTIFICATION_MESSAGE:
        {
            out<< getGameId()<<getFactionId()<<getPassword();
            out.device()->seek(0);
            out << quint16(arrBlock.size() - sizeof(quint16));
            tcpSocket->write(arrBlock);
            return true;
        }
    case GET_REPORT_MESSAGE:
    case IS_REPORT_AVAILABLE_MESSAGE:
    case AUTENTIFICATION_FAILURE_MESSAGE:
    case AUTENTIFICATION_ACKNOWLEDGE_MESSAGE:
    case REPORT_AVAILABLE_MESSAGE:
    case NO_NEW_REPORT_MESSAGE:
    case NO_REPORT_MESSAGE:
    case THE_TURN_IS_RUNING_MESSAGE:
    case GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE:
        {
            out<< getGameId()<<getTurnNum();
            out.device()->seek(0);
            out << quint16(arrBlock.size() - sizeof(quint16));
            tcpSocket->write(arrBlock);
    return true;
        }
    case SEND_ORDERS_MESSAGE:
    case SEND_REPORT_MESSAGE:
        {
            return sendWithFragmentation(tcpSocket);
        }
    default:
        {
            std::cerr<<"Unknown message type "<<getMsgType()<<std::endl;
            return false;
        }
    }   
    return true;
}



//// Fragmentation layer format:
//// | Fragmentation Sequence num (2) | Max number of Fragments (2) | Data (size) |

bool PbmMessage::sendWithFragmentation(QTcpSocket *tcpSocket)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out <<getMsgBody();
    quint32 size = arrBlock.size();
    int BLOCK_SIZE = 32000;
    int fragNum = 1;
    int maxFragNum = 1;
    if(size < 1)
    {
        maxFragNum = 1;
    }
    else
    {
        maxFragNum = (size - 1)/BLOCK_SIZE + 1;
    }
    for (quint32 offset = 0; offset < size; offset += BLOCK_SIZE)
    {
        QByteArray fragment;
        QDataStream outf(&fragment, QIODevice::WriteOnly);
        QByteArray f = arrBlock.mid(offset,32000);
        outf.setVersion(QDataStream::Qt_4_2);
        outf << quint16(0) <<getMsgType()<< getGameId()<<getTurnNum()<<quint16(fragNum)<<quint16(maxFragNum) << f;
        outf.device()->seek(0);
        outf << quint16(fragment.size() - sizeof(quint16));
        //std::cerr<<getMsgType()<<"Sent fragment "<<fragNum <<" of "<<maxFragNum <<" size "<<f.size() <<"("<<fragment.size()- sizeof(quint16) <<")"<<std::endl;
         fragNum++;
        tcpSocket->write(fragment);
    }
    return true;
}



bool PbmMessage::receiveWithFragmentation(QDataStream & in)

{
    quint16 fragNum;
    quint16 maxFragNum;
    QByteArray fragment;

    in>> fragNum >> maxFragNum >> fragment;
    //std::cout<<"Received fragment "<<fragNum <<" of "<<maxFragNum
    //        <<" size " << fragment.size()<<std::endl;

    if(fragNum == 1) // this is a new fragment
    {
        lastFragNum_ = 1;
        arrBlock_.clear();
        isClosed_ = false;
        arrBlock_.append(fragment);
    }
    else
    {
        if(fragNum != lastFragNum_ + 1)
        {
            std::cout<<"Wrong. Segment "<<lastFragNum_ + 1
                    << " is missing"<<std::endl;
            return false;
        }
        lastFragNum_++;
        arrBlock_.append(fragment);

    }


    if(fragNum  == maxFragNum)// this is a last fragment
    {
        isClosed_ = true;
        QDataStream aggregatedData(arrBlock_);
        aggregatedData>>body_;
    }
    else
    {
        isClosed_ = false;
    }

    return true;
}



bool  PbmMessage::receive(QTcpSocket *tcpSocket)
{
    qint32 type;
    QString gameId;
    QString factionId;
    QString password;
    QByteArray arrBlock;


    // Got all fragments. Now


    QDataStream in(tcpSocket) ;
    in.setVersion(QDataStream::Qt_4_2);
    qint64 sz = tcpSocket->bytesAvailable();
    if (sz < sizeof(quint16))
    {
       // std::cout<<" No available bytes in socket "<<sz<<"\n ";
        isPartialyRead_ = false;
        return false;
    }

    if(!isPartialyRead_)
    {
        in>>nextBlockSize;
    }

    if (sz < nextBlockSize)
    {
        isPartialyRead_ = true;
        //std::cout<<" Less available bytes in socket "<<sz<<" than " <<nextBlockSize<<endl;
        return false;
    }
    isPartialyRead_ = false;

 // std::cout<<" available bytes in socket "<<sz<<" ";
 //std::cerr<<"Array size "<<arrBlock_.size()<<endl;


       in>>type >>gameId;

    switch(type)
    {
    case AUTENTIFICATION_MESSAGE:
        {
            in >>factionId>>password;
            setGameId(gameId);
            setFactionId(factionId);
            setPassword(password);
            isClosed_ = true;
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
        {
            //out<< msg.getGameId()<<msg.getTurnNum();
            in >> turnNum_;
             setGameId(gameId);
            isClosed_ = true;
            break;
        }
    case SEND_ORDERS_MESSAGE:
    case SEND_REPORT_MESSAGE:
        {
            in >> turnNum_;
            type_ = (MsgType)type;
            setGameId(gameId);
            bool result = receiveWithFragmentation(in);
            //sz = tcpSocket->bytesAvailable();
             //std::cout<<"\t\t\t After receiving available bytes in socket "<<sz<<"\n ";
            return result;
        }
    case ERROR:
        {
            //  ?
            break;
        }
   default:
        {
            std::cerr<<"receive: Unknown message type "<<type;
            //std::cout<<" nextBlockSize "<<nextBlockSize<<endl;
            return false;
        }
    }

    type_ = (MsgType)type;
    return true;

}


QString PbmMessage::printOut()
{
    QString out = "Message type: " + QString::number(type_) + " ";

  switch(type_)
    {
    case AUTENTIFICATION_MESSAGE:
        {
            out = out + " AUTENTIFICATION_MESSAGE "
                      + gameId_ + " " + factionId_ + " " + password_;
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
      {
            out += gameId_ + " Turn:  "+ QString::number(turnNum_);
            break;
        }
    case SEND_ORDERS_MESSAGE:
    case SEND_REPORT_MESSAGE:
        {
            out  += gameId_ + " Message: " + body_;
            break;
        }
       case ERROR:
        {
            out  += "Client Error\n";
            break;
        }
    default:
        {
            std::cerr<<"Unknown message type "<<getMsgType()<<std::endl;
            out += "Unknown message type " ;
        }
    }
  return out;
}



QString PbmMessage::nameOut()
{
    switch(type_)
    {
    case AUTENTIFICATION_MESSAGE:
        {
            return " AUTENTIFICATION_MESSAGE ";
        }
    case GET_REPORT_MESSAGE:
        {
            return " GET_REPORT_MESSAGE " + QString::number(turnNum_);
        }
    case IS_REPORT_AVAILABLE_MESSAGE:
        {
            return " IS_REPORT_AVAILABLE_MESSAGE " + QString::number(turnNum_);
        }
    case AUTENTIFICATION_ACKNOWLEDGE_MESSAGE:
        {
            return " AUTENTIFICATION_ACKNOWLEDGE_MESSAGE ";
        }
    case AUTENTIFICATION_FAILURE_MESSAGE:
        {
            return " AUTENTIFICATION_FAILURE_MESSAGE ";
        }
    case REPORT_AVAILABLE_MESSAGE:
        {
            return " REPORT_AVAILABLE_MESSAGE " + QString::number(turnNum_);
        }
    case NO_NEW_REPORT_MESSAGE:
        {
            return " NO_NEW_REPORT_MESSAGE " + QString::number(turnNum_);
        }
    case NO_REPORT_MESSAGE:
        {
            return " NO_REPORT_MESSAGE " + QString::number(turnNum_);
        }
    case THE_TURN_IS_RUNING_MESSAGE:
        {
            return " THE_TURN_IS_RUNING_MESSAGE " + QString::number(turnNum_);
        }
    case GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE:
        {
            return " GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE "
                    + QString::number(turnNum_);
        }
    case SEND_ORDERS_MESSAGE:
        {
            return " SEND_ORDERS_MESSAGE " + QString::number(turnNum_);
        }
    case SEND_REPORT_MESSAGE:
        {
            return " SEND_REPORT_MESSAGE " + QString::number(turnNum_);
        }
    case ERROR:
        {
            return " ERROR ";
        }
    default:
        {
            std::cerr<<"Unknown message type "<<getMsgType()<<std::endl;
            return "Unknown message type " ;
        }
    }
}

