#include "logger.h"
#include "protocol.h"

MLogger logger;

int main(int argc, char *argv[]) {
    logger.setLevel(LOGLEVEL_INFO);
    return 0;
}
