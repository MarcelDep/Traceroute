/* This project is implementation of popular computer network protocol called traceroute. 
But what traceroute is? It's basically a protocol that trace packet's route. When packet is 
unpacked by a router then it sends a signal back to host that send orginal packet. More specifically
traceroute utulize TTL (time to live) function that when packet's is TTL = 0 then router sends back ICMP
"Time Exceeded" messege to source host. So traceroute sends packet to choosed host starting with TTL = 0 
and incrise its value for each "Time Exceeded" message (this means for each router).*/

// https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch https://man7.org/linux/man-pages/man3/getaddrinfo.3.html https://man7.org/linux/man-pages/man3/sockaddr.3type.html

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>

// Variables
static int TTL = 1;
static char dstIp4[16];
static struct addrinfo hints = { 0 };
static struct addrinfo *res;

// CTRL + ALT + Strzałka -> dodaj nowy kursor
// CTRL + D -> dodaj kursor przy matchującym wzorcu
// CTRL + R -> mądry znajdź i zamień (refactor)

// Function to retrieve destination IPv4 address user can choose 
const char * getIpAddresses() {
    // Choosing destination IP address
    int n1, n2, n3, n4;
    char * pointerToDstIp = dstIp4;
    printf("Choose destination IP address.\n");
    // NOTE: scanf nie sprawdza długości bufora, do którego pisze - giga niebezpieczne! (ta funkcja zakłada, że ma odpowiednio dużo pamięci i robi co chce)
    // NOTE: użyj read_line z biblioteki unixowej
    scanf("%s", pointerToDstIp);
    // Ensureing proper IP address format 
    int properFormat = sscanf(dstIp4, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
    if (properFormat != 4) {
        printf("You've used wrong IP address format. Proper format is [0-225].[0-225].[0-225].[0-225]\n");
        // printf("Trying to resolve domain name...");
        exit(EXIT_FAILURE);
    }
    if ((n1 < 0 || n1 > 255) || (n2 < 0 || n2 > 255) || (n3 < 0 || n3 > 255) || (n4 < 0 || n4 > 255)) {
        printf("Each octet has to have value between 0 and 255.\n");
        // printf("Trying to resolve domain name...");
        exit(EXIT_FAILURE);
    }
    return dstIp4;
}

// Function to send ICMP Echo messages 
int sendEchoMess() { 
    struct sockaddr_in dstadd;
    // Filling structs with proper data
    dstadd.;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_addr = &dstadd;
    // Create a RAW socket
    int socket();
}

int main() {
    const char * dstIPAdd = getIpAddresses();
    printf("%s\n", dstIPAdd);

    return 0;
}