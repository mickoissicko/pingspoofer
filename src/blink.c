#include <stdio.h>
#include <windows.h>
#include <windivert.h>

#define MAXBUF  0xFFFF

int main() {
    HANDLE handle;
    UINT packetLen;
    BYTE packet[MAXBUF];
    WINDIVERT_ADDRESS addr;

    // Open the WinDivert device
    handle = WinDivertOpen("true", WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        printf("Failed to open the WinDivert device.\n");
        return 1;
    }

    // Flag to indicate whether to seize packets
    BOOL seizePackets = FALSE;

    // Loop to intercept and stop outbound packets
    while (1) {
        // Receive a packet
        if (!WinDivertRecv(handle, packet, sizeof(packet), &addr, &packetLen)) {
            printf("Failed to receive packet (%d)\n", GetLastError());
            continue;
        }

        // Modify or process the packet as needed

        // Check if packet seizing is enabled
        if (seizePackets) {
            // Do not send the packet (seize it)
            continue;
        }

        // Send the packet
        if (!WinDivertSend(handle, packet, packetLen, &addr, NULL)) {
            printf("Failed to send packet (%d)\n", GetLastError());
            continue;
        }
    }

    // Close the WinDivert handle
    WinDivertClose(handle);

    return 0;
}
