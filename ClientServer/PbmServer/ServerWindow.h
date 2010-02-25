#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <fstream>
#include <QMap>
#include <QtGui/QMainWindow>
class QLCDNumber;
class QLabel;
struct ServerConfiguration{
    QMap<QString,QString> users;
    QString gameName;
    QString playersPath;
    QString gameDir;
    QString reportsDir;
    QString ordersDir;
    QString lockFileName;
    int turn;
};

namespace Ui {
    class ServerWindow;
}

class ServerWindow : public QMainWindow {
    Q_OBJECT
public:
    ServerWindow(QWidget *parent = 0);
    ~ServerWindow();
public slots:
    void slotPrintMessage (QString message);
    void writeLogMessage (QString message);

    void runTurn();
    void sendTurn();
    void rewindTurn();

protected:
    bool configure();
    bool setLock();
    bool removeLock();
    ServerConfiguration  configuration;
    int port_;
    std::ofstream logFile;
    Ui::ServerWindow *m_ui;
    QLCDNumber * lcdNum;
    QLabel * lockLabel;


private slots:
    void on_actionTurn_Back_triggered();
    void on_actionSend_Turn_triggered();
    void on_actionRun_Turn_triggered();
};

#endif // SERVERWINDOW_H
