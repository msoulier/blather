#include <stdlib.h>
#include <thread>
#include <vector>
#include <signal.h>
#include <unistd.h>

#include "logger.hpp"
#include "protocol.hpp"
#include "mnetwork.hpp"
#include "session.hpp"

#define VERSION "0.1"

const int MAX_FAILURES = 3;

std::vector<std::thread*> g_threads;
int g_port = 0;

int accept_connections(TcpNetworkManager &netman, ProtocolHandler &protocol) {
    SESSIONID sessionid = 0;
    mlog.debug() << "going into accept" << std::endl;
    if ( (sessionid = netman.accept()) ) {
        mlog.debug() << "netman sessionid is " << sessionid << std::endl;
        ServerSessionHandler *session = new ServerSessionHandler(&netman, &protocol, sessionid);
        mlog.info() << "accepted network connection - starting session thread" << std::endl;
        std::thread *session_thread = new std::thread(&ServerSessionHandler::run, session);
        g_threads.push_back(session_thread);
        return 1;
    } else {
        mlog.warn() << "failed to set up network connection" << std::endl;
        return 0;
    }
}

int parse_arguments(int argc, char *argv[]) {
    if (argc < 2) {
        mlog.error("Usage: %s [-d|--debug] <port>", argv[0]);
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        if ((arg == "-d") || (arg == "--debug")) {
            mlog.setLevel(MLoggerVerbosity::debug);
        } else {
            g_port = atoi(arg.c_str());
            if (g_port == 0) {
                mlog.error() << "listen port must be a positive integer" << std::endl;
                return 0;
            }
        }
    }
    return 1;
}

void shutdown_handler(int signum) {
    // Set the atomic boolean defined in session.hpp
    write(2, "===> SHUTDOWN\n", 13);
    alarm(1);
    g_shutdown_asap = true;
}

int main(int argc, char *argv[]) {
    mlog.setDefaults();
    mlog.setLevel(MLoggerVerbosity::info);
    mlog.info("blather server version %s", VERSION);

    signal(SIGTERM, &shutdown_handler);
    signal(SIGINT, &shutdown_handler);

    mlog.info() << "blather server told to listen on "
                << "0.0.0.0" << ":" << g_port << std::endl;

    if (! parse_arguments(argc, argv)) {
        return 1;
    }

    TcpNetworkManager netman;
    ProtocolHandlerV1 protocol;

    if (netman.listen(g_port) < 0) {
        mlog.error("listen error");
        return 1;
    }
    mlog.info() << "listening on " << "0.0.0.0" << ":" << g_port << std::endl;

    int failures = 0;
    for (;;) {
        if (g_shutdown_asap) {
            break;
        }
        // Note: accept_connections adds a session thread to g_threads.
        if (accept_connections(netman, protocol)) {
            mlog.info() << "accept successful - threads running: " << g_threads.size() << std::endl;
        } else {
            mlog.error() << "Error handling connection: " << g_threads.size() << " threads running" << std::endl;
            // Quit if there are no threads running.
            if (g_threads.size() == 0) {
                mlog.error() << "Exiting, network error with zero sessions running." << std::endl;
                return 1;
            }
            if (++failures > MAX_FAILURES) {
                mlog.error() << "Exiting, hit max network errors." << std::endl;
                return 2;
            }
            mlog.debug() << "Sessions running, attempting to continue." << std::endl;
        }
    }

    // FIXME: need shutdown code, and must join threads
    mlog.info() << "exiting" << std::endl;

    return 0;
}
