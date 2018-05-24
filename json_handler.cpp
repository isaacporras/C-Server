#include "json_handler.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QFile>
#include <QJsonArray>
#include <iostream>
using namespace std;
#include <QTextStream>

JSON_Handler::JSON_Handler()
{

}
//void JSON_Handler:: writeOnJSON_User(QString username, QString name,QString Age,QString password, QString Genero_Favorito){
//    QFile file_before("/Users/IsaacPorras/Desktop/C-Server/Users.json");
//    file_before.open(QIODevice::ReadOnly);
//    QString jsonInicial;
//    QJsonValue JsonInicial(jsonInicial);

//   QJsonObject jsonInicial_object = JsonInicial.toObject();
//   //...................................................///

//   QJsonArray jsonArray;
//   jsonArray.append(jsonInicial);

//    QJsonObject newUser;
//    newUser.insert("Username", QJsonValue::fromVariant(username));
//    newUser.insert("Name", QJsonValue::fromVariant(name));
//    newUser.insert("Age", QJsonValue::fromVariant(Age));
//    newUser.insert("password", QJsonValue::fromVariant(password));
//    newUser.insert("GeneroFavorito", QJsonValue::fromVariant(Genero_Favorito));


//    QJsonDocument doc2(newUser);
//    QString newUserString(doc2.toJson(QJsonDocument::Compact));
//    newUserString.remove(0,1);

//     QString FinalJSON = jsonInicial + newUserString;
//     if(jsonInicial_object.isEmpty() || jsonInicial == ""){
//         FinalJSON = "{"+ jsonInicial + newUserString+"}";
//     }

//     cout<<FinalJSON.toStdString()<<endl;

//    //termina de escribir //
//     QJsonValue finalJsonValue(FinalJSON);

//    jsonArray.append(newUser)

//    QFile file_during;
//    file_during.setFileName("/Users/IsaacPorras/Desktop/C-Server/Users.json");
//    file_during.open(QIODevice::WriteOnly);
//    QJsonDocument documento = QJsonDocument(jsonArray);
//    /* Check it opened OK */
//    if(!file_during.isOpen()){
//        std::cout<< "El archivo ya estaba abierto"<<endl;
//    }

//    /* Point a QTextStream object at the file */
//    QTextStream outStream(&file_during);
//    /* Write the line to the file */
//    outStream << documento.toJson();



//}
