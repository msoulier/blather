#include "logger.hpp"
#include "protocol.hpp"

Blogger logger;

int main(int argc, char *argv[]) {
    logger.info("Hello from client");

    return 0;
}
