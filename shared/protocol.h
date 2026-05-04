#pragma once
#include <string>
#include <sstream>

enum class PacketType
{
    LOGIN,
    CHAT,
    JOIN,
    USERS,
    DM,
    SYSTEM,
    EXIT
};

struct Packet
{
    PacketType type;
    std::string sender;
    std::string room;
    std::string target;
    std::string message;
};

inline std::string serialize(const Packet& p)
{
    std::stringstream ss;

    ss
        << (int)p.type << "|"
        << p.sender << "|"
        << p.room << "|"
        << p.target << "|"
        << p.message;

    return ss.str();
}

inline Packet deserialize(const std::string& data)
{
    Packet p;
    std::stringstream ss(data);
    std::string token;

    getline(ss, token, '|');
    p.type = (PacketType)std::stoi(token);

    getline(ss, p.sender, '|');
    getline(ss, p.room, '|');
    getline(ss, p.target, '|');
    getline(ss, p.message);

    return p;
}