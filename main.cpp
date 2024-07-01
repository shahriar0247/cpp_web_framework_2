#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <fstream>

using namespace std;

int main(int arg, char *argv[])
{
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(argv[1]));
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (struct sockaddr *)&serverAddress,
         sizeof(serverAddress));

    listen(serverSocket, 5);

    int clientSocket = accept(serverSocket, nullptr, nullptr);

    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    cout << "Message from client: " << buffer
         << endl;

    const char *response = "HTTP/1.1 200 OK \
Content-Length: 88 \
Content-Type: text/html \n\n";

    std::string line, text;
    std::ifstream in("index.html");
    while (std::getline(in, line))
    {
        text += line + "\n";
    }
    const char *data = text.c_str();

    char *buffer2 = new char[strlen(response) + strlen(data) + 1 + 1];
    strcpy(buffer2, response);
    strcat(buffer2, " ");
    strcat(buffer2, data);

    send(clientSocket, buffer2, strlen(buffer2), 0);

    close(serverSocket);

    return 0;
}
