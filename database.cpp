#include "database.h"

Database::Database(const QString dbname, const QString dataPath) :databaseName(dbname), filename(dataPath) {
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

void Database::createTable(const QString name) {
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
        for(int i = 0; i < 100; i++){
            QString word = file.readLine();
            word.remove(word.length() - 1, 1);
            insert(i, word, 0, name);
        }
    }
}

void Database::stop(const QString name) {
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


void Database::insert(const int id, const QString word, const int learned, const QString table) {
    QSqlQuery query;
    query.prepare("INSERT INTO " + table + " values (:id, :word, :learned)");
    query.bindValue(":id", id);
    query.bindValue(":word", word);
    query.bindValue(":learned", learned);
    if(!query.exec()) {
        qDebug() << "Error on insert " << query.lastError();
    }
}

QString Database::getWord(const QString table) {
    auto word = selectDidntLearn(table);
    updateLearnLevel(word.id, word.learned, table);
    return word.word;
}

QString Database::getLearnedWord(const QString table) {
    auto word = selectLearnedWord(table);
    updateLearnLevel(word.id, word.learned, table);
    minLearnedLevel = word.learned;
    return word.word;
}

bool Database::addWord(const QString word, const QString table) {
    QSqlQuery query;
    query.prepare("SELECT * FROM " + table + " WHERE word = ?");
    query.addBindValue(word);
    if(!query.exec()){
        qDebug() << __FUNCSIG__ << query.lastError();
    }
    if(query.next()){ return false; }
    
    query.clear();
    if(!query.exec("SELECT COUNT(*) FROM " + table)){
        qDebug() << __FUNCSIG__ << query.lastError();
    }
    query.next();
    insert(query.value(0).toInt() + 1, word, 0, table);
    return true;
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

Database::Word Database::selectDidntLearn(const QString table) {
    Word word;
    QSqlQuery query;
    if(!query.exec("SELECT * from " + table + " ORDER BY Learned LIMIT 1")) {
        qDebug() << "Error in func " << __FUNCSIG__;
        qDebug() << query.lastError();
    }
    else{
        query.next();
        word.id = query.value(0).toInt();
        word.word = query.value(1).toString();
        word.learned = query.value(2).toInt();
    }
    return word;
}

Database::Word Database::selectLearnedWord(const QString table) {
    Word word;
    QSqlQuery query;
    query.prepare("SELECT * FROM " + table + " WHERE Learned > ? ORDER BY Learned LIMIT 1");
    query.addBindValue(minLearnedLevel);
    if(!query.exec()){
        qDebug() << "Error in func " << __FUNCSIG__;
        qDebug() << query.lastError();
    }
    else {
        query.next();
        word.id = query.value(0).toInt();
        word.word = query.value(1).toString();
        word.learned = query.value(2).toInt();
    }
    return word;
}

void Database::updateLearnLevel(const int id, const int prewLevel, const QString table) {
    QSqlQuery query;
    query.prepare("update " + table + " set Learned = ? where Id = ?");
    query.addBindValue(prewLevel + 1);
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
            qDebug() << "Learned: " << query.value(2).toInt();
        }
    }
}
