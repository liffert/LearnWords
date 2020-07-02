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
    Word selectDidntLearn(const QString table);
    Word selectLearnedWord(const QString table);
public:
    bool addWord(const QString word, const QString table);
    QString getWord(const QString table);   
    QString getLearnedWord(const QString table);
private:
    void insert(const int id, const QString word, const int learned, const QString table);
    QString databaseName;
    QString filename;
    void updateLearnLevel(const int id, const int prewLevel, const QString table);
public:   
    Database(const QString dbname, const QString dataPath);
    void createTable(const QString name);
    void stop(const QString name);
private:
    int minLearnedLevel = 0;    
public:
    void selectAll();
};

