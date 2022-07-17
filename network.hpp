#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>

#define MAX_BUFFER 1024

enum NetworkManagerMode {
    UNSET=1,
    CLIENT,
    SERVER
};

/*
 * A manager for multiple sockets, either as a client or server
 * depending on how it was invoked.
 */
class NetworkManager
{
public:
    // Construtor
    NetworkManager();
    // Copy constructor
    NetworkManager(NetworkManager &source);
    // Destructor
    ~NetworkManager(void);
    // Assignment operator
    void operator=(NetworkManager &source);
    // A print method for sending data an iostream
    void Print(std::ostream *os);
    // Write data to an open socket
    ssize_t write(const std::string msg);
    // Read data from an open socket
    ssize_t read(std::string &buffer);

private:
    // Reusable Copy method
    void Copy(NetworkManager &source);

protected:
    // The socket file descriptor
    int m_sockfd;
    // The current server connection fd: FIXME: array
    int m_serverfd;
    // The bind port.
    int m_bind_port;
    // Client or server?
    NetworkManagerMode m_mode;

    // Setting the mode.
    void set_mode(NetworkManagerMode mode);
};

std::ostream &operator<<(std::ostream &os, NetworkManager &manager);

class TcpNetworkManager: public NetworkManager
{
public:
    TcpNetworkManager();
    ~TcpNetworkManager();
    // Connect to a remote host as a client.
    int connect_to(std::string host, std::string port);
    int listen(int port);
    int accept();
private:

};

std::ostream &operator<<(std::ostream &os, TcpNetworkManager &manager);

class UdpNetworkManager: public NetworkManager
{
public:
    UdpNetworkManager();
    ~UdpNetworkManager();

private:

};

std::ostream &operator<<(std::ostream &os, UdpNetworkManager &manager);

#endif
