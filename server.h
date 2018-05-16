#ifndef SERVER_H
    #define SERVER_H
#include <iostream>
using namespace std;
    #include <QtNetwork>
    #include <QObject>
    #include <QtNetwork/QTcpServer>
    #include <QtNetwork/QTcpSocket>
    #include <QtCore>
    #include <QXmlStreamWriter>
    #include <QXmlStreamReader>
    #include <QDomNodeList>
    #include <QDomDocument>


class Server: public QTcpServer
{
    Q_OBJECT
    public:

              Server(QObject * parent = 0 , quint16 port = 1723);

              virtual  ~Server();
private slots:
              void startRead();
              void acceptConnection();

              void disconnected();
//              void analize_XML(string xml);
            public:

              QTcpSocket * client;
              Server *servidor;

        };

#endif // SERVER_H
