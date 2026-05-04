#include "ui.h"
#include <iostream>
#include <windows.h>

void setColor(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void drawBanner()
{
    setColor(11);
    std::cout << "=====================================\n";
    std::cout << "            CORDCHAT\n";
    std::cout << "=====================================\n";
    setColor(7);
}

void printSystem(const std::string& msg)
{
    setColor(14);
    std::cout << "[SYSTEM] " << msg << "\n";
    setColor(7);
}

void printChat(const std::string& user, const std::string& msg)
{
    setColor(10);
    std::cout << "[" << user << "]";
    setColor(7);
    std::cout << ": " << msg << "\n";
}

void printDM(const std::string& user, const std::string& msg)
{
    setColor(13);
    std::cout << "[DM from " << user << "]";
    setColor(7);
    std::cout << ": " << msg << "\n";
}