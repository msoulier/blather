#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.hpp"
#include "network.hpp"

/*
 * NetworkConnection
 */

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
    *os << "NetworkConnection: ";
    switch (this->conntype) {
        case TCPCONN:
            *os << "<TCP> ";
            break;
        case UDPCONN:
            *os << "<UDP> ";
            break;
        default:
            *os << "**Unknown** ";
    }
    *os << this->address << ":" << this->port;
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

/*
 * NetworkManager
 */

NetworkManager::NetworkManager() : m_sockfd(0)
{}

NetworkManager::~NetworkManager()
{}

/*
 * TcpNetworkManager
 */
TcpNetworkManager::TcpNetworkManager()
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpNetworkManager::~TcpNetworkManager()
{}

int TcpNetworkManager::connect_to(std::string host, std::string port)
{
    mlog.debug("connect to %s:%d", host.c_str(), port.c_str());

    struct addrinfo hints;
    struct addrinfo *result = NULL;
    struct addrinfo *rp = NULL;
    int s, sfd, rv;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // AF_UNSPEC for v4 or v6
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = IPPROTO_TCP;

    s = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
    if (s != 0) {
        mlog.error("getaddrinfo: %s", gai_strerror(s));
        rv = -1;
        goto CLEANUP;
    }

    mlog.debug("looping over lookup results");
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        mlog.debug("connection attempt");
        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) < 0) {
            mlog.error("connect: %s", strerror(errno));
            continue;
        }
        mlog.info("successful connection");
        break;
    }

    if (rp == NULL) {
        mlog.error("could not connect");
        rv = -1;
        goto CLEANUP;
    }

    rv = 1;

CLEANUP:
    if (result != NULL)
        freeaddrinfo(result);
    return rv;
}
