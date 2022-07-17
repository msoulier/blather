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
    int run();
    int handle(std::string data);
private:
    NetworkManager *m_manager;
    ProtocolHandler *m_protocol;
};

std::ostream &operator<<(std::ostream &os, SessionHandler &handler);

#endif
