#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

bool isKeyPressed(int keyCode)
{
    return (GetAsyncKeyState(keyCode) & 0x8000) != 0;
}

void disable_network_adapter()
{
    system("netsh interface set interface \"WiFi 2\" admin=disable");
}

void enable_network_adapter()
{
    system("netsh interface set interface \"WiFi 2\" admin=enable");
}

int main()
{
    printf("Press 'T' to disable network adapter, 'Y' to enable: ");

    while (true)
    {
        if (isKeyPressed('T'))
        {
            disable_network_adapter();
        }
        else if (isKeyPressed('Y'))
        {
            enable_network_adapter();
        }
    }

    return 0;
}
