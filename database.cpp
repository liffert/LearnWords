#include "database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);    
    if(db.open()){
        QSqlQuery query;
        if(db.tables().empty()){
            create();
            qDebug() << "Created new database";
        }
    }
    else{
        qDebug() << "Error on database opening";
    }
}

void Database::create() {
    QSqlQuery query;
    if(!query.exec("create table words (id integer primary key, word varchar(50), learned boolean)")){
        qDebug() << "Error on create";
    }

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Can`t open file";
    }
    else{
        for(int i = 0; i < 10; i++){
            QString word = file.readLine();
            word.remove(word.length() - 1, 1);
            insert(i, word, false);
        }
    }
}

void Database::insert(int id, QString word, bool learned) {
    QSqlQuery query;
    query.prepare("INSERT INTO words values (:id, :word, :learned)");
    query.bindValue(":id", id);
    query.bindValue(":word", word);
    query.bindValue(":learned", learned);
    if(!query.exec()) {
        qDebug() << "Error on insert";
    }
}

QString Database::getWord() {
    auto word = selectDidntLearn();
    checkLearned(word.id);
    return word.word;
}

Database::Word Database::selectDidntLearn() {
    Word word;
    QSqlQuery query;
    if(!query.exec("SELECT * from words WHERE Learned = false LIMIT 1")) {
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

void Database::checkLearned(int id) {
    QSqlQuery query;
    query.prepare("update words set Learned = true where Id = ?");
    query.addBindValue(id);
    if(!query.exec()){
        qDebug() << query.lastError();
    }
}

void Database::selectAll() {
    QSqlQuery query;
    if(!query.exec("select * from words")){
        qDebug() << "Error on select";
    }
    while (query.next()){
        qDebug() << "Id: " << query.value(0).toInt();
        qDebug() << "Word: " << query.value(1).toString();
        qDebug() << "Learned: " << query.value(2).toString();
    }
}
