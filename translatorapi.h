#pragma once
#include <QtNetwork>
#include <QJsonDocument>
#include <QUrl>
#include <QtDebug>

class TranslatorApi {
private:
    QNetworkAccessManager* manager;
    QMap<QByteArray, QByteArray> requestSettings;
    std::pair<QByteArray, QByteArray> getCsvPair(const std::string line) const;
    QByteArray getRequestBody(const QString word) const;
    QString getTranslateFromJson(const QJsonDocument reply) const;
    QUrl URL;
    
    void setHeaderFromConfigFile(const std::string requestConfPath);
public:
    TranslatorApi(const std::string requestConfPath);

    QString getReply(const QString word) const;

};

