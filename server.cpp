#include "server.h"

#include <iostream>

#include <string>
#include <QDomDocument>


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



    void Server::acceptConnection()
    {
      client = nextPendingConnection();

      connect(client, SIGNAL(readyRead()), this, SLOT(startRead()));
      connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));

      qDebug() << "New client from:" << client->peerAddress().toString();
    }



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
            readXML(line);


        }
    }




    void Server::sendMessage(QString data){
        client->write(QString(data+"\n").toUtf8());
        client->waitForBytesWritten(1000);
    }



    void Server::disconnected()
    {
        qDebug() << "Client disconnected:" << client->peerAddress().toString();
    }

    void Server::readXML(QString XML){
        QDomDocument doc;
        doc.setContent(XML);

        QDomElement operation = doc.documentElement();

        QString startTag = operation.tagName();

        qDebug()<<"The ROOT tag is"<<startTag;

        QDomElement username = operation.firstChild().toElement();
        QString data = username.tagName();


        qDebug()<<"The FirstChild is"<<data;
        qDebug()<<"The operation code is:"<< operation.attribute("ID");

//        QDomElement usernameElement = operation.firstChild().toElement();

        QString UserName = username.firstChild().toText().data();
        qDebug()<<"The username is: "<< UserName;


        }
















