#include <stdio.h>

// Call getaddrinfo() to fill this up
// - Used to prep socket address structures for subsequent use
struct addrInfo {
    int     ai_flags;           // AI_PASSIVE, AI_CANONNAME, etc.
    int     ai_family;          // AF_INET, AF_INET6, AF_UNSPEC (IPv4 / IPv6)
    int     ai_socktype;        // SOCK_STREAM, SOCK_DGRAM
    int     ai_protocol;        // use 0 for "any"
    size_t  ai_addrlen;         // size of ai_addr in bytes
    struct sockaddr *ai_addr;   // struct sock_addr_in or _in6
    char    *ai_canonname;      // full canonical hostname
    struct addrinfo *ai_next;   // linked list, next node
};

// - Holds socket address
struct sockaddr {
    unsigned char       sa_family;   // address family, AF_xxx
    char                sa_data[14]; // 14 bytes of protocol address
};

// - Can be used in place of sockaddr when calling connection
// - Can be casted to sockaddr and vice - versa
// - IPv4 only
struct sockaddr_in {
    short int           sin_family; // Address family, AF_INET
    unsigned short int  sin_port;   // Port number
    struct in_addr      sin_addr;   // Internet Address
    unsigned char       sin_zero[8];// Same size as struct sockaddr, will be set to 0 with memset
};


// (IPv4 only)
// Internet address (historical)
struct in_addr {
    __uint32_t s_addr; // 32
};

// IPv6 variants
struct sockaddr_in6 {
    __uint16_t      sin6_family;    // address family
    __uint16_t      sin6_port;      // port number, Network Byte Order
    __uint16_t      sin6_flowinfo;  // IPv6 flow information
    struct in6_addr sin6_addr;      // IPv6 address
    __uint16_t      sin6_scope_id;  // Scope ID
};

struct in6_addr {
    unsigned char   s6_addr[16];    // IPv6 address
};

// Storage to hold structures large enough for IPv6 and IPv4 addresses
struct sockaddr_storage {
    //sa_family_t     ss_family;

    // Padding, implementation specific
    // char    __ss_pad1[_SS_PAD1SIZE];
    // __int64_t  __ss_align;
    // char    __ss_pad2[_SS_PAD2SIZE];    
};

int main() {
    int socketDescriptor;

    struct sockaddr_in sa;
    struct sockaddr_in6 sa6;

    // Converts address to a struct
    // - inet_pton -> internet addr to presentation to network
    // inet_pton(AF_INET, "10.12.110.57", &(sa.sin_addr)); // IPv4
    // inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr)); // IPv6

    // Converts struct to address
    /*
        // IPv4 
        
        char ip4[INET_ADDRSTRLEN]; // space to hold the IPv4 string
        struct sockaddr_in sa; // pretend this is loaded with something
        
        inet_ntop(AF_INET, &(sa.sin_addr), ip4, INET_ADDRSTRLEN);
        
        printf("The IPv4 address is: %s\n", ip4);
        
        
        // IPv6:
        
        char ip6[INET6_ADDRSTRLEN]; // space to hold the IPv6 string
        struct sockaddr_in6 sa6; // pretend this is loaded with something
        
        inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);
        
        printf("The address is: %s\n", ip6);
    */
}