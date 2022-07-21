#include "logger.hpp"
#include "protocol.hpp"

ProtocolHandler::ProtocolHandler() {}

ProtocolHandler::ProtocolHandler(std::map<std::string,int> messages) :
    m_messages(messages)
{}

ProtocolHandler::ProtocolHandler(ProtocolHandler &source) {
    Copy(source);
}

ProtocolHandler::~ProtocolHandler() {}

void ProtocolHandler::operator=(ProtocolHandler &source) {
    Copy(source);
}

void ProtocolHandler::Copy(ProtocolHandler &source) {
}

void ProtocolHandler::Print(std::ostream *os) {
    *os << "ProtocolHandler: ";
}

std::string ProtocolHandler::compose(const std::string msg) {
    // Right now the protocol is simple. Append \r\n as a delimiter and
    // make sure that it is a valid message.
    std::string composed;
    if (is_valid(msg)) {
        composed = msg + "\r\n";
    } else {
        throw std::runtime_error("invalid message");
    }
    return composed;
}

// FIXME: shouldn't this function be virtual
std::vector<std::string> ProtocolHandler::interpret(std::string &buffer) {
    std::vector<std::string> messages;
    // Look for all \r\n delimited messages.
    std::string::size_type n;
    const std::string delim = "\r\n";
    for (;;) {
        n = buffer.find(delim);
        if (n == std::string::npos) {
            break;
        }
        std::string msg = buffer.substr(0, n);
        mlog.debug() << "interpret found: " << msg << std::endl;
        messages.push_back(msg);
        buffer = buffer.substr(n+delim.size());
        mlog.debug() << "interpret buffer now: " << buffer << std::endl;
    }
    return messages;
}

bool ProtocolHandler::is_valid(std::string msg) {
    auto it = m_messages.find(msg);
    if (it == m_messages.end()) {
        return false;
    } else {
        return true;
    }
}

ProtocolHandlerV1::ProtocolHandlerV1() :
    ProtocolHandler( {{"PING", 1}, {"PONG", 1}} )
{}

ProtocolHandlerV1::~ProtocolHandlerV1()
{}
