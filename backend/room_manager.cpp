#include "room_manager.h"
#include <fstream>

void RoomManager::saveMessage(const std::string& line)
{
    std::ofstream file("data/history.txt", std::ios::app);

    if (file.is_open())
        file << line << "\n";
}