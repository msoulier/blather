#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"

int main(int argc, char *argv[]) {
    logger.setLevel(LOGLEVEL_INFO);
    logger.info() << "Hello from client" << std::endl;

    return 0;
}
