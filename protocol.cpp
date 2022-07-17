#include "mlogger.hpp"
#include "protocol.hpp"

ProtocolHandler::ProtocolHandler() {}

ProtocolHandler::ProtocolHandler(ProtocolHandler &source) {
    Copy(source);
}

ProtocolHandler::~ProtocolHandler() {}

void ProtocolHandler::operator=(ProtocolHandler &source) {
    Copy(source);
}

void ProtocolHandler::Print(std::ostream *os) {
    *os << "ProtocolHandler: ";
}

ssize_t ProtocolHandler::send_msg(const std::string msg) {
    return 0;
}

std::string ProtocolHandler::next_msg() {
    std::string msg;
    return msg;
}

void ProtocolHandler::Copy(ProtocolHandler &source) {
}
