#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtCore>
#include <QDebug>
#include <QVariant>
#include <QStringList>
#include <QMap>

class Protocol
{
    public:
        enum MessageType { CLIENT_INFO = 0, MESSAGE, MULTICAST, XML, NEW_CLIENT, CLIENT_EXIT, CLIENTS_LIST };

        Protocol();
        Protocol(const QString &fullMessage);
        Protocol(const Protocol &p);
        ~Protocol();
        Protocol & operator = (const Protocol &p);
        friend QDebug operator << (QDebug d, const Protocol &p);

        MessageType getHeader() const { return header; }
        void setHeader(const MessageType &type) { header = type; }
        QString getBody() const { return body; }
        void setBody(const QString &message) { body = message; }
        QMap<QString,QVariant> getOtherCommands() const { return otherCommands; }
        QVariant getCommandValue(const QString &key) const { return otherCommands.value(key.toLower()); }
        void setOtherCommands(const QString &name, const QString &value) { otherCommands.insert(name,value); }
        void setOtherCommands(const QMap<QString,QVariant> &commands) { otherCommands = commands; }
        void setFullMessage(const QString &fullMessage);
        QString getFullMessage() const;

    private:
        MessageType header;
        QString body;
        QMap<QString,QVariant> otherCommands;
};

#endif // PROTOCOL_H
