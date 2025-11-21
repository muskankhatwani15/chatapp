#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

bool running = true;

void receive_messages(SOCKET socket) {
    char buffer[1024];
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(socket, buffer, sizeof(buffer) - 1, 0);
        
        if (bytes <= 0) {
            std::cout << "\nDisconnected from server\n";
            running = false;
            break;
        }
        
        buffer[bytes] = '\0';
        std::cout << buffer;
        std::cout.flush();
    }
}

int main(int argc, char* argv[]) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
    
    std::string server_ip = "127.0.0.1";
    int port = 8080;
    
    if (argc > 1) {
        server_ip = argv[1];
    }
    if (argc > 2) {
        port = std::atoi(argv[2]);
    }
    
    SOCKET client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket == INVALID_SOCKET) {
        std::cerr << "Failed to create socket\n";
        WSACleanup();
        return 1;
    }
    
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    
    if (inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    
    if (connect(client_socket, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Failed to connect to server\n";
        closesocket(client_socket);
        WSACleanup();
        return 1;
    }
    
    std::cout << "Connected to chat server!\n";
    std::cout << "Enter your name: ";
    std::string name;
    std::getline(std::cin, name);
    
    send(client_socket, name.c_str(), name.length(), 0);
    
    std::cout << "\n=== Chat Started ===\n";
    std::cout << "Type your messages (Ctrl+C to exit)\n\n";
    
    std::thread receive_thread(receive_messages, client_socket);
    
    std::string message;
    while (running && std::getline(std::cin, message)) {
        if (message.empty()) continue;
        
        message += "\n";
        if (send(client_socket, message.c_str(), message.length(), 0) == SOCKET_ERROR) {
            std::cout << "Failed to send message\n";
            break;
        }
    }
    
    running = false;
    closesocket(client_socket);
    
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
    
    WSACleanup();
    return 0;
}
