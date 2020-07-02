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
    QByteArray getRequestBody(const QString chat_id, const QString message) const;
    QUrl URL;
    QByteArray getUpdate() const;
    
public:
    TelegramApi(const std::string requestConfPath);
    bool pushMessage(const QString chat_id, const QString message) const;
};

