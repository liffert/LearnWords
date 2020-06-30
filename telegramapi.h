#pragma once
#include <QtNetwork>
#include <QUrl>
#include <QJsonDocument>
#include <vector>

class TelegramApi {
private:
    QNetworkAccessManager *manager;
public:
    struct messageStruct{
        QString user;
        QString updateId;
        QString message;
    };
    using Message = struct messageStruct;
    std::vector<Message> TelegramApi::getMessages() const;
private:
    QByteArray getRequestBody(QString chat_id, QString message);
    QUrl URL;
    QByteArray getUpdate() const;
    
public:
    TelegramApi(std::string requestConfPath);
    bool pushMessage(QString chat_id, QString message);
};

