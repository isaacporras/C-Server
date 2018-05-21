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
        \
        void recorridoPreOrder(UserNode *nodo){
            if(nodo != 0){
                std::cout << "El recorrido del arbol es: " << nodo->username.toStdString() << std::endl;
                recorridoPreOrder(nodo->left);
                recorridoPreOrder(nodo->right);
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
