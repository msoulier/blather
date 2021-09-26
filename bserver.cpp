#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"

Blogger logger;

int main(int argc, char *argv[]) {
    logger.info("Hello from server");

    return 0;
}
