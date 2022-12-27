//g++ -o testCli testCli.cpp -lpthread

#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include "thread"
using namespace std;
//Client side

void fun()
{
    //create a message buffer
    char msg[1500];
    //setup a socket and connection tools
    struct hostent* host = gethostbyname("192.168.50.218");
    sockaddr_in sendSockAddr;
    bzero((char*)&sendSockAddr, sizeof(sendSockAddr));
    sendSockAddr.sin_family = AF_INET;
    sendSockAddr.sin_addr.s_addr =
            inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    sendSockAddr.sin_port = htons(8888);
    int clientSd = socket(AF_INET, SOCK_STREAM, 0);
    //try to connect...
    int status = connect(clientSd,
                         (sockaddr*) &sendSockAddr, sizeof(sendSockAddr));
    if(status < 0)
    {
        cout<<"Error connecting to socket!"<<endl;
        return;
    }
    cout << "Connected to the server!" << endl;
    int bytesRead, bytesWritten = 0;
    struct timeval start1, end1;
    gettimeofday(&start1, NULL);

    memset(&msg, 0, sizeof(msg));//clear the buffer
    strcpy(msg, "hello");

    send(clientSd, (char*)&msg, strlen(msg), 0);
    std::this_thread::sleep_for (std::chrono::seconds(30));
    close(clientSd);
}

int main(int argc, char *argv[]){

    std::thread t[20];
    for(int i = 0; i < 20; i++){
        t[i] = std::thread(fun);
    }
    for(int i = 0; i < 20; i++){
        t[i].join();
    }
}
