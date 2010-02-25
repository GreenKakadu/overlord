/****************************************************************************
                         MainClientWindow.cpp

                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <iostream>//
#include <fstream>
#include <QFileDialog>
#include <QToolButton>
#include <QLabel>
#include <QPixmap>
#include <QTime>
#include <QHostInfo>
#include <QLCDNumber>
#define CLIENT_VERSION "0.5.1"
#include "MainClientWindow.h"
#include "ui_MainClientWindow.h"
#include "../PbmCommon/FileParser.h"
#include "../PbmCommon/PbmMessage.h"
#include "../PbmCommon/Common.h"

using namespace std;
MainClientWindow::MainClientWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainClientWindow)
{
    ui->setupUi(this);
        setWindowTitle(tr("Overlord Client"));

    QPixmap pix;
    pix.load(QString::fromUtf8
                      (":/new/prefix1/icons/Sent-Mail-128x128.png"));
    setWindowIcon(pix);


    fileDialog = new QFileDialog(this);
    conectionLed = new Led();
    sendLed = new Led();
    receiveLed = new Led();
    lcdNum = new QLCDNumber();
    lcdNum->setSegmentStyle(QLCDNumber::Filled);

    // Set LCD text color
    {
        QPalette palette;
        QBrush brush(QColor(0, 0, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(0, 170, 255, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Dark, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush1);

        lcdNum->setPalette(palette);
        lcdNum->setSegmentStyle(QLCDNumber::Filled);
    }// Set LCD text color end

    statusText = new QLabel("Orders file is not selected");
    ui->statusBar->addPermanentWidget(statusText,1);
    ui->statusBar->addPermanentWidget(lcdNum,0);
    ui->statusBar->addPermanentWidget(conectionLed,0);
    ui->statusBar->addPermanentWidget(sendLed,0);
    ui->statusBar->addPermanentWidget(receiveLed,0);
    connect(fileDialog, SIGNAL(currentChanged(QString)),
            this, SLOT(fileNameChanged(QString)));
    logFile.open("client.log");
    if(!logFile.is_open())
    {
        ui->logWindow->insertPlainText("Failed to open log file");
    }
    ui->logWindow->insertPlainText(QString("Running version ")
                                   + CLIENT_VERSION + "\n");
    logFile<<"Running version " <<CLIENT_VERSION<<endl;
    // Configuration
    configWin = new QWidget(0);
    ui_configWin = new Ui::Form();
    ui_configWin->setupUi(configWin);
    connect (ui->actionQuit, SIGNAL(triggered()), configWin, SLOT(close()));
    connect(ui_configWin->okButton,SIGNAL(clicked()),
            this,SLOT(okButton_clicked()));
    ui->actionSend->setEnabled(false);\
    ui->actionReceive->setEnabled(false);
    ui->actionReceive_report_for_turn->setEnabled(false);
    configFileName_ = "client.ini";
    orderFileName_ = "";
    currentTurn_ = 0;

    askReport = new QDialog(0);
    ui_askReport = new Ui::Dialog();
    ui_askReport->setupUi(askReport);

    connect(ui_askReport->okButton,SIGNAL(clicked()),
            this,SLOT(turnNumOk_clicked()));
    connect(ui_askReport->cancelButton,SIGNAL(clicked()),
            this,SLOT(turnNumCancel_clicked()));

    connection = new ConnectionController();
    connect(connection,SIGNAL(printMessage(QString)),
            this,SLOT(printMessage(QString)));
    connect(connection,SIGNAL(writeLogMessage(QString)),
            this,SLOT(writeLogMessage(QString)));
    connect(connection,SIGNAL(printStatus(QString)),
            this,SLOT(writeStatusMessage(QString)));
    connect(connection,SIGNAL(stateChaged(ConnectionState)),
            this,SLOT(updateLeds(ConnectionState)));
    connect(connection,SIGNAL(sendLedSignal(QColor,bool)),
            this,SLOT(updateSendLed(QColor,bool)));
    connect(connection,SIGNAL(receiveLedSignal(QColor,bool)),
            this,SLOT(updateReceiveLed(QColor,bool)));
    connect(connection,SIGNAL(setReceiveButtonSignal(bool)),
            this,SLOT(setReceiveButton(bool))); 
    connect(connection,SIGNAL(gotNewReport(int)),
            this,SLOT(newReport(int)));
    connect(connection,SIGNAL(setTurnNumSignal(int)),
            this,SLOT(setTurnNum(int)));

//    connect(connection,SIGNAL(gotNewReport(int)),
//            this,SLOT(newReport(int)));

    //"pbem.homeip.net"
 // Read Configuration file

    bool isConfigured = configure();
    if(isConfigured)
    {
        ui_configWin->ipAddress->setText(address_)     ;
        ui_configWin->port->setText(QString::number(port_));
        ui_configWin->gameName->setText(gameName_);
        ui_configWin->userId->setText(userId_);
        ui_configWin->password->setText(password_);
        //ui_configWin->turnNumSpinBox->setValue(currentTurn_);
        //lcdNum->display(currentTurn_);
        //lcdNum->setDisabled(true);
        lcdNum->hide();
        connection->init(gameName_, userId_, password_, currentTurn_);
    }
    lastTurnReceived = -1;
    for(int n=0; n <= currentTurn_; n++)
    {
        QString fileName = /*conf_->playersPath + */ "report."
                           + QString::number(n);
        if(QFile::exists(fileName))
        {
          lastTurnReceived = n;
        }
    }
    if(lastTurnReceived >= 0 )
    {
        ui->logWindow->insertPlainText(
                QString("Latest received report is for turn ") +
                QString::number(lastTurnReceived) +  "\n");
    }
    // Start periodic timer that will be used for periodic actions
    timeoutTimer = new QTimer(this);
    connect(timeoutTimer, SIGNAL(timeout()), this, SLOT(periodicCheck()));
    timeoutTimer->start(60000);
    sendOrdersTimeout= new QTimer(this);
    connect(sendOrdersTimeout, SIGNAL(timeout()), this, SLOT(sendOrdersTimeoutExpired()));


}



