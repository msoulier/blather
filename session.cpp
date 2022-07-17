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
    std::string buffer;
    ssize_t bytes;
    // Loop forever, and compose incoming data into messages
    // separated by \r\n.
    for(;;) {
        std::size_t found = 0;
        // FIXME: must append to the buffer, not clobber it
        bytes = m_manager->read(buffer);
        if (bytes == 0) {
            mlog.debug() << "read 0 bytes" << std::endl;
            return 1;
        } else if (bytes < 0) {
            mlog.error() << "read error" << std::endl;
            return 0;
        } else {
            // Did we receive a full message? Or more?
            for (;;) {
                // FIXME: Look for all \r\n delimiters
                // Need a protocol handler to do this.
                if ((found = buffer.find("\r\n")) > 0) {
                    mlog.debug() << "found is " << found << std::endl;
                    std::string msg = buffer.substr(0, found);
                    mlog.debug() << "msg is " << msg << std::endl;
                    handle(msg);
                }
                break;
            }
        }
    }
}

int SessionHandler::handle(std::string data) {
    mlog.info() << "SessionHandler::handle: " << data << std::endl;
    if (data == "PING") {
        m_manager->write("PONG\r\n");
    }
    return 1;
}
