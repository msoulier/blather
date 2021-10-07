#include <iostream>

#include "logger.hpp"
#include "network.hpp"

NetworkConnection::NetworkConnection(): conntype(TCPCONN),
                                        address(""),
                                        port(0)
{
}

NetworkConnection::NetworkConnection(ConnType ctype,
                                     std::string address,
                                     uint16_t port)
{
    SetConnType(ctype);
    SetAddress(address);
    SetPort(port);
}

NetworkConnection::~NetworkConnection()
{
}

NetworkConnection::NetworkConnection(NetworkConnection &source)
{
    Copy(source);
}

void NetworkConnection::operator=(NetworkConnection &source)
{
    Copy(source);
}

void NetworkConnection::Copy(NetworkConnection &source)
{
}

void NetworkConnection::Print(std::ostream *os)
{
    *os << "NetworkConnection: " << this->address << ":" << this->port;
}

std::ostream &operator<<(std::ostream &os, NetworkConnection &conn)
{
    conn.Print(&os);
    return os;
}

void NetworkConnection::SetAddress(std::string address)
{
    this->address = address;
}

void NetworkConnection::SetPort(uint16_t port)
{
    this->port = port;
}

void NetworkConnection::SetConnType(ConnType ctype)
{
    this->conntype = ctype;
}

std::string NetworkConnection::GetAddress()
{
    return this->address;
}

uint16_t NetworkConnection::GetPort()
{
    return this->port;
}

ConnType NetworkConnection::GetConnType()
{
    return this->conntype;
}
