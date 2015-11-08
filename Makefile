VERSION=2.3.2
RELEASE=1

CC=gcc
STRIP=strip
CCFLAGS=-Wall

INSTALL=/usr/bin/install
PREFIX=/usr/local/bin

PKGNAME=cidr-${VERSION}

all	:
	$(CC) $(CCFLAGS) cidr.c network.c -o cidr
	$(STRIP) cidr

install :
	$(INSTALL) cidr $(PREFIX)
clean:
	rm -f cidr cidr.o 
