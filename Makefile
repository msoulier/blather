current_makefile := $(lastword $(MAKEFILE_LIST))
makefile_dir := $(shell dirname $(current_makefile))
include $(makefile_dir)/Makefile.arch

CC = g++
CFLAGS = -Wall -std=c++11 -DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK -Wno-unknown-warning-option
COBJS = bclient.o protocol.o logger.o
SOBJS = bserver.o protocol.o logger.o
LIBS = -lboost_log -lpthread -lboost_system

ifeq ($(OS),DARWIN)
	CFLAGS += -I/usr/local/Cellar/boost/1.76.0/include
	LIBS = -lboost_log-mt -lpthread -lboost_thread-mt -lboost_system-mt
endif

all: bclient bserver

help:
	@echo "Targets: help all bclient bserver"
	@echo "Arch is $(ARCH)"
	@echo "OS is $(OS)"
	@echo COMP_VER is $(COMP_VER)"
	@echo COMP_NAME is $(COMP_NAME)"

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
