#include "user_manager.h"
#include "../shared/config.h"

void UserManager::addUser(int sock, const std::string& name)
{
    users[sock] = { sock, name, DEFAULT_ROOM };
}

void UserManager::removeUser(int sock)
{
    users.erase(sock);
}

bool UserManager::exists(const std::string& name)
{
    for (auto& u : users)
        if (u.second.name == name)
            return true;

    return false;
}

void UserManager::setRoom(int sock, const std::string& room)
{
    if (users.count(sock))
        users[sock].room = room;
}

std::string UserManager::getName(int sock)
{
    if (users.count(sock))
        return users[sock].name;

    return "";
}

std::string UserManager::getRoom(int sock)
{
    if (users.count(sock))
        return users[sock].room;

    return "";
}

std::vector<User> UserManager::getAll()
{
    std::vector<User> v;

    for (auto& x : users)
        v.push_back(x.second);

    return v;
}

std::vector<User> UserManager::getRoomUsers(const std::string& room)
{
    std::vector<User> v;

    for (auto& x : users)
        if (x.second.room == room)
            v.push_back(x.second);

    return v;
}

int UserManager::getSocketByName(const std::string& name)
{
    for (auto& x : users)
        if (x.second.name == name)
            return x.second.socket;

    return -1;
}