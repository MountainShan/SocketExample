/***************SimpelSniffer.c*************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <net/if.h>
#include <sys/ioctl.h>

#define BUFFER_MAX 65535
#define TRUE 1

int main(int argc, char *argv[]) {
    int sock, n_read, i;        
    uint8_t buffer[BUFFER_MAX];
    struct sockaddr_ll sll;
    struct ifreq ifr;
    n_read = 0;
    if((sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP))) < 0) {
        fprintf(stdout, "create socket error/n");
        exit(0);
    }
    bzero(&sll, sizeof(struct sockaddr_ll));
    bzero(&ifr, sizeof(struct ifreq));
    /* First Get the Interface Index */
    strncpy((char *)ifr.ifr_name, "enp2s0", IFNAMSIZ);
    if((ioctl(sock, SIOCGIFINDEX, &ifr)) == -1) {
        printf("Error getting Interface index !\n"); exit(-1);
    }

    /* Bind our raw socket to this interface */
    sll.sll_family = AF_PACKET;
    sll.sll_ifindex = ifr.ifr_ifindex;
    sll.sll_protocol = htons(ETH_P_IP);
    if((bind(sock, (struct sockaddr *)&sll, sizeof(sll)))== -1)
    {
        perror("Error binding raw socket to interface\n");
        exit(-1);
    }
        
    /*while(TRUE) {
        memset(buffer,'\0',BUFFER_MAX);
        n_read = read(sock, buffer, BUFFER_MAX);
        fprintf(stdout, "Success: %d\n", n_read);
        for (i=0;;i++) {
            if (buffer[i] == '\0')break;
            fprintf(stdout, "%x ", buffer[i]);
        }
        printf("\n");
    }*/
    for(i=0;i<60;i++){
        buffer[i] = 0x01;
    }
    for(i=0;i<strlen(buffer);i++){
        printf("%x ", buffer[i]);
    }   
    printf("\n");
    if (n_read = write(sock, &buffer, strlen(buffer)) != strlen(buffer)){
        printf("Could only send %d bytes of packet of length %d\n", n_read, strlen(buffer));
        exit(-1);
    }


    return 0;
}
