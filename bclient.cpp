#include <errno.h>
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

    std::string msg("GET / HTTP/1.1\r\n\r\n");
    mlog.info() << "sending " << msg << std::endl;
    int bytes_sent = netman.write(msg);
    mlog.info() << "sent " << bytes_sent << " bytes" << std::endl;
    if (bytes_sent < 0) {
        perror("write");
        return 1;
    }

    mlog.info() << "reading response" << std::endl;
    std::string buffer;
    int bytes_recv = netman.read(buffer);
    mlog.info() << "received " << bytes_recv << " bytes" << std::endl;
    if (bytes_recv < 0) {
        perror("read");
        return 1;
    }

    mlog.debug() << "response: " << buffer << std::endl;

    return 0;
}
