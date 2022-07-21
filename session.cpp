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
    mlog.info() << "SessionHandler::handle: " << data << std::endl;
    if (data == "PING") {
        m_manager->write("PONG\r\n");
    }
    return 1;
}

ClientSessionHandler::ClientSessionHandler() {}

ClientSessionHandler::ClientSessionHandler(NetworkManager *manager, ProtocolHandler *protocol) :
    SessionHandler(manager, protocol)
{}

ClientSessionHandler::~ClientSessionHandler() {}

int ClientSessionHandler::run() {
    mlog.debug() << "in ClientSessionHandler::run" << std::endl;
    std::string msg("PING");
    mlog.debug() << "sending " << msg << std::endl;
    int bytes_sent = m_manager->write(m_protocol->compose(msg));
    mlog.debug() << "sent " << bytes_sent << " bytes" << std::endl;
    if (bytes_sent < 0) {
        perror("write");
        return 0;
    }

    mlog.debug() << "reading response" << std::endl;
    std::string buffer;
    int bytes_recv = m_manager->read(buffer);
    mlog.debug() << "received " << bytes_recv << " bytes" << std::endl;
    if (bytes_recv < 0) {
        perror("read");
        return 0;
    }

    mlog.debug() << "msg was " << buffer << std::endl;

    if (buffer == "PONG") {
        return 1;
    } else {
        mlog.error() << "bad response from server: " << buffer << std::endl;
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
        // FIXME: must append to the buffer, not clobber it
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
