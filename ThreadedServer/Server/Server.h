#ifndef Server_H
#define Server_H

#include <QStringList>
#include <QTcpServer>
#include <QThread>
#include "ClientThread.h"
#include "Protocol.h"

class Server : public QTcpServer
{
    Q_OBJECT

    public:
        Server(QObject *parent = 0);
        ~Server();

        Client * getClient(int _id) const;
        Client * getClientByName(const QString &_name) const;
        QList<Client*> getClients() const;
		void writeToClients(const QString &_msg);

    protected:
        void incomingConnection(int socketDescriptor);

    private:
        QStringList fortunes;
        QList<ClientThread*> clientList;
        void clientToClientMsg(const Protocol &p);

    private slots:
        void processMsg(const QString &msg);
        void threadFinished(int threadId);
        void multicastMsg(Client *client, const Protocol &p);
};

#endif
