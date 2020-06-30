#pragma once
#include <QtNetwork>
#include "translatorapi.h"
#include "database.h"
#include "telegramapi.h"
#include <iostream>
#include <QTime>
#include <chrono>
#include <memory>
#include <algorithm>

class Controller {
private:
   std::unique_ptr<Database> database = nullptr;
   std::unique_ptr<TranslatorApi> translator = nullptr;
   std::unique_ptr<TelegramApi> telegram = nullptr;
   static constexpr unsigned short int interval = 1;
   std::vector<std::pair<QString, QString>> updates;

public:
    Controller(std::string telegramSettingsPath, std::string TranslatorSettingsPath);
private:
    void updatesProcessing();
    void sendWords();
    void sendLearnedWords();
public:
    void startWorkingDay();
};

