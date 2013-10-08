#include <QDateTime>
#include "ClientThread.h"
#include "version.h"

ClientThread::ClientThread(int _id, QObject *parent) : QThread(parent)
{
    socketId = _id;
	quit = false;
}

void ClientThread::run()
{
    client = new Client(socketId);

	connect(client, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

    client->write("Welcome to the Server!");

	QLog_Trace("ClientThread", "New client from " + client->peerAddress().toString());

	while (!quit)
	{
		client->waitForReadyRead(1);
		for (int i = msgToWrite.size() - 1; i >= 0; i--)
		{
			client->write(msgToWrite.at(i).toLatin1());
			msgToWrite.removeAt(i);
		}
	}
}

void ClientThread::readyRead()
{
    emit receivedMessage(client->readAll());
}

void ClientThread::disconnected()
{
    emit clientClosed(socketId);

    client->deleteLater();

	quit = true;
}

void ClientThread::sendMessage(const QString &msg)
{
	QLog_Trace("ClientThread", "ClientThread::sendMessage()");
	msgToWrite.prepend(msg);
}
