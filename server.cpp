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
     Usuarios_Tree = new UsersDB_Tree();
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
        QString OperationTag = operation.tagName();
        qDebug()<<"The root tag is"<< OperationTag;
        qDebug()<<"The operation code is:"<< operation.attribute("ID");

        QDomNode n = operation.firstChild();

        //Comienza a leer los campos del XML//
        //***********************************************************//
        QDomElement usernameElement = n.toElement();

        QString UserName = usernameElement.firstChild().toText().data();


        qDebug()<<"The username is: "<< UserName;

        //***********************************************************//

        QDomElement NameElement = n.nextSibling().toElement();

        QString Name = NameElement.firstChild().toText().data();


        qDebug()<<"The Name is: "<< Name;

        //***********************************************************//

        QDomElement AgeElement = n.nextSibling().nextSibling().toElement();

        QString Age = AgeElement.firstChild().toText().data();


        qDebug()<<"The Age is: "<< Age;

        //***********************************************************//

        QDomElement PasswordElement = n.nextSibling().nextSibling().nextSibling().toElement();

        QString PassWord = PasswordElement.firstChild().toText().data();


        qDebug()<<"The Password is: "<< PassWord;

        //***********************************************************//

        QDomElement FavGenElement = n.nextSibling().nextSibling().nextSibling().nextSibling().toElement();

        QString Genero = FavGenElement.firstChild().toText().data();


        qDebug()<<"The Genero is: "<< Genero;

        QString encontrado = Usuarios_Tree->buscarUsuario(Usuarios_Tree->root,UserName);
        qDebug()<<"Se encontro en el arbol: " << encontrado;
        //Ingresa el usuario en el arbol //
        Usuarios_Tree->insertarNodo(UserName,Name,Age,Genero,PassWord);
        //Imprime el arbol para verificar que se inserto //
        Usuarios_Tree->recorridoPreOrder(Usuarios_Tree->getRoot());

        qDebug()<<"Se terminó de recorrer";

        Send_Buscado_Answer(XML, encontrado);





        }

    void Server::Send_Buscado_Answer(QString XML,QString encontrado){

        QDomDocument doc;
        doc.setContent(XML);

        QDomElement operation = doc.documentElement();

        QDomNode n = operation.firstChild();

        //Lee el nombre de usuario que se busca XML//
        //***********************************************************//
        QDomElement usernameElement_Buscado = n.toElement();

        QString UserName_Buscado = usernameElement_Buscado.firstChild().toText().data();

        //***********************************************************//

        //Crea el documento que va a enviar //

        QDomDocument xml_respuesta_buscado;
        QDomElement root = xml_respuesta_buscado.createElement("OperationCode");
        root.setAttribute("ID","2");
        xml_respuesta_buscado.appendChild(root);

//        QDomAttr atributo = xml_respuesta_buscado.createAttribute("ID");


        QDomElement tag = xml_respuesta_buscado.createElement("Encontrado");
        root.appendChild(tag);

        QDomText t = xml_respuesta_buscado.createTextNode(encontrado);
        tag.appendChild(t);

        QString xml_to_print = xml_respuesta_buscado.toString();
        qDebug() << "(Servidor) La respuesta a buscado es:" << xml_to_print;

//        sendMessage(xml_respuesta_buscado.toString());

    }













