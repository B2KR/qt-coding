#include <QtCore>
#include "Server.h"
#include "Client.h"
#include "version.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

Server::~Server()
{
}

void Server::incomingConnection(int socketDescriptor)
{
	ClientThread *client = new ClientThread(socketDescriptor);
	connect(client, SIGNAL(receivedMessage(QString)), this, SLOT(processMsg(QString)));
	connect(client, SIGNAL(clientClosed(int)), this, SLOT(threadFinished(int)), Qt::DirectConnection);
	connect(client, SIGNAL(finished()), client, SLOT(deleteLater()));

	clientList.append(client);

	client->start();
}

void Server::processMsg(const QString &msg)
{
    ClientThread *clSender = (ClientThread*)sender();

    QLog_Trace("Server", "Received missage from client [" + clSender->getClient()->peerAddress().toString() + "]");

    Protocol protocol(msg);

    if (protocol.getHeader() == Protocol::CLIENT_INFO)
    {
        Client *client = clSender->getClient();
        client->setName(protocol.getOtherCommands().value("name").toString());

        //Notify the clients that new one is connected and then the Server sends the client list to the new client
        Protocol newUser;
        newUser.setHeader(Protocol::NEW_CLIENT);
        newUser.setBody("New user has enter!");
        newUser.setOtherCommands("user", protocol.getOtherCommands().value("name").toString());

        Protocol clientsList;
        clientsList.setHeader(Protocol::CLIENTS_LIST);
        clientsList.setBody("List of connected clients");

        for (int i = 0; i < clientList.size(); i++)
        {
            if (clientList.at(i)->getClient()->getId() != client->getId())
            {
                clientList.at(i)->sendMessage(newUser.getFullMessage().toLatin1());
                clientsList.setOtherCommands("user"+QVariant(clientList.at(i)->getClient()->getId()).toString(),clientList.at(i)->getClient()->getName());
            }
        }

        clSender->sendMessage(clientsList.getFullMessage().toLatin1());
        //client->waitForBytesWritten();
    }
    else if (protocol.getHeader() == Protocol::MULTICAST)
        multicastMsg(clSender->getClient(), protocol);
    else if (protocol.getHeader() == Protocol::MESSAGE)
        clientToClientMsg(protocol);
}

void Server::clientToClientMsg(const Protocol &p)
{
    for (int i = 0; i < clientList.size(); i++)
    {
        if (clientList.at(i)->getClient()->getName() == p.getCommandValue("receiver") and p.getBody() != "")
        {
            clientList.at(i)->sendMessage(p.getFullMessage().toLatin1());
            //clientList.at(i)->waitForBytesWritten();
            break;
        }
    }
}

void Server::threadFinished(int threadId)
{
	QString userExited;

	for (int i = 0; i < clientList.size(); i++)
	{
		if (clientList.at(i)->getThreadId() == threadId)
		{
			userExited = clientList.at(i)->getClient()->getName();
			clientList.removeAt(i);
		}
	}

	//Message to the other clients
	Protocol *newUser = new Protocol();
	newUser->setHeader(Protocol::CLIENT_EXIT);
	newUser->setBody("User has left!");
	newUser->setOtherCommands("user", userExited);
	
	for (int i = 0; i < clientList.size(); i++)
		clientList.at(i)->sendMessage(newUser->getFullMessage().toLatin1());

	delete newUser;

	QLog_Trace("Server", "Thread finished");
}

void Server::multicastMsg(Client *client, const Protocol &p)
{
	if (client != NULL)
	{
		Protocol *newMsg = new Protocol();
		newMsg->setHeader(Protocol::MESSAGE);
		newMsg->setBody(p.getBody());
		newMsg->setOtherCommands("sender", client->getName());

		QLog_Trace("Server", "Server message: " + p.getBody());

		for (int i = 0; i < clientList.size(); i++)
		{
			if (clientList.at(i)->getClient()->getId() != client->getId())
				clientList.at(i)->sendMessage(newMsg->getFullMessage());
			else
			{
				Protocol *pAux = new Protocol();
				pAux->setHeader(Protocol::MESSAGE);
				pAux->setBody("Message sent!");
				pAux->setOtherCommands("sender","SERVER");
				clientList.at(i)->sendMessage(pAux->getFullMessage());
				delete pAux;
			}
		}
		delete newMsg;
	}
}

QList<Client*> Server::getClients() const
{
    QList<Client*> clients;

    for (int i = 0; i < clientList.size(); i++)
        clients.append(clientList.at(i)->getClient());

    return clients;
}

Client * Server::getClient(int _id) const
{
    for (int i = 0; i < clientList.size(); i++)
        if (clientList.at(i) and clientList.at(i)->getClient()->getId() == _id)
            return clientList.at(i)->getClient();

    return NULL;
}

Client * Server::getClientByName(const QString &_name) const
{
    for (int i = 0; i < clientList.size(); i++)
        if (clientList.at(i) and clientList.at(i)->getClient()->getName() == _name)
                return clientList.at(i)->getClient();

    return NULL;
}

void Server::writeToClients(const QString &_msg)
{
	for (int i = 0; i < clientList.size(); i++)
		clientList.at(i)->sendMessage(_msg.toLatin1());
}
