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

// CTRL + ALT + Strzałka -> dodaj nowy kursor
// CTRL + D -> dodaj kursor przy matchującym wzorcu
// CTRL + R -> mądry znajdź i zamień (refactor)

#define _XOPEN_SOURCE 700
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
#include <stdbool.h>
#include <time.h>

#define MAX_DATA_SIZE_IN_IP_HEADER 40 // In IP header data field can have maximum of 40 bytes  
#define BYTES_USED_TO_CALC_CHECKSUM_IN_ICMP 16

// Variables
static int TTL = 1;
static struct addrinfo hints = { 0 };
static struct addrinfo *res; 

struct IpHeader {
    unsigned int version : 4;
    unsigned int IHL : 4;
    uint8_t TOS;
    uint16_t TotalLenght;
    uint16_t Identification;
    unsigned int Flags : 3;
    unsigned int FragmentOffset : 13; 
    uint8_t TTL;
    uint8_t Protocol;
    uint16_t HeaderChecksum;
    uint32_t SourceAddress;
    uint32_t DestinationAddress;
    char options[MAX_DATA_SIZE_IN_IP_HEADER];
} ipHeader;
int ICMPBytes[BYTES_USED_TO_CALC_CHECKSUM_IN_ICMP];
int currentBit = 0;

// CTRL + ALT + Strzałka -> dodaj nowy kursor
// CTRL + D -> dodaj kursor przy matchującym wzorcu
// CTRL + R -> mądry znajdź i zamień (refactor)

// Convert decimal numbers into their binary representation
void decimal_to_binary(uint8_t number) {
    int i, n = number, binArray[16];
    memset(binArray, 0, sizeof(binArray)); // Before we calculate binary representation, we have to fill whole array with 0's
>>>>>>> e647d4be3b3335d3ddf4657251fbed5ed22d24dc
    for (i = 0; n > 0; i++) {
        binArray[i] = n % 2;
        n = n / 2;
        i++;
    }
    for (i = size; i >= 0; i--) {
        IPHeaderBinVersion[currentBit] = binArray[i];
        currentBit++;
    }
    for (int j = i - 1; j >= 0; j--) {
        ICMPBytes[currentBit] = binArray[j];
        currentBit++;
    }
}

// Calculate checksum in ICMP
uint16_t calculate_icmp_checksum(uint8_t type, uint8_t code) {
    // 1. Change all numbers into binary representation
    decimal_to_binary(type);
    decimal_to_binary(code);
    // 2. Convert number into decimal
    uint16_t decimalChecksum;
    int power_of_2 = 0;
    for (int j = 0; j <= 15; j++) {
        decimalChecksum += ICMPBytes[j] * 2^(power_of_2);
        power_of_2++;
    }
    // 3. Put the result into ICMP ckecksum field in struct 
    return(decimalChecksum);
}

// Returns if ICMP packet has reached its destination
bool ICMP_time_exceeded() {
    if () {
        return false;
    }
    else {
        return true;
    }
}

// Send ICMP Echo messages 
void send_echo_mess(const char * hostname) { 
    // Destination IP address will be resolved with getaddrinfo() function
    struct addrinfo hint; 
    struct addrinfo *result;
    // We have to allocate proper amount of memory to our hint struct - we fill whole struct with 0's, and fill it with proper data
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_RAW;
    hint.ai_protocol = IPPROTO_ICMP;
    hints.ai_flags = AI_PASSIVE;
    // The rest of struct is 0

    // Step 1. Resolve the hostname to an address if needed (DNS)
    int status = getaddrinfo(hostname, NULL, &hint, &result);
    if (status != 0) {
        printf("An error has occoured: %d\n", status);
        exit(EXIT_FAILURE);
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
    
    // If DNS reseolved hostname into an IP address then, we have to write it as a dst IP address in our struct
    if (address_string != NULL) {
        hint.ai_addr = address_string;
    }

    // Step 2. Create a RAW socket for specifying ICMP protocol
    int sockfd = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (sockfd == -1) {
        printf("SOCKET ERROR !! %d\n\r", sockfd);
        exit(EXIT_FAILURE);
    }

    // Step 3. Creating an ICMP packet of type 8
    struct icmppkt {
    uint8_t type;
    uint8_t code;
    u_int16_t checksum;
    u_int16_t identifier;
    } icmppkt;

    icmppkt.type = 8; // Echo request
    icmppkt.code = 0; // Code for echo requests 
    icmppkt.checksum = calculate_icmp_checksum(icmppkt.type, icmppkt.code); // Calculated ckecksum
    icmppkt.identifier = 100; // ICMP packet identifier
    char * sendbuff = sizeof(icmppkt); // Buffer used to send message
    char * recvbuff = sizeof(icmppkt); // Buffer used to read message

    // 4.  Record the start time of traceroute
    clock_t whole_time_start;
    whole_time_start = clock();

    // Loop that ends when package reaches its destination
    while (ICMP_time_exceeded() == true) {
        // 5.  Send out the packet 
        int failedPackets = 0;
        ssize_t answer; 
        int sent_packet = sendto(sockfd, sendbuff, sizeof(sendbuff), 0, hint.ai_addr, sizeof(hint.ai_addr));
        // If you haven't succeed with sending ICMP packets report an error, else wait for an answer 
        if (sent_packet < 0) {
            printf("SENDING ERROR !! %d\n\r", sent_packet);
            exit(EXIT_FAILURE);
        } 
        else {
            printf("Sent %d byte packet... ", bytes_sent);
            // 5.1  Record the start time of single packet 
            clock_t start;
            start = clock();
            // 5.2  Wait (listen) for a ICMP type 0 (Reply)
            answer = recvfrom(sockfd, recvbuff, sizeof(recvbuff), 0, hint.ai_addr, sizeof(hint.ai_addr));
            if (answer < 0) {
                printf("REVIVING ERROR !! %d\n\r", answer);
                exit(EXIT_FAILURE);
            }
            // 5.3  Check the Id byte to make sure it is a reply to our Echo packet
            
            // 5.4  If the Id byte is not a match, wait for another packet if the timeout was not reached yet

            // 5.5  Record the end time
            clock_t end;
            end = clock();

            // 5.6 Print results

        }
    } 
    
    // 7.  Record the end time
    clock_t whole_time_end;
    whole_time_end = clock();

    // 8. Print the result
    
    // At the end we have to clean up result struct info
    freeaddrinfo(result);
}

int main(int agrc, char *argv[]) {
    // const char * dstIPAdd = getIpAddresses();
    // printf("%s\n", dstIPAdd);
    const char * hostname = argv[1];
    send_echo_mess(hostname);

    return 0; 
}