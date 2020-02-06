#include "socket.h"
#include <unistd.h>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
using namespace Dz::Socket;

BaseSocket::BaseSocket (int socketId):
    socketId(socketId)
{
    if (socketId == -1) {
            close();
    }
}

BaseSocket::~BaseSocket()
{
    if (socketId == invalidSocketId)
    {
        // This object has been closed or moved.
        // So we don't need to call close.
        return;
    }

    try
    {
        close();
    }
    catch(...)
    {
        // We should log this
        // TODO: LOGGING CODE HERE
    }
}

void BaseSocket::close()
{
    if (socketId == invalidSocketId)
    {
    }
    while(true)
    {
        int state = ::close(socketId);
        if (state == invalidSocketId)
        {
            break;
        }
    }
    socketId = invalidSocketId;
}

void BaseSocket::swap(BaseSocket& other) noexcept
{
    using std::swap;
    swap(socketId,   other.socketId);
}

BaseSocket::BaseSocket(BaseSocket&& move) noexcept
    : socketId(invalidSocketId)
{
    move.swap(*this);
}

BaseSocket& BaseSocket::operator=(BaseSocket&& move) noexcept
{
    move.swap(*this);
    return *this;
}

std::size_t DataSocket::getMessageData(char* buffer, std::size_t size) {
    std::size_t bytesRead = 0;
    std::size_t dataMax = size;
    // read till reach maximum data size or receive EOL
    while (dataMax > 0) {
        int got = ::recv(getSocketId(), buffer + bytesRead, dataMax,0);
        if (got == -1) {
            throw std::runtime_error("recv error");
        }
        if (got == 0) {
            break;
        }

        dataMax -= got;
        bytesRead += got;
        if (buffer[bytesRead -1] == '\0') {
            break;
        }
    }
    return bytesRead;
}
void DataSocket::putMessageData(const char* buffer, std::size_t size) {
    std::size_t sentData = 0;
    char* p = &dataBuf[0];
    memcpy(p, buffer, size);
    dataBuf[size] = '\0';
    while (sentData < size +1) {
        int sent = ::send(getSocketId(), p + sentData, size + 1  - sentData,0);
        if (sent == -1) {
            throw std::runtime_error("send error");
        }
        sentData += sent;
    }
}

DataSocket::DataSocket (int SocketId) :
    BaseSocket(SocketId)
{

}

ConnectionSocket::ConnectionSocket(std::string const& host, int port) :
    DataSocket((int)(::socket(PF_INET, SOCK_STREAM, 0)))
{
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(port);
    serverAddr.sin_addr.s_addr  = inet_addr(host.c_str());

    if (::connect(getSocketId(), (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
    {
        // printf ("[%s][%d] Can't connect\n", __FILE__, __LINE__);
        close();
        throw std::runtime_error("Can't connect");
    }
}

ServerSocket::ServerSocket(int port) :
    BaseSocket((int)(::socket(PF_INET, SOCK_STREAM, 0)))
{
    struct sockaddr_in serverAddr;
    bzero((char*)&serverAddr, sizeof(serverAddr));
    serverAddr.sin_family       = AF_INET;
    serverAddr.sin_port         = htons(port);
    serverAddr.sin_addr.s_addr  = INADDR_ANY;

    if (::bind(getSocketId(), (struct sockaddr *) &serverAddr, sizeof(serverAddr)) != 0)
    {
        // printf ("[%s][%d] Can't bind\n", __FILE__, __LINE__);
        close();
        throw std::runtime_error("Can't bind");
    }

    if (::listen(getSocketId(), maxConnectionBacklog) != 0)
    {
        // printf ("[%s][%d] Can't listen\n", __FILE__, __LINE__);
        close();
        throw std::runtime_error("Can't listen");
    }
}

DataSocket ServerSocket::accept() {
	socklen_t sinSize;
	struct sockaddr_storage theirAddr; // connector's address information

    sinSize = sizeof (theirAddr);
    int newSocket = (::accept(getSocketId(), (struct sockaddr *)& theirAddr, &sinSize));
    if (newSocket == -1) {
        // printf ("[%s][%d] Can't accept\n", __FILE__, __LINE__);
        close();
        throw std::runtime_error("Can't accept");
    }
    return DataSocket(newSocket);
}
