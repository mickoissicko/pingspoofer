#include <stdio.h>
#include <stdint.h>
#include <Winsock2.h>
#include "J:\git\.mc\pingspoofer\src\libraries\WinDivert.h"

#define MAXBUF 0xFFFF

void add_delay_to_packet(uint8_t *packet, uint32_t delay_ms) {
    Sleep(delay_ms);
}

int main() {
    HANDLE handle = INVALID_HANDLE_VALUE;
    char filter[] = "outbound";
    char packet[MAXBUF];
    UINT packet_len;
    WINDIVERT_ADDRESS addr;

    handle = WinDivertOpen(filter, WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening WinDivert: %d\n", GetLastError());
        return 1;
    }

    printf("Blink is active.\n");

    while (1) {
        
        if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packet_len)) {
            fprintf(stderr, "Error while receiving packet: %d\n", GetLastError());
            continue;
        }
        add_delay_to_packet((uint8_t*)packet, 5);
        if (!WinDivertSend(handle, packet, packet_len, &addr, NULL)) {
            fprintf(stderr, "Error while sending packet with delay: %d\n", GetLastError());
            continue;
        }
    }

    if (handle != INVALID_HANDLE_VALUE) {
        WinDivertClose(handle);
    }

    return 0;
}
