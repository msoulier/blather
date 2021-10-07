#include <iostream>

#include "network.hpp"

int main(void) {
    NetworkConnection tcpconn(TCPCONN, "192.168.0.1", 5000);

    std::cout << tcpconn << std::endl;

    return 0;
}
