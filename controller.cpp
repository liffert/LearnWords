#include "controller.h"

Controller::Controller(const std::string telegramSettingsPath, const std::string TranslatorSettingsPath, const QString dataPath, const QString dbname) {
    translator = std::make_unique<TranslatorApi>(TranslatorSettingsPath);
    database = std::make_unique<Database>(dbname, dataPath);
    telegram = std::make_unique<TelegramApi>(telegramSettingsPath);
    
}

void Controller::startWorkingDay() {
    const QTime whenEnd(21, 0, 0, 0);
    const QTime whenStart(10, 0, 0, 0);
    
    QTime time;
    
    auto startWork = std::chrono::system_clock::now();
    bool start = false;
    bool sendLearned = true;
    
    while(1){
        updatesProcessing();
        if(!start){
            if(QTime::currentTime() >= whenStart && QTime::currentTime() < whenEnd){
                start = true;
                sendWords();
                startWork = std::chrono::system_clock::now();
            }
        }
        else{ 
            if(QTime::currentTime() >= whenEnd){
                start = false;
                break;   
            }
            else if(std::chrono::system_clock::now() - startWork >= std::chrono::hours(interval)) {
               sendWords();
               startWork = std::chrono::system_clock::now();
               sendLearned = true;
            }
            else if(std::chrono::system_clock::now() - startWork >= std::chrono::minutes(interval * 30) && sendLearned){
                sendLearnedWords();
                sendLearned = false;
            }
        }
    }
}

void Controller::updatesProcessing() {
    auto messages = telegram->getMessages();
    for(const auto &iter : updates) {
        auto iterator = std::remove_if(messages.begin(), messages.end(), [&iter](auto first){
            return (first.user == iter.first && first.updateId == iter.second);
        });
        messages.erase(iterator, messages.end());
    }
    if(!messages.empty()){
        auto tables = database->getTables();
        for(auto iter = messages.begin(); iter != messages.end(); iter++){
            if(iter->message == "/start"){
                database->createTable(database->tablePref + iter->user);
                updates.push_back(std::make_pair(iter->user, iter->updateId));
            }
            else if(iter->message == "/stop"){
                database->stop(database->tablePref + iter->user);
                updates.push_back(std::make_pair(iter->user, iter->updateId));
            }
            else{
                
            auto command = iter->message.split(" ");
                if(command.length() == 2){
                    if(command.at(0) == "/add"){
                        if(database->addWord(command.at(1), database->tablePref + iter->user)){
                            telegram->pushMessage(iter->user, "Succesfull add word " + command.at(1));
                        }
                        else{
                            telegram->pushMessage(iter->user, "Word " + command.at(1) + " is exist");
                        }
                    }
                    updates.push_back(std::make_pair(iter->user, iter->updateId));
                }
            }
        }
    }
}

void Controller::sendWords() const {
    QStringList tables = database->getTables();
    for(const auto &iter : tables){
        QString word = database->getWord(iter);
        telegram->pushMessage(iter.mid(1, iter.length()), word + " it is\n" + translator->getReply(word));
    }
}

void Controller::sendLearnedWords() const {
    QStringList tables = database->getTables();
    for(const auto &iter : tables) {
        QString word = database->getLearnedWord(iter);
        telegram->pushMessage(iter.mid(1, iter.length()), word + " it is\n" + translator->getReply(word));
    }
}
