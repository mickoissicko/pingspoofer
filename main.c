#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define PACKET_SIZE 64

// Calculate the checksum for ICMP packet
unsigned short calculateChecksum(unsigned short *buffer, int length) {
    unsigned long sum = 0;
    unsigned short *ptr = buffer;
    unsigned short checksum;

    while (length > 1) {
        sum += *ptr++;
        length -= 2;
    }

    if (length == 1) {
        sum += *(unsigned char *)ptr;
    }

    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    checksum = (unsigned short)~sum;

    return checksum;
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        fprintf(stderr, "Failed to initialize Winsock\n");
        return 1;
    }

    SOCKET sockfd;
    struct sockaddr_in dest;
    char packet[PACKET_SIZE];
    int sentBytes;

    // Create a raw socket
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd == INVALID_SOCKET) {
        fprintf(stderr, "Failed to create raw socket\n");
        WSACleanup();
        return 1;
    }

    // Set destination details
    memset(&dest, 0, sizeof(dest));
    dest.sin_family = AF_INET;

    while (1) {
        // Create an ICMP Echo Request packet
        memset(packet, 0, sizeof(packet));
        char* icmp = packet;
        icmp[0] = 8;               // Type: ICMP Echo Request
        icmp[1] = 0;               // Code
        icmp[2] = 0;               // Checksum (initially set to 0)
        icmp[3] = 0;               // Identifier (32 bits)
        icmp[4] = 0;
        icmp[5] = 0;               // Sequence number (32 bits)
        icmp[6] = 0;
        memset(&icmp[8], 0xa5, sizeof(packet) - 8);  // Data

        // Calculate the ICMP checksum
        unsigned short checksum = calculateChecksum((unsigned short *)icmp, sizeof(packet));
        icmp[2] = (checksum >> 8) & 0xff;
        icmp[3] = checksum & 0xff;

        // Delay the packet before sending
        srand((unsigned int)time(NULL));
        int delay = rand() % 500 + 500;  // Random delay between 500 and 1000 milliseconds
        Sleep(delay);

        // Send the ICMP packet
        sentBytes = sendto(sockfd, packet, sizeof(packet), 0, (struct sockaddr *)&dest, sizeof(dest));
        if (sentBytes == SOCKET_ERROR) {
            fprintf(stderr, "Failed to send ICMP packet: %d\n", WSAGetLastError());
            closesocket(sockfd);
            WSACleanup();
            return 1;
        }
    }

    closesocket(sockfd);
    WSACleanup();
    return 0;
}
