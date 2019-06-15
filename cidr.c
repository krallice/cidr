/*************************************************************************

    cidr.c -- determine CIDR network and broadcast address from
    IPV4 ip address and netmask
    Copyright (C) 2000 Robert L. Lineberger 
    Heavily Modified 2015 Stephen Rozanc.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***************************************************************************/

#include "cidr.h"
#include "network.h"

// Prototypes:
static void usage(char *);
static void invalid(int, char *);
static void print_version(void);

const char *version_val="2.4.1";

int main( int argc, char *argv[] ) {

	// Define our network structure:
	network_t network;

	// If not enough (or too many) arguments are supplied, bomb out:
	if ( argc < 2 ) {
		usage(argv[0]);
	} else if ( argc > 3 ) {
		invalid(0, argv[0]);
	// Else if we have exactly 2 arguments (filename + switch):
	} else if ( argc == 2 ) {
		// Check to see if user is requesting version or help information:
		if (strncmp((const char *) argv[1],"-v",2) == 0) {
			print_version();
		} else if (strncmp((const char *) argv[1],"-h",2) == 0) {
			usage(argv[0]);
		// Else end user is entering CIDR notation, let's check:
		} else {
			char *ip = NULL;
			char *cidr = NULL;
			if ( splitCIDR(&network, argv[1], ip, cidr) != 1 ) {
				invalid(1, argv[1]);
			}
		}
	// End user has entered 2 arguments, hopefully meaning ip + subnet mask:
	} else if ( argc == 3 ) {
		// Check to see if our IP/Masks are valid:
		if ( setIPAddress(&network, argv[1]) != 1 ) {
			invalid(1, argv[1]);
		}
		if ( setSubnetMask(&network, argv[2]) != 1 ) {
			invalid(2, argv[2]);
		}
	}
	printNetworkDetails(&network);
	return 0;
}

// Print Usage Patterns to end user:
static void usage(char *arg) {

	printf("\ncidr version %s October, 2015\n", version_val);
	printf("Version 2.4.X Heavily Modified 2015 Stephen Rozanc\n");
	printf("Original - Copyright 2000 Robert L. Lineberger\n\n");
	printf("https://github.com/krallice\n");
	printf("robert@geeksoul.com\n");
	printf("http://geeksoul.com/robert/cidr.html\n");
	printf("License: GPL\n\n");

	printf("Contributors:\n");
	printf("Stephen Rozanc\n");
	printf("David A. Bandel\n");
	printf("Iain Lea\n");
	printf("Herman Robers\n\n");

	printf(
	"Usage:\n\n"
	"%s\t\t\t\tprint usage information (this message)\n"
	"%s [-v]\t\t\tprint version information\n"
	"%s [ipaddress/prefix]\tuse short form CIDR notation ie. 198.51.100.0/24 \n"
	"%s [ipaddress] [subnetmask]\tuse full subnet mask ie. 198.51.100.0 255.255.255.0"
	"     \n\n",arg,arg,arg,arg);

	exit(EXIT_FAILURE);
}

// Print Version information to end user:
static void print_version(void) {

	fprintf(stderr,"cidr version %s\n",version_val);
	exit(EXIT_SUCCESS);
}

static void invalid(int argnum, char* procpath)
{
 printf("invalid argument: argv[%d]\n",argnum);
 usage(procpath);
 exit(EXIT_FAILURE);
}
