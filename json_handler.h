#ifndef JSON_HANDLER_H
#define JSON_HANDLER_H
#include <iostream>
using namespace std;
#include <QString>
#include <QDomDocument>
#include <QDir>
#include <QDebug>
#include <usersdb_tree.h>
#include <server.h>
#include <btree.h>

class JSON_Handler
{
public:
    JSON_Handler();
    static void writeOnJSON_User(QString username, QString name,QString Age,QString password, QString Genero_Favorito);
    static void ChargeUsersOnTree(UsersDB_Tree *tree);
    static void ChargeSongsNameOnTree(BTree *tree);
    void writeOnJSON_Songs(QString Nombre, QString Genero,QString Artista,QString Album, QString Year, QString Lyrics, QString Playlist);
    QString getUsersName(QString username);
    void writeJSON_Stars(QString songname, QString stars);

};

#endif // JSON_HANDLER_H
