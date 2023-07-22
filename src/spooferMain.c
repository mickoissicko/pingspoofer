#define UNICODE
#define _UNICODE
#define _WIN32_WINNT 0x0600

#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include <Winsock2.h>
#include <conio.h>

#include "J:\\git\\.mc\\pingspoofer\\src\\libraries\\WinDivert.h"

#pragma comment(lib, "J:\\git\\.mc\\pingspoofer\\src\\libraries\\WinDivert.lib")

#define MAXBUF 0xFFFF

// Function to add delay to packets
void add_delay(uint8_t *packet, uint32_t delay_ms) {
    Sleep(delay_ms);
}

// Function to check if the program has administrator privileges
BOOL IsElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION elevation;
        DWORD dwSize;

        if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
            fRet = elevation.TokenIsElevated;
        }

        CloseHandle(hToken);
    }

    return fRet;
}

int main() {
    // Check if the program has administrator privileges
    if (!IsElevated()) {
        wprintf(L"Administrator privileges are required to run this program.\n");
        wprintf(L"Please run the program as an administrator.\n");
        return 1;
    }

    // Your existing code continues from here...
    // ...

    // Rest of the code (without the elevation check)...

    return 0;
}
