#include "network.h"
#include <vector>

Network::Network() {
    manager = new QNetworkAccessManager();
    requestSettings =  {{"Ocp-Apim-Subscription-Key", "put your key here"},
                              {"Ocp-Apim-Subscription-Region", "westeurope"},
                              {"Content-Type", "application/json"}};
    URL.setUrl("https://learnwords.cognitiveservices.azure.com/translator/text/v3.0/translate?api-version=3.0&to=ru");
}

QString Network::getReply(QString word) {
    QNetworkRequest request(URL);

    for(auto iterJson = requestSettings.begin(); iter != requestSettings.end(); iter++){
        request.setRawHeader(iter.key(), iter.value());
    }

    QNetworkReply *reply = manager->post(request, getRequestBody(word));
    while(!reply->isFinished()){
        qApp->processEvents();
    }

    return getTranslateFromJson(QJsonDocument::fromJson(reply->readAll()));
}

QByteArray Network::getRequestBody(QString word) {
    QJsonArray arr;
    QJsonObject json;
    QJsonDocument doc;

    json.insert("Text", word);
    arr.push_back(json);
    doc.setArray(arr);

    return QString::fromStdString(doc.toJson().toStdString()).toUtf8();
}


QString Network::getTranslateFromJson(QJsonDocument reply) {
    return reply.array().at(0).toObject()["translations"].toArray().at(0).toObject()["text"].toString();
}
