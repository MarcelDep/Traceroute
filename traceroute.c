/* This project is implementation of popular computer network protocol called traceroute. 
But what traceroute is? It's basically a protocol that trace packet's route. When packet is 
unpacked by a router then it sends a signal back to host that send orginal packet. More specifically
traceroute utulize TTL (time to live) function that when packet's is TTL = 0 then router sends back ICMP
"Time Exceeded" messege to source host. So traceroute sends packet to choosed host starting with TTL = 0 
and incrise its value for each "Time Exceeded" message (this means for each router).*/

// Variables

int TTL;

struct ip_addresses
{
    char src_ip[12];
    char dst_ip[12];
};

// In this function we are going to get a source IP address of user PC and destination IP address user can choose

struct ip_addresses get_IP_Addresses() {
    
}

int main() {
    
}
