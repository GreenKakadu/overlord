/****************************************************************************
                         main.cpp
    Entry point for Pbm Client Application
                         -------------------
     begin                : Mon Jul  21 19:24:42 IST 2009
     copyright            : (C) 2009 by Alex Dribin
     email                : Alex.Dribin@gmail.com
***************************************************************************/
#include <QtGui/QApplication>
#include "MainClientWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainClientWindow w;
    w.show();
    return a.exec();
}
