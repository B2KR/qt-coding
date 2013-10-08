#include <QtGui>
#include <QtNetwork>
#include "Tester.h"
#include "Protocol.h"

QString Tester::IP = "84.120.186.212";
int Tester::PORT = 5555;
QString Tester::user = "";

Tester::Tester(const QString &nif) : ui(new Ui::Tester), mutex(QMutex::Recursive)
{
    setAttribute(Qt::WA_DeleteOnClose);

    user = nif;
    ui->setupUi(this);

	ui->pbSend->setEnabled(false);
	ui->pteMsg->setEnabled(false);
	ui->cbUsers->setEnabled(false);

    ui->pbConnect->setEnabled(true);
    ui->pbStop->setEnabled(false);
    ui->progressBar->hide();

    socket = new QTcpSocket(this);
    //socket->connectToHost(QHostAddress::LocalHost, PORT);

    ui->progressBar->hide();
    ui->progressBar->setSizePolicy(QSizePolicy::Preferred,
                               QSizePolicy::Ignored);

    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMsg()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(connectionClosedByServer()));

    connect(ui->pbConnect, SIGNAL(clicked()), this, SLOT(restoreConnection()));
    connect(ui->pbSend, SIGNAL(clicked()), this, SLOT(sendMsg()));
    connect(ui->pbStop, SIGNAL(clicked()), this, SLOT(closeSocket()));
    connect(ui->pbClose, SIGNAL(clicked()), this, SLOT(close()));

    addAction(ui->actionClose);
    //stablishConnection();
}

Tester::~Tester()
{
    socket->deleteLater();
}

void Tester::connected()
{
	ui->pbSend->setEnabled(true);
	ui->pteMsg->setEnabled(true);
	ui->cbUsers->setEnabled(true);

    QMutexLocker locker(&mutex);

    Protocol *p = new Protocol();
    p->setHeader(Protocol::CLIENT_INFO);
    p->setBody("Informatino about socket name");
    p->setOtherCommands("name",user);

    socket->write(p->getFullMessage().toLatin1());
    socket->waitForBytesWritten();

    ui->statusLabel->setText(tr("Connected to host."));
    ui->progressBar->hide();
    ui->pbConnect->setEnabled(false);
    ui->pbStop->setEnabled(true);
}

void Tester::sendMsg()
{
    QMutexLocker locker(&mutex);

    QString clientToSend = ui->cbUsers->currentText();
    Protocol p;

    if (clientToSend == "ALL")
    {
        p.setHeader(Protocol::MULTICAST);
        p.setBody(ui->pteMsg->toPlainText());
        p.setOtherCommands("sender",user);
    }
    else if (clientToSend != "")
    {
        p.setHeader(Protocol::MESSAGE);
        p.setBody(ui->pteMsg->toPlainText());
        p.setOtherCommands("receiver",clientToSend);
		p.setOtherCommands("sender",user);
    }

    socket->write(p.getFullMessage().toLatin1());
    socket->waitForBytesWritten();
}

void Tester::readMsg()
{
    QMutexLocker locker(&mutex);

    QString infoBuffer = ui->plainTextEdit->toPlainText();

    QString socketMsg = socket->readAll();
    Protocol p(socketMsg);

    QString line;
    if (p.getHeader() == Protocol::NEW_CLIENT)
    {
        line = ">> " + p.getCommandValue("user").toString() + " has enter to the chat.\n\n";
        ui->cbUsers->addItem(p.getCommandValue("user").toString());
    }
    else if (p.getHeader() == Protocol::CLIENT_EXIT)
    {
        line = ">> " + line + p.getCommandValue("user").toString() + " has left the chat.\n\n";
        ui->cbUsers->removeItem(ui->cbUsers->findText(p.getCommandValue("user").toString()));
    }
    else if (p.getHeader() == Protocol::CLIENTS_LIST)
    {
        QList<QVariant> clientsName = p.getOtherCommands().values();

        for (int i = 0; i < clientsName.size(); i++)
            ui->cbUsers->addItem(clientsName.at(i).toString());
    }
	else if (p.getHeader() == Protocol::MESSAGE)
	{
		line = "@" + p.getCommandValue("sender").toString() + " says: \n        " + p.getBody() + "\n\n";
	}
	else
        line = line + ">> " + socketMsg;

    ui->plainTextEdit->setPlainText(infoBuffer + line + "\n\n");
}

void Tester::closeSocket()
{
    socket->close();
    ui->statusLabel->setText(tr("Connection closed by client."));
    QString infoBuffer = ui->plainTextEdit->toPlainText();
    ui->plainTextEdit->setPlainText("Connection closed by client.\n");
	ui->cbUsers->clear();
	ui->cbUsers->addItem("");
	ui->cbUsers->addItem("ALL");
    connectionClosedByServer();
}

void Tester::connectionClosedByServer()
{
	ui->pbSend->setEnabled(false);
	ui->pteMsg->setEnabled(false);
	ui->cbUsers->setEnabled(false);

    ui->pbConnect->setEnabled(true);
    ui->pbStop->setEnabled(false);
    ui->progressBar->hide();

    ui->cbUsers->clear();
}

void Tester::restoreConnection()
{
    //connectToHost(IP, PORT);
    ui->statusLabel->setText(tr("Connecting to server..."));
    ui->progressBar->show();
    socket->connectToHost(QHostAddress::LocalHost, PORT);
    ui->plainTextEdit->setPlainText("Connecting to server...\n\n");
}
