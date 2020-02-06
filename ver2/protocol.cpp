#include "protocol.h"

using namespace Dz::Socket;

void ProtocolSimple::sendMessage(std::string const& message)
{
    // printf ("[%s][%d] Send [%s]\n", __FILE__, __LINE__, message.c_str());
    socket_.putMessageData(message.c_str(), message.size());
}
void ProtocolSimple::recvMessage(std::string& message) {
    char* buffer = &message[0];
    message.clear();

    std::size_t got =0;
    do
    {
         got = socket_.getMessageData(buffer, dataMax);
    } while (got == 0);
    // printf ("[%s][%d] Recv [%s]\n", __FILE__, __LINE__, message.c_str());
}
