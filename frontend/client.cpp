#include <iostream>
#include <string>
#include <winsock2.h>
#include <conio.h>
#pragma comment(lib,"ws2_32.lib")

#include "../shared/config.h"
#include "../shared/protocol.h"
#include "ui.h"

SOCKET sock;
std::string username;
std::string room = DEFAULT_ROOM;

void sendPacket(const Packet& p)
{
    std::string data = serialize(p);
    send(sock, data.c_str(), (int)data.size(), 0);
}

void checkIncoming()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(sock, &readfds);

    timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    int activity = select(0, &readfds, NULL, NULL, &tv);

    if (activity > 0 && FD_ISSET(sock, &readfds))
    {
        char buffer[BUFFER_SIZE];
        ZeroMemory(buffer, BUFFER_SIZE);

        int bytes = recv(sock, buffer, BUFFER_SIZE, 0);

        if (bytes > 0)
        {
            Packet p = deserialize(buffer);

            if (p.type == PacketType::CHAT)
                printChat(p.sender, p.message);
            else
                printSystem(p.message);
        }
    }
}

int main()
{
    WSADATA data;
    WSAStartup(MAKEWORD(2,2), &data);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(SERVER_PORT);
    hint.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);

    connect(sock, (sockaddr*)&hint, sizeof(hint));

    drawBanner();

    std::cout << "Enter username: ";
    getline(std::cin, username);

    Packet login;
    login.type = PacketType::LOGIN;
    login.sender = username;
    login.room = room;
    sendPacket(login);

    std::string line;

    while (true)
    {
        checkIncoming();

        if (_kbhit())
        {
            char c = _getch();

            if (c == 13) // Enter
            {
                std::cout << "\n";

                if (line == "/quit")
                    break;

                if (!line.empty())
                {
                    Packet msg;
                    msg.type = PacketType::CHAT;
                    msg.sender = username;
                    msg.room = room;
                    msg.message = line;

                    sendPacket(msg);
                }

                line.clear();
            }
            else if (c == 8) // Backspace
            {
                if (!line.empty())
                {
                    line.pop_back();
                    std::cout << "\b \b";
                }
            }
            else
            {
                line += c;
                std::cout << c;
            }
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}