#/* ===================================================
# * Copyright (C) 2017 chenshuangping All Right Reserved.
# *      Author: mincore@163.com
# *    Filename: Makefile
# *     Created: 2017-05-31 13:48
# * Description: 
# * ===================================================
# */
CC=@echo -e "cc \033[32m$@\033[0m";g++

DEPLIBDIR=dep/lib
DEPINCDIR=dep/include
BINDIR=bin
TARGET=$(BINDIR)/core
TEST=$(BINDIR)/test

CXXFLAGS=-g -Wall -Werror -std=c++11
CXXFLAGS+=-I$(DEPINCDIR) -Isrc

OBJS=$(patsubst %.cpp,%.o,$(shell find src -name *.cpp))
TEST_OBJS=$(patsubst %.cpp,%.o,$(shell find test -name *.cpp))
TEST_OBJS+=$(patsubst %main.o,,$(OBJS))

DEPS=\
	 -lpthread\
	 -lleveldb\
	 -lprotobuf\
	 -lreadline\
	 -lopencv_core

#LDFLAGS+=-Wl,-Bsymbolic,-rpath=$(BINDIR)
LDFLAGS+=$(DEPS)

$(shell cd ./src/msg/pb; ./gen.sh;)

all:$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

$(TEST): $(TEST_OBJS) 
	$(CC) $^ $(LDFLAGS) $(DEPLIBDIR)/libgtest.a -o $@

%.o:%.cpp
	$(CC) $< -c -o $@ $(CXXFLAGS)

.PHONY : clean

clean:
	@rm $(OBJS) $(TARGET) $(TEST) $(TEST_OBJS) -f
