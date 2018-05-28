#include "json_handler.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QFile>
#include <QJsonArray>
#include <iostream>
using namespace std;
#include <QTextStream>
#include <QDomDocument>



JSON_Handler::JSON_Handler()
{

}
void JSON_Handler::ChargeSongsNameOnTree(BTree *tree){
    if(!QDir("Metadata").exists()){
        QDir direc(QDir::currentPath());
        direc.mkdir("Metadata");
        return;
    }
//    else if (tree->root != nullptr){
//        return;
//    }
    else{
        QDir dir(QDir::currentPath() + "/Metadata");

            qDebug() << "Scanning: " << dir.path();

            QStringList fileList = dir.entryList();
            for (int i=0; i < fileList.count(); i++)
            {
                if( fileList[i] != "." && fileList[i] != ".."){

                    qDebug() << "Song Found: " << fileList[i];

                    QString file_contend;
                    QFile file;
                    file.setFileName(QDir::currentPath() + "/Metadata/" + fileList[i]);
                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                    file_contend = file.readAll();
                    file.close();
                    cout<< file_contend.toStdString()<<endl;
                     QJsonDocument JsonDocument = QJsonDocument::fromJson(file_contend.toUtf8());
                     QJsonObject sett2 = JsonDocument.object();
                     QJsonValue NameValue = sett2.value(QString("Nombre"));
                     cout<< NameValue.toString().toStdString()<<endl;


//                     tree->insert(NameValue.toString());

                }

            }
            qDebug()<<"---------FINISHED-------";








    }
}
void JSON_Handler::ChargeUsersOnTree(UsersDB_Tree *tree){
    if(!QDir("Users").exists()){
        QDir direc(QDir::currentPath());
        direc.mkdir("Users");
        return;
    }
    else if (tree->root != nullptr){
        return;
    }
    else{
        QDir dir(QDir::currentPath() + "/Users");

            qDebug() << "Scanning: " << dir.path();

            QStringList fileList = dir.entryList();
            for (int i=0; i < fileList.count(); i++)
            {
                if( fileList[i] != "." && fileList[i] != ".."){

                    qDebug() << "User Found: " << fileList[i];

                    QString file_contend;
                    QFile file;
                    file.setFileName(QDir::currentPath() + "/Users/" + fileList[i]);
                    file.open(QIODevice::ReadOnly | QIODevice::Text);
                    file_contend = file.readAll();
                    file.close();
                    cout<< file_contend.toStdString()<<endl;
                     QJsonDocument JsonDocument = QJsonDocument::fromJson(file_contend.toUtf8());
                     QJsonObject sett2 = JsonDocument.object();
                     QJsonValue NameValue = sett2.value(QString("Name"));
                     cout<< NameValue.toString().toStdString()<<endl;

                     QJsonValue Usernamevalue = sett2.value(QString("Username"));
                     cout<< Usernamevalue.toString().toStdString()<<endl;

                     QJsonValue Edadvalue = sett2.value(QString("Age"));
                     cout<< Edadvalue.toString().toStdString()<<endl;



                     QJsonValue PassWordvalue = sett2.value(QString("Password"));
                     cout<< PassWordvalue.toString().toStdString()<<endl;

                     QJsonValue GeneroFavValue = sett2.value(QString("GeneroFavorito"));
                     cout<< GeneroFavValue.toString().toStdString()<<endl;

                     tree->insertarNodo(Usernamevalue.toString(),NameValue.toString(),Edadvalue.toString(),GeneroFavValue.toString(),
                                       PassWordvalue.toString());


                }
                tree->recorridoPreOrder(tree->root);
            }
            qDebug()<<"---------FINISHED-------";

    }

}
void JSON_Handler::writeOnJSON_Songs(QString Nombre, QString Genero,QString Artista,QString Album, QString Year,QString Lyrics,QString playlist){
    if(!QDir("Metadata").exists()){
        cout <<"ERROR" <<endl;
    }
    else{
        QDir direc(QDir::currentPath());
        direc.mkdir("Metadata");
    }
    QFile file(QDir::currentPath() + "/Metadata/" + Nombre + ".json");
    file.open(QIODevice::WriteOnly);
    QString jsonInicial;
    QJsonValue JsonInicial(jsonInicial);

   QJsonObject jsonInicial_object = JsonInicial.toObject();
   //...................................................///

   QJsonArray jsonArray;
   jsonArray.append(jsonInicial);

    QJsonObject newSong;
    newSong.insert("Nombre", QJsonValue::fromVariant(Nombre));
    newSong.insert("Genero", QJsonValue::fromVariant(Genero));
    newSong.insert("Artista", QJsonValue::fromVariant(Artista));
    newSong.insert("Album", QJsonValue::fromVariant(Album));
    newSong.insert("Year", QJsonValue::fromVariant(Year));
    newSong.insert("Letra", QJsonValue::fromVariant(Lyrics));
    newSong.insert("Playlist", QJsonValue::fromVariant(playlist));


    QJsonDocument documento = QJsonDocument(newSong);

    QTextStream outStream(&file);
    /* Write the line to the file */
    outStream << documento.toJson();
}
void JSON_Handler:: writeOnJSON_User(QString username, QString name,QString Age,QString password, QString Genero_Favorito){
    if(!QDir("Users").exists()){
        cout <<"ERROR" <<endl;
    }
    else{
        QDir direc(QDir::currentPath());
        direc.mkdir("Users");
    }
    QFile file(QDir::currentPath() + "/Users/" + username + ".json");
    file.open(QIODevice::WriteOnly);
    QString jsonInicial;
    QJsonValue JsonInicial(jsonInicial);

   QJsonObject jsonInicial_object = JsonInicial.toObject();
   //...................................................///

   QJsonArray jsonArray;
   jsonArray.append(jsonInicial);

    QJsonObject newUser;
    newUser.insert("Username", QJsonValue::fromVariant(username));
    newUser.insert("Name", QJsonValue::fromVariant(name));
    newUser.insert("Age", QJsonValue::fromVariant(Age));
    newUser.insert("Password", QJsonValue::fromVariant(password));
    newUser.insert("GeneroFavorito", QJsonValue::fromVariant(Genero_Favorito));


    QJsonDocument documento = QJsonDocument(newUser);

    QTextStream outStream(&file);
    /* Write the line to the file */
    outStream << documento.toJson();

}
