#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"

#define VERSION "0.1"

int main(int argc, char *argv[]) {
    mlog.setDefaults();
    mlog.info("blather server version %s", VERSION);

    return 0;
}
