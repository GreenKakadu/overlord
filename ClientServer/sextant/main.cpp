#include <QApplication>
// Overlord Engine definitions start
#include <iostream>
#include "GameConfig.h"
#include "DataManipulator.h"
#include "BasicCombatManager.h"
#include "MainClientWindow.h"

#ifndef VERSION
  #define VERSION "1.1.0"
#endif
string GameConfig::version = VERSION;
QApplication * pApp;
BasicCombatManager * combatManager = new BasicCombatManager();
// Overlord Engine definitions end
int main(int argc, char **argv)
{
    Q_INIT_RESOURCE(images);

    QApplication app(argc, argv);
    pApp = &app;
    MainClientWindow window;
    window.show();


    return app.exec();
}
