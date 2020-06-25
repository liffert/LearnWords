#include "controller.h"

Controller::Controller(std::string telegramSettingsPath, std::string TranslatorSettingsPath) {
    translator = std::make_unique<TranslatorApi>(TranslatorSettingsPath);
    database = std::make_unique<Database>();
    telegram = std::make_unique<TelegramApi>(telegramSettingsPath);
    
}

void Controller::startWorkingDay() {
    const QTime whenEnd(21, 0, 0, 0);
    auto startWork = std::chrono::system_clock::now();
    while(1){
        updatesProcessing();
        if(QTime::currentTime() >= whenEnd){
            break;   
        }
        else if(std::chrono::system_clock::now() - startWork >= std::chrono::hours(interval)) {
            QStringList tables = database->getTables();
            for(const auto &iter : tables){
                QString word = database->getWord(iter);
                telegram->pushMessage(iter.mid(1, iter.length()), word + " it is " + translator->getReply(word));
            }
            startWork = std::chrono::system_clock::now();
        }
    }
}

void Controller::updatesProcessing() {
    auto messages = telegram->getMessages();
    if(!messages.empty()){
        QStringList toStop;
        auto tables = database->getTables();    
        for(auto iter = messages.begin(); iter != messages.end(); iter++){
            if(iter->second == "/start"){
                database->createTable(database->tablePref + iter->first);
                auto temp = std::remove(toStop.begin(), toStop.end(), iter->first);
                toStop.erase(temp, toStop.end());
            }
            else if(iter->second == "/stop"){
                toStop.push_back(iter->first);
            }
        }
        for(const auto &iter : toStop){
            database->stop(database->tablePref + iter);
        }
    }
}
