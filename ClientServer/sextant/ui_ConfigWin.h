/********************************************************************************
** Form generated from reading UI file 'ConfigWin.ui'
**
** Created
**      by: Qt User Interface Compiler version 4.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGWIN_H
#define UI_CONFIGWIN_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QLabel *ipAddressLabel;
    QLineEdit *ipAddress;
    QLabel *portLabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QLabel *gameNameLabel;
    QLabel *userlabel;
    QLabel *passLabel;
    QLineEdit *gameName;
    QLineEdit *userId;
    QLineEdit *password;
    QLineEdit *port;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QString::fromUtf8("Form"));
        Form->resize(452, 488);
        ipAddressLabel = new QLabel(Form);
        ipAddressLabel->setObjectName(QString::fromUtf8("ipAddressLabel"));
        ipAddressLabel->setGeometry(QRect(10, 30, 131, 16));
        ipAddress = new QLineEdit(Form);
        ipAddress->setObjectName(QString::fromUtf8("ipAddress"));
        ipAddress->setGeometry(QRect(152, 30, 101, 22));
        portLabel = new QLabel(Form);
        portLabel->setObjectName(QString::fromUtf8("portLabel"));
        portLabel->setGeometry(QRect(300, 35, 41, 16));
        okButton = new QPushButton(Form);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setGeometry(QRect(30, 250, 78, 26));
        cancelButton = new QPushButton(Form);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setGeometry(QRect(340, 250, 78, 26));
        gameNameLabel = new QLabel(Form);
        gameNameLabel->setObjectName(QString::fromUtf8("gameNameLabel"));
        gameNameLabel->setGeometry(QRect(30, 80, 55, 16));
        userlabel = new QLabel(Form);
        userlabel->setObjectName(QString::fromUtf8("userlabel"));
        userlabel->setGeometry(QRect(30, 120, 55, 16));
        passLabel = new QLabel(Form);
        passLabel->setObjectName(QString::fromUtf8("passLabel"));
        passLabel->setGeometry(QRect(30, 170, 71, 16));
        gameName = new QLineEdit(Form);
        gameName->setObjectName(QString::fromUtf8("gameName"));
        gameName->setGeometry(QRect(150, 80, 121, 22));
        userId = new QLineEdit(Form);
        userId->setObjectName(QString::fromUtf8("userId"));
        userId->setGeometry(QRect(150, 120, 121, 22));
        password = new QLineEdit(Form);
        password->setObjectName(QString::fromUtf8("password"));
        password->setGeometry(QRect(150, 160, 121, 22));
        port = new QLineEdit(Form);
        port->setObjectName(QString::fromUtf8("port"));
        port->setGeometry(QRect(340, 30, 51, 22));

        retranslateUi(Form);
        QObject::connect(cancelButton, SIGNAL(clicked()), Form, SLOT(close()));

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0, QApplication::UnicodeUTF8));
        ipAddressLabel->setText(QApplication::translate("Form", "IP Address of Server:", 0, QApplication::UnicodeUTF8));
        ipAddress->setInputMask(QString());
        portLabel->setText(QApplication::translate("Form", "port:", 0, QApplication::UnicodeUTF8));
        okButton->setText(QApplication::translate("Form", "OK", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("Form", "Cancel", 0, QApplication::UnicodeUTF8));
        gameNameLabel->setText(QApplication::translate("Form", "Game", 0, QApplication::UnicodeUTF8));
        userlabel->setText(QApplication::translate("Form", "faction IDl", 0, QApplication::UnicodeUTF8));
        passLabel->setText(QApplication::translate("Form", "Password", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGWIN_H
