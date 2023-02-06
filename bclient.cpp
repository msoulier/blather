#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <thread>

#include "logger.hpp"
#include "protocol.hpp"
#include "network.hpp"
#include "session.hpp"

#define VERSION "0.1"

// Set by parse_arguments
std::string host;
std::string port;
// The session thread.
std::thread *g_session;

SESSIONID connect_server(TcpNetworkManager &netman, std::string host, std::string port) {
    SESSIONID sessionid = 0;
    mlog.info() << "blather client told to connect to "
                << host << ":" << port << std::endl;

    if ( (sessionid = netman.connect_to(host, port)) ) {
        mlog.debug() << "netman sessionid is " << sessionid << std::endl;
    } else {
        mlog.error("connection error");
        return 0;
    }
    mlog.info() << "connected to " << host << ":" << port << std::endl;

    return sessionid;
}

int parse_arguments(int argc, char *argv[]) {
    if (argc < 3) {
        mlog.error("Usage: %s [-d|--debug] <host> <port>", argv[0]);
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

void shutdown() {
    alarm(1);
    g_shutdown_asap = true;
}

void shutdown_handler(int signum) {
    // Set the atomic boolean defined in session.hpp
    write(2, "===> SHUTDOWN\n", 13);
    shutdown();
}

int handle_user(ClientSessionHandler &session) {
    std::string command;
    for (;;) {
        // FIXME: pull in gnu readline
        std::cout << "> " << std::flush;
        //std::cin >> command;
        std::getline(std::cin, command);
        mlog.debug() << "Read command: " << command << std::endl;
        // FIXME: need user session handler
        if (command == "quit") {
            shutdown();
            return 0;
        }
        session.say(command);
    }
}

int main(int argc, char *argv[]) {
    mlog.setDefaults();
    mlog.setLevel(MLoggerVerbosity::info);
    mlog.info("blather client version %s", VERSION);

    signal(SIGTERM, &shutdown_handler);
    signal(SIGINT, &shutdown_handler);

    if (! parse_arguments(argc, argv)) {
        return 1;
    }

    TcpNetworkManager netman;
    ProtocolHandlerV1 protocol;

    SESSIONID client_sessionid = 0;

    if ( (client_sessionid = connect_server(netman, host, port)) ) {
        mlog.info() << "Connected to server at " << host << ":" << port << std::endl;
    } else {
        mlog.error() << "Failed to connect to server at " << host << ":" << port << std::endl;
        return 1;
    }

    ClientSessionHandler session(&netman, &protocol, client_sessionid);
    // Run the session in its own thread, and let the main thread handle user interaction.
    g_session = new std::thread(&ClientSessionHandler::run, &session);

    // Server connection is up, time to start talking.
    return handle_user(session);
}
