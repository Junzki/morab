// server.cpp - Morab CGI Server
#include "morab.h"


#pragma comment(lib, "Ws2_32.lib")

// TODO:
// - Add socket server for windows/*nix platforms.
//   - Windows: WinSock2 + Windows Select
//   - *nix: BSD Socket + Select
//   - `Libevent` is considerable for alternative.

int __cdecl main(void)
{
    WSADATA data;
    if (0 != WSAStartup(MAKEWORD(2, 2), &data))
    {
        return 1;
    }


    WSACleanup();
    return 0;
}
