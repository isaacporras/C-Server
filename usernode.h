#ifndef USERNODE_H
#define USERNODE_H


#include <iostream>
#include <string>
#include "QString"

using std::string;

class UserNode {
public:


    public:
        UserNode *left;
        UserNode *right;
        UserNode *padre;

        QString username;
        QString name;
        QString age;
        QString generofavorito;
        QString password;

    UserNode(QString UserName, QString Name,QString Age, QString GeneroFavorito, QString Password){

        username = UserName;
        name = Name;
        age = Age;
        generofavorito = GeneroFavorito;
        password = Password;


        left = 0;
        right = 0;
        padre = 0;

    }

    public:

        int getUserNameLength(){
            return this->username.length();
        }



};


#endif // USERNODE_H
