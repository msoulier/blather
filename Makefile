CC = g++
CFLAGS = -Wall -std=c++11 -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK -Wno-unknown-warning-option
COBJS = bclient.o protocol.o logger.o
SOBJS = bserver.o protocol.o logger.o
LIBS = -lboost_log-mt -lpthread -lboost_thread-mt -lboost_system-mt

CFLAGS += -I/usr/local/Cellar/boost/1.76.0/include

all: bclient bserver

help:
	@echo "Targets: help all bclient bserver"

bclient: $(COBJS)
	$(CC) -o bclient $(COBJS) $(LIBS)

bserver: $(SOBJS)
	$(CC) -o bserver $(COBJS) $(LIBS)

bclient.o: bclient.cpp protocol.hpp logger.hpp
	$(CC) $(CFLAGS) -c bclient.cpp

bserver.o: bserver.cpp protocol.hpp logger.hpp
	$(CC) $(CFLAGS) -c bserver.cpp

protocol.o: protocol.cpp protocol.hpp
	$(CC) $(CFLAGS) -c protocol.cpp

logger.o: logger.cpp logger.hpp
	$(CC) $(CFLAGS) -c logger.cpp

clean:
	rm -f $(COBJS) $(SOBJS) bclient bserver
