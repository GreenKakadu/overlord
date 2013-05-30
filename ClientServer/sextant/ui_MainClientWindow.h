/********************************************************************************
** Form generated from reading UI file 'MainClientWindow.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINCLIENTWINDOW_H
#define UI_MAINCLIENTWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPlainTextEdit>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainClientWindow
{
public:
    QAction *actionOpen;
    QAction *actionQuit;
    QAction *actionConnect;
    QAction *actionConfiguration;
    QAction *actionSend;
    QAction *actionReceive;
    QAction *actionReceive_report_for_turn;
    QAction *actionMap;
    QAction *actionLocation;
    QAction *actionUnit;
    QAction *actionFaction;
    QAction *actionCombat;
    QAction *actionOpenGame;
    QAction *actionClose;
    QAction *actionSave;
    QAction *actionImport_Report;
    QWidget *centralWidget;
    QPlainTextEdit *logWindow;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuVeiw;
    QMenu *menuGame;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainClientWindow)
    {
        if (MainClientWindow->objectName().isEmpty())
            MainClientWindow->setObjectName(QString::fromUtf8("MainClientWindow"));
        MainClientWindow->resize(1086, 1086);
        actionOpen = new QAction(MainClientWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/new/prefix1/icons/Folder.png"), QSize(), QIcon::Normal, QIcon::On);
        actionOpen->setIcon(icon);
        actionQuit = new QAction(MainClientWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/new/prefix1/icons/Standby.png"), QSize(), QIcon::Normal, QIcon::On);
        actionQuit->setIcon(icon1);
        actionConnect = new QAction(MainClientWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        actionConnect->setCheckable(true);
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/new/prefix1/icons/Web.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon2.addFile(QString::fromUtf8(":/new/prefix1/icons/Cancel.png"), QSize(), QIcon::Normal, QIcon::On);
        actionConnect->setIcon(icon2);
        actionConfiguration = new QAction(MainClientWindow);
        actionConfiguration->setObjectName(QString::fromUtf8("actionConfiguration"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/new/prefix1/icons/Tools.png"), QSize(), QIcon::Normal, QIcon::On);
        actionConfiguration->setIcon(icon3);
        actionSend = new QAction(MainClientWindow);
        actionSend->setObjectName(QString::fromUtf8("actionSend"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/new/prefix1/icons/Mail.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionSend->setIcon(icon4);
        actionReceive = new QAction(MainClientWindow);
        actionReceive->setObjectName(QString::fromUtf8("actionReceive"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/new/prefix1/icons/Download.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReceive->setIcon(icon5);
        actionReceive_report_for_turn = new QAction(MainClientWindow);
        actionReceive_report_for_turn->setObjectName(QString::fromUtf8("actionReceive_report_for_turn"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/new/prefix1/icons/At.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReceive_report_for_turn->setIcon(icon6);
        actionMap = new QAction(MainClientWindow);
        actionMap->setObjectName(QString::fromUtf8("actionMap"));
        actionLocation = new QAction(MainClientWindow);
        actionLocation->setObjectName(QString::fromUtf8("actionLocation"));
        actionUnit = new QAction(MainClientWindow);
        actionUnit->setObjectName(QString::fromUtf8("actionUnit"));
        actionFaction = new QAction(MainClientWindow);
        actionFaction->setObjectName(QString::fromUtf8("actionFaction"));
        actionCombat = new QAction(MainClientWindow);
        actionCombat->setObjectName(QString::fromUtf8("actionCombat"));
        actionOpenGame = new QAction(MainClientWindow);
        actionOpenGame->setObjectName(QString::fromUtf8("actionOpenGame"));
        actionClose = new QAction(MainClientWindow);
        actionClose->setObjectName(QString::fromUtf8("actionClose"));
        actionSave = new QAction(MainClientWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        actionImport_Report = new QAction(MainClientWindow);
        actionImport_Report->setObjectName(QString::fromUtf8("actionImport_Report"));
        centralWidget = new QWidget(MainClientWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        logWindow = new QPlainTextEdit(centralWidget);
        logWindow->setObjectName(QString::fromUtf8("logWindow"));
        logWindow->setGeometry(QRect(13, 3, 571, 311));
        logWindow->setReadOnly(true);
        MainClientWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainClientWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1086, 24));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuVeiw = new QMenu(menuBar);
        menuVeiw->setObjectName(QString::fromUtf8("menuVeiw"));
        menuGame = new QMenu(menuBar);
        menuGame->setObjectName(QString::fromUtf8("menuGame"));
        MainClientWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainClientWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        mainToolBar->setEnabled(true);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(4);
        sizePolicy.setVerticalStretch(4);
        sizePolicy.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy);
        mainToolBar->setMovable(false);
        mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
        mainToolBar->setIconSize(QSize(48, 48));
        mainToolBar->setFloatable(false);
        MainClientWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainClientWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainClientWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuVeiw->menuAction());
        menuBar->addAction(menuGame->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionConnect);
        menuFile->addAction(actionSend);
        menuFile->addAction(actionReceive);
        menuFile->addAction(actionReceive_report_for_turn);
        menuFile->addSeparator();
        menuFile->addAction(actionConfiguration);
        menuFile->addAction(actionQuit);
        menuFile->addAction(actionImport_Report);
        menuVeiw->addAction(actionMap);
        menuVeiw->addAction(actionLocation);
        menuVeiw->addAction(actionUnit);
        menuVeiw->addAction(actionFaction);
        menuVeiw->addAction(actionCombat);
        menuGame->addAction(actionOpenGame);
        menuGame->addAction(actionClose);
        menuGame->addAction(actionSave);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionConnect);
        mainToolBar->addAction(actionSend);
        mainToolBar->addAction(actionReceive);
        mainToolBar->addAction(actionReceive_report_for_turn);
        mainToolBar->addAction(actionConfiguration);
        mainToolBar->addAction(actionQuit);
        mainToolBar->addAction(actionMap);
        mainToolBar->addAction(actionLocation);
        mainToolBar->addAction(actionUnit);
        mainToolBar->addAction(actionFaction);
        mainToolBar->addAction(actionCombat);

        retranslateUi(MainClientWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainClientWindow, SLOT(close()));

        QMetaObject::connectSlotsByName(MainClientWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainClientWindow)
    {
        MainClientWindow->setWindowTitle(QApplication::translate("MainClientWindow", "MainClientWindow", 0, QApplication::UnicodeUTF8));
        actionOpen->setText(QApplication::translate("MainClientWindow", "Select Orders", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainClientWindow", "Select Orders File", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionQuit->setText(QApplication::translate("MainClientWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionConnect->setText(QApplication::translate("MainClientWindow", "Connect", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionConnect->setToolTip(QApplication::translate("MainClientWindow", "Connect to Server", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionConfiguration->setText(QApplication::translate("MainClientWindow", "Configuration", 0, QApplication::UnicodeUTF8));
        actionSend->setText(QApplication::translate("MainClientWindow", "Send", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSend->setToolTip(QApplication::translate("MainClientWindow", "Send orders to Server", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionReceive->setText(QApplication::translate("MainClientWindow", "Receive report from Server", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionReceive->setToolTip(QApplication::translate("MainClientWindow", "Receive report from Server", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionReceive_report_for_turn->setText(QApplication::translate("MainClientWindow", "Receive report for turn...", 0, QApplication::UnicodeUTF8));
        actionMap->setText(QApplication::translate("MainClientWindow", "Map", 0, QApplication::UnicodeUTF8));
        actionLocation->setText(QApplication::translate("MainClientWindow", "Location", 0, QApplication::UnicodeUTF8));
        actionUnit->setText(QApplication::translate("MainClientWindow", "Unit", 0, QApplication::UnicodeUTF8));
        actionFaction->setText(QApplication::translate("MainClientWindow", "Faction", 0, QApplication::UnicodeUTF8));
        actionCombat->setText(QApplication::translate("MainClientWindow", "Combat", 0, QApplication::UnicodeUTF8));
        actionOpenGame->setText(QApplication::translate("MainClientWindow", "Open new game", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionOpenGame->setToolTip(QApplication::translate("MainClientWindow", "Open Game", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionClose->setText(QApplication::translate("MainClientWindow", "Close Game", 0, QApplication::UnicodeUTF8));
        actionSave->setText(QApplication::translate("MainClientWindow", "Save Game", 0, QApplication::UnicodeUTF8));
        actionImport_Report->setText(QApplication::translate("MainClientWindow", "Import Report", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainClientWindow", "File", 0, QApplication::UnicodeUTF8));
        menuVeiw->setTitle(QApplication::translate("MainClientWindow", "Veiw", 0, QApplication::UnicodeUTF8));
        menuGame->setTitle(QApplication::translate("MainClientWindow", "Game", 0, QApplication::UnicodeUTF8));
        mainToolBar->setWindowTitle(QApplication::translate("MainClientWindow", "toolbar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainClientWindow: public Ui_MainClientWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINCLIENTWINDOW_H
