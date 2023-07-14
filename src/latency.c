#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <windivert.h>

#define MAXBUF  0xFFFF
#define LAG_MIN "0"
#define LAG_MAX "1000"
#define DEFAULT_LAG 100

void applyLag(HANDLE handle, UINT lagTime) {
    WINDIVERT_ADDRESS addr;
    UINT packetLen;
    UINT sendLen;
    BYTE packet[MAXBUF];

    while (!kbhit()) {
        if (WinDivertRecv(handle, packet, sizeof(packet), &addr, &packetLen, &sendLen)) {
            Sleep(lagTime);
            if (!WinDivertSend(handle, packet, packetLen, &addr, &sendLen)) {
                printf("Failed to send packet (%d)\n", GetLastError());
                continue;
            }
        }
    }
}

void packetLagMenu(HANDLE handle) {
    int choice;
    UINT lagTime = DEFAULT_LAG;

    printf("Packet Lag Menu\n");
    printf("1. Apply 100ms Lag\n");
    printf("2. Apply 200ms Lag\n");
    printf("3. Apply 300ms Lag\n");
    printf("4. Custom Lag\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            lagTime = 100;
            break;
        case 2:
            lagTime = 200;
            break;
        case 3:
            lagTime = 300;
            break;
        case 4:
            printf("Enter the custom lag time (in milliseconds): ");
            scanf("%u", &lagTime);
            break;
        default:
            printf("Invalid choice\n");
            return;
    }

    printf("Applying lag...\n");
    applyLag(handle, lagTime);
    printf("Lag application finished\n");
}

int main() {
    HANDLE handle = WinDivertOpen("true", WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        printf("Failed to open the WinDivert device.\n");
        return 1;
    }

    int choice;
    printf("Menu:\n");
    printf("1. Packet Lag\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        packetLagMenu(handle);
    } else {
        printf("Invalid choice\n");
    }

    WinDivertClose(handle);
    return 0;
}
