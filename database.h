#pragma once
#include <QtSQL>
class Database {
private:
    struct wordline {
        QString word;
        int id;
        bool learned;
    };
public:
    const QString tablePref = "T";
    QStringList getTables() const;
    using Word = struct wordline;
private:
    QSqlDatabase db;
public:
    Word selectDidntLearn(QString table);
    QString getWord(QString table);   
private:
    void insert(int id, QString word, bool learned, QString table);
    QString databaseName = "db.LearnWordsBase";
    QString filename = "data.txt";
    void checkLearned(int id, QString table);    
public:   
    void createTable(QString name);
    void stop(QString name);
    void selectAll();
    Database();
};

