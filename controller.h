#pragma once
#include <QtNetwork>
#include "translatorapi.h"
#include "database.h"
#include "telegramapi.h"
#include <iostream>
#include <QTime>
#include <chrono>
#include <memory>

class Controller {
private:
   std::unique_ptr<Database> database = nullptr;
   std::unique_ptr<TranslatorApi> translator = nullptr;
   std::unique_ptr<TelegramApi> telegram = nullptr;
   static constexpr unsigned short int interval = 1;

public:
    Controller(std::string telegramSettingsPath, std::string TranslatorSettingsPath);
private:
    void updatesProcessing();
    void sendWords();
    void sendLearnedWords();
public:
    void startWorkingDay();
};

