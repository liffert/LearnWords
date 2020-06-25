#include <QCoreApplication>
#include "controller.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Controller controller("telegramkey.txt", "TranslatorApiSettings.csv");
    const QTime whenStart(10, 0, 0, 0);
    while(1){
        if(QTime::currentTime() >= whenStart){
            controller.startWorkingDay();
        }
    }
    
    return a.exec();
}

