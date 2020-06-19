#include "database.h"
#include <QFile>
Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);

    if(db.open()){
        QSqlQuery query;
        if(!query.exec("select 0 from words")){
            create();
            qDebug() << "Created new database";
        }
        else{
            checkLearned(0);
            selectAll();
        }
    }
    else{
        qDebug() << "Error on database opening";
    }
}

void Database::create() {
    QSqlQuery query;
    if(!query.exec("create table words (id integer primary key, word varchar(50), translate varchar(50), learned boolean)")){
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
            insert(i, word, "", false);
        }
    }
    selectAll();
}

void Database::insert(int id, QString word, QString translate, bool learned) {
    QSqlQuery query;
    query.prepare("INSERT INTO words values (:id, :word, :translate, :learned)");
    query.bindValue(":id", id);
    query.bindValue(":word", word);
    query.bindValue(":translate", translate);
    query.bindValue(":learned", learned);
    if(!query.exec()) {
        qDebug() << "Error on insert";
    }
}

void Database::checkLearned(int id) {
    QSqlQuery query;
    query.prepare("update words set Learned = true where Id = ?");
    query.addBindValue(id);
    query.exec();
    qDebug() << query.lastError();
}

void Database::selectAll() {
    QSqlQuery query;
    if(!query.exec("select * from words")){
        qDebug() << "Error on select";
    }
    while (query.next()){
        qDebug() << "Id: " << query.value(0).toInt();
        qDebug() << "Word: " << query.value(1).toString();
        qDebug() << "Translate: " << query.value(2).toString();
        qDebug() << "Learned: " << query.value(3).toString();
    }
}
