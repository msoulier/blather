#include "mlogger.hpp"
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

std::vector<std::string> ProtocolHandler::interpret(std::string &buffer) {
    std::vector<std::string> messages;
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
