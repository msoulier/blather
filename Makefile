current_makefile := $(lastword $(MAKEFILE_LIST))
makefile_dir := $(shell dirname $(current_makefile))
include $(makefile_dir)/Makefile.arch

CC = g++
CFLAGS = -Wall -std=c++11 -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK -Wno-unknown-warning-option -I../mikelibcpp
COBJS = bclient.o protocol.o network.o
SOBJS = bserver.o protocol.o network.o
LIBS = -lpthread -lboost_system -lmikecpp
OS := $(shell uname -s)
LDFLAGS = -L../mikelibcpp

ifeq ($(OS),Darwin)
	CFLAGS += -DDARWIN -I/usr/local/include -I/usr/local/Cellar/boost/1.76.0/include
	LIBS += -lboost_log-mt -lboost_thread-mt -lboost_system-mt
	LDFLAGS + =-L/usr/local/lib
endif

ifeq ($(OS),Linux)
	LDFLAGS += -L/usr/lib/x86_64-linux-gnu
	CFLAGS += -DLINUX -static
	LIBS += -lboost_thread -lboost_system
endif

all: mikelibcpp bclient bserver

help:
	@echo "Targets: help all bclient bserver test"
	@echo "Arch is $(ARCH)"
	@echo "OS is $(OS)"
	@echo "COMP_VER is $(COMP_VER)"
	@echo "COMP_NAME is $(COMP_NAME)"
	@echo "CFLAGS is $(CFLAGS)"
	@echo "LDFLAGS is $(LDFLAGS)"
	@echo "LIBS is $(LIBS)"

mikelibcpp:
	(cd ../mikelibcpp && make)

test.o: test.cpp network.hpp
	$(CC) $(CFLAGS) -c test.cpp

test: test.o network.o
	$(CC) -o unittest test.o network.o $(LIBS)
	./unittest

bclient: $(COBJS)
	$(CC) -o bclient $(LDFLAGS) $(COBJS) $(LIBS)

bserver: $(SOBJS)
	$(CC) -o bserver $(LDFLAGS) $(COBJS) $(LIBS)

bclient.o: bclient.cpp protocol.hpp network.hpp ../mikelibcpp/libmikecpp.a
	$(CC) $(CFLAGS) -c bclient.cpp

bserver.o: bserver.cpp protocol.hpp network.hpp ../mikelibcpp/libmikecpp.a
	$(CC) $(CFLAGS) -c bserver.cpp

protocol.o: protocol.cpp protocol.hpp
	$(CC) $(CFLAGS) -c protocol.cpp

logger.o: logger.cpp logger.hpp
	$(CC) $(CFLAGS) -c logger.cpp

network.o: network.cpp network.hpp
	$(CC) $(CFLAGS) -c network.cpp

clean:
	rm -f *.o bclient bserver unittest
	(cd ../mikelibcpp && make clean)
