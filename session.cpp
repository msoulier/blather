#include "network.hpp"
#include "protocol.hpp"
#include "session.hpp"
#include "logger.hpp"

std::atomic<bool> g_shutdown_asap{false};

SessionHandler::SessionHandler() {}

SessionHandler::SessionHandler(NetworkManager *manager, ProtocolHandler *protocol, SESSIONID sessionid) :
    m_manager(manager),
    m_protocol(protocol),
    m_sessionid(sessionid)
{
    mlog.debug() << "starting session with sessionid " << m_sessionid << std::endl;
}

SessionHandler::~SessionHandler() {}

int SessionHandler::run() {
    throw std::runtime_error("should not be called directly");
}

int SessionHandler::handle(std::string data) {
    throw std::runtime_error("should not be called directly");
    return 0;
}

ClientSessionHandler::ClientSessionHandler() {}

ClientSessionHandler::ClientSessionHandler(NetworkManager *manager, ProtocolHandler *protocol, SESSIONID sessionid) :
    SessionHandler(manager, protocol, sessionid)
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
        // If we're supposed to shutdown, then do so.
        if (g_shutdown_asap) {
            mlog.info() << "ClientSessionHandler::run shutting down" << std::endl;
            break;
        }
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
    // FIXME: shutdown the network connection
    return 1;
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

ServerSessionHandler::ServerSessionHandler(NetworkManager *manager, ProtocolHandler *protocol, SESSIONID sessionid) :
    SessionHandler(manager, protocol, sessionid)
{}

ServerSessionHandler::~ServerSessionHandler() {}

int ServerSessionHandler::run() {
    mlog.debug() << "in ServerSessionHandler::run - m_sessionid is " << m_sessionid << std::endl;
    std::string buffer;
    ssize_t bytes;
    // Loop until something ends the session.
    for(;;) {
        buffer.clear();
        if (g_shutdown_asap) {
            break;
        }
        bytes = m_manager->read(buffer, m_sessionid);
        if (bytes == 0) {
            mlog.debug() << "read 0 bytes" << std::endl;
            return 1;
        } else if (bytes < 0) {
            mlog.error() << "read error" << std::endl;
            return 0;
        } else {
            mlog.debug() << "read " << bytes << " bytes" << std::endl;
            m_partial.append(buffer);
            std::vector<std::string> messages = m_protocol->interpret(m_partial);
            for (auto msg : messages) {
                mlog.debug() << "found message " << msg << std::endl;
                handle(msg);
            }
        }
    }
    return 0;
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
