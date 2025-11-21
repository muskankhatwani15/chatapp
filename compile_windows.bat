@echo off
echo Compiling Windows Chat Application...
echo.

echo Checking for g++ compiler...
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ not found!
    echo.
    echo Please install MinGW or MSYS2:
    echo - MinGW: https://sourceforge.net/projects/mingw-w64/
    echo - MSYS2: https://www.msys2.org/
    echo.
    pause
    exit /b 1
)

echo Compiling chat_server.exe...
g++ -std=c++17 -o chat_server.exe chat_server_windows.cpp -lws2_32
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile chat_server.exe
    pause
    exit /b 1
)

echo Compiling chat_client.exe...
g++ -std=c++17 -o chat_client.exe chat_client_windows.cpp -lws2_32
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to compile chat_client.exe
    pause
    exit /b 1
)

echo.
echo SUCCESS! Compiled successfully.
echo.
echo To run:
echo   1. Start server: chat_server.exe
echo   2. Start clients: chat_client.exe (in separate windows)
echo.
pause
