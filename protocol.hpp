#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <vector>
#include <map>

class ProtocolHandler
{
public:
    // Construtor
    ProtocolHandler();
    // Constructor with a map initializer.
    ProtocolHandler(std::map<std::string,int> messages);
    // Copy constructor
    ProtocolHandler(ProtocolHandler &source);
    // Destructor
    ~ProtocolHandler(void);
    // Assignment operator
    void operator=(ProtocolHandler &source);
    // A print method for sending data to an iostream
    void Print(std::ostream *os);
    // Compose a message and return it - adds \r\n
    std::string compose(const std::string msg);
    // Given a block of data from a buffer, extract any messages
    // and return them as a vector of std::strings, and modify
    // the buffer, leaving any remainder.
    std::vector<std::string> interpret(std::string &buffer);

private:
    // Reusable Copy method
    void Copy(ProtocolHandler &source);
    // Is this a valid message?
    bool is_valid(std::string msg);

protected:
    // A map of messages to integers. Simple existence for now, but may map
    // to more meaningful data later.
    std::map<std::string,int> m_messages;
};

std::ostream &operator<<(std::ostream &os, ProtocolHandler &handler);

class ProtocolHandlerV1 : public ProtocolHandler {
public:
    ProtocolHandlerV1();
    ~ProtocolHandlerV1();
};

#endif
