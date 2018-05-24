#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <iostream>
#include <string>
#include "usersdb_tree.h"
using namespace std;


///
/// Clase Server
/// Encargada de la administración de las variables y del manejo de la memoria. Se comunica con la clase Client.
///

class Server: public QTcpServer
{

    Q_OBJECT
public:
    UsersDB_Tree *Usuarios_Tree;
    Server(QObject * parent = 0 , quint16 port = 1380);
    virtual  ~Server();
    void sendMessage(QString data);
    void readXML_to_Regist(QString XML);
    void Send_Buscado_Answer(QString xml,QString encontrado);
    void Search_User_Login(QString XML);
    void saveSong(QString xml);
    void addNewPlaylist(QString XML);

private slots:

    void acceptConnection();
    void startRead();
    void disconnected();

public:


    bool firstTime;



    QTcpSocket * client;
    Server *servidor;





};


#endif // SERVER_H
