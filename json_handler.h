#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H
#include <iostream>
using namespace std;
#include <QString>
#include <QDomDocument>
#include <QDir>
#include <QDebug>
#include <usersdb_tree.h>

class JSON_Handler
{
public:
    JSON_Handler();
    static void writeOnJSON_User(QString username, QString name,QString Age,QString password, QString Genero_Favorito);
    static void ChargeUsersOnTree(UsersDB_Tree *tree);
};

#endif // JSON_HANDLER_H
