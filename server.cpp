#include "server.h"
    #include <iostream>
    using namespace std;



    Server::Server(QObject* parent , quint16 port): QTcpServer(parent)
    {
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
            if (line == "Close"){
                servidor->close();
                servidor->acceptConnection();
                return;
            }


            QString string1;
            QXmlStreamReader reader(string1);
            QXmlStreamWriter writer(&string1);
            // use QXmlStreamWriter class to generate the XML
            writer.setAutoFormatting(true);
            writer.writeStartDocument();

            writer.writeStartElement("xml");
            writer.writeStartElement("Data");
            writer.writeAttribute("Message", line.toUtf8().constData());
            writer.writeCharacters("Mi mensaje");
            writer.writeEndElement();
            writer.writeEndElement();
            writer.writeEndDocument();
            cout << "Client :\n" << string1.toStdString() << endl;
            string str2 =  string1.toStdString();
            QString str = QString::fromUtf8(str2.c_str());
            client->write(("I've have recieved the message:" + str ).toUtf8());
            client->waitForBytesWritten(3000);
        }

    }

    void Server::disconnected()
    {

        qDebug() << "Client disconnected:" << client->peerAddress().toString();

        client->write(QString("Server : I wish you didn't leave ):\n").toUtf8());
        client->waitForBytesWritten(3000);
    }

