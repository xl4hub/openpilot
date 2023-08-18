#include <cstdint>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>

int  canindex = 0;
int  canSockets[256];

int vcan_open(const char *canifc)
{
    struct sockaddr_can addr;
    struct ifreq ifr;

    int s = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (s < 3) {
        fprintf(stderr, "Error while opening socket for %s\n", canifc);
        return -1;
    }

    strcpy(ifr.ifr_name, canifc);
    if (ioctl(s, SIOCGIFINDEX, &ifr) < 0) {
        fprintf(stderr, "Error while getting interface index for %s\n", canifc);
        close(s);
        return -1;
    }

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "Error while binding can=%s\n", canifc);
        close(s);
        return -1;
    }
    return s;
}

int vcan_socket(int bus)
{
    if (bus > 255) {
        return -1;
    }
    if (canSockets[bus] == 0) {
        char canifc[16];
        sprintf(canifc, "vcan%d", canindex);

        int s = vcan_open(canifc);
        if (s < 0) {
            fprintf(stderr, "No can interface for canbus %d\n", bus);
        } else {
            printf("Opened canbus %d on interface %s\n", bus, canifc);
            canindex++;
        }
        canSockets[bus] = s;
    }
    return canSockets[bus];
}

int vcan_write(int s, int msgid, const uint8_t *data, int size)
{
    struct can_frame frame;
    frame.can_id = msgid;
    frame.can_dlc = size;
    memcpy(frame.data, data, size);
    return (int)write(s, &frame, sizeof(frame));
}

void vcan_output(int bus, int msgid, const uint8_t *data, int size)
{
    int s = vcan_socket(bus);
    if (s < 3) {
        return;
    }
    int num = vcan_write(s, msgid, data, size);
#if 1
    (void)num;
#else
    static int n=0;
    if (n++ < 100) {
        printf("s=%d, vcan%d num=%d %X#", s, bus, num, msgid);
        for (int i = 0; i < size; i++) {
            printf("%02X", data[i]);
        }
        printf("\n");
    }
#endif
}
