#include <iostream>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <stdlib.h>
#include <unistd.h>

#include "logger.hpp"
#include "network.hpp"

/*
 * NetworkManager
 */

NetworkManager::NetworkManager() :
    m_bind_port(0),
    m_mode(NetworkManagerMode::UNSET),
    m_latest_sessionid(0)
{}

NetworkManager::~NetworkManager()
{}

void NetworkManager::set_mode(NetworkManagerMode mode) {
    // Can only set mode from UNSET.
    if (m_mode == NetworkManagerMode::UNSET) {
        m_mode = mode;
    } else {
        throw std::runtime_error("mode already set");
    }
}

ssize_t NetworkManager::write(const std::string msg, SESSIONID sessionid) {
    assert( m_mode != NetworkManagerMode::UNSET );
    int fd;
    if (m_sessionmap.count(sessionid) == 1) {
        fd = m_sessionmap[sessionid];
    } else {
        mlog.error() << "write called with unknown sessionid " << sessionid << std::endl;
        return -1;
    }

    if (fd == 0) {
        mlog.warn() << "write called on a closed socket" << std::endl;
        return -1;
    }
    assert( msg.size() <= MAX_BUFFER );
    int bytes = ::write(fd, msg.c_str(), msg.size());
    return bytes;
}

ssize_t NetworkManager::read(std::string &buffer, SESSIONID sessionid) {
    assert( m_mode != NetworkManagerMode::UNSET );
    int fd;
    if (m_sessionmap.count(sessionid) == 1) {
        fd = m_sessionmap[sessionid];
    } else {
        mlog.error() << "read called with unknown sessionid " << sessionid << std::endl;
        return -1;
    }
    char cbuffer[MAX_BUFFER];
    size_t size = MAX_BUFFER;
    int bytes_read = ::read(fd, cbuffer, size);
    if (bytes_read < 0) {
        perror("read");
        return bytes_read;
    }
    buffer = cbuffer;
    return bytes_read;
}

/*
 * TcpNetworkManager
 */
TcpNetworkManager::TcpNetworkManager()
{
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
}

TcpNetworkManager::~TcpNetworkManager()
{}

SESSIONID TcpNetworkManager::connect_to(std::string host, std::string port)
{
    set_mode(NetworkManagerMode::CLIENT);
    mlog.debug() << "connect to " << host << ":" << port << std::endl;

    struct addrinfo hints;
    struct addrinfo *result = NULL;
    struct addrinfo *rp = NULL;
    int s, rv;

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
        m_sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (m_sockfd == -1)
            continue;

        mlog.debug("connection attempt");
        if (connect(m_sockfd, rp->ai_addr, rp->ai_addrlen) < 0) {
            mlog.error("connect: %s", strerror(errno));
            continue;
        }
        mlog.info("successful connection");
        // FIXME: set the m_bind_port after connecting
        m_sessionmap[++m_latest_sessionid] = m_sockfd;
        break;
    }

    if (rp == NULL) {
        mlog.error("could not connect");
        rv = -1;
        goto CLEANUP;
    }

    rv = m_latest_sessionid;

CLEANUP:
    if (result != NULL)
        freeaddrinfo(result);
    return rv;
}

int TcpNetworkManager::listen(int port) {
    struct sockaddr_in servaddr;
    set_mode(NetworkManagerMode::SERVER);

    // socket create and verification
    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd == -1) {
        perror("socket creation failed...\n");
        return 0;
    }
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    // FIXME: need a bind parameter
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    // Binding newly created socket to given IP and verification
    if ((bind(m_sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) {
        perror("socket bind failed...\n");
        return 0;
    }

    // Now server is ready to listen and verification
    if ((::listen(m_sockfd, 5)) != 0) {
        perror("Listen failed...\n");
        return 0;
    }
    return 1;
}

SESSIONID TcpNetworkManager::accept() {
    socklen_t len;
    struct sockaddr_in client;
    int serverfd = 0;

    len = sizeof(client);

    // Accept the data packet from client and verification
    serverfd = ::accept(m_sockfd, (struct sockaddr*)&client, &len);
    if (serverfd < 0) {
        perror("server accept failed...\n");
        return 0;
    }
    m_sessionmap[++m_latest_sessionid] = serverfd;

    return m_latest_sessionid;
}
