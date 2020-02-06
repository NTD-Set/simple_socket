# simple_socket
* ver1 is pure C  socket code which i used to verify the concept
* ver2 is a simple C++ wrapper of C socket
# build 
Run-time environment: GNU/Linux 64 bits 

`cd ver2`

`make all`

# usage 

1. Go to ver2 directory
2. Run

    `./server`
    
    To start the server
3. In other terminal process, run

    `./client 127.0.0.1`

    To connect the client to the server which will be display the messages in order they were received  and save it to DATA.txt

4. To disconect a client, type 'EXIT' as user input

5. Server can work with multiple client at the same time