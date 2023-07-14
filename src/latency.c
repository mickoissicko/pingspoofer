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
    BYTE packet[MAXBUF];

    while (!kbhit()) {
        if (WinDivertRecv(handle, packet, sizeof(packet), &addr, &packetLen)) {
            Sleep(lagTime);
        }
    }
}

int main() {
    HANDLE handle = WinDivertOpen("true", WINDIVERT_LAYER_NETWORK, 0, 0);
    if (handle == INVALID_HANDLE_VALUE) {
        printf("Failed to open the WinDivert device.\n");
        return 1;
    }

    UINT lagTime = DEFAULT_LAG;

    printf("Menu:\n");
    printf("1. Apply Lag\n");
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        printf("Enter lag time in milliseconds (0 - 1000): ");
        scanf("%u", &lagTime);
        if (lagTime > 1000) {
            printf("Invalid lag time. Setting to default (%u ms).\n", DEFAULT_LAG);
            lagTime = DEFAULT_LAG;
        }
        printf("Applying lag of %u ms.\n", lagTime);
        applyLag(handle, lagTime);
    } else {
        printf("Invalid choice.\n");
    }

    WinDivertClose(handle);
    return 0;
}
