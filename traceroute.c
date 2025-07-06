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
int TTL = 1;

struct ip_addresses {
    char src_ip[16];
    char dst_ip[16];
};

struct ip_addresses getIpAddresses() {
    // Choosing destination IP address
    int n1, n2, n3, n4;
    // Variables for ip addresses 
    char hostSrcIp[16];
    char choosedDstIp[16];

    struct ip_addresses ip_a = { 0 };

    char * pointerToDstIp = choosedDstIp;
    printf("Choose destination IP address.\n");
    // NOTE: scanf nie sprawdza długości bufora, do którego pisze - giga niebezpieczne! (ta funkcja zakłada, że ma odpowiednio dużo pamięci i robi co chce)
    // NOTE: użyj read_line z biblioteki unixowejw
    scanf("%s", pointerToDstIp);
    printf("%s", choosedDstIp);
    // Ensureing proper IP address format 
    // TODO: zamiast %d, które oczekuje "dowolnej liczby" możesz użyć format specifier'a, który oczekuje explicite 8 bitowej liczby bez znaku [0, 255]
    int properFormat = sscanf(choosedDstIp, "%d.%d.%d.%d", &n1, &n2, &n3, &n4);
    if (properFormat != 4) {
        printf("You've used wrong IP address format. Proper format is [0-225].[0-225].[0-225].[0-225]\n");
        exit(EXIT_FAILURE);
    }
    if ((n1 < 0 || n1 > 255) || (n2 < 0 || n2 > 255) || (n3 < 0 || n3 > 255) || (n4 < 0 || n4 > 255)) {
        printf("Each octet has to have value between 0 and 255.\n");
        exit(EXIT_FAILURE);
    }
    // If user have given proper destination IP address, then program will retvie their source IP address
    else {
        
    }
    // struct ip_addresses obtainedIpAddresses = {.src_ip = ip_a.hostSrcIp,
    //                                            .dst_ip = ip_a.choosedDstIp};
    return obtainedIpAddresses;
}

int main() {
    struct ip_addresses ip;
    ip = getIpAddresses();
    printf("%s %s", ip.src_ip, ip.dst_ip);
}