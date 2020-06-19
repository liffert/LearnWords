#pragma once
#include <QtNetwork>
#include <QUrl>
#include <QtDebug>
#include <QEventLoop>
#include <QObject>
#include <QMap>

class Network {
public:
    Network();

    QString getReply(QString word);

private:
    QNetworkAccessManager *manager;
    QByteArray getRequestBody(QString word);
    QMap<QByteArray, QByteArray> requestSettings;
    QUrl URL;
    QString getTranslateFromJson(QJsonDocument reply);
};
