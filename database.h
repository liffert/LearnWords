#pragma once
#include <QtSql>
#include <QDebug>

class Database
{
public:
    Database();


private:
    QString databaseName = "db.LearnWordsBase";
    QSqlDatabase db;
    void create();
    void insert(int id, QString word, QString translate, bool learned);
    void checkLearned(int id);
    void selectAll();

    QString filename = ":data.txt";
};
