#include "network.hpp"
#include "protocol.hpp"
#include "session.hpp"
#include "logger.hpp"

SessionHandler::SessionHandler() {}

SessionHandler::SessionHandler(NetworkManager *manager, ProtocolHandler *protocol) :
    m_manager(manager),
    m_protocol(protocol)
{}

SessionHandler::~SessionHandler() {}

int SessionHandler::run() {
    throw std::runtime_error("should not be called directly");
}

int SessionHandler::handle(std::string data) {
    throw std::runtime_error("should not be called directly");
    return 0;
}

ClientSessionHandler::ClientSessionHandler() {}

ClientSessionHandler::ClientSessionHandler(NetworkManager *manager, ProtocolHandler *protocol) :
    SessionHandler(manager, protocol)
{}

ClientSessionHandler::~ClientSessionHandler() {}

int ClientSessionHandler::run() {
    // Kickstart the session by testing the connection to the server.
    mlog.debug() << "in ClientSessionHandler::run" << std::endl;
    std::string msg("PING");
    mlog.debug() << "sending " << msg << std::endl;
    int bytes_sent = m_manager->write(m_protocol->compose(msg));
    mlog.debug() << "sent " << bytes_sent << " bytes" << std::endl;
    if (bytes_sent < 0) {
        perror("write");
        return 0;
    }

    std::string buffer;
    ssize_t bytes;
    // Loop until something ends the session.
    for(;;) {
        buffer.clear();
        // FIXME: must go into select on network and console fds
        bytes = m_manager->read(buffer);
        if (bytes == 0) {
            mlog.debug() << "read 0 bytes" << std::endl;
            return 1;
        } else if (bytes < 0) {
            mlog.error() << "read error" << std::endl;
            return 0;
        } else {
            m_partial.append(buffer);
            std::vector<std::string> messages = m_protocol->interpret(m_partial);
            for (auto msg : messages) {
                mlog.debug() << "found message " << msg << std::endl;
                handle(msg);
            }
        }
    }
}

int ClientSessionHandler::handle(std::string data) {
    mlog.info() << "ClientSessionHandler::handle: " << data << std::endl;
    if (data == "PONG") {
        return 1;
    } else {
        mlog.error() << "ClientSessionHandler::handle: unknown message '" << data << "'" << std::endl;
        return 0;
    }
}

ServerSessionHandler::ServerSessionHandler() {}

ServerSessionHandler::ServerSessionHandler(NetworkManager *manager, ProtocolHandler *protocol) :
    SessionHandler(manager, protocol)
{}

ServerSessionHandler::~ServerSessionHandler() {}

int ServerSessionHandler::run() {
    mlog.debug() << "in ServerSessionHandler::run" << std::endl;
    std::string buffer;
    ssize_t bytes;
    // Loop until something ends the session.
    for(;;) {
        buffer.clear();
        // FIXME: must go into select on all sessions' fds
        bytes = m_manager->read(buffer);
        if (bytes == 0) {
            mlog.debug() << "read 0 bytes" << std::endl;
            return 1;
        } else if (bytes < 0) {
            mlog.error() << "read error" << std::endl;
            return 0;
        } else {
            m_partial.append(buffer);
            std::vector<std::string> messages = m_protocol->interpret(m_partial);
            for (auto msg : messages) {
                mlog.debug() << "found message " << msg << std::endl;
                handle(msg);
            }
        }
    }
}

int ServerSessionHandler::handle(std::string data) {
    mlog.info() << "ServerSessionHandler::handle: " << data << std::endl;
    if (data == "PING") {
        m_manager->write("PONG\r\n");
        return 1;
    } else {
        mlog.error() << "ClientSessionHandler::handle: unknown message '" << data << "'" << std::endl;
        return 0;
    }
}
