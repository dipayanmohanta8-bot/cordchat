#pragma once
#include <string>
#include <map>
#include <vector>

struct User
{
    int socket;
    std::string name;
    std::string room;
};

class UserManager
{
private:
    std::map<int, User> users;

public:
    void addUser(int sock, const std::string& name);
    void removeUser(int sock);

    bool exists(const std::string& name);

    void setRoom(int sock, const std::string& room);

    std::string getName(int sock);
    std::string getRoom(int sock);

    std::vector<User> getAll();
    std::vector<User> getRoomUsers(const std::string& room);

    int getSocketByName(const std::string& name);
};