#include <iostream>
#include <vector>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include "../shared/config.h"
#include "../shared/protocol.h"
#include "user_manager.h"
#include "room_manager.h"

UserManager userManager;
RoomManager roomManager;

void sendPacket(int sock, const Packet& p)
{
    std::string data = serialize(p);
    send(sock, data.c_str(), (int)data.size(), 0);
}

void broadcastRoom(const std::string& room, const Packet& p)
{
    std::vector<User> users = userManager.getRoomUsers(room);

    for (size_t i = 0; i < users.size(); ++i)
        sendPacket(users[i].socket, p);
}

int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);

    SOCKET listener = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(SERVER_PORT);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listener, (sockaddr*)&hint, sizeof(hint));
    listen(listener, SOMAXCONN);

    std::vector<SOCKET> clients;

    std::cout << "CordChat server running on port " << SERVER_PORT << "\n";

    while (true)
    {
        fd_set readfds;
        FD_ZERO(&readfds);

        FD_SET(listener, &readfds);

        for (size_t i = 0; i < clients.size(); ++i)
            FD_SET(clients[i], &readfds);

        select(0, &readfds, NULL, NULL, NULL);

        if (FD_ISSET(listener, &readfds))
        {
            SOCKET client = accept(listener, NULL, NULL);
            clients.push_back(client);
            std::cout << "New client connected\n";
        }

        char buffer[BUFFER_SIZE];

        for (size_t i = 0; i < clients.size();)
        {
            SOCKET client = clients[i];

            if (!FD_ISSET(client, &readfds))
            {
                ++i;
                continue;
            }

            ZeroMemory(buffer, BUFFER_SIZE);

            int bytes = recv(client, buffer, BUFFER_SIZE, 0);

            if (bytes <= 0)
            {
                closesocket(client);
                clients.erase(clients.begin() + i);
                continue;
            }

            Packet p = deserialize(buffer);

            if (p.type == PacketType::LOGIN)
                userManager.addUser(client, p.sender);
            else if (p.type == PacketType::CHAT)
                broadcastRoom(p.room, p);
            else if (p.type == PacketType::USERS)
            {
                std::vector<User> users = userManager.getAll();
                std::string list = "Online: ";

                for (size_t j = 0; j < users.size(); ++j)
                    list += users[j].name + " ";

                Packet reply;
                reply.type = PacketType::SYSTEM;
                reply.message = list;
                sendPacket(client, reply);
            }

            ++i;
        }
    }
}