MainClientWindow::~MainClientWindow()
{
    ofstream out;
    out.open ("client.ini");
    out << "IP " << address_.toStdString()<<endl;
    out << "port " << port_<<endl;
    out << "name " << userId_.toStdString()<<endl;
    out << "game " << gameName_.toStdString()<<endl;
    out << "password "<< password_.toStdString()<<endl;
    //out << "turn "<< currentTurn_<<endl;
    out.close();
    logFile.close();
    delete ui;
}


bool MainClientWindow::configure()
{
    FileParser * parser = new FileParser(configFileName_.toAscii());
    bool isConfigured = false;
//    currentTurn_ = 0;
    if(parser->status >0)
    {
            do
              {
                parser->getLine();
                if (parser->matchKeyword("IP"))
                        {
                                address_  = parser->getWord().c_str();
                                ipAdr_ = getIpAddress(address_);
                                continue;
                        }
                if (parser->matchKeyword("port"))
                        {
                                port_ = parser->getInteger();
                                if((port_ <0) || (port_ > 65535))
                                {
                                    port_ = 0;
                                }
                                continue;
                        }
                if (parser->matchKeyword("game"))
                        {
                                gameName_ =QString(parser->getWord().c_str());
                                continue;
                        }
                if (parser->matchKeyword("name"))
                        {
                                userId_ =QString(parser->getWord().c_str());
                                continue;
                        }
                if (parser->matchKeyword("password"))
                        {
                                password_ =QString(parser->getWord().c_str());
                                continue;
                        }

//                if (parser->matchKeyword("turn"))
//                        {
//                                currentTurn_ = parser->getInteger();
//                                if((currentTurn_ <0) || (currentTurn_ > 65535))
//                                {
//                                    currentTurn_ = 0;
//                                }
//                                someTurn = currentTurn_;
//                                continue;
//                        }
             } while (! parser ->eof() );

           isConfigured =true;
    }
    else
    {
        ui->logWindow->insertPlainText
                (QString("Can't find file ") + configFileName_);
    }

    delete parser;
    return isConfigured;
}



