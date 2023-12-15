#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <list>
#include <algorithm>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

constexpr int PORT = 20248;

std::list<int> clients;
std::mutex clients_mutex;

void broadcast(int sender, const std::string& message) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (int client : clients) {
        if (client != sender) {
            send(client, message.c_str(), message.size(), 0);
        }
    }
}

void handle_client(int client_socket) {
    char buffer[1024];
    int bytes_received;

    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::string message(buffer);

        if (message.find("Your IP is") == 0) {
            std::cout << "Connection from " << message.substr(11) << std::endl;
        }

        std::cout << "Received message from client " << client_socket << ": " << buffer << std::endl;

        broadcast(client_socket, message);
    }

    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.remove(client_socket);
    }

#ifdef _WIN32
    closesocket(client_socket);
#else
    close(client_socket);
#endif
}


int main() {
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error initializing winsock" << std::endl;
        return 1;
    }
#endif

    int server_socket, client_socket;
    sockaddr_in server_address{}, client_address{};
    socklen_t client_address_len = sizeof(client_address);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }
\
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind the socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        std::cerr << "Error binding to port " << PORT << std::endl;
        return 1;
    }

    if (listen(server_socket, 5) == -1) {
        std::cerr << "Error listening for connections" << std::endl;
        return 1;
    }

    std::cout << "Press ESCAPE to terminate program" << std::endl;
    std::cout << "Starting up TCP server" << std::endl;

    while (true) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);

        if (client_socket == -1) {
            std::cerr << "Error accepting connection" << std::endl;
            return 1;
        }

        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            clients.push_back(client_socket);
        }

        std::thread(handle_client, client_socket).detach();
    }

#ifdef _WIN32
    closesocket(server_socket);
    WSACleanup();
#else
    close(server_socket);
#endif

    return 0;
}