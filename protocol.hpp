#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

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
    // A print method for sending data an iostream
    void Print(std::ostream *os);
    // Send a message - wrapper around write - adds \r\n
    ssize_t send_msg(const std::string msg);
    // Block
    std::string next_msg();

private:
    // Reusable Copy method
    void Copy(ProtocolHandler &source);
};

std::ostream &operator<<(std::ostream &os, ProtocolHandler &handler);

class ProtocolHandlerV1 : public ProtocolHandler {
};

#endif
