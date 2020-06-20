#pragma once
#include <QtNetwork>
#include <QUrl>
#include <QJsonDocument>

class TelegramApi {
private:
    QNetworkAccessManager *manager;
    QByteArray getRequestBody(QString chat_id, QString message);
    QUrl URL;
    QByteArray getUpdate() const;
    
public:
    TelegramApi(std::string requestConfPath);
    QStringList getUsers() const;
    bool pushMessage(QString chat_id, QString message);
};

