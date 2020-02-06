#include <cstdlib>
#include <iostream>
#include <vector>
#include <thread>
#include "socket.h"
#include "protocol.h"
#include <mutex>
#include <fstream> 
#include <stdlib.h>

namespace {
    std::vector <std::thread> workers;
    std::mutex file_mutex;
    FILE* fp = nullptr;

    void save_data(std::string const& data) {
        std::lock_guard<std::mutex> guard(file_mutex);
        fp = fopen("./DATA.txt", "at");
        if (!fp) {
            printf ("can't open file \n");
        }
        printf ("saving  [%s]\n", data.c_str());
        fprintf(fp, "%s\n", data.c_str());
        fclose(fp);
    }

    void each_thread_does (Dz::Socket::DataSocket accept) {
        ProtocolSimple connection(accept);
        while (true) {
            std::string message;
            connection.recvMessage(message);
            printf ("[Got Data] %s\n", message.c_str());
            save_data(message);
            connection.sendMessage("OK");
        }
    }

}

int main (int argc, char* argv[]) {
    Dz::Socket::ServerSocket server(8080);
    int                  finished    = 0;

    while(!finished)
    {
        Dz::Socket::DataSocket accept  = server.accept();
        printf ("New client connected\n");
        workers.push_back(std::thread(each_thread_does, std::move(accept)));
    }
    for (auto& t: workers) {
        if (t.joinable()) {
            t.join();
        }
    }
}
