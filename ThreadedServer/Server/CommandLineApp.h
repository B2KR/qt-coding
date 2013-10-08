#ifndef CommandLineApp_H
#define CommandLineApp_H

#include <QObject>
#include "Server.h"
#include <iostream>

using namespace std;

class CommandLineApp : public QObject
{
    Q_OBJECT

    signals:
        void quitApp();

    public:
        CommandLineApp(QObject *parent = 0);
		~CommandLineApp();

	public slots:
        void exec();

    private:
        Server *server;
		QThread *rootServerThread;
};

#endif
