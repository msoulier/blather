#include <errno.h>
#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"

#define VERSION "0.1"

int connect_server(TcpNetworkManager &netman, std::string host, std::string port) {
    mlog.info() << "blather client told to connect to "
                << host << ":" << port << std::endl;

    if (netman.connect_to(host, port) < 0) {
        mlog.error("connection error");
        return 0;
    }
    mlog.info() << "connected to " << host << ":" << port << std::endl;

    std::string msg("PING");
    mlog.info() << "sending " << msg << std::endl;
    int bytes_sent = netman.send_msg(msg);
    mlog.info() << "sent " << bytes_sent << " bytes" << std::endl;
    if (bytes_sent < 0) {
        perror("write");
        return 0;
    }

    mlog.info() << "reading response" << std::endl;
    std::string buffer;
    int bytes_recv = netman.read(buffer);
    mlog.info() << "received " << bytes_recv << " bytes" << std::endl;
    if (bytes_recv < 0) {
        perror("read");
        return 0;
    }

    mlog.debug() << "response: " << buffer << std::endl;

    return 1;
}

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

    TcpNetworkManager netman;

    if (connect_server(netman, host, port)) {
        mlog.info() << "Connected to server at " << host << ":" << port << std::endl;
    } else {
        mlog.error() << "Failed to connect to server at " << host << ":" << port << std::endl;
        return 1;
    }

    // Server connection is up, time to start talking.
    return 1;
}