void MainClientWindow::on_actionOpen_triggered()
{
    fileDialog->show();
}



void MainClientWindow::fileNameChanged(QString name)
{
   statusText->setText(name);
   orderFileName_ = name;
   if(connection->getState()==CONNECTED)
   {
     ui->actionSend->setEnabled(true);
   }
}


void MainClientWindow::on_actionConfiguration_triggered()
{
   configWin->show();
}


QString MainClientWindow::getIpAddress(QString address)
{
    QRegExp rx
("0*(2(5[0-5]|[0-4]\\d)|1?\\d{1,2})(\\.0*(2(5[0-5]|[0-4]\\d)|1?\\d{1,2})){3}");

    if( rx.exactMatch(address))
    {
        return address;
    }
    else
    {
        cout<<" Resolving IP address for " << address.toStdString()<<endl;
        QHostInfo info = QHostInfo::fromName(address);
        if(info.error() == QHostInfo::NoError )
        {
            return info.addresses().first().toString();
        }
        if(info.error()  == QHostInfo::HostNotFound )
        {
            ui->logWindow->insertPlainText
                    ("Host " + address + " not found");
            return address;
        }
        if(info.error()  == QHostInfo:: UnknownError)
        {
            ui->logWindow->insertPlainText
                    ("Can't find address for host " + address);
            return address;
        }

    }
    return address;
}


void MainClientWindow::okButton_clicked()
{
    address_      = ui_configWin->ipAddress->text()  ;
    ipAdr_ = getIpAddress(address_);
    port_       = ui_configWin->port->text().toInt();
    if((port_ <0) || (port_ > 65535))
    {
        port_ = 0;
    }
    gameName_   = ui_configWin->gameName->text();
    userId_     = ui_configWin->userId->text();
    password_   = ui_configWin->password->text();
    //currentTurn_ = ui_configWin->turnNumSpinBox->value();
    //lcdNum->display(currentTurn_);
    connection->init(gameName_, userId_, password_, currentTurn_);
    configWin->close();
}



// Timeout is over.
void MainClientWindow::periodicCheck()
{
    switch(connection->getState())
    {
    case CONNECTING:
    case AUTHORIZING:
        break; //Do nothing here
    case AUTHORIZATION_FAILED:
    case CONNECTION_FAILED:
    case UNKNOWN:
        //Perform new connection attempt
        break;
    case CONNECTED:
        {
            if(connection)
            {
                connection->checkNewTurn(currentTurn_);
            }
            break;
        }
     default:
        {
        }
    }
}



void MainClientWindow::on_actionConnect_triggered()
{
    switch(connection->getState())
    {
    case CONNECTED:
    case CONNECTING:
    case AUTHORIZING:
        ui->actionConnect->setChecked(false);
        connection->close();
        break;
    case CONNECTION_FAILED:
    case UNKNOWN:
        //Perform new connection attempt
        //cout << "Connection attempt"<<endl;
        ui->actionConnect->setChecked(true);    
        connection->start(ipAdr_, port_);
        break;
    case AUTHORIZATION_FAILED:
        ui->actionConnect->setChecked(true);
        connection->sendAuthMessage();
        break;
    default:
        {
        }
    }
}



void MainClientWindow::on_actionSend_triggered()
{
    switch(connection->getState())
    {

    case CONNECTED:
        {
        bool result =  connection->sendDataMessage(orderFileName_);
        if(result)
        {
            this->sendLed->setColor(Qt::green,true);
            sendOrdersTimeout->start(5000);
        }
        else
        {
            this->sendLed->setColor(Qt::red);
        }
        break;
    }
    case CONNECTING:
    case AUTHORIZING:
    case AUTHORIZATION_FAILED:
    case CONNECTION_FAILED:
    case UNKNOWN:
        break; //Do nothing here or try to connect and send later
    default:
        {
        }

    }
}
void MainClientWindow::sendOrdersTimeoutExpired()
{
    sendLed->setColor(Qt::red);
    sendOrdersTimeout->stop();
}

