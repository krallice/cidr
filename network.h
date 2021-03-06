/*************************************************************************

    network.h -- determine CIDR network and broadcast address from
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

#ifndef NETWORK_H_CIDR
#define NETWORK_H_CIDR

/*
sockaddr_in structure prototype from <netinet/in.h>:

struct sockaddr_in {
    short            sin_family;   // e.g. AF_INET
    unsigned short   sin_port;     // e.g. htons(3490)
    struct in_addr   sin_addr;     // see struct in_addr, below
    char             sin_zero[8];  // zero this if you want to
};

struct in_addr {
    unsigned long s_addr; 
};
*/

typedef struct network_struct {
	struct in_addr host;
	struct in_addr mask;
	struct in_addr network;
	struct in_addr broadcast;
} network_t;

#define STRLEN 64

// Setters:
int setSubnetMask(network_t *, char *); 
int setIPAddress(network_t *, char *);
int splitCIDR(network_t *, char *, char *, char *);
void convertCIDRToNetmask(network_t *, int);
void calculateNetAndBroadcast(network_t *); 

// Getters:
void getIPAddress(network_t *, char *, int l);
void getSubnetMask(network_t *, char *, int l);
void getNetworkAddress(network_t *, char *, int l);
void getBroadcastAddress(network_t *, char *, int l);
void getWildcardMask(network_t *, char *, int l);
void getLastUsable(network_t *, char *, int l);
void getFirstUsable(network_t *, char *, int l);
void getSecondUsable(network_t *, char *, int l);
void getThirdUsable(network_t *, char *, int l);
void printNetworkDetails(network_t*);
int getNetworkSize(network_t *);

#endif
