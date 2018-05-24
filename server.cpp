#include "server.h"

#include <iostream>

#include <string>
#include <QDomDocument>
#include <json_handler.h>


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
            cout << "Client :\n" << line.toUtf8().constData() << endl;

            QDomDocument doc;
            doc.setContent(line);

            QDomElement operation = doc.documentElement();
            QString OperationTag = operation.tagName();

            qDebug()<<"The operation code is:"<< operation.attribute("ID");

            QString message = "I have received the message:" + line.toUtf8();
            qDebug() << message;

            if(operation.attribute("ID") == "1"){
                //Se quiere registrar un usuario
                readXML_to_Regist(line);
            }
            else if (operation.attribute("ID") == "2"){
                //El cliente mando a buscar al servidor(Se usa cuando se quiere loggear)//
                Search_User_Login(line);
            }
            else if (operation.attribute("ID") == "3"){
                //Se esta intentando cargar una cancion //
                cout<<"Se quiere ingresar una cancion"<<endl;
                saveSong(line);

            }
            else if (operation.attribute("ID") == "4"){
                //Se esta intentando cargar una cancion //
                cout<<"Se quiere ingresar una cancion"<<endl;
                addNewPlaylist(line);

            }

        }
    }
    /**Crea una carpeta con las canciones del playlist request //
     * @brief Server::addNewPlaylist
     */
    void Server::addNewPlaylist(QString XML){
        QDomDocument doc;
        doc.setContent(XML);

        QDomElement operation = doc.documentElement();
        QString OperationTag = operation.tagName();

        QDomNode n = operation.firstChild();

        //Lee el nombre del playlist del XML//
        //***********************************************************//
        QDomElement PlayListElement = n.toElement();

        QString Playlist =PlayListElement.firstChild().toText().data();

        cout <<"Current Path"<< QDir::currentPath().toStdString()<<endl;
        qDebug()<<"The playlist is: "<< Playlist;
        if(QDir("PlayLists").exists()){
            QDir dir(QDir::currentPath() + "/PlayLists");
            dir.mkdir(Playlist);
        }
        else{
            QDir().mkdir("PlayLists");
        }


        cout << QDir::currentPath().toStdString() <<",exist?"<<QDir(Playlist).exists()<<endl;
    }
    void Server::saveSong(QString xml){


        QDomDocument doc;
        doc.setContent(xml);

        QDomElement operation = doc.documentElement();
        QString OperationTag = operation.tagName();
        qDebug()<<"The root tag is"<< OperationTag;
        qDebug()<<"The operation code is:"<< operation.attribute("ID");

        QDomNode n = operation.firstChild();

        //Lee los bytes del XML//
        //***********************************************************//
        QDomElement SongBytesElement = n.toElement();

        QString SongBytes = SongBytesElement.firstChild().toText().data();


        qDebug()<<"The Bytes of the song are is: "<< SongBytes;

        //***********************************************************//

        QDomElement NameElement = n.nextSibling().toElement();

        QString Name = NameElement.firstChild().toText().data();


        qDebug()<<"The Nombre is: "<< Name;

        //***********************************************************//

        QDomElement GeneroElement = n.nextSibling().nextSibling().toElement();

        QString Genero = GeneroElement.firstChild().toText().data();


        qDebug()<<"The Genero is: "<< Genero;

        //***********************************************************//

        QDomElement ArtistaElement = n.nextSibling().nextSibling().nextSibling().toElement();

        QString Artista = ArtistaElement.firstChild().toText().data();


        qDebug()<<"The Artista is: "<< Artista;

        //***********************************************************//

        QDomElement AlbumElement = n.nextSibling().nextSibling().nextSibling().nextSibling().toElement();

        QString Album = AlbumElement.firstChild().toText().data();


        qDebug()<<"The Album is: "<< Album;

        //***********************************************************//

        QDomElement YearElement = n.nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().toElement();

        QString Year = YearElement.firstChild().toText().data();


        qDebug()<<"The Year is: "<< Year;

        //***********************************************************//

        QDomElement LetraElement = n.nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().toElement();

        QString Letra = LetraElement.firstChild().toText().data();


        qDebug()<<"The Letra is: "<< Letra;

        //***********************************************************//

        QDomElement PlaylistElement = n.nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().toElement();

        QString Playlist = PlaylistElement.firstChild().toText().data();


        qDebug()<<"The Playlist is: "<< Playlist;
    }





    void Server::sendMessage(QString data){

        client->write(QString(data+"\n").toUtf8());
        client->waitForBytesWritten(10000);
    }
    void Server::Search_User_Login(QString XML){

        QDomDocument doc;
        doc.setContent(XML);

        QDomElement operation = doc.documentElement();
        QString OperationTag = operation.tagName();


        QDomNode n = operation.firstChild();

        //Lee el usuario del XML//
        //***********************************************************//

        QDomElement usernameElement = n.toElement();

        QString UserName = usernameElement.firstChild().toText().data();


        qDebug()<<"The username is: "<< UserName;

        QDomElement password = n.nextSibling().toElement();

        QString contrasena = password.firstChild().toText().data();


        qDebug()<<"La contrasena digitada: "<< contrasena;

        QString encontrado = Usuarios_Tree->buscarUsuario(Usuarios_Tree->root,UserName);

        cout<<"Encontrado: " << encontrado.toStdString() <<endl;
        QString contrasena_encontrada = "no se ha inicializado la busqueda";


        if(encontrado == "true"){
            contrasena_encontrada = Usuarios_Tree->buscarContraseña(Usuarios_Tree->root,contrasena , UserName);
            cout<<"Contrasena encontrada: " << contrasena_encontrada.toStdString() <<endl;
            if(contrasena_encontrada == "true"){
                sendMessage("true");
            }
            else if(contrasena_encontrada == "false"){
                sendMessage("false");
            }
        }
        else{
            sendMessage("false");
        }
    }



    void Server::disconnected()
    {
        qDebug() << "Client disconnected:" << client->peerAddress().toString();
    }
    /**Se encarga de pasear el xml que contiene la informacion de registro de un usuario
     * @brief Server::readXML_to_Regist
     * @param XML
     */
    void Server::readXML_to_Regist(QString XML){

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
        if(encontrado == "false"){
        Usuarios_Tree->insertarNodo(UserName,Name,Age,Genero,PassWord);
//        JSON_Handler json_writer;
//        json_writer.writeOnJSON_User(UserName,Name,Age,PassWord,Genero);
        }

        //Imprime el arbol para verificar que se inserto //
        Usuarios_Tree->recorridoPreOrder(Usuarios_Tree->getRoot());

        qDebug()<<"Se terminó de recorrer";


        Send_Buscado_Answer(XML, encontrado);


        }
    /**Recibe un xml, le extrae el nombre de usuario y contrasena. Busca al usuario y retorna true si lo encontro ,false  caso contrario
     * @brief Server::Send_Buscado_Answer
     * @param XML
     * @param encontrado
     */

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


        qDebug() << "(Servidor) La respuesta a buscado es:" << xml_respuesta_buscado.toString();

        sendMessage(encontrado);

    }


















