#pragma once
#include <QtNetwork>
#include <QUrl>
#include <QJsonDocument>
#include <vector>

class TelegramApi {
private:
    QNetworkAccessManager *manager;
public:
    std::vector<std::pair<QString, QString>> TelegramApi::getMessages() const;
private:
    QByteArray getRequestBody(QString chat_id, QString message);
    QUrl URL;
    QByteArray getUpdate() const;
    
public:
    TelegramApi(std::string requestConfPath);
    bool pushMessage(QString chat_id, QString message);
};

