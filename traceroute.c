/* This project is implementation of popular computer network protocol called traceroute. 
But what traceroute is? It's basically a protocol that trace packet's route. When packet is 
unpacked by a router then it sends a signal back to host that send orginal packet. More specifically
traceroute utulize TTL (time to live) function that when packet's is TTL = 0 then router sends back ICMP
"Time Exceeded" messege to source host. So traceroute sends packet to choosed host starting with TTL = 0 
and incrise its value for each "Time Exceeded" message (this means for each router).*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Variables
int TTL;

struct ip_addresses
{
    char src_ip[15];
    char dst_ip[15];
};

int main() {
    int n1, n2, n3, n4;
    struct ip_addresses ip_a;
    char * pointerToDstIp = ip_a.dst_ip;
    printf("Choose destination IP address.\n");
    scanf("%s", pointerToDstIp);
    int properFormat = sscanf(ip_a.dst_ip, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
    if (properFormat != 4) {
        printf("You've used wrong IP address format. Proper format is [0-225].[0-225].[0-225].[0-225]\n");
        exit(EXIT_FAILURE);
        }
        if ((n1 < 0 || n1 > 255) || (n2 < 0 || n2 > 255) || (n3 < 0 || n3 > 255) || (n4 < 0 || n4 > 255)) {
        printf("Each octet has to have value between 0 and 255.\n");
        exit(EXIT_FAILURE);
        }
        else {
        printf("Gitówa\n");
    }
    return 0;
}