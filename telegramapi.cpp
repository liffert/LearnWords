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
}

std::vector<TelegramApi::Message> TelegramApi::getMessages() const {
    QJsonDocument doc = QJsonDocument::fromJson(getUpdate());
    std::vector<Message> messages;
    for(int i = 0; i < doc["result"].toArray().size(); i++){
        QString id = QString::number(doc["result"][i]["message"]["chat"]["id"].toInt());
        QString text = doc["result"][i]["message"]["text"].toString();
        QString updateId = QString::number(doc["result"][i]["update_id"].toInt());
        messages.push_back({id, updateId, text});       
    }
    return messages;
}
QByteArray TelegramApi::getRequestBody(QString chat_id, QString message) {
    QJsonObject json;
    QJsonDocument doc;
    json.insert("chat_id", chat_id);
    json.insert("text", message);
    doc.setObject(json);
    
    return doc.toJson();
}

QByteArray TelegramApi::getUpdate() const {
    QNetworkRequest request(URL.url() + "getUpdates");
    QNetworkReply *reply = manager->post(request, "");
    
    while(!reply->isFinished()){
        qApp->processEvents();
    }
    return reply->readAll();
}


bool TelegramApi::pushMessage(QString chat_id, QString message) {
    QNetworkRequest request(URL.url() + "sendMessage");
    request.setRawHeader("Content-Type", "application/json");
    QNetworkReply *reply = manager->post(request, getRequestBody(chat_id, message));
    
    while(reply->isFinished()){
        qApp->processEvents();
    }
    return true;
}
