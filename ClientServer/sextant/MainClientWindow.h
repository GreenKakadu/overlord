#ifndef MAINCLIENTWINDOW_H
#define MAINCLIENTWINDOW_H

/****************************************************************************
                         MainClientWindow.h
                    Provides interface functionality
                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <fstream>
#include <QtGui/QMainWindow>
#include <QTcpSocket>
#include "ui_ConfigWin.h"
#include "ui_askReportDialog.h"
#include "ConnectionController.h"
#include "../PbmCommon/Led.h"

class QFileDialog;
class QLabel;
class QTimer;
class QLCDNumber;
class ViewManager;
class GameFacade;
class ContextManager;

namespace Ui
{
    class MainClientWindow;
}

class LocationEntity;
class FactionEntity;
class TokenEntity;

struct GameViewConf {
    QString gameViewFileName;
    FactionEntity * currentFaction;
    TokenEntity * currentUnit;
    LocationEntity * currentLocation;
    bool isUsingOpenGL;
    bool isUsingAntialiasing;
    int zoomLevel;
};

class MainClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainClientWindow(QWidget *parent = 0);
    ~MainClientWindow();
    friend class ViewManager;
    GameViewConf gameViewConf;
public slots:
    void setReceiveButton(bool);
    void setTurnNum(int);
    void closeEvent(QCloseEvent * event);
signals:

    void showMapView();
    void showLocationView();
    void showUnitView();
    void showFactionView();
    void exitApplication();
    void closeView();
private slots:
    void turnNumOk_clicked();
    void turnNumCancel_clicked();
    void on_actionReceive_report_for_turn_triggered();
    void on_actionReceive_triggered();
    void on_actionSend_triggered();
    void on_actionConnect_triggered();
    void on_actionConfiguration_triggered();
    void on_actionOpen_triggered();
    void on_actionClose_triggered();
    void on_actionOpenGame_triggered();
    void on_actionImport_Report_triggered();
    void okButton_clicked();
    void fileNameChanged(QString );
    void gameChanged(QString );
    void reportImport(QString );
    void periodicCheck();
    void printMessage(QString msg);
    void writeLogMessage(QString msg);
    void writeStatusMessage(QString msg);
    void updateLeds(ConnectionState);
    void updateSendLed(QColor,bool);
    void updateReceiveLed(QColor,bool);
    void newReport(int turn);
    void sendOrdersTimeoutExpired();
    void on_actionMap_triggered();
    void on_actionLocation_triggered();
    void on_actionUnit_triggered();
    void on_actionFaction_triggered();
//======================================================
    void showMousePos(int x, int y);


private:
    bool configureClient();
    bool configureGame();
    QString getIpAddress(QString address);
    bool loadGame(QString name);
    void closeGame();
private:
    Ui::MainClientWindow *ui;
    QFileDialog *fileDialog;
    QFileDialog *fileDialogGame;
    QFileDialog *fileDialogImport;
    QLabel * statusText ;
    Led * conectionLed;
    Led * sendLed;
    Led * receiveLed;
    QLCDNumber * lcdNum;
    QWidget * configWin;
    Ui::Form * ui_configWin;
    Ui::Dialog * ui_askReport;
    QDialog * askReport;
    // Connection parameters:
    QString address_;
    QString ipAdr_;
    bool isIpAddress;
    int port_;
    // Game parameters:
    QString gameName_;
    QString userId_;
    QString password_;
    int currentTurn_;
    int someTurn;
    int lastTurnReceived;
    QString configFileName_; // File that contains configuration of the Sextant itself
    QString gameConfigFileName_;   // File that contains configuration of the current game
    QString orderFileName_;

    QTimer * timeoutTimer;
    QTimer * sendOrdersTimeout;
    ConnectionController * connection;
    std::ofstream logFile;
    ViewManager * viewManager_;
    ContextManager * contextManager_;
//    GameFacade * gameFacade;

};

#endif // MAINCLIENTWINDOW_H
