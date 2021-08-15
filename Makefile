CC = g++
CFLAGS = -Wall -std=c++11 -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK
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

bclient.o: bclient.cc protocol.h logger.h
	$(CC) $(CFLAGS) -c bclient.cc

bserver.o: bserver.cc protocol.h logger.h
	$(CC) $(CFLAGS) -c bserver.cc

protocol.o: protocol.cc protocol.h
	$(CC) $(CFLAGS) -c protocol.cc

logger.o: logger.cc logger.h
	$(CC) $(CFLAGS) -c logger.cc

clean:
	rm -f $(COBJS) $(SOBJS) bclient bserver
