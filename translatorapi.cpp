#include "translatorapi.h"
#include <fstream>

TranslatorApi::TranslatorApi(std::string requestConfPath) {        
    setHeaderFromConfigFile(requestConfPath);
	manager = new QNetworkAccessManager();
	URL.setUrl("https://learnwords.cognitiveservices.azure.com/translator/text/v3.0/translate?api-version=3.0&to=ru");
}
void TranslatorApi::setHeaderFromConfigFile(std::string requestConfPath) {
    std::ifstream settingsFile(requestConfPath);
    if(!settingsFile.is_open()) {
        qDebug() << "Warning. Settings file doesn`t exist";
    }
    else {
        while(!settingsFile.eof()){
            std::string line;
            std::getline(settingsFile, line);
            auto keyValue = getCsvPair(line);
            requestSettings.insert(keyValue.first, keyValue.second);
        }
    }
}
std::pair<QByteArray, QByteArray> TranslatorApi::getCsvPair(std::string line) {
    std::string first;
    std::string second;
    bool flag = true;
    for(unsigned long i = 0; i < line.size(); i++){
        if(line.at(i) == ';'){
            flag = false;
            continue;
        }
        if(flag){ first.push_back(line.at(i)); }
        else{ second.push_back(line.at(i)); }
    }
    return std::make_pair(QByteArray::fromStdString(first), QByteArray::fromStdString(second));
}

QByteArray TranslatorApi::getRequestBody(QString word) {
	QJsonArray arr;
	QJsonObject json;
	QJsonDocument doc;

	json.insert("Text", word);
	arr.push_back(json);
	doc.setArray(arr);

	return doc.toJson();
}

QString TranslatorApi::getTranslateFromJson(QJsonDocument reply) {
    return reply.array().at(0).toObject()["translations"].toArray().at(0).toObject()["text"].toString();
}



QString TranslatorApi::getReply(QString word) {
	QNetworkRequest request(URL);

	for (auto iter = requestSettings.begin(); iter != requestSettings.end(); iter++) {
		request.setRawHeader(iter.key(), iter.value());
	}

	QNetworkReply* reply = manager->post(request, getRequestBody(word));
	while (!reply->isFinished()) {
		qApp->processEvents();
	}

	return getTranslateFromJson(QJsonDocument::fromJson(reply->readAll()));
}
