/* Joseph Garcia */
/* ECE 331 Homework 9 */
/* Sheaff */

1.

Frodo Routing Table
Destination	Gateway		Genmask		Flags 	Metric 	Ref   Use      Iface
18.0.0.0	0.0.0.0         255.248.0.0      U       ?      ?     ?        eth0
127.0.0.0	0.0.0.0         0.0.0.0          U  	 ?      ?     ?		lo   /* loopback address */
0.0.0.0         18.6.7.8        0.0.0.0          UG      ?      ?     ?        eth0  /* interface with Elessar  on it's eth0 */

Elessar (the router) Routing Table 
Destination	Gateway		Genmask		Flags Metric Ref   Use 		Iface
0.0.0.0		76.99.33.1      0.0.0.0		UG       ?     ?    ?		eth1 
76.99.0.0       0.0.0.0         255.255.128.0   U        ?     ?    ?           eth1 
127.0.0.0       0.0.0.0         255.0.0.0       U        ?     ?    ?		lo
18.0.0.0        0.0.0.0         255.248.0.0     U        ?     ?    ?		eth0
141.33.33.0     0.0.0.0         255.255.255.192 U        ?     ?    ?		eth2
	
2. 

Journey Leg		Source IP	Destination IP		Source MAC		Destination MAC
Frodo to Elessar	18.4.8.9        141.33.33.6          	00:00:00:00:00:11	55:00:00:00:00:00
Elessar to Legolas      18.4.8.9	141.33.33.6		66:00:00:00:00:00	00:00:00:00:00:22

3. 

Journey Leg		Source IP	Destination IP		Source MAC		Destination MAC
Legolas to Elessar	141.33.33.6	76.99.33.1		00:00:00:00:00:22	66:00:00:00:00:00
Elessar to Gandalf      141.33.33.6	76.99.33.1		77:00:00:00:00:00	FF:00:00:00:00:00

4. (note that minor and major correspond to the smaller and larger segments of a segmented group, respectively)

Group        Network Address    Broadcast Address     Subnet Mask
Think_minor  123.210.32.0       123.210.35.255        255.255.252.0
Blue Man     123.210.36.0       123.210.39.255        255.255.252.0
E            123.210.40.0       123.210.41.255        255.255.254.0
Hug_minor    123.210.42.0       123.210.43.255        255.255.254.0
Hug_major    123.210.44.0       123.210.47.255        255.255.252.0
Think_major  123.210.48.0       123.210.63.255        255.255.240.0

5.

a.
www.perlfect.com/articles/sockets.shtml		

b.

#!/usr/bin/perl

use strict;
use warnings;   # Preamble, including strict and warnings for debugging and to increase code quality.
use IO::Socket;
 
my $socket = new IO::Socket::INET(Proto => 'tcp',    # use TCP protocol
				  Reuse => 1,          # allows us to reuse port for next program in case this socket isn't properly closed.
                                  PeerAddr => "www.wunderground.com",   # address of site that is delivering information.
                                  PeerPort => 80);       # Port web server is communicating to us from

die "Couldn't create socket.\n" unless $socket;  # error check

print $socket "GET /\n\r"; # command to web server, grabs newlines and carriage returns.
 
while(<$socket>) {
        print $_,"\n";   # prints html information from the website.
}

close $socket;   # close the socket.

6.
	sudo apt-get install lighttpd php5-cgi sqlite3 sqlitebrowser php5-cli php5-gd
	sudo lighty-enable-mod fastcgi
	sudo lighty-enable-mod fastcgi-php
	sudo service lighttpd force-reload
	(symbolic links to ~/conf-available/15-fastcgi-php.conf and ~/conf-available/10-fastcgi.conf in /etc/lighttpd/conf-enabled/ already done)

7. 

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>

int main(int argc, char* argv[])
{
	int fd = 0;  /* file descriptor representing the socket. */
	int func_ret = 0;   /* general storage variable for function returns. */
	struct ifreq ethintf;  /* struct to represent Ethernet network interface */
	char eth[4] = {'e','t','h','0'};  /* "eth0" string */
	int k = 0;	/* looping variable */

	for (k = 0; k < 4; k++) {
		ethintf.ifr_name[k] = eth[k]; /* use "eth0" string as a flag so that sockaddr hw_addr struct is instantiated with the MAC address. */
	}
	
	if ((fd = socket(AF_INET,SOCK_DGRAM, IPPROTO_IP)) == -1) {
		printf("Unable to create socket.\n");              /* Create the socket, make sure it was created without issue. */
		close(fd);
		return errno;
	}
	
	if ( (func_ret = ioctl(fd, SIOCGIFHWADDR, &ethintf)) < 0) {
		printf(" Unable to fetch information regarding the ethernet interface.\n"); /* instantiate hw_addr struct member of ethintf struct */
		return errno; 
	}

	printf("%02X:%02X:%02X:%02X:%02X:%02X\n", 
	(unsigned char)(ethintf.ifr_hwaddr.sa_data[0]),		   /* print the hardware address in standard ethernet mac address format. (members are unsigned hexadecimal values) */
	(unsigned char)(ethintf.ifr_hwaddr.sa_data[1]),
	(unsigned char)(ethintf.ifr_hwaddr.sa_data[2]),
	(unsigned char)(ethintf.ifr_hwaddr.sa_data[3]),
	(unsigned char)(ethintf.ifr_hwaddr.sa_data[4]),
	(unsigned char)(ethintf.ifr_hwaddr.sa_data[5]));
 
	close(fd);             /* close file descriptor. */
	return 0; /* successful program execution */
}

8.

#!/usr/bin/perl
 
use strict;
use warnings;  # warnings and strict to improve code quality, bignum for 48 bit number printing.
use bignum;
 
if ($ARGV[0] !~ m/(([ABCDEF]|\d){2}\:){5}(([ABCDEF]|\d)){2}/i) {
	print "Format of MAC not recognized. Quitting Program.\n";  # verify that the MAC address from command line is valid.
        die;
}
 
my $MAC = $ARGV[0];   # grab MAC address from command line 
my $prefix = "0x";   # used to make MAC address string a recognizable hexadecimal value.
 
my @MAC_array = split("[:]", $MAC);
my $MAC_string = join("", @MAC_array);   # split MAC address into array of hexadecimal values, and then concatenate them into a string.
 
my $new_MAC_string = $prefix . $MAC_string;  # prefix the string with 0x so that it becomes a recognizable hexadecimal number
print $new_MAC_string + 0,"\n";  # add 0 to the string to put it in a numerical context, then print it.


