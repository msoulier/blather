#include "mlogger.hpp"
#include "protocol.hpp"
#include "network.hpp"

MLogger logger;

int main(int argc, char *argv[]) {
    logger.setLevel(LOGLEVEL_INFO);
    logger.info() << "Hello from server" << std::endl;

    return 0;
}
