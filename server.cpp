#include "server.h"

#include <iostream>


#include <QDomDocument>
#include <json_handler.h>

#include <QTest>
#include <string>
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
//            cout << "Client :\n" << line.toUtf8().constData() << endl;

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
                //Se esta intentando cargar una Playlist //
                cout<<"Se quiere ingresar una cancion"<<endl;
                addNewPlaylist(line);

            }
            else if (operation.attribute("ID") == "5"){
                //Peticion para saber que playlists existen//
                cout<<"Se quiere ingresar una cancion"<<endl;
                QDir dir(QDir::currentPath() + "/PlayLists");
                QStringList list = dir.entryList(QDir::AllEntries | QDir::Dirs);
                if(list.isEmpty()){
                    cout<<"finished"<<endl;
                    sendMessage("finished");
                    return;
                }
                else{
                int i = 1;
                while(i < list.size())

                {
                    QTest::qSleep (100);
                    cout<<"Playlist name: "<<list.at(i).toStdString()<<endl;

                    sendMessage(list.at(i));
                    i = i + 1;

                }
                QTest::qSleep (100);
                cout<<"finished"<<endl;
                sendMessage("finished");
                }
            }


            else if (operation.attribute("ID") == "13"){
                //Se esta intentando reproducir una cancion via streaming //
//                cout<<"Se quiere reproducir una cancion"<<endl;
                QDomNode n = operation.firstChild();
                QDomElement PlaylistElement = n.toElement();

                QString PlaylistName = PlaylistElement.firstChild().toText().data();

                QDomElement SongElement = n.nextSibling().toElement();
                QString SongName = SongElement.firstChild().toText().data();

                QDomElement ChunkElement = n.nextSibling().nextSibling().toElement();
                QString Chunk = ChunkElement.firstChild().toText().data();

                playSong(PlaylistName,SongName,Chunk);


            }
            else if (operation.attribute("ID") == "6"){
                // //
                cout<<"Se pidieron los datos de las canciones"<<endl;
                SendSongs_Names();

            }
            else if (operation.attribute("ID") == "9"){
                // //
                cout<<"Se pidio la metadata de una cancion"<<endl;
                QDomNode n = operation.firstChild();
                QDomElement SongNameElement = n.toElement();

                QString SongName = SongNameElement.firstChild().toText().data();
                sendMetadata(SongName);


            }
            else if (operation.attribute("ID") == "7"){
                //Se pidio cargar el arbol con usuarios"//
                cout<<"Se pidio cargar el arbol con los usuarios"<<endl;
                JSON_Handler UsersCharger;
                UsersCharger.ChargeUsersOnTree(Usuarios_Tree);
                cout<< "El recorrido en el arbol es desde Server.cpp es:" <<endl;
                Usuarios_Tree->recorridoPreOrder(Usuarios_Tree->root);
            }
            else if(operation.attribute("ID") == "8"){
                cout<<"Se pidio cargar la metadata en arboles con las canciones"<<endl;
                JSON_Handler SongsCharger;
                SongsCharger.ChargeSongsNameOnTree(SongsNameTree);

            }
            else if (operation.attribute("ID") == "10"){
                QDomNode n = operation.firstChild();
                //Se pidio el nombre de un usuario //
                QDomElement UserNameElement = n.toElement();

                QString UserName = UserNameElement.firstChild().toText().data();

                JSON_Handler handler;


                sendMessage(handler.getUsersName(UserName));


            }
        }
    }
    void Server::sendMetadata(QString songname){
        //Peticion para saber que playlists existen//
        cout<<"Busqueda a profundidad en los archivos"<<endl;



        QString file_contend;
        QFile file;
        file.setFileName(QDir::currentPath() + "/Metadata/" + songname.remove(songname.length()-4, songname.length())+ ".json");
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        file_contend = file.readAll();
        file.close();
        cout<< file_contend.toStdString()<<endl;
         QJsonDocument JsonDocument = QJsonDocument::fromJson(file_contend.toUtf8());
         QJsonObject sett2 = JsonDocument.object();


         QJsonValue NameValue = sett2.value(QString("Nombre"));
         cout<< NameValue.toString().toStdString()<<endl;
         sendMessage(NameValue.toString());
         QTest::qSleep (50);

         QJsonValue Generovalue = sett2.value(QString("Genero"));
         cout<< "Genero:"<<Generovalue.toString().toStdString()<<endl;
         sendMessage(Generovalue.toString());
         QTest::qSleep (50);

         QJsonValue Artistavalue = sett2.value(QString("Artista"));
         cout<<"Artista:"<< Artistavalue.toString().toStdString()<<endl;
         sendMessage(Artistavalue.toString());
         QTest::qSleep (50);

         QJsonValue Albumvalue = sett2.value(QString("Album"));
         cout<<"Album:"<< Albumvalue.toString().toStdString()<<endl;
         sendMessage(Albumvalue.toString());
         QTest::qSleep (50);

         QJsonValue YearValue = sett2.value(QString("Year"));
         cout<< "Year:"<<YearValue.toString().toStdString()<<endl;
         sendMessage(YearValue.toString());
         QTest::qSleep (100);

         QJsonValue LetraValue = sett2.value(QString("Letra"));
         cout<< "Letra:"<<LetraValue.toString().toStdString()<<endl;
         sendMessage(LetraValue.toString());
         QTest::qSleep (150);

       qDebug()<<"---------FINISHED-------";
       sendMessage("finished");
    }

    void Server::SendSongs_Names(){
        //Peticion para saber que playlists existen//
        cout<<"Busqueda a profundidad en los archivos"<<endl;

        QDir dir(QDir::currentPath() + "/PlayLists");

            qDebug() << "Scanning: " << dir.path();

            QStringList fileList = dir.entryList();
            for (int i=0; i < fileList.count(); i++)
            {
                if( fileList[i] != "." && fileList[i] != ".."){
                    QTest::qSleep (50);
                    qDebug()<<"---------Playlist Start-------";
                    sendMessage("PlayList Start");
                    QTest::qSleep (50);
                    qDebug() << "PlaylistFound: " << fileList[i];
                    sendMessage(fileList[i]);
                    QTest::qSleep (50);
                    QString newPath = QString("%1/%2").arg(dir.absolutePath()).arg(fileList.at(i));
                    QDir dirSongs(newPath);
                    QStringList SongsList = dirSongs.entryList();
                    for(int j =0; j < SongsList.count(); j ++){

                        if(SongsList[j] != "." && SongsList[j]!= ".."){
                            qDebug()<<"---------Song Start-------";
                            QTest::qSleep (50);
                            sendMessage(SongsList[j]);
                            QTest::qSleep (50);
                            qDebug()<<"Las canciones son:" <<SongsList[j];
                            qDebug()<<"---------Song Start-------";
                        }

                    }

                    QTest::qSleep (50);
                    qDebug()<<"---------Playlist End-------";
                    sendMessage("PlayList End");
                    QTest::qSleep (50);


                }
            }
            qDebug()<<"---------FINISHED-------";
            sendMessage("finished");


        }

    void Server::playSong(QString playlist,QString song ,QString counter1){

        ///
        /// Se necesita analizar el XML (line) y obtener un parámetro especial que indica el chunk cargado en el
        /// cliente. Éste le ayuda al servidor para saber cuantos chunks se debe saltar y cual seleccionar para,
        /// enviarlo al cliente.
        ///

        int counter = 0;
        if (counter1 != "0"){
            counter = counter1.toInt();
        }

        string messageRecived = song.toStdString();

        //cout << "Client :\n" << messageRecived << endl;

//        QString str1 = QString("FUNCIONA....");

        ///
        /// El path para la canción (fileName) debe referenciar a la carpeta preasignada para guardar los MP3.
        ///

        QString fileName = QDir::currentPath() + "/PlayLists/"+ playlist +"/"+song;

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){

            ///
            /// Hay que indicar un mensaje de error en caso de no poder acceder/abrir el archivo, ya que existe un
            /// thread en el servidor abierto esperando por una respuesta.
            ///

            return;
        }

        QByteArray array = file.readAll();

        QString information = array.mid(195500*counter,200000).toBase64().constData();
        file.close();

        ///
        /// El XML a enviar (information) debe contener el chunk de bytes para ser reproducido en el servidor.
        ///

        sendMessage(information);

        /*QString str = QString::fromUtf8(messageRecived.c_str());
        client->write(("I've have recieved the message:" + str ).toUtf8());*/

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

        QString Playlist = PlayListElement.firstChild().toText().data();

        cout <<"Current Path"<< QDir::currentPath().toStdString()<<endl;
        qDebug()<<"The playlist is: "<< Playlist;
        if(QDir("PlayLists").exists()){
            QDir dir(QDir::currentPath() + "/PlayLists");
            dir.mkdir(Playlist);

        }
        else{
            QDir().mkdir("PlayLists");
            QDir direc(QDir::currentPath() + "/PlayLists/");
            direc.mkdir(Playlist);

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


//        qDebug()<<"The Bytes of the song are is: "<< SongBytes;

        //***********************************************************//

        QDomElement NameElement = n.nextSibling().toElement();

        QString Name = NameElement.firstChild().toText().data();


        qDebug()<<"The Nombre is: "<< Name;

//        SongsNameTree->insert(Name);

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


        QDomElement PlaylistElement = n.nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().toElement();

        QString Playlist = PlaylistElement.firstChild().toText().data();


        qDebug()<<"The Playlist is: "<< Playlist;

        //***********************************************************//

        QDomElement LetraElement = n.nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().nextSibling().toElement();

        QString Letra = LetraElement.firstChild().toText().data();


        qDebug()<<"The Letra is: "<< Letra;

        //***********************************************************//


        generateMP3(SongBytes,Playlist,Name);

        JSON_Handler SongHandler;
        SongHandler.writeOnJSON_Songs(Name,Genero,Artista,Album,Year,Letra,Playlist);


    }


    void Server::generateMP3(QString data, QString carpeta,QString nombre){

            QString information = data;
                    QByteArray array = QByteArray::fromBase64(information.toLocal8Bit().trimmed());
                    cout<<"El path donde se guarda es:" <<(QDir::currentPath() +"/PlayLists/"+ carpeta +"/"+ nombre+".mp3").toStdString()<<endl;
                    QFile file(QDir::currentPath() +"/PlayLists/"+ carpeta +"/"+ nombre+".mp3");
                    file.open(QIODevice::WriteOnly);
                    file.write(array);
                    file.close();
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

        Usuarios_Tree->recorridoPreOrder(Usuarios_Tree->root);

        QString encontrado = Usuarios_Tree->buscarUsuario(Usuarios_Tree->root,UserName);

        cout<<"Encontrado: " << encontrado.toStdString() <<endl;
        QString contrasena_encontrada = "no se ha inicializado la busqueda";


        if(encontrado == "true"){
            contrasena_encontrada = Usuarios_Tree->buscarContrasena(Usuarios_Tree->root,contrasena , UserName);
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
        JSON_Handler json_writer;
        json_writer.writeOnJSON_User(UserName,Name,Age,PassWord,Genero);
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


















