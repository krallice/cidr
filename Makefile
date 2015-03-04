VERSION=2.3.2
RELEASE=1

CC=gcc
STRIP=strip
CCFLAGS=-Wall

SOL7FLAGS=-lsocket -lnsl
SOL8FLAGS=-lresolv -lnsl


PKGNAME=cidr-${VERSION}

all	:
	$(CC) $(CCFLAGS) cidr.c -o cidr
	$(STRIP) cidr

solaris8:
	$(CC) $(CCFLAGS) $(SOL8FLAGS) cidr.c -o cidr
	$(STRIP) cidr

solaris7:
	$(CC) $(CCFLAGS) $(SOL7FLAGS) cidr.c -o cidr
	$(STRIP) cidr

clean:
	rm -f cidr cidr.o \
          /usr/src/redhat/SOURCES/${PKGNAME}.tar.gz \
          /usr/src/redhat/SRPMS/${PKGNAME}-${RELEASE}.src.rpm
	rm -rf ${PKGABR}

srpm:
	( cd ..; \
          tar zchvf /usr/src/redhat/SOURCES/${PKGNAME}.tar.gz \
          cidr-${VERSION}/Makefile \
          cidr-${VERSION}/cidr.c \
          cidr-${VERSION}/cidr.1 \
          cidr-${VERSION}/cidr.spec \
          cidr-${VERSION}/README \
          cidr-${VERSION}/ChangeLog \
          cidr-${VERSION}/rfc1878.txt \
          cidr-${VERSION}/gpl.txt \
	)
	rpm -bs cidr.spec

rpm:    srpm
	rpm --rebuild \
	  /usr/src/redhat/SRPMS/${PKGNAME}-${RELEASE}.src.rpm
