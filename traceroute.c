/* This project is implementation of popular computer network protocol called traceroute. 
But what traceroute is? It's basically a protocol that trace packet's route. When packet is 
unpacked by a router then it sends a signal back to host that send original packet. More specifically
traceroute utulize TTL (time to live) function that when packet's is TTL = 0 then router sends back ICMP
"Time Exceeded" message to source host. So traceroute sends packet to chose host starting with TTL = 0 
and incrise its value for each "Time Exceeded" message (this means for each router).*/

// https://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch https://man7.org/linux/man-pages/man3/getaddrinfo.3.html https://man7.org/linux/man-pages/man3/sockaddr.3type.html

/* 
To properly send ICMP packets that are crucial for traceroute implementation I had to implement this 10 steps:
    1.  Create a RAW socket for specifying ICMP protocol
    2.  Resolve the hostname to an address if needed (DNS)
    3.  Create ICMP packet of type 8 (Echo) 
        3.1. Fill the payload (optional)
        3.2. Calculate the checksum
    4.  Record the start time
    5.  Send out the packet 
    6.  Wait for a ICMP type 0 (Reply)
    7.  Record the end time
    8.  Check the Id byte to make sure it is a reply to our Echo packet
    9.  If the Id byte is not a match, wait for another packet if the timeout was not reached yet
    10. Print the result
I found these steps in the following link: https://github.com/janwilmans/explain_icmp_ping (in README.md file)
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <unistd.h>

// Variables
static int TTL = 1;
static char dstIp4[16];
static struct addrinfo hints = { 0 };
static struct addrinfo *res;
<<<<<<< HEAD
const char * destination;
=======
static struct in_addr dstIpAddr;
>>>>>>> cf85765d2127e4aefd8a7d227c60e219b1c35300

// CTRL + ALT + Strzałka -> dodaj nowy kursor
// CTRL + D -> dodaj kursor przy matchującym wzorcu
// CTRL + R -> mądry znajdź i zamień (refactor)

// Function to retrieve destination IPv4 address user can choose 
<<<<<<< HEAD
// const char * getIpAddresses() {
//     // Choosing destination IP address
//     int n1, n2, n3, n4;
//     char * pointerToDstIp = dstIp4;
//     printf("Choose destination IP address.\n");
//     // NOTE: scanf nie sprawdza długości bufora, do którego pisze - giga niebezpieczne! (ta funkcja zakłada, że ma odpowiednio dużo pamięci i robi co chce)
//     // NOTE: użyj read_line z biblioteki unixowej
//     scanf("%s", pointerToDstIp);
//     // Ensureing proper IP address format 
//     int properFormat = sscanf(dstIp4, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
//     if (properFormat != 4) {
//         printf("You've used wrong IP address format. Proper format is [0-225].[0-225].[0-225].[0-225]\n"); 
//         exit(EXIT_FAILURE);
//     }
//     if ((n1 < 0 || n1 > 255) || (n2 < 0 || n2 > 255) || (n3 < 0 || n3 > 255) || (n4 < 0 || n4 > 255)) {
//         printf("Each octet has to have value between 0 and 255.\n");
//         exit(EXIT_FAILURE);
//     }
//     return dstIp4;
// }

// Function to send ICMP Echo messages 
int sendEchoMess(const char * hostname) { 
    // Destination IP address will be resolved with getaddrinfo() function
    struct addrinfo hint; 
    struct addrinfo *result;
    // We have to allocate proper amount of memory to our hint struct - we fill whole struct with 0's, and fill it with proper data
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    // The rest of struct is 0

    // Step 1. Create a RAW socket for specifying ICMP protocol
    socket(AF_INET, SOCK_STREAM, IPPROTO_ICMP);

    // Step 2. Resolve the hostname to an address if needed (DNS)
    int status = getaddrinfo(hostname, NULL, &hint, &result);
    if (status != 0) {
        printf("An error has occoured: %d\n", status);
        return EXIT_FAILURE;
    }

    struct addrinfo *tmp = result;
    void *addr;
    char address_string[INET6_ADDRSTRLEN];

    if (tmp->ai_family == AF_INET) {
        addr = &((struct sockaddr_in*)tmp->ai_addr)->sin_addr;
    }

    if (tmp->ai_family == AF_INET) {
        addr = &((struct sockaddr_in6*)tmp->ai_addr)->sin6_addr;
    }

    inet_ntop(tmp->ai_family, addr, address_string, sizeof(address_string));

    printf("Address: %s\n", address_string);
    
    // At the end we have to clean up result struct info
    freeaddrinfo(result);
    return 0;
=======
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
    // Dodaj konwertację char * na uint32_t za pomocą komendy inet_pton
}

// Function to send ICMP Echo messages 
int sendEchoMess() { 
    struct sockaddr_in dstSock;
    int raw_socket;
    // Filling structs with proper data
    // Dodaj structa in_addr do którego włozysz adres docelowy z getIpAddress
    dstSock.sin_family = AF_INET;
    dstSock.sin_port = 7;
    dstSock.sin_addr = &
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_addr = &dstSock;
    // Create a RAW socket
    raw_socket = socket(AF_INET, SOCK_RAW, );
>>>>>>> cf85765d2127e4aefd8a7d227c60e219b1c35300
}

int main(int agrc, char *argv[]) {
    // const char * dstIPAdd = getIpAddresses();
    // printf("%s\n", dstIPAdd);
    const char * hostname = argv[1];
    sendEchoMess(hostname);

    return 0;
}