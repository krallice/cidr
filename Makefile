VERSION=2.3.2
RELEASE=1

CC=gcc
STRIP=strip
CCFLAGS=-Wall

PKGNAME=cidr-${VERSION}

all	:
	$(CC) $(CCFLAGS) cidr.c network.c -o cidr
	$(STRIP) cidr

clean:
	rm -f cidr cidr.o 
