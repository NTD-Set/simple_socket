#ifndef VER3_SOCKET
#define VER3_SOCKET

#include <string>
#include <vector>
#include <sstream>

namespace Dz {
namespace Socket {
// An RAII base class for handling sockets.
// Socket is movable but not copyable.
class BaseSocket
{

    int     socketId;
    protected:
        static constexpr int invalidSocketId      = -1;

        // Designed to be a base class not used used directly.
        BaseSocket(int socketId);
        int getSocketId() const {return socketId;}
    public:
        virtual ~BaseSocket();

        // Moveable but not Copyable
        BaseSocket(BaseSocket&& move)               noexcept;
        BaseSocket& operator=(BaseSocket&& move)    noexcept;
        void swap(BaseSocket& other)                noexcept;
        BaseSocket(BaseSocket const&)               = delete;
        BaseSocket& operator=(BaseSocket const&)    = delete;

        // User can manually call close
        void close();
};

class DataSocket: public BaseSocket
{
public:
    DataSocket (int socketId);
    std::size_t getMessageData(char* buffer, std::size_t size);
    void putMessageData(const char* buffer, std::size_t size);
private:
    static constexpr int maxSizeOfBuffer= 1025; // we need 1 more space to store EOL char
    char dataBuf[maxSizeOfBuffer];
};

class ConnectionSocket: public DataSocket
{
public:
    ConnectionSocket (std::string const& host, int port);
};

class ServerSocket: public BaseSocket
{
public:
    ServerSocket(int port);

    DataSocket accept();
private:
    static constexpr int maxConnectionBacklog = 5;
};

}
}
#endif
