#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"

#define VERSION "0.1"

int main(int argc, char *argv[]) {
    mlog.setDefaults();
    mlog.setLevel(MLoggerVerbosity::debug);
    mlog.info("blather client version %s", VERSION);

    if (argc < 3) {
        mlog.error("Usage: %s <host> <port>", argv[0]);
        return 1;
    }
    std::string host(argv[1]);
    std::string port(argv[2]);
    mlog.info() << "blather client told to connect to "
                << host << ":" << port << std::endl;

    TcpNetworkManager netman;

    if (netman.connect_to(host, port) < 0) {
        mlog.error("connection error");
        return 1;
    }
    mlog.info() << "connected to " << host << ":" << port << std::endl;
    return 0;
}
