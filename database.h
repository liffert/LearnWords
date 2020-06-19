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
    using Word = struct wordline;
private:
    QSqlDatabase db;
public:
    Word selectDidntLearn();
private:
    void insert(int id, QString word, bool learned);
    QString databaseName = "db.LearnWordsBase";
    QString filename = "data.txt";
    
public:
    QString getWord();   
private:
    void checkLearned(int id);
    void create();

    
public:
    void selectAll();
    Database();
};

