#ifndef BTREE_H
#define BTREE_H


#include<iostream>
using namespace std;
#include <QString>

#include "btreenode.h"

#include <iostream>

class BTree{
public:
    /**
     * elemento raíz
     */
    BTreeNode *root = nullptr;

    /**Agrega un elemento en el árbol
     *
     * @param val elemento por agregar
     */
    void insert(QString val);

    /**Elimina un elemento de la lista a partir de un nodo específico
     *
     * @param val elemento por eliminar
     * @param myNode elemento a partir del cual eliminar
     */
    void _delete(QString val,BTreeNode *myNode);

    /**Busca un elemento en el arbol
     *
     * @param elemento elemento buscado
     * @return nullptr en caso de no encontrarlo
     */
    QString search(QString elemento);
private:
    BTreeNode* createNode(QString val, BTreeNode *child);
    void addValToNode(QString val, int pos, BTreeNode *node, BTreeNode *child);
    void splitNode(QString val, QString pval, int pos, BTreeNode *node,BTreeNode *child, BTreeNode **newNode);
    int setValueInNode(QString val, QString pval,BTreeNode *node, BTreeNode **child);
    void copySuccessor(BTreeNode *myNode, int pos);
    void removeVal(BTreeNode *myNode, int pos);
    void doRightShift(BTreeNode *myNode, int pos);
    void doLeftShift(BTreeNode *myNode, int pos);
    void mergeNodes(BTreeNode *myNode, int pos);
    void adjustNode(BTreeNode *myNode, int pos);
    int delValFromNode(QString val,BTreeNode *myNode);
    QString searchAux(QString name ,BTreeNode *myNode);
};
#endif // BTREE_H
