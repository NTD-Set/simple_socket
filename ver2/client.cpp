#include <cstdlib>
#include <iostream>
#include <vector>
#include <thread>
#include "socket.h"
#include "protocol.h"
namespace {
}

int main (int argc, char* argv[]) {
    int finished = 0;
    Dz::Socket::ConnectionSocket connect (argv[1], 8080);
    ProtocolSimple connection (connect);
    while (!finished)
    {
        std::string message;
        std::string response;
        std::cin >> message;
        if (message == "EXIT") {
            finished = 1;
            continue;
        }
        connection.sendMessage(message);
        connection.recvMessage(response);
        printf ("[Got Response] %s\n", response.c_str());
    }
}
