#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <iostream>
#include <vector>

enum ConnType { TCPCONN, UDPCONN };

class NetworkConnection
{
public:
    // Constructor
    NetworkConnection();
    // Constructor with params.
    NetworkConnection(ConnType ctype, std::string address, uint16_t port);
    // Desctructor
    ~NetworkConnection();
    // Copy constructor
    NetworkConnection(NetworkConnection &source);
    // Assignment operator
    void operator=(NetworkConnection &source);
    // For a string rep of the object
    void Print(std::ostream *os);
    // Setters
    void SetAddress(std::string address);
    void SetPort(uint16_t port);
    void SetConnType(ConnType ctype);
    // Getters
    std::string GetAddress();
    uint16_t GetPort();
    ConnType GetConnType();

private:
    // The type of connection
    ConnType conntype;
    // Reusable Copy method
    void Copy(NetworkConnection &source);
    // Remote host
    std::string address;
    // Remote port
    uint16_t port;
};

std::ostream &operator<<(std::ostream &os, NetworkConnection &conn);

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
    ssize_t send(const std::string msg);
    // Read data from an open socket
    ssize_t recv(std::string &buffer);

private:
    // The list of current connections as a server
    std::vector<NetworkConnection*> server_connections;
    // The network connection as a client.
    NetworkConnection m_client_connection;
    // Reusable Copy method
    void Copy(NetworkManager &source);

protected:
    // The socket file descriptor
    int m_sockfd;
};

std::ostream &operator<<(std::ostream &os, NetworkManager &manager);

class TcpNetworkManager: public NetworkManager
{
public:
    TcpNetworkManager();
    ~TcpNetworkManager();
    // Connect to a remote host as a client.
    int connect_to(std::string host, std::string port);
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
