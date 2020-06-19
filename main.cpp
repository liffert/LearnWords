#include <QCoreApplication>
#include <QtNetwork>

#include "translatorapi.h"
#include "database.h"
#include <chrono>




int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    TranslatorApi translator("TranslatorApiSettings.csv");
    Database database;
    auto startWork = std::chrono::system_clock::now();
    unsigned int counter = 10;
    const unsigned int amount = 10;
    std::chrono::time_point<std::chrono::system_clock> whenstart(std::chrono::hours(19));
    
    while(1){
        if(counter == amount){
            if(std::chrono::system_clock::now() >= whenstart){
                counter = 1;
                startWork = std::chrono::system_clock::now();
                QString word = database.getWord();
                qDebug() << word << "\t" << translator.getReply(word);
            }
        }
        else{
            if(std::chrono::system_clock::now() - startWork >= std::chrono::minutes(1)){
                startWork = std::chrono::system_clock::now();
                counter++;
                QString word = database.getWord();
                qDebug() << word << "\t" << translator.getReply(word);
            }
        }
    }
     
    return a.exec();
}

