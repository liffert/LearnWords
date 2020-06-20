#include <QCoreApplication>
#include <QtNetwork>

#include "translatorapi.h"
#include "database.h"
#include "telegramapi.h"
#include <iostream>
#include <QTime>
#include <chrono>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    TranslatorApi translator("TranslatorApiSettings.csv");
    Database database;
    TelegramApi telegram("telegramkey.txt");
    
    
    
    
    unsigned int counter = 10;
    const unsigned int amount = 10;
    
    const QTime whenStart(10, 0, 0, 0);
    const QTime whenEnd(21, 0, 0, 0);
    constexpr int interval = 2;
    auto startWork = std::chrono::system_clock::now();

    while(1){
       if(counter == amount){
           if(QTime::currentTime() >= whenStart && QTime::currentTime() < whenEnd){
                counter = 1;
                QStringList users = telegram.getUsers();
                QStringList tables = database.getTables();
                for(const auto &iter : users){
                    if(std::find(tables.begin(), tables.end(), database.tablePref + iter) == tables.end()){
                        database.createTable(database.tablePref + iter);
                    }
                    QString word = database.getWord(database.tablePref + iter);
                    telegram.pushMessage(iter, word + " it is " + translator.getReply(word));
                }
                startWork = std::chrono::system_clock::now();
            }
        }
        else if(QTime::currentTime() >= whenEnd){
            if(counter < 10){
                QStringList users = telegram.getUsers();
                QStringList tables = database.getTables();
                for(const auto &iter : users){
                    if(std::find(tables.begin(), tables.end(), database.tablePref + iter) == tables.end()){
                        database.createTable(database.tablePref + iter);
                    }
                }
            }
            counter = 10;
        }
        else {
            if(std::chrono::system_clock::now() - startWork >= std::chrono::minutes(interval)){
                counter++;
                QStringList users = telegram.getUsers();
                QStringList tables = database.getTables();
                for(const auto &iter : users){
                    if(std::find(tables.begin(), tables.end(), database.tablePref + iter) == tables.end()){
                        database.createTable(database.tablePref + iter);
                    }
                    QString word = database.getWord(database.tablePref + iter);
                    telegram.pushMessage(iter, word + " == " + translator.getReply(word));
                    if(counter == amount){
                       telegram.pushMessage(iter, "That`s all for today"); 
                    }
                }
                startWork = std::chrono::system_clock::now();
            }
        }
    }
    return a.exec();
}

