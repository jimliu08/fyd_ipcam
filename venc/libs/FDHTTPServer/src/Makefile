MAJOR=1
MINOR=0
PATCH=0
VERSION=$(MAJOR).$(MINOR).$(PATCH)
LIBNAME=fdhttpserver

CXX = arm-linux-g++
CFLAGS += -O3 -Wall -Wextra -std=c++0x
#CFLAGS += -O3 -Wall -Wextra -pedantic -std=c++0x
#CFLAGS += -fPIC -g -O3 -Wall -Wextra -pedantic -std=c++11
CPPFLAGS += -I. -I../include -I../../../opensrc/http-parser/include

LDFLAGS += -shared -Wl,-soname,lib$(LIBNAME).so.$(MAJOR) -L. -L../../../opensrc/http-parser/lib
LIBS += -lpthread

.PHONY: all clean

all: lib$(LIBNAME).so.$(VERSION)

lib$(LIBNAME).so.$(VERSION): fd_http_server.o fd_http_connection.o uri.o utils.o http_msg.o http_request_dispatcher.o not_implement_request_handler.o
	$(CXX) $^ $(LDFLAGS) $(LIBS) -o $@

%.o: %.cpp
	$(CXX) -fPIC $(CFLAGS) $(CPPFLAGS) -c $^

clean:
	-rm -f *.o *.so*

