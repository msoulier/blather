#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"

int main(int argc, char *argv[]) {
    logger.setDefaults();
    logger.info("Hello from server");

    return 0;
}
