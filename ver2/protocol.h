#ifndef VER3_PROTOCOL
#define VER3_PROTOCOL

#include <string>
#include <cstdlib>
#include "socket.h"

class ProtocolSimple
{
public:
    ProtocolSimple(Dz::Socket::DataSocket &socket) : socket_(socket)
    {
    }

    void sendMessage(std::string const &message);
    void recvMessage(std::string &message);

private:
    Dz::Socket::DataSocket &socket_;
    static constexpr std::size_t dataMax = 1024;
};
#endif
