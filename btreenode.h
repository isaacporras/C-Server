#ifndef BTREENODE_H
#define BTREENODE_H

#include <QString>

#define MAX 4
#define MIN 2

struct BTreeNode {
    QString  val[MAX + 1];
    int count;
    BTreeNode *link[MAX + 1];
};

#endif // BTREENODE_H
