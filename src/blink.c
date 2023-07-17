#include <stdio.h>
#include <stdint.h>
#include <Winsock2.h>
#include <conio.h>
#include "J:\git\.mc\pingspoofer\src\libraries\WinDivert.h"

#pragma (comment,lib J:\git\.mc\pingspoofer\src\libraries\WinDivert.lib)

#define MAXBUF 0xFFFF

void add_delay(uint8_t *packet, uint32_t delay_ms) {
    Sleep(delay_ms);
}

int main() {
    HANDLE handle = INVALID_HANDLE_VALUE;
    char filter[] = "outbound";
    char packet[MAXBUF];
    UINT packet_len;
    WINDIVERT_ADDRESS addr;
    int running = 0;

    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prevMode;
    GetConsoleMode(hInput, &prevMode);
    SetConsoleMode(hInput, prevMode & ~ENABLE_PROCESSED_INPUT);

    printf("Currently packet seizing is off. Press Y to stop, T to start.\n");

    while (1) {
        // keybind listener
        if (GetAsyncKeyState('Y') & 0x8000) {
            if (running) {
                running = 0;
                printf("Packet seizing is off.\n");
                if (handle != INVALID_HANDLE_VALUE) {
                    WinDivertClose(handle);
                    handle = INVALID_HANDLE_VALUE;
                }
            }
        }

        // keybind listener
        if (GetAsyncKeyState('T') & 0x8000) {
            if (!running) {
                running = 1;
                printf("Blink is active.\n");

                // Initialize WinDivert
                handle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
                if (handle == INVALID_HANDLE_VALUE) {
                    fprintf(stderr, "Error opening WinDivert: %d\n", GetLastError());
                    return 1;
                }
            }
        }

        if (!running) {
            Sleep(10); // delay for keystroke listener
            continue;
        }

        if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packet_len)) {
            fprintf(stderr, "Error while receiving packet: %d\n", GetLastError());
            continue;
        }

        // keystroke response time delay
        add_delay((uint8_t*)packet, 5);

        if (!WinDivertSend(handle, packet, packet_len, &addr, NULL)) {
            fprintf(stderr, "Error while sending packet BLINK ON: %d\n", GetLastError());
            continue;
        }
    }

    SetConsoleMode(hInput, prevMode);

    if (handle != INVALID_HANDLE_VALUE) {
        WinDivertClose(handle);
    }

    return 0;
}