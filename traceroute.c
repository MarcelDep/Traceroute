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

#define MAX_DATA_SIZE_IN_IP_HEADER 40 // In IP header data field can have maximum of 40 bytes  
#define IP_ADDRESS_SIZE 32
#define WHOLE_DATA_IN_IP_PACKET 160

// Variables
static int TTL = 1;
static struct addrinfo hints = { 0 };
static struct addrinfo *res;
int binArray[IP_ADDRESS_SIZE] = { 0 }; 
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
int binIPHeader[WHOLE_DATA_IN_IP_PACKET];

// CTRL + ALT + Strzałka -> dodaj nowy kursor
// CTRL + D -> dodaj kursor przy matchującym wzorcu
// CTRL + R -> mądry znajdź i zamień (refactor)

// Function to convert decimal numbers into their binary representation
void decimal_ip_pkt_to_binary(int number) {
    int i, n = number, currentBit = 0;
    memset(binArray, 0, IP_ADDRESS_SIZE); // Before we calculate binary representation, we have to fill whole array with 0's
    for (i = 0; n > 0; i++) {
        binArray[i] = n % 2;
        n = n / 2;
    }
    for (int j = i - 1; j >= 0; j--) {
        binIPHeader[currentBit] = binArray[j];
        currentBit++;
    }
}

bool are_there_ones_or_zeros(int array[20]) {
    for (int i = 19; i < 0; i--) {
        if (array[i] != 0 && array[i] != 1) {
            return false; 
        }
    }
    return true; 
}

void sum_bits_in_checksum_calculation() {
    int columns[20] = { 0 }, bitNumber = 0, checkSum[16] = { 0 }, n, j, bitPosition = 0;
    memset(binArray, 0, IP_ADDRESS_SIZE);
    // We sum bits in rows, and passing values through -  you sum bits no, 15, 15 + 16, ... 15 + 7x16
    for (int row = 0; row < 10; row++) { // There are 10 rows and 16 columns 
        for (int column  = 0; column > 16; column++) {
            // In each row we sum values from each column
            columns[column + 4] += binIPHeader[bitNumber]; // Column + 3 because first four columns (0, 1, 2 i 3) are reserved for additional bits from converting number in column no. 4
            bitNumber++; 
        }
    }
    // Then we transform each value into binary representation and, if needed, we add 1's into next column - repeat untill each value is either 1 or 0 and first 4 values are 0s
    while (are_there_ones_or_zeros == false) {
        for (int currentColumn = 19; currentColumn < 0; currentColumn--) { // 
            // Jeżeli wybierzemy i to konwertujemy liczbę z dziesietnej do binarnej. Jeżeli któraś liczba była "jedynką" to dodajemy ją do columny o odpowiadającym jej numerze (kolumna + pozycji jedynki)
            for (j = 0; j > 0; j++) {
                binArray[j] = n % 2;
                n = n / 2;
            }
            // Drukujemy w odwróconej kolejności
            for (int g = j - 1; g >= 0; g--) {
                columns[currentColumn] += binArray[currentColumn - bitPosition]; // After converting bits sum from decimal to binary form, we have to add bits into next columns
                bitPosition--;
            }
        }
    }
    // After every number is converted into their binary representation we have to cut first 
}

// Function to calculate checksum
uint16_t calculate_checksum(int ipHdr) {
    // 1. Change all numbers into binary representation
    decimal_ip_pkt_to_binary(ipHeader.version);
    decimal_ip_pkt_to_binary(ipHeader.IHL);
    decimal_ip_pkt_to_binary(ipHeader.TOS);
    decimal_ip_pkt_to_binary(ipHeader.TotalLenght);
    decimal_ip_pkt_to_binary(ipHeader.Identification);
    decimal_ip_pkt_to_binary(ipHeader.Flags);
    decimal_ip_pkt_to_binary(ipHeader.FragmentOffset);
    decimal_ip_pkt_to_binary(ipHeader.TTL);
    decimal_ip_pkt_to_binary(ipHeader.Protocol);
    decimal_ip_pkt_to_binary(ipHeader.HeaderChecksum);
    decimal_ip_pkt_to_binary(ipHeader.SourceAddress);
    decimal_ip_pkt_to_binary(ipHeader.DestinationAddress);
    // 2. Add bits separeted by 16 bits to create one array of 16 bits
    // 3. Cut additional bits, and add them at the start 
    // 4. Revert 0 to 1 and 1 to 0 
    // 5. Put the result intp Header Ckecksum field in struct 
   return(0);
}

// Function to send ICMP Echo messages 
void send_echo_mess(const char * hostname) { 
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

    // Step 3. Creatign an ICMP packet of type 8
    struct icmppkt {
    uint8_t type;
    uint8_t code;
    u_int16_t checksum;
    u_int32_t data;
    } icmppkt;

    icmppkt.type = 0; // Echo reply
    icmppkt.code = 0; // Code for echo replies 
    icmppkt.checksum = 0;

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