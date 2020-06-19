#pragma once
#include <QtNetwork>
#include <QJsonDocument>
#include <QUrl>
#include <QtDebug>

class TranslatorApi {
private:
    QNetworkAccessManager* manager;
    QMap<QByteArray, QByteArray> requestSettings;
    std::pair<QByteArray, QByteArray> getCsvPair(std::string line);
    QByteArray getRequestBody(QString word);
    QString getTranslateFromJson(QJsonDocument reply);
    QUrl URL;
    
    void setHeaderFromConfigFile(std::string requestConfPath);
public:
    TranslatorApi(std::string requestConfPath);

    QString getReply(QString word);

};

