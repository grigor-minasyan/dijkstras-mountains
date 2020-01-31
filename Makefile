CXX=g++
LD=g++
CC=g++

TARGET =  all 

SRCS = main.cc

OBJS = $(SRCS:.cc=.o)

EXE = $(SRCS:.cc=)

BRIDGES_INCL = $(HOME)/Downloads/bridges-cxx-3.0.0/include
#IFLAGS = -I$(BRIDGES_INCL)  -I/usr/local/curl763/include -I$(BRIDGES_INCL)/rapidjson
IFLAGS = -I/usr/local/curl763/include
CPPFLAGS = -c -O3 $(IFLAGS) -std=c++2a 

LDFLAGS =  -L/usr/local/curl763/lib 
LIBS =  -lcurl

.SUFFIXES: .cc

.cpp.o:  
	$(CC) $(CPPFLAGS)  $< -o $@

all : a.out

a.out: main.o
	$(CC) -g main.o $(LDFLAGS) $(LIBS)

clean:
	rm -rf *.o a.out core
