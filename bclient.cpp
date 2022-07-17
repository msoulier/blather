#include <errno.h>
#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"
#include "session.hpp"

#define VERSION "0.1"

int connect_server(TcpNetworkManager &netman, std::string host, std::string port) {
    mlog.info() << "blather client told to connect to "
                << host << ":" << port << std::endl;

    if (netman.connect_to(host, port) < 0) {
        mlog.error("connection error");
        return 0;
    }
    mlog.info() << "connected to " << host << ":" << port << std::endl;

    // FIXME: this should be handled by the ProtocolHandler
    std::string msg("PING");
    mlog.debug() << "sending " << msg << std::endl;
    int bytes_sent = netman.write(msg + "\r\n");
    mlog.debug() << "sent " << bytes_sent << " bytes" << std::endl;
    if (bytes_sent < 0) {
        perror("write");
        return 0;
    }

    mlog.debug() << "reading response" << std::endl;
    std::string buffer;
    int bytes_recv = netman.read(buffer);
    mlog.debug() << "received " << bytes_recv << " bytes" << std::endl;
    if (bytes_recv < 0) {
        perror("read");
        return 0;
    }

    mlog.debug() << "msg was " << buffer << std::endl;

    if (buffer == "PONG") {
        return 1;
    } else {
        mlog.error() << "bad response from server: " << buffer << std::endl;
        return 0;
    }
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
    ProtocolHandler protocol;
    SessionHandler session(&netman, &protocol);

    if (connect_server(netman, host, port)) {
        mlog.info() << "Connected to server at " << host << ":" << port << std::endl;
    } else {
        mlog.error() << "Failed to connect to server at " << host << ":" << port << std::endl;
        return 1;
    }

    // Server connection is up, time to start talking.
    // session.run();
    return 1;
}
