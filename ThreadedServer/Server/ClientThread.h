#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include "Client.h"

class ClientThread : public QThread
{
    Q_OBJECT

    signals:
        void receivedMessage(const QString &msg);
        void clientClosed(int id);

    public:
        explicit ClientThread(int _id, QObject *parent = 0);
        void run();
        void sendMessage(const QString &msg);
        Client * getClient() const { return client; }
		int getThreadId() const { return socketId; }
        
    public slots:
        void readyRead();
        void disconnected();

    private:
        Client *client;
		bool quit;
		QStringList msgToWrite;
        int socketId;
        
};

#endif // CLIENTTHREAD_H
