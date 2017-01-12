/* Joseph Garcia */
/* ECE 331 HW 8 */
/* Sheaff */

1.
	Network Address = (IP & NM) = 45.23.0.0
	Broadcast Address = (IP | ~NM (one's complement)) = 45.23.63.255

2. 
	Total number of addresses: 16384
	Number of assignable addresses: 16382 

3.
	a) 
		http://www.thegeekstuff.com/2011/12/c-socket-programming/
	b)
		The tutorial is completed.
	c) 
		(1). socket(int domain, int type, int protocol): The function to create an unbounded socket in some communications domain. The user specifies the address family used in the domain using "domain", the type of socket communication semantics using "type", and the communications protocol is specified using "protocol", often 0 (results in default protocol for the address family and type being used). Returns a positive integer (the socket descriptor) on success, otherwise -1 is returned. 

		(2) bind(int sockfd, const struct sockaddr *addr,socklen_t addrlen): In this function, the address specified by "addr" is assigned to the socket that has socket descriptor "sockfd". "addrlen" is size (in bytes) of the sockaddr structure to which "addr" points. Returns 0 on success, -1 returned otherwise.

		(3) listen(int socket, int backlog): The listen() function marks the socket described by "sockfd" as a listening (passive) socket. The "backlog" term is the length of the waiting connections queue. If the number of waiting connections exceeds "backlog", new clients that attempt to connect will be rejected. Returns 0 on successful completion, -1 otherwise. 

		(4) accept(int socket, struct sockaddr *restrict address, socklen_t *restrict address_len): A listening socket (that called listen() before) "socket" takes the first connection on the pending connections list (backlog) and creates a new socket of the same socket type protocol and address family as the listening socket. "address" is either NULL or points to the structure (of length "address_len") which holds the address of the newly connected socket. Function returns 0 on success, -1 on failure. 

		(5)  getaddrinfo(const char *restrict nodename, const char *restrict servname, const struct addrinfo *restrict hints, struct addrinfo **restrict res):     getaddrinfo() generates a "dynamically allocated linked list" of addrinfo structures from a human-readable address form such as a hostname or IP address. "service" is often a port number or service name, "hints" is either NULL or an addrinfo struct pointer to help specify service (e.g. TCP), and "res" points to the linked list of addrinfo structs. 0 returned on success, -1 otherwise. 

		(6) connect(int socket, const struct sockaddr *address, socklen_t address_len): The connect() function is used to create a connection with a socket that has socket descriptor "socket", binding it to an address if "socket" has not been bound to a local address. "address" points to a sockaddr structure that contains the "peer" socket's address, and the length of the sockaddr struct is stored in "address_len". Function returns 0 on sucess, -1 otherwise. 

4.
#!/usr/bin/perl    # The shebang line

use strict;
use warnings;     # to help with debugging

my $arrlen = @ARGV;   # find number of arguments passed

if ($arrlen > 1) {
        print "Too many value passed, quitting program.\n"; # make sure only one value is passed
        die;
}

if (($ARGV[0] < 0) or ($ARGV[0] > ((2**32) - 1))) {
        print "Invalid number, quitting program\n";   # check that we have an unsigned integer (32 bits) that was passed
        die;
}

printf "%b\n", $ARGV[0];          # print the acceptable value in binary

5.
#!/usr/bin/perl

#User passes in ip address for first parameter, netmask for second parameter

my $ipaddr = $ARGV[0];
my $netmask = $ARGV[1];

foreach my $var (@ARGV) {
        if($var =~ /^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})$/) { #capture each component of the address`
                if( (($1 < 0) or ($1 > 255)) or (($2 < 0) or ($2 > 255)) or (($3 < 0) or ($3 > 255)) or (($4 < 0) or ($4 > 255))) {
                        print "ip address is no good.\n";  #check to make sure legitimate IP address was passed.
                        die;
                }
        }
}

my @ipaddls = split("[.]", $ipaddr);
my @netmasls = split("[.]", $netmask);  # take ip addresses, separate the four components

my $ip_val = unpack("I", pack("CCCC",@ipaddls));  #take 4 sep. numbers from ip address them, "concatenate" them into one 32 bit number.
my $mask_val = unpack( "I", pack( "CCCC",@netmasls ) ); # 32-bit number consists of four 8-bit unsigned chars (0-255).

my $nw_val = ( $ip_val & $mask_val ); # calculate 32-bit value for network address
my $bc_val = ( $ip_val | ~$mask_val ); # calculate 32-bit value for the broadcast address

my @nw_list = unpack( "CCCC", pack( "I",$nw_val ) ); #separate 32-bit number into four number fields again.
my $nw_final = join(".",@nw_list);  #take the numbers, join them into an ip address form

my @bc_list = unpack( "CCCC", pack( "I",$bc_val ) ); #separate 32-bit number into four number fields again.
my $bc_final = join(".",@bc_list);  #take the numbers, join them into an ip address form

print("Network Address is $nw_final and Broadcast address is $bc_final.\n");

6. 110101010011

(Drew this one on the actual paper.)
