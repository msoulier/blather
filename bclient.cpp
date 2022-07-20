#include <errno.h>
#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"
#include "session.hpp"

#define VERSION "0.1"

// Set by parse_arguments
std::string host;
std::string port;

int connect_server(TcpNetworkManager &netman, std::string host, std::string port) {
    mlog.info() << "blather client told to connect to "
                << host << ":" << port << std::endl;

    if (netman.connect_to(host, port) < 0) {
        mlog.error("connection error");
        return 0;
    }
    mlog.info() << "connected to " << host << ":" << port << std::endl;

    return 1;
}

int parse_arguments(int argc, char *argv[]) {
    if (argc < 3) {
        mlog.error("Usage: %s <host> <port>", argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if ((arg == "-d") || (arg == "--debug")) {
            mlog.setLevel(MLoggerVerbosity::debug);
        } else if (host.empty()) {
            host = arg;
        } else {
            port = arg;
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    mlog.setDefaults();
    mlog.setLevel(MLoggerVerbosity::info);
    mlog.info("blather client version %s", VERSION);

    if (! parse_arguments(argc, argv)) {
        return 1;
    }

    TcpNetworkManager netman;
    ProtocolHandlerV1 protocol;
    ClientSessionHandler session(&netman, &protocol);

    if (connect_server(netman, host, port)) {
        mlog.info() << "Connected to server at " << host << ":" << port << std::endl;
    } else {
        mlog.error() << "Failed to connect to server at " << host << ":" << port << std::endl;
        return 1;
    }

    // Server connection is up, time to start talking.
    return session.run();
}
