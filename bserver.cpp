#include <stdlib.h>

#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"
#include "session.hpp"

#define VERSION "0.1"

int accept_connections(TcpNetworkManager &netman, SessionHandler &session) {
    // FIXME: must evolve to handling multiple connections
    mlog.debug() << "going into accept" << std::endl;
    if (netman.accept()) {
        mlog.info() << "accepted network connection - starting session" << std::endl;
        return session.run();
    } else {
        mlog.warn() << "failed to set up network connection" << std::endl;
        return 0;
    }
}

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
    ProtocolHandler protocol;
    SessionHandler session(&netman, &protocol);

    if (netman.listen(port) < 0) {
        mlog.error("listen error");
        return 1;
    }
    mlog.info() << "listening on " << "0.0.0.0" << ":" << port << std::endl;

    // FIXME: need to spawn a thread for each connection
    for (;;) {
        if (accept_connections(netman, session)) {
            mlog.debug() << "Successfully finished handling connection" << std::endl;
        } else {
            mlog.error() << "Error handling connection" << std::endl;
        }
    }

    mlog.info() << "exiting" << std::endl;

    return 0;
}
