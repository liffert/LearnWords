#include <QCoreApplication>
#include "controller.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Controller controller("telegramkey.txt", "TranslatorApiSettings.csv");
    controller.startWorkingDay();
    
    return a.exec();
}

