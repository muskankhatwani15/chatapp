#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

struct Client {
    SOCKET socket;
    std::string name;
};

std::vector<Client> clients;
std::mutex clients_mutex;

void broadcast_message(const std::string& message, SOCKET sender_socket) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    for (const auto& client : clients) {
        if (client.socket != sender_socket) {
            send(client.socket, message.c_str(), message.length(), 0);
        }
    }
}

void handle_client(SOCKET client_socket) {
    char buffer[1024];
    std::string client_name;

    // Get client name
    int bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes <= 0) {
        closesocket(client_socket);
        return;
    }
    buffer[bytes] = '\0';
    client_name = std::string(buffer);

    // Add client to list
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.push_back({ client_socket, client_name });
    }

    std::string join_msg = "*** " + client_name + " joined the chat ***\n";
    std::cout << join_msg;
    broadcast_message(join_msg, client_socket);

    // Handle messages
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0) {
            break;
        }

        buffer[bytes] = '\0';
        std::string message = client_name + ": " + std::string(buffer);
        std::cout << message;
        broadcast_message(message, client_socket);
    }

    // Remove client and notify others
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        clients.erase(
            std::remove_if(clients.begin(), clients.end(),
                [client_socket](const Client& c) { return c.socket == client_socket; }),
            clients.end()
        );
    }

    std::string leave_msg = "*** " + client_name + " left the chat ***\n";
    std::cout << leave_msg;
    broadcast_message(leave_msg, INVALID_SOCKET);

    closesocket(client_socket);
}

int main(int argc, char* argv[]) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    int port = 8080;
    if (argc > 1) {
        port = std::atoi(argv[1]);
    }

    SOCKET server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket\n";
        WSACleanup();
        return 1;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    if (listen(server_socket, 10) == SOCKET_ERROR) {
        std::cerr << "Failed to listen\n";
        closesocket(server_socket);
        WSACleanup();
        return 1;
    }

    std::cout << "Chat server started on port " << port << "\n";
    std::cout << "Waiting for clients...\n\n";

    while (true) {
        sockaddr_in client_addr{};
        int client_len = sizeof(client_addr);
        SOCKET client_socket = accept(server_socket, (sockaddr*)&client_addr, &client_len);

        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Failed to accept client\n";
            continue;
        }

        std::thread client_thread(handle_client, client_socket);
        client_thread.detach();
    }

    closesocket(server_socket);
    WSACleanup();
    return 0;
}
