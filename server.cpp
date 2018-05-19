#include "server.h"

#include <iostream>

#include <string>

///
/// Clase Server
/// Encargada de la administración de las variables y del manejo de la memoria. Se comunica con la clase Client.
///

using namespace std;

    Server::Server(QObject* parent , quint16 port): QTcpServer(parent)
    {

        firstTime = true;
        if(!servidor){
            servidor = this;
        }
      connect(this, SIGNAL(newConnection()),this, SLOT(acceptConnection()));

      if (!this->listen(QHostAddress::Any, port)) {
              qDebug() << "ERROR";
          }
    }

    Server::~Server()
    {
      delete client;
      close();
    }

    ///
    ///  Método -> acceptConnection
    ///  Inicia la comunicación con algún cliente que la esté solicitando
    ///

    void Server::acceptConnection()
    {
      client = nextPendingConnection();

      connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
      connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));

      qDebug() << "New client from:" << client->peerAddress().toString();
    }

    ///
    ///  Método -> startRead
    ///  Lee la información en JSON enviada por el cliente.
    ///

    void Server::startRead()
    {
        while(client->canReadLine())
        {
            QString line = QString::fromUtf8(client->readAll().trimmed());
            if(line == "Close"){

            }
            cout << "Client :\n" << line.toUtf8().constData() << endl;
            QString message = "I have received the message:" + line.toUtf8();
            qDebug() << message;
            sendMessage(message);

        }
    }


    ///
    ///  Método -> sendMessage
    ///  Parámetro -> data
    ///  Envía la información data, que contiene los resultados de la administración de memoria
    ///

    void Server::sendMessage(QString data){
        client->write(QString(data+"\n").toUtf8());
        client->waitForBytesWritten(1000);
    }

    ///
    ///  Método -> disconnected
    ///  Notifica cuando el Cliente actual pierde la comunicación.
    ///

    void Server::disconnected()
    {
        qDebug() << "Client disconnected:" << client->peerAddress().toString();
    }
