#pragma once
#include <QtSQL>
class Database {
private:
    struct wordline {
        QString word;
        int id;
        int learned;
    };
public:
    const QString tablePref = "T";
    QStringList getTables() const;
    using Word = struct wordline;
private:
    QSqlDatabase db;
    Word selectDidntLearn(QString table);
    Word selectLearnedWord(QString table);
public:
    QString getWord(QString table);   
    QString getLearnedWord(QString table);
private:
    void insert(int id, QString word, int learned, QString table);
    QString databaseName = "db.LearnWordsBase";
    QString filename = "data.txt";
    void updateLearnLevel(int id, int prewLevel, QString table);
    int minLearnedLevel = 0;    
public:   
    void createTable(QString name);
    void stop(QString name);
    void selectAll();
    Database();
};

