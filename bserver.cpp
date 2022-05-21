#include <stdlib.h>

#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"

#define VERSION "0.1"

int main(int argc, char *argv[]) {
    mlog.setDefaults();
    mlog.setLevel(MLoggerVerbosity::debug);
    mlog.info("blather server version %s", VERSION);

    if (argc < 2) {
        mlog.error("Usage: %s <port>", argv[0]);
        return 1;
    }
    int port = atoi(argv[1]);
    if (port == 0) {
        mlog.error() << "listen port must be a positive integer" << std::endl;
        return 1;
    }
    mlog.info() << "blather server told to listen on "
                << "0.0.0.0" << ":" << port << std::endl;

    TcpNetworkManager netman;

    if (netman.listen(port) < 0) {
        mlog.error("listen error");
        return 1;
    }
    mlog.info() << "listening on " << "0.0.0.0" << ":" << port << std::endl;
    mlog.debug() << "going into accept" << std::endl;
    netman.accept(NetworkHandler());

    return 0;
}
