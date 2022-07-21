#ifndef _SESSION_H_
#define _SESSION_H_

/*
 * The SessionHandler is responsible for handling the NetworkManager
 * at a higher level, either as a client or as a server. It has a main
 * loop in the run method to manage a single session. When it returns
 * this session is over.
 */
class SessionHandler {
public:
    SessionHandler();
    SessionHandler(NetworkManager *manager, ProtocolHandler *protocol);
    ~SessionHandler();
    virtual int run();
    int handle(std::string data);
protected:
    NetworkManager *m_manager;
    ProtocolHandler *m_protocol;
    std::string m_partial;
};

std::ostream &operator<<(std::ostream &os, SessionHandler &handler);

class ClientSessionHandler : public SessionHandler {
public:
    ClientSessionHandler();
    ClientSessionHandler(NetworkManager *manager, ProtocolHandler *protocol);
    ~ClientSessionHandler();
    int run();
};

std::ostream &operator<<(std::ostream &os, ClientSessionHandler &handler);

class ServerSessionHandler : public SessionHandler {
public:
    ServerSessionHandler();
    ServerSessionHandler(NetworkManager *manager, ProtocolHandler *protocol);
    ~ServerSessionHandler();
    int run();
};

std::ostream &operator<<(std::ostream &os, ServerSessionHandler &handler);

#endif
