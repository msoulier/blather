current_makefile := $(lastword $(MAKEFILE_LIST))
makefile_dir := $(shell dirname $(current_makefile))
include $(makefile_dir)/Makefile.arch

CC = g++
CFLAGS = -Wall -std=c++17 -I../mikelibcpp
COBJS = bclient.o protocol.o network.o logger.o
SOBJS = bserver.o protocol.o network.o logger.o
LIBS = -lasan -lpthread -lmikecpp
OS := $(shell uname -s)
LDFLAGS = -L../mikelibcpp

ifeq ($(DEBUG),1)
	CFLAGS += -ggdb -fsanitize=address
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
	cd ../mikelibcpp && make DEBUG=$(DEBUG)

test.o: test.cpp network.hpp
	$(CC) $(CFLAGS) -c test.cpp

test: test.o network.o
	$(CC) -o unittest test.o network.o $(LDFLAGS) $(LIBS)
	./unittest

bclient: $(COBJS)
	$(CC) -o bclient $(LDFLAGS) $(COBJS) $(LIBS)

bserver: $(SOBJS)
	$(CC) -o bserver $(LDFLAGS) $(SOBJS) $(LIBS)

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
