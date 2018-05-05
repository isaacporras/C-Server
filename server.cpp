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


            QString string1 = line.toUtf8().constData();
            QXmlStreamReader reader(string1);
            QXmlStreamWriter writer(&string1);
//            // use QXmlStreamWriter class to generate the XML
//            writer.setAutoFormatting(true);
//            writer.writeStartDocument();

//            writer.writeStartElement("xml");
//            writer.writeStartElement("Data");
//            writer.writeAttribute("Message", line.toUtf8().constData());
//            writer.writeCharacters("Mi mensaje");
//            writer.writeEndElement();
//            writer.writeEndElement();
//            writer.writeEndDocument();
            cout << "Client :\n" << string1.toStdString() << endl;


            analize_XML(string1.toStdString());

            client->write(("I've have recieved the message:" + string1 ).toUtf8());
            client->waitForBytesWritten(3000);
        }

    }

    void Server::disconnected()
    {

        qDebug() << "Client disconnected:" << client->peerAddress().toString();

        client->write(QString("Server : I wish you didn't leave ):\n").toUtf8());
        client->waitForBytesWritten(3000);
    }

    void Server::analize_XML(string xml){

        QXmlStreamReader reader(QString::fromUtf8(xml.c_str()));

        if (reader.readNextStartElement()) {
            cout<<"Inside first if"<<endl;
                if (reader.name() == "Message"){
                    while(reader.readNextStartElement()){
                        if(reader.name() == "Cancion"){
                            QString s = reader.readElementText();
                            if(s == "Close"){
                                servidor->close();
                                servidor->acceptConnection();

                            }
                            cout<<"Info inside message:" << s.toStdString() <<endl;
                        }
                        else
                            reader.skipCurrentElement();
                    }
                }
                else
                    reader.raiseError(QObject::tr("Incorrect file"));
            }
   }
