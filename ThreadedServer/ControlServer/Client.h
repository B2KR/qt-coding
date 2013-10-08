#ifndef Client_H
#define Client_H

#include <QTcpSocket>
#include <QTimer>
#include <QMap>
#include <QSet>
#include <QStringList>
#include <QHostAddress>

class Client : public QTcpSocket
{
    Q_OBJECT

    public:
        Client(int _id = -1, const QString &_name = "", QObject *parent = 0);
        Client & operator = (const Client &c);

        void setName(const QString &_name) { m_name = _name; }
        QString getName() const { return m_name; }
        void setId(int _id) { setSocketDescriptor(_id); }
        int getId() const { return socketDescriptor(); }
        QString getIpAddress() const { return peerAddress().toString(); }

    private:
        QString m_name;
};

#endif // Client_H
