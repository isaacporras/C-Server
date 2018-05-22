#ifndef USERSDB_TREE_H
#define USERSDB_TREE_H


#include <iostream>
#include <string>
#include <usernode.h>
using std::string;
/**
 * @brief The BinaryTree class clase que maneja la estructura del arbol binario
 */
class UsersDB_Tree {

    public:
        UserNode *root;


    public:
        /**
             * @brief BinaryTree inicia el arbol binario
             */
            UsersDB_Tree(){
                root = 0;
            }

    public:
        UserNode* getRoot(){
            return root;
        }

    public:

        void insertarNodo(QString UserName, QString Name,QString Age, QString GeneroFavorito, QString Password){

            string posFinal;
            UserNode *padre = 0;
            UserNode *corredor = this->root;
            UserNode *temporal = new UserNode(UserName, Name, Age, GeneroFavorito, Password);


            if(root == 0){

                this->root = temporal;


                return;

            }


            else{

                while(corredor != nullptr){


                    if(corredor->username.length() >= UserName.length()){

                        if (corredor->left == nullptr){
                            temporal->padre = corredor;
                            padre = corredor;
                            posFinal = "izquierda";

                        }

                        corredor = corredor->left;



                    }
                    else if(corredor->getUserNameLength() < UserName.length()){

                        if(corredor->right == 0){
                            temporal->padre = corredor;
                            padre = corredor;
                            posFinal = "derecha";
                        }

                        corredor = corredor->right;




                    }
                }

                if(posFinal == "izquierda"){

                    padre->left = temporal;

                }
                if(posFinal =="derecha"){

                    padre->right = temporal;

                }



            }
        }
    public:

        void recorridoPreOrder(UserNode *nodo){
            if(nodo != 0){
                std::cout << "El recorrido del arbol es: " << nodo->username.toStdString() << std::endl;
                recorridoPreOrder(nodo->left);
                recorridoPreOrder(nodo->right);
            }
        }

public:

public:

    QString buscarUsuario(UserNode *nodo, QString username_buscado){
        int repeticiones = encontrar_repeticiones(nodo,username_buscado);
        if(repeticiones == 0){
            return "false";
        }
        else if(repeticiones!= 0){
            return "true";
        }
    }
    int encontrar_repeticiones(UserNode *nodo, QString username_buscado){
        if (nodo)
            {
                return encontrar_repeticiones(nodo->left, username_buscado) +
                       encontrar_repeticiones(nodo->right, username_buscado) +
                       (nodo->username == username_buscado ? 1 : 0);
            }

            return 0;
    }
    QString buscarContraseña(UserNode *nodo,QString user_name){
        if(nodo){
            if(nodo->username == user_name){
                return nodo->password;
            }
            buscarContraseña(nodo->left,user_name);
            buscarContraseña(nodo->right,user_name);
        }
    }


public:
        UserNode *recorrerIzquierda(UserNode* nodo) {
            if (nodo->left != nullptr) {
                return recorrerIzquierda( nodo->left);
            }
            return nodo;
        }

    };


#endif // USERSDB_TREE_H
