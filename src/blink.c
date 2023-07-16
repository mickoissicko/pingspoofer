#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <Winsock2.h>
#include <conio.h>
#include "WinDivert.h"

#define MAXBUF 0xFFFF

void add_delay(uint8_t *packet, uint32_t delay_ms) {
    Sleep(delay_ms);
}

int main() {
    HANDLE handle;
    char filter[] = "outbound";
    char packet[MAXBUF];
    UINT packet_len;
    WINDIVERT_ADDRESS addr;
    int running = 1;

    // Initialize WinDivert
    handle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening WinDivert: %d\n", GetLastError());
        return 1;
    }

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, prevMode & ~ENABLE_PROCESSED_INPUT);

    printf("Blink is running. Press Y to stop, T to start.\n");

    while (running) {
        // Check if a key is pressed
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'Y' || ch == 'y') {
                running = 0;
                printf("Blink stopped.\n");
            } else if (ch == 'T' || ch == 't') {
                running = 1;
                printf("Blink resumed.\n");
            }
        }

        if (!running) {
            Sleep(100); // Small delay when not running to avoid high CPU usage
            continue;
        }

        if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packet_len)) {
            fprintf(stderr, "Error while receiving packet: %d\n", GetLastError());
            continue;
        }

        // Simulate latency by adding a 2-second delay to the packet
        add_delay((uint8_t*)packet, 2000);

        if (!WinDivertSend(handle, packet, packet_len, &addr, NULL)) {
            fprintf(stderr, "Error while sending packet: %d\n", GetLastError());
            continue;
        }
    }

    // Restore the original console input mode
    SetConsoleMode(hInput, prevMode);

    WinDivertClose(handle);
    return 0;
}