void MainClientWindow::printMessage(QString msg)
{
    ui->logWindow->insertPlainText(msg);
}
void MainClientWindow::writeLogMessage(QString message)
{
        logFile << (QTime::currentTime ().toString() + " "
                + message).toStdString().c_str()<<endl;
}



void MainClientWindow::writeStatusMessage(QString msg)
{
    statusText->setText(msg);
}




void MainClientWindow::setReceiveButton(bool value)
{
     ui->actionReceive->setEnabled(value);
}



void MainClientWindow::updateLeds(ConnectionState state)
{
    switch(state)
    {

    case CONNECTED:
        conectionLed->setColor(Qt::green,false);
        if((orderFileName_ != "") && QFile::exists(orderFileName_))
        {
                    ui->actionSend->setEnabled(true);
        }
        ui->actionReceive->setEnabled(true);
         ui->actionReceive_report_for_turn->setEnabled(true);
       break;
    case CONNECTING:
        conectionLed->setColor(Qt::yellow,true);
        ui->actionSend->setEnabled(false);
        ui->actionReceive->setEnabled(false);
        ui->actionReceive_report_for_turn->setEnabled(false);
        break;
    case AUTHORIZING:
        conectionLed->setColor(Qt::green,true);
        ui->actionSend->setEnabled(false);
        ui->actionReceive->setEnabled(false);
         ui->actionReceive_report_for_turn->setEnabled(false);

        break;
    case AUTHORIZATION_FAILED:
    case CONNECTION_FAILED:
        conectionLed->setColor(Qt::red,true);
        ui->actionConnect->setChecked(false);
        ui->actionSend->setEnabled(false);
        ui->actionReceive->setEnabled(false);
          ui->actionReceive_report_for_turn->setEnabled(false);
      break;
    case UNKNOWN:
        conectionLed->setColor(Qt::gray,false);
        ui->actionSend->setEnabled(false);
        ui->actionReceive->setEnabled(false);
        ui->actionReceive_report_for_turn->setEnabled(false);
        break; //Do nothing here or try to connect and send later
    default:
        {
        }

    }
}
void MainClientWindow::updateSendLed(QColor color,bool isBlinking)
{
    sendLed->setColor(color, isBlinking);
    if(color == Qt::green && isBlinking == false)
    {
        sendOrdersTimeout->stop();
    }
}
void MainClientWindow::updateReceiveLed(QColor color,bool isBlinking)
{
        receiveLed->setColor(color, isBlinking);
}

void MainClientWindow::on_actionReceive_triggered()
{
    connection->sendControlMessage(GET_REPORT_MESSAGE,currentTurn_);
}

void MainClientWindow::on_actionReceive_report_for_turn_triggered()
{
    ui_askReport->spinBox->setValue(someTurn);
    askReport->show();
}


void MainClientWindow::turnNumOk_clicked()
{
    someTurn = ui_askReport->spinBox->value();
    connection->sendControlMessage(GET_REPORT_MESSAGE,someTurn);
    askReport->close();

}
void MainClientWindow::turnNumCancel_clicked()
{
    askReport->close();
}


// Should include some visual indication
void MainClientWindow::newReport(int turn)
{
    if(lastTurnReceived < turn)
    {
        lastTurnReceived = turn;
        //lcdNum->display(turn);
    }
//    if(currentTurn_ < turn)// One of old turns
//    {
//        currentTurn_ = turn;
//        lcdNum->display(turn);
//    }
//    if(currentTurn_ == turn) // Latest turn
//    {
//        currentTurn_ = turn +1;
//        lcdNum->display(turn);
//    }
}



void MainClientWindow::setTurnNum(int turn)
{
    lcdNum->show();
    lcdNum->display(turn);
    currentTurn_ = turn;
}
