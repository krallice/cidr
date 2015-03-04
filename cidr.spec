%define ver      2.3.2
%define rel      1
%define prefix   /

Summary: command-line tool for determining RFC 1878 addresses.
Name: cidr
Version: %ver
Release: %rel
Copyright: GPL
Group: Applications/Engineering
Source: cidr-%{ver}.tar.gz
Buildroot: /tmp/%name-buildroot
URL: http://geeksoul.com/robert/cidr.html
%description

cidr is a tiny command-line tool for determining network and broadcast
addresses a la RFC 1878. It takes an IP address and netmask and outputs
the network address, broadcast address, and total number of addresses
for the corresponding subnet. The IP can be in binary, decimal, hex,
or dotted quad. The mask can be in binary, decimal, hex, dotted quad,
or prefix.


%changelog

* Fri Jul 20 2001 Herman Robers <herman@pinewood.nl>
- Modified for version 2.3.2

* Thu Jul 19 2001 Herman Robers <herman@pinewood.nl>
- Made SPEC file

%prep
%setup
%build
make

%install
mkdir -p $RPM_BUILD_ROOT/usr/bin $RPM_BUILD_ROOT/usr/man/man1
install -m 755 cidr $RPM_BUILD_ROOT/usr/bin/cidr
install -m 644 cidr.1 $RPM_BUILD_ROOT/usr/man/man1/cidr.1
gzip $RPM_BUILD_ROOT/usr/man/man1/cidr.1

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc README ChangeLog rfc1878.txt gpl.txt
/usr/bin/cidr
/usr/man/man1/cidr.1.gz

