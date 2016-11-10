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
	
	// Save return status:
	int maskret;
	maskret = inet_pton(AF_INET, mask, &(n->mask) );
	if ( maskret == 1 ) {
		calculateNetAndBroadcast(n);
	}
	return maskret;
}
void calculateNetAndBroadcast(network_t *n) {
	// Bitwise calculate our network and broadcast addresses:
	n->network.s_addr = n->host.s_addr & n->mask.s_addr;
	n->broadcast.s_addr = n->host.s_addr | ~n->mask.s_addr;
}
void convertCIDRToNetmask(network_t *n, int i_cidr){
	int mask = ~(0xFFFFFFFF >> i_cidr);
	n->mask.s_addr = htonl(mask);
	calculateNetAndBroadcast(n);
}
int splitCIDR(network_t *n, char *fullstring, char *ip, char *cidr) {

	ip = strtok(fullstring,"/");
	cidr = strtok(NULL, "/");

	// Validity checks:
	if ( ip == NULL || cidr == NULL ) {
		return 2;
	}
	int i_cidr = atoi(cidr);
	if ( i_cidr > 32 || i_cidr < 1 ) {
		return 2;
	}
	if ( setIPAddress(n, ip) != 1 ) {
		return 3;
	}
	convertCIDRToNetmask(n, i_cidr);
	return 1;
}
void getIPAddress(network_t *n, char *s, int l) {
	inet_ntop(AF_INET, &(n->host), s, l);
}
void getSubnetMask(network_t *n, char *s, int l) {
	inet_ntop(AF_INET, &(n->mask), s, l);
}
void getNetworkAddress(network_t *n, char *s, int l) {
	inet_ntop(AF_INET, &(n->network), s, l);
}
void getBroadcastAddress(network_t *n, char *s, int l) {
	inet_ntop(AF_INET, &(n->broadcast), s, l);
}
void getWildcardMask(network_t *n, char *s, int l){
	struct in_addr wc;
	wc.s_addr = ~n->mask.s_addr;
	inet_ntop(AF_INET, &wc, s, l);
}
void getLastUsable(network_t *n, char *s, int l){
	struct in_addr f;
	f.s_addr = htonl(ntohl(n->broadcast.s_addr) - 1);
	inet_ntop(AF_INET, &f, s, l);
}
void getFirstUsable(network_t *n, char *s, int l){
	struct in_addr f;
	// Convert our network address to host endian, increment by 1, and then reconvert
	// back to network endian order:
	f.s_addr = htonl(ntohl(n->network.s_addr) + 1);
	inet_ntop(AF_INET, &f, s, l);
}
void getSecondUsable(network_t *n, char *s, int l){
	struct in_addr f;
	f.s_addr = htonl(ntohl(n->network.s_addr) + 2);
	inet_ntop(AF_INET, &f, s, l);
}
void getThirdUsable(network_t *n, char *s, int l){
	struct in_addr f;
	f.s_addr = htonl(ntohl(n->network.s_addr) + 3);
	inet_ntop(AF_INET, &f, s, l);
}
int getNetworkSize(network_t *n){
	int ns;
	ns = ntohl(n->broadcast.s_addr) - ntohl(n->network.s_addr) - 1;
	return ns;
}
int getBitmask(network_t *n){
	unsigned int mask = ntohl((unsigned int)n->mask.s_addr);
	int counter = 0;
	while (mask){
		counter++;
		mask &= (mask - 1);
	}
	return counter;
}
void printNetworkDetails(network_t *n) {

	char subnetMask[STRLEN] = "";
	char hostAddress[STRLEN] = "";
	char networkAddress[STRLEN] = "";
	char broadcastAddress[STRLEN] = "";
	char wildcardMask[STRLEN] = "";
	char lastUsable[STRLEN] = "";
	char firstUsable[STRLEN] = "";
	char secondUsable[STRLEN] = "";
	char thirdUsable[STRLEN] = "";
	int networkSize;
	int bitMask;

	getIPAddress(n, hostAddress, STRLEN);
	getSubnetMask(n, subnetMask, STRLEN);
	getNetworkAddress(n, networkAddress, STRLEN);
	getBroadcastAddress(n, broadcastAddress, STRLEN);
	getWildcardMask(n, wildcardMask, STRLEN);
	getLastUsable(n, lastUsable, STRLEN);
	getFirstUsable(n, firstUsable, STRLEN);
	getSecondUsable(n, secondUsable, STRLEN);
	getThirdUsable(n, thirdUsable, STRLEN);
	networkSize = getNetworkSize(n);
	bitMask = getBitmask(n);

	printf("\n");
	printf("Host address            - %s\n", hostAddress);
	printf("Network Address		- %s (%s/%d)\n", networkAddress, networkAddress, bitMask);
	printf("Subnet Mask		- %s\n", subnetMask);
	printf("Broadcast Address	- %s\n", broadcastAddress);
	printf("Wildcard Address	- %s\n", wildcardMask);
	printf("Network Prefix (CIDR)	- /%d\n", bitMask);
	printf("Network Range		- %s - %s (%d Addresses)\n", networkAddress, broadcastAddress, networkSize+2);
	printf("Usable Range		- %s - %s (%d Usable Hosts)\n", firstUsable, lastUsable,networkSize);
	printf("\n");
	printf("A IP			- %s\n", firstUsable);
	printf("B IP			- %s\n", secondUsable);
	printf("C IP			- %s\n", thirdUsable);
	printf("GW IP			- %s\n", lastUsable);
	printf("\n");

	// Provisioning Assistance:

}
