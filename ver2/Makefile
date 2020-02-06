CLIENTOBJS    = client.o socket.o protocol.o
SERVEROBJS    = server.o socket.o protocol.o
CXX     = g++
DEBUG   = -g
CXXFLAGS  = -Wall $(DEBUG) -std=c++14 -pthread
LDFLAGS = -pthread

all:	client server
clean:
	rm -f *.o client server

server: $(SERVEROBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
client: $(CLIENTOBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)
