#ifndef Tester_H
#define Tester_H

#include <QDialog>
#include <QTcpSocket>
#include <QTimer>
#include <QMutex>
#include "ui_Tester.h"

namespace Ui
{
    class Tester;
}

class Tester : public QDialog
{
    Q_OBJECT

    signals:
        void updateModel();

    public:
        Tester(const QString &nif = "");
        ~Tester();
        static QString user;

    public slots:
        void setUserLogged(const QString &nif) { user = nif; }

    private slots:
        void connected();
        void readMsg();
        void connectionClosedByServer();
        void restoreConnection();
        void sendMsg();
        void closeSocket();

    private:
        Ui::Tester *ui;
        QMutex mutex;
        static QString IP;
        static int PORT;
        QTcpSocket *socket;
        //GadaSocket *socket;
};

#endif
