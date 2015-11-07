/*************************************************************************

    network.c -- determine CIDR network and broadcast address from
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

int setIPAddress(network_t *n, char *host) {
	return inet_pton(AF_INET, host, &(n->host) );
}

int setSubnetMask(network_t *n, char *mask) {
	return inet_pton(AF_INET, mask, &(n->mask) );
}

void getIPAddress(network_t *n, char *s, int l) {
	inet_ntop(AF_INET, &(n->host), s, l);
}

void getSubnetMask(network_t *n, char *s, int l) {
	inet_ntop(AF_INET, &(n->mask), s, l);
}

void getNetworkAddress(network_t *n, char *s, int l) {

	struct in_addr netAddr;
	netAddr.s_addr = n->host.s_addr & n->mask.s_addr;
	inet_ntop(AF_INET, &netAddr, s, l);
}

void printNetworkDetails(network_t *n) {

	char subnetMask[STRLEN] = "";
	char hostAddress[STRLEN] = "";
	char networkAddress[STRLEN] = "";

	getIPAddress(n, hostAddress, STRLEN);
	getSubnetMask(n, subnetMask, STRLEN);
	getNetworkAddress(n, networkAddress, STRLEN);

	printf("Host Address ... %s\n", hostAddress);
	printf("---\n");
	printf("Network Address ... %s\n", networkAddress);
	printf("Subnet Mask ... %s\n", subnetMask);
}
