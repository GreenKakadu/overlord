/****************************************************************************
                         PbmMessage.h
 Implementation of generic part of Client-server protocol
 Message structure, Message construction, sending and receiving.
 Some long messages may be fragmentated and should be assembled on receiving.
                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#ifndef PBMMESSAGE_H
#define PBMMESSAGE_H
#include <QtGlobal>
#include <QString>
class QTcpSocket;
enum MsgType {
    AUTENTIFICATION_MESSAGE = 0,
    GET_REPORT_MESSAGE = 1,
    IS_REPORT_AVAILABLE_MESSAGE = 2,
    AUTENTIFICATION_FAILURE_MESSAGE = 3,
    AUTENTIFICATION_ACKNOWLEDGE_MESSAGE = 4,
    REPORT_AVAILABLE_MESSAGE = 5,
    SEND_REPORT_MESSAGE = 6,
    NO_REPORT_MESSAGE = 7,
    NO_NEW_REPORT_MESSAGE = 8,
    THE_TURN_IS_RUNING_MESSAGE = 9,
    GOT_ORDERS_ACKNOWLEDGEMENT_MESSAGE = 10,
    SEND_ORDERS_MESSAGE = 11,
    ERROR = 98,
    LAST_MESSAGE_TYPE = 99

};

class PbmMessage
{
public:
    PbmMessage();

    static PbmMessage makeControlMessage(MsgType type, QString gameId,
                                      quint16 turnNum =0);
    static PbmMessage makeAuthMessage(QString gameId, QString factionId,
                                      QString password);
    static PbmMessage makeDataMessage(MsgType type,QString gameId,
                                      quint16 turnNum, QString body);
    static PbmMessage makeSendFileMessage(MsgType type, QString gameId,
                                      quint16 turnNum, QString fileName);



    bool send(QTcpSocket *tcpSocket);
    bool sendWithFragmentation(QTcpSocket *tcpSocket);
    bool receive(QTcpSocket *tcpSocket);
    bool receiveWithFragmentation(QDataStream & in);
    inline void setMsgType(MsgType type){type_ = type;}
    inline MsgType getMsgType(){return type_;}
    inline void setTurnNum(quint16 turnNum){turnNum_ = turnNum;}
    inline quint16 getTurnNum(){return turnNum_ ;}
    inline void setGameId(QString gameId){gameId_ = gameId;}
    inline QString getGameId(){return gameId_;}

    inline void setFactionId(QString factionId){factionId_ = factionId;}
    inline QString getFactionId(){return factionId_;}

    inline void setPassword(QString password){password_ = password;}
    inline QString getPassword(){return password_;}
    inline void setMsgBody(QString body){body_ = body;}
    inline QString  getMsgBody(){return body_;}
    bool isValid();
    QString printOut();
    QString nameOut();
    inline bool isClosed() {return isClosed_;}

protected:
    MsgType type_; // Message type
    quint16 turnNum_;
    QString gameId_;
    QString factionId_;
    QString password_;
    QString body_;
    quint32 crc; // Message CRC
    bool isPartialyRead_;
    qint16 nextBlockSize;
    bool isClosed_;
    QByteArray arrBlock_;
    quint16 lastFragNum_;

};

#endif // PBMMESSAGE_H
