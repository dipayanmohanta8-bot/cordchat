@echo off
echo Building CordChat...
echo.

g++ backend\server.cpp backend\user_manager.cpp backend\room_manager.cpp -o server.exe -lws2_32 -std=gnu++17

g++ frontend\client.cpp frontend\ui.cpp -o client.exe -lws2_32 -std=gnu++17

echo.
echo Build complete.
pause
