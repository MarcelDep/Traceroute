/* This project is implementation of popular computer network protocol called traceroute. 
But what traceroute is? It's basically a protocol that trace packet's route. When packet is 
unpacked by a router then it sends a signal back to host that send orginal packet. More specifically
traceroute utulize TTL (time to live) function that when packet's is TTL = 0 then router sends back ICMP
"Time Exceeded" messege to source host. So traceroute sends packet to choosed host starting with TTL = 0 
and incrise its value for each "Time Exceeded" message (this means for each router).*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Variables
int TTL = 1;
char dstIp4[16];
struct sockaddr_in dstadd;

unsigned int getIpAddresses() {
    // Choosing destination IP address
    int n1, n2, n3, n4;
    char * pointerToDstIp = dstIp4;
    printf("Choose destination IP address.\n");
    // NOTE: scanf nie sprawdza długości bufora, do którego pisze - giga niebezpieczne! (ta funkcja zakłada, że ma odpowiednio dużo pamięci i robi co chce)
    // NOTE: użyj read_line z biblioteki unixowej
    scanf("%s", pointerToDstIp);
    printf("%s\n", dstIp4);
    // Ensureing proper IP address format 
    int properFormat = sscanf(dstIp4, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
    if (properFormat != 4) {
        printf("You've used wrong IP address format. Proper format is [0-225].[0-225].[0-225].[0-225]\n");
        exit(EXIT_FAILURE);
    }
    if ((n1 < 0 || n1 > 255) || (n2 < 0 || n2 > 255) || (n3 < 0 || n3 > 255) || (n4 < 0 || n4 > 255)) {
        printf("Each octet has to have value between 0 and 255.\n");
        exit(EXIT_FAILURE);
    }
    // Converting inputted IP address from char to decimal value
    inet_pton(AF_INET, dstIp4, &(dstadd.sin_addr.s_addr));
    return dstadd.sin_addr.s_addr;
}

int main() {
    unsigned int dstIPAdd = getIpAddresses();
    printf("%u\n", dstIPAdd);
    return 0;
}