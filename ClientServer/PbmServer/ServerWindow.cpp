#include "ServerWindow.h"
#include "ui_ServerWindow.h"
#define SERVER_VERSION "0.5.0"
#include <QtGui>
#include "PbmTcpServer.h"
#include "../PbmCommon/Led.h"
#include "../PbmCommon/FileParser.h"

ServerWindow::ServerWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ServerWindow)
{
    m_ui->setupUi(this);
        setWindowTitle(tr("Overlord Server"));
        QPixmap px;
        px.load(QString::fromUtf8
                 (":/new/prefix1/resources/email-server-128x128.png"));
        setWindowIcon(px);
        lcdNum = new QLCDNumber();
    lcdNum->setSegmentStyle(QLCDNumber::Filled);
     m_ui->statusbar->addPermanentWidget(lcdNum,0);

     lockLabel = new QLabel(this);
        QPixmap pix;
        pix.load(QString::fromUtf8 (":/new/prefix1/icons/Key.png"));
    lockLabel->setPixmap(pix.scaled(24,24));
      m_ui->statusbar->addWidget(lockLabel,0);
     lockLabel->hide();

    logFile.open("server.log",fstream::app);
    if(!logFile.is_open())
    {
       m_ui->textEdit->append("Failed to open log file");
    }
    m_ui->textEdit->append(QString(QTime::currentTime ().toString() + " Running version ") + SERVER_VERSION + "\n");
    logFile<<"Running version " <<SERVER_VERSION<<endl;
    if(!configure())
    {
        m_ui->textEdit->append("Invalid configuration");
    }
    else
    {
        PbmTcpServer * pbmTcpServer = new PbmTcpServer(port_, &configuration);
        connect(pbmTcpServer, SIGNAL(signalPrintMessage(QString)),
                this, SLOT(slotPrintMessage(QString)));
        connect(pbmTcpServer, SIGNAL(signalLogMessage(QString)),
                this, SLOT (writeLogMessage(QString)));
        pbmTcpServer->start();
        if(QFile::exists(configuration.lockFileName))
        {
           // lockLabel->hide();
        }
    }
}

ServerWindow::~ServerWindow()
{
    delete m_ui;
}
void ServerWindow::slotPrintMessage (QString message)
{
    m_ui->textEdit->append(QTime::currentTime ().toString() + " " + message);

}

void ServerWindow::writeLogMessage (QString message)
{
    logFile << (QTime::currentTime ().toString() + " "
                + message).toStdString().c_str()<<endl;
}


bool ServerWindow::configure()
{
    QString configFileName_ = "server.ini";
    QString userId;
    QString password;
    FileParser * parser = new FileParser(configFileName_.toAscii());
    bool isConfigured = false;
    configuration.turn = -1;
    configuration.gameName = "";
    configuration.playersPath = "";
    configuration.gameDir ="";
    configuration.reportsDir ="reports";
    configuration.ordersDir = "orders";
    configuration.lockFileName = "turn.lock";
    port_ = 0;
    if(parser->status >0)
    {
        do
        {
            parser->getLine();
            if (parser->matchKeyword("player"))
            {
                userId   = QString(parser->getWord().c_str());
                password = QString(parser->getWord().c_str());
                configuration.users[userId] = password;
                continue;
            }
            if (parser->matchKeyword("turn"))
            {
                configuration.turn = parser->getInteger();
                if((configuration.turn <0) || (configuration.turn > 65535))
                {
                    configuration.turn = -1;
                    delete parser;
                    return false;
                }
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
                configuration.gameName =QString(parser->getWord().c_str());
                continue;
            }
            if (parser->matchKeyword("path"))
            {
                configuration.playersPath =QString(parser->getWord().c_str());
                continue;
            }
            if (parser->matchKeyword("GameDir"))
            {
                configuration.gameDir =QString(parser->getWord().c_str());
                continue;
            }
            if (parser->matchKeyword("ReportsDir"))
            {
                configuration.reportsDir =QString(parser->getWord().c_str());
                continue;
            }
            if (parser->matchKeyword("OrdersDir"))
            {
                configuration.ordersDir =QString(parser->getWord().c_str());
                continue;
            }

        } while (! parser ->eof() );


        // Verify configuration
        if(configuration.gameName=="")
        {
          m_ui->textEdit->append("Game name is missed");
        }
        else if(configuration.turn == -1)
        {
            m_ui->textEdit->append("The turn number is missed");
        }
        else
        {
            isConfigured =true;
            lcdNum->display(configuration.turn);
        }
    }
    else
    {
        m_ui->textEdit->append(QString("Can't find file ") + configFileName_);
    }
    delete parser;

    return isConfigured;
}
void ServerWindow::runTurn()
{
    setLock();
}
void ServerWindow::sendTurn()
{
    // remove lock
    // advance turn
    configuration.turn++;
    lcdNum->display(configuration.turn);
    removeLock();
}
void ServerWindow::rewindTurn()
{
    setLock();
    configuration.turn--;
    // rewind data files
    lcdNum->display(configuration.turn);
    removeLock();
}
bool ServerWindow::setLock()
{
//     QFile file(configuration.lockFileName);
            ofstream lockFile (configuration.lockFileName.toAscii());
            lockFile.close();
            lockLabel->show();
            return true;
}
bool ServerWindow::removeLock()
{
    QDir dir(".");
    dir.remove(configuration.lockFileName.toAscii());
    lockLabel->hide();
    return true;
}





void ServerWindow::on_actionRun_Turn_triggered()
{
   setLock();
}

void ServerWindow::on_actionSend_Turn_triggered()
{
   sendTurn();
}

void ServerWindow::on_actionTurn_Back_triggered()
{
   rewindTurn();
}
