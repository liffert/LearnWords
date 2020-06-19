#pragma once
#include <QtNetwork>
#include <QUrl>
#include <QJsonDocument>

class TelegramApi {
private:
    QNetworkAccessManager *manager;
    QByteArray getRequestBody(QString char_id, QString message);
    QUrl URL;
    QString getUpdate();
    
public:
    TelegramApi(std::string requestConfPath);
    bool pushMessage(QString message);
};

