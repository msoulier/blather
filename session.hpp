#ifndef _SESSION_H_
#define _SESSION_H_

#include <atomic>

#include "mnetwork.hpp"

extern std::atomic<bool> g_shutdown_asap;

/*
 * The SessionHandler is responsible for handling the NetworkManager
 * at a higher level, either as a client or as a server. It has a main
 * loop in the run method to manage a single session. When it returns
 * this session is over.
 */
class SessionHandler {
public:
    SessionHandler();
    SessionHandler(NetworkManager *manager, ProtocolHandler *protocol, SESSIONID sessionid);
    ~SessionHandler();
    // Disable the copy constructor and the assignment operator.
    SessionHandler(SessionHandler &source) = delete;
    SessionHandler& operator=(const SessionHandler& source) = delete;
    int say(std::string data);
    virtual int run();
    virtual int handle(BlatherMessage message);
protected:
    NetworkManager *m_manager;
    ProtocolHandler *m_protocol;
    std::string m_partial;
    SESSIONID m_sessionid;
};

std::ostream &operator<<(std::ostream &os, SessionHandler &handler);

class ClientSessionHandler : public SessionHandler {
public:
    ClientSessionHandler();
    ClientSessionHandler(NetworkManager *manager, ProtocolHandler *protocol, SESSIONID sessionid);
    ~ClientSessionHandler();
    // Disable the copy constructor and the assignment operator.
    ClientSessionHandler(ClientSessionHandler &source) = delete;
    ClientSessionHandler& operator=(const ClientSessionHandler& source) = delete;
    int run();
    int handle(BlatherMessage message);
};

std::ostream &operator<<(std::ostream &os, ClientSessionHandler &handler);

class ServerSessionHandler : public SessionHandler {
public:
    ServerSessionHandler();
    ServerSessionHandler(NetworkManager *manager, ProtocolHandler *protocol, SESSIONID sessionid);
    ~ServerSessionHandler();
    // Disable the copy constructor and the assignment operator.
    ServerSessionHandler(ServerSessionHandler &source) = delete;
    ServerSessionHandler& operator=(const ServerSessionHandler& source) = delete;
    int run();
    int handle(BlatherMessage message);
};

std::ostream &operator<<(std::ostream &os, ServerSessionHandler &handler);

#endif
