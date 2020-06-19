#include "telegramapi.h"
#include <fstream>

TelegramApi::TelegramApi(std::string requestConfPath) {
    std::ifstream settingsFile(requestConfPath);
    if(!settingsFile.is_open()){
         qDebug() << "Warning. Settings file doesn`t exist";
    }
    else{
        std::string line;
        std::getline(settingsFile, line);
        URL.setUrl("https://api.telegram.org/bot" + QString::fromStdString(line) + "/");
    }
    manager = new QNetworkAccessManager();
    getUpdate();
}
QByteArray TelegramApi::getRequestBody(QString chat_id, QString message) {
    QJsonObject json;
    QJsonDocument doc;
    json.insert("chat_id", chat_id);
    json.insert("text", message);
    doc.setObject(json);
    
    return doc.toJson();
}

QString TelegramApi::getUpdate() {
    QNetworkRequest request(URL.url() + "getUpdates");
    QNetworkReply *reply = manager->post(request, "");
    
    while(!reply->isFinished()){
        qApp->processEvents();
    }
    qDebug() << reply->readAll();
    return QString();
}


bool TelegramApi::pushMessage(QString message) {
    QNetworkRequest request(URL.url() + "sendMessage");
    request.setRawHeader("Content-Type", "application/json");
    QNetworkReply *reply = manager->post(request, getRequestBody("128819689", message));
    
    while(reply->isFinished()){
        qApp->processEvents();
    }
    qDebug() << reply->readAll();
    return true;
}
