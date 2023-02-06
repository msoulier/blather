#include "logger.hpp"
#include "protocol.hpp"

BlatherMessage::BlatherMessage()
{}

BlatherMessage::BlatherMessage(BlatherMessageType mtype, std::string payload)
    : m_type(mtype),
      m_payload(payload)
{}

BlatherMessage::~BlatherMessage()
{}

std::string BlatherMessage::print() {
    std::stringstream stream;
    stream << "BlatherMessage: " << "[" << message_map.at(m_type) << "]" << " " << m_payload << std::endl;
    return stream.str();
}

std::string BlatherMessage::transmit() {
    std::string onwire;
    mlog.debug() << "transmit message: " << *this << std::endl;
    onwire = message_map.at(m_type) + " " + m_payload + "\r\n";
    return onwire;
}

BlatherMessageType BlatherMessage::message_map_to_type(std::string smtype) {
    for (const auto& [key, value] : message_map) {
        if (smtype == value) {
            return key;
        }
    }
    return BlatherMessageType::INVALID;
}

void BlatherMessage::receive(std::string data) {
    // Find the first space. Everything up to there is the message type.
    size_t index = data.find_first_of(" ");
    if (index == std::string::npos) {
        // No spaces found.
        throw std::runtime_error("Invalid message - no type header");
    }
    std::string smtype(data.substr(0, index));
    m_type = message_map_to_type(smtype);
    if (m_type == BlatherMessageType::INVALID) {
        throw std::runtime_error("Invalid message - invalid type");
    }
    m_payload = data.substr(index);
}

void BlatherMessage::set_type(BlatherMessageType mtype)
{
    m_type = mtype;
}

void BlatherMessage::set_payload(std::string payload)
{
    m_payload = payload;
}

BlatherMessageType BlatherMessage::get_type()
{
    return m_type;
}

std::string BlatherMessage::get_payload()
{
    return m_payload;
}

std::ostream &operator<<(std::ostream &stream, BlatherMessage &message) {
    return stream << message.print();
} 

ProtocolHandler::ProtocolHandler() {}

ProtocolHandler::ProtocolHandler(ProtocolHandler &source) {
    Copy(source);
}

ProtocolHandler::~ProtocolHandler() {}

void ProtocolHandler::operator=(ProtocolHandler &source) {
    Copy(source);
}

void ProtocolHandler::Copy(ProtocolHandler &source) {
}

// FIXME: shouldn't this function be virtual
std::vector<BlatherMessage> ProtocolHandler::interpret(std::string &buffer) {
    std::vector<BlatherMessage> messages;
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
        // Turn the raw data into a BlatherMessage object.
        BlatherMessage message;
        message.receive(msg);
        messages.push_back(message);
        buffer = buffer.substr(n+delim.size());
        mlog.debug() << "interpret buffer now: " << buffer << std::endl;
    }
    return messages;
}

ProtocolHandlerV1::ProtocolHandlerV1()
{}

ProtocolHandlerV1::~ProtocolHandlerV1()
{}
