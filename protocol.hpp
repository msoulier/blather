#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <vector>
#include <map>

enum BlatherMessageType
{
    PING = 0,
    PONG,
    SAY
};

const std::map<BlatherMessageType,std::string> message_map = {
    {BlatherMessageType::PING, "PING"},
    {BlatherMessageType::PONG, "PONG"},
    {BlatherMessageType::SAY, "SAY"}
    };

class BlatherMessage
{
public:
    BlatherMessage();
    BlatherMessage(BlatherMessageType mtype, std::string payload);
    ~BlatherMessage();
    // A print method for sending data to an iostream
    std::string print();
    // Take the message and return an std::string suitable for putting
    // on the wire. This method adds the \r\n delimeter at the end.
    std::string transmit();
    // Take the wire protocol data and use it to populate the current
    // object.
    void receive(std::string data);
    // Getters and setters.
    void set_type(BlatherMessageType mtype);
    void set_payload(std::string payload);
    BlatherMessageType get_type();
    std::string get_payload();

private:
    BlatherMessageType m_type;
    std::string m_payload;
};

std::ostream &operator<<(std::ostream &stream, BlatherMessage &message);

class ProtocolHandler
{
public:
    // Construtor
    ProtocolHandler();
    // Copy constructor
    ProtocolHandler(ProtocolHandler &source);
    // Destructor
    ~ProtocolHandler(void);
    // Assignment operator
    void operator=(ProtocolHandler &source);
    // Given a block of data from a buffer, extract any messages
    // and return them as a vector of std::strings, and modify
    // the buffer, leaving any remainder.
    std::vector<BlatherMessage> interpret(std::string &buffer);

private:
    // Reusable Copy method
    void Copy(ProtocolHandler &source);
    // Is this a valid message?
    bool is_valid(std::string msg);
};

class ProtocolHandlerV1 : public ProtocolHandler {
public:
    ProtocolHandlerV1();
    ~ProtocolHandlerV1();
};

#endif
