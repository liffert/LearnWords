#include "database.h"

Database::Database() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);    
    if(!db.open()){
        qDebug() << "Error on database opening";
    }
    else{
        auto tables = db.tables();
        if(std::find(tables.begin(), tables.end(), "Stopped") == tables.end()){
            QSqlQuery query;
            if(!query.exec("create table Stopped (id varchar(50) primary key)")){
                qDebug() << "Error on create stoppend table " << query.lastError();
            }
        }
    }
}

void Database::createTable(QString name) {
    QSqlQuery query;
    if(!query.exec("SELECT * FROM Stopped")){
        qDebug() << "Error on check in stopped" << query.lastError();
        return;
    }
    else{
        while(query.next()){
            if(query.value(0).toString() == name){
                query.prepare("DELETE FROM Stopped WHERE id = ?");
                query.addBindValue(name);
                if(!query.exec()){
                    qDebug() << "Error on delete" << query.lastError();
                }
            }
            return;
        }
        auto tables = db.tables();
        if(std::find(tables.begin(), tables.end(), name) != tables.end()){
            return;
        }
    }
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

void Database::stop(QString name) {
    QSqlQuery query;
    if(!query.exec("SELECT * FROM Stopped")){
        qDebug() << "Error on delete" << query.lastError();
    }
    else{
        while(query.next()){
            if(query.value(0).toString() == name){
                return;
            }
        }
        query.prepare("INSERT INTO Stopped values (:id)");
        query.bindValue(":id", name);
        if(!query.exec()){
            qDebug() << "Errro on insert in stopped " << query.lastError();
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
    QSqlQuery query;
    QStringList tables;
    for(const auto &iter : db.tables()){
        query.prepare("SELECT * FROM Stopped WHERE id = ?");
        query.addBindValue(iter);
        if(!query.exec()){
            qDebug () << "Error on select from stopped where " << query.lastError();
        }
        else{
            if(!query.next() && iter != "Stopped"){
                tables.push_back(iter);
            }
        }
    }
    return tables;
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
