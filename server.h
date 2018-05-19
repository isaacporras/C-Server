#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork>
#include <QObject>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>


///
/// Clase Server
/// Encargada de la administración de las variables y del manejo de la memoria. Se comunica con la clase Client.
///

class Server: public QTcpServer
{

    Q_OBJECT
public:

    Server(QObject * parent = 0 , quint16 port = 7373);
    virtual  ~Server();
    void sendMessage(QString data);

private slots:

    void acceptConnection();
    void startRead();
    void disconnected();

public:

    ///
    /// firstTime
    /// Indica cuando es la primera vez que se comunica el Cliente con el Servidor
    ///

    bool firstTime;

    ///
    /// client, servidor
    /// Punteros que referencian el Servidor y Cliente actual
    ///

    QTcpSocket * client;
    Server *servidor;

    ///
    /// vl
    /// Objeto que maneja la memoria dentro del servidor
    ///




};


#endif // SERVER_H
