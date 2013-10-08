#include "Client.h"

Client::Client(const int id, const QString &_name, QObject *parent) : QTcpSocket(parent)
{
    if (id != -1)
        setSocketDescriptor(id);
    if (_name != "")
        m_name = _name;
}

Client & Client::operator = (const Client &c)
{
    if (this != &c)
    {
        m_name = c.m_name;
        setSocketDescriptor(c.getId());
    }

    return *this;
}
