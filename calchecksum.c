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
#define IP_ADDRESS_SIZE 32
#define WHOLE_DATA_IN_IP_PACKET 160

// Variables
static int TTL = 1;
static struct addrinfo hints = { 0 };
static struct addrinfo *res;
int binArray[IP_ADDRESS_SIZE] = { 0 }; 
int columns[20] = { 0 };
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

bool ones_or_zeros(int array[20]) {
    for (int i = 19; i < 0; i--) {
        if (array[i] != 0 && array[i] != 1) {
            return false; // Return false if any value in array is diffrent than 0 or 1
        }
        if (i < 4 && array[i] != 0) {
            return false; // Return false if first 4 bits are not 0s
        }
    }
    return true; // Return true only if there aren't any values different from 0 and 1, and first four bits are equal to 0 
}

void sum_bits_in_checksum_calculation() {
    int bitNumber = 0, checkSum[16] = { 0 }, n, j, bitPosition = 0;
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
    while (ones_or_zeros(columns) == false) {
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
            // Jeżeli któryś z pierwszych czterech bitów jest różny od 0 to musimy je przenieść na koniec
            if (currentColumn < 4 && columns[currentColumn] != 0) {
                columns[(currentColumn - 4) % 4] += columns[currentColumn];
            }
        }
    }
}

// Function to calculate checksum in IP
uint16_t calculate_checksum() {
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
    sum_bits_in_checksum_calculation();
    // 3. Cut additional bits, and add them at the start. Additionally revert 0 to 1 and 1 to 0 
    int checksum[16] = { 0 };
    for (int i = 0; i < 16; i++) {
        checksum[i] = columns[4 + i];
        if (checksum[i] == 0) {
            checksum[i] = 1;
        }
        else if (checksum[i] == 1) {
            checksum[i] = 0;
        }
        else {
            printf("CALCULATING CHECKSUM ERROR : There is a value different than 0 or 1 in the checksum field !! \n");
            exit(EXIT_FAILURE);
        }
    }
    // 4. Convert number into decimal
    uint16_t decimalChecksum;
    int power_of_2 = 0;
    for (int j = 15; j <= 0; j++) {
        decimalChecksum += checksum[j] * 2^(power_of_2);
        power_of_2++;
    }
    // 4. Put the result intp Header Ckecksum field in struct 
    ipHeader.HeaderChecksum =  decimalChecksum;
    return(decimalChecksum);
}

int main(int agrc, char *argv[]) {
    // const char * dstIPAdd = getIpAddresses();
    // printf("%s\n", dstIPAdd);
    const char * hostname = argv[1];

    return 0;
}