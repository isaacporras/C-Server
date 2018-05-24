#include <QCoreApplication>
#include "server.h"
#include <json_handler.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);


    Server* server = new Server();


    return a.exec();
}
