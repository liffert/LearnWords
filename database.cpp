#include "database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);    
    if(!db.open()){
        qDebug() << "Error on database opening";
    }
    else{
        selectAll();
    }
}

void Database::createTable(QString name) {
    QSqlQuery query;
    if(!query.exec("create table " + name + " (id integer primary key, word varchar(50), learned boolean)")){
        qDebug() << "Error on create " << query.lastError();;
    }

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Can`t open file";
    }
    else{
        for(int i = 0; i < 10; i++){
            QString word = file.readLine();
            word.remove(word.length() - 1, 1);
            insert(i, word, false, name);
        }
    }
}

void Database::insert(int id, QString word, bool learned, QString table) {
    QSqlQuery query;
    query.prepare("INSERT INTO " + table + " values (:id, :word, :learned)");
    query.bindValue(":id", id);
    query.bindValue(":word", word);
    query.bindValue(":learned", learned);
    if(!query.exec()) {
        qDebug() << "Error on insert " << query.lastError();
    }
}

QString Database::getWord(QString table) {
    auto word = selectDidntLearn(table);
    checkLearned(word.id, table);
    return word.word;
}

QStringList Database::getTables() const {
    return db.tables();
}

Database::Word Database::selectDidntLearn(QString table) {
    Word word;
    QSqlQuery query;
    if(!query.exec("SELECT * from " + table + " WHERE Learned = false LIMIT 1")) {
        qDebug() << "Error in func " << __FUNCSIG__;
        qDebug() << query.lastError();
    }
    else{
        query.next();
        word.id = query.value(0).toInt();
        word.word = query.value(1).toString();
        word.learned = query.value(2).toBool();
    }
    return word;
}

void Database::checkLearned(int id, QString table) {
    QSqlQuery query;
    query.prepare("update " + table + " set Learned = true where Id = ?");
    query.addBindValue(id);
    if(!query.exec()){
        qDebug() << query.lastError();
    }
}

void Database::selectAll() {
    QSqlQuery query;
    for(const auto &iter : db.tables()){
        qDebug() << iter;
        if(!query.exec("select * from " + iter)){
            qDebug() << "Error on select " << query.lastError();;
        }
        while (query.next()){
            qDebug() << "Id: " << query.value(0).toInt();
            qDebug() << "Word: " << query.value(1).toString();
            qDebug() << "Learned: " << query.value(2).toString();
        }
    }
}
