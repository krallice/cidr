/*************************************************************************

    cidr.c -- determine CIDR network and broadcast address from
    IPV4 ip address and netmask
    Copyright (C) 2000 Robert L. Lineberger 

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

#include <stdio.h>
#include <stdlib.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

void usage(char *);
void range_error(int);
void invalid(int);
void hosts(unsigned long int,
           unsigned long int,
           unsigned long int,
           int,
           struct in_addr,
           struct in_addr );
void print_version(void);

const char *version_val="2.3.2";

int main(int argc, char *argv[])
{

 struct in_addr in;
 struct in_addr ma;
 struct in_addr lowhost;
 struct in_addr highhost;

 unsigned long int address;
 unsigned long int mask;
 unsigned long int maskval; 
 unsigned long int i,temp;

 int binaddr;
 int decaddr;
 int hexaddr;
 int quadaddr;
 int binmask;
 int decmask;
 int hexmask;
 int quadmask;
 int prefix; 
 int validhex;
 int hostlist;
 int cidraddress;
 int cidrprefix;
 int cidrhostlist;
 int getversion;
 
 double range;

 char *ip[3];
 char *holdaddress;
 char *holdprefix;
 const char *sep="/";

 for(i=0;i<3;i++)
  ip[i]=(char *)NULL;

 holdaddress=(char *)NULL;
 holdprefix=(char *)NULL;

 binaddr=1;
 decaddr=1;
 hexaddr=1;
 quadaddr=1;
 binmask=1;
 decmask=1;
 hexmask=1;
 quadmask=1;
 prefix=1;
 validhex=1;
 hostlist=1;
 cidraddress=1;
 cidrprefix=1;
 cidrhostlist=1;
 getversion=1; 

 if(argc!=5)
 {
  if(argc==6)
  {
   hostlist=strncmp((const char *)argv[5],"-H",2);
   if(hostlist!=0)
    usage(argv[0]); 
  }

  else if(argc==2||argc==3)
  {
   getversion=strncmp((const char *) argv[1],"-v",2);
   if(!getversion)
    print_version();

   cidraddress=0;
   cidrprefix=0;

   if(argc==3)
   {
    cidrhostlist=strncmp((const char *)argv[2],"-H",2);
    if(cidrhostlist!=0)
     usage(argv[0]);
   }

   if(strlen(argv[1])>18)
   {
    fprintf(stderr,"argv[1] too long\n");
    exit(EXIT_FAILURE);
   }

   for(i=0;i<3;i++)
   {
    if( (ip[i]=malloc(strlen(argv[1])+1)) == NULL )
    {
     fprintf(stderr,"ip[i] malloc() failed\n");
     exit(EXIT_FAILURE);
    }
   }
  
   strncpy(ip[2],argv[1],18);

   holdaddress=strtok(ip[2],sep);
   if(holdaddress==NULL)
    invalid(1);


   strncpy(ip[0],holdaddress,strlen(holdaddress));
   if(ip[0]==NULL)
    invalid(1);

   holdprefix=strtok(NULL,sep);
   if(holdprefix==NULL)
    invalid(1);  


   strncpy(ip[1],holdprefix,strlen(holdprefix));
   if(ip[1]==NULL)
    invalid(1); 


  }
  else
   usage(argv[0]);
 }

/********************* mask value used with prefix *******************/

if(argc==5||argc==6)
{
 maskval=0xffffffff;
 maskval>>=(32-atoi(argv[4]));
 maskval<<=(32-atoi(argv[4]));
}
else
{
 maskval=0xffffffff;
 maskval>>=(32-atoi(ip[1]));
 maskval<<=(32-atoi(ip[1]));
}

/******************* address type *************************/

if(argc==5||argc==6)
{
 binaddr=strncmp((const char *)argv[1],"-b",2);
 decaddr=strncmp((const char *)argv[1],"-d",2);
 hexaddr=strncmp((const char *)argv[1],"-h",2);
 quadaddr=strncmp((const char *)argv[1],"-q",2);

/******************* mask type ****************************/

 binmask=strncmp((const char *)argv[3],"-b",2);
 decmask=strncmp((const char *)argv[3],"-d",2);
 hexmask=strncmp((const char *)argv[3],"-h",2);
 quadmask=strncmp((const char *)argv[3],"-q",2);
 prefix=strncmp((const char *)argv[3],"-p",2);
}

/********* check binary address *********/
 
 if(binaddr==0)  
 {
  if(strlen(argv[2])>32)
  {
   range_error(2);
  }
  in.s_addr=htonl(strtoul((const char *)argv[2],(char **)NULL,2)); 
 }

/********** check decimal address *************/

 else if(decaddr==0)
 {
  address=strtoul((const char *)argv[2],(char **)NULL,10); 
  range=strtod((const char *)argv[2],(char **)NULL); 
  if(range<0||range>0xffffffff)
  {
   range_error(2); 
  }
  in.s_addr=htonl(address);
 }

/******** check hexadecimal address ********/

 else if(hexaddr==0)
 {

  validhex=1;

  if(strlen(argv[2])>8)
  {
   validhex=0;

   if(strncmp((const char *)argv[2],"0x",2) == 0 )
   {
    validhex=1;
    if( strlen(argv[2])>10)
    {
     validhex=0;
     range_error(2);
    }
   } 
   else
   {
    invalid(2);
   } 
  }

  if(validhex)
  {
   in.s_addr=htonl(strtoul((const char *)argv[2],(char **)NULL,16)); 
  }

 }

/*********** check dotted-quad address ************/

 else if(quadaddr==0)
 {
  if( inet_aton(argv[2],&in) == 0 )
  {
    invalid(2);
  }
 }

 else if(cidraddress==0)
 {
  if( inet_aton(ip[0],&in) == 0 )
  {
    invalid(1);
  }
 }


/*********** unrecognized input ************/
 
 else
  invalid(1);

/************************************************************************/



/************** check binary mask *************/

 if(binmask==0)
 {
  if(strlen(argv[4])>32)
  {
   range_error(4);
  }
  mask=strtoul((const char *)argv[4],(char **)NULL,2); 
 }

/*************** check decimal mask ****************/

 else if(decmask==0)
 {
  address=strtoul((const char *)argv[4],(char **)NULL,10);
  range=strtod((const char *)argv[4],(char **)NULL);
  if(range<0||range>0xffffffff)
  {
   range_error(4);
  }
  mask=strtoul((const char *)argv[4],(char **)NULL,10); 
 }

/*************** check hexadecimal mask *************/

 else if(hexmask==0)
 {

  validhex=1;

  if(strlen(argv[4])>8)
  {
   validhex=0;

   if(strncmp((const char *)argv[4],"0x",2) == 0 )
   {
    validhex=1;
    if( strlen(argv[4])>10)
    {
     validhex=0;
     range_error(4);
    }
   } 
   else
   {
    invalid(4);
   } 
  }

  if(validhex)
  {
   mask=strtoul((const char *)argv[4],(char **)NULL,16); 
  }

 }


/************* check dotted-quad mask **************/

 else if(quadmask==0)
 {
  if( inet_aton(argv[4],&ma) == 0 )
  {
    invalid(4);
  }
  mask=ntohl(ma.s_addr);
 }

/*************** check prefix ******************/

 else if(prefix==0)
 {
  if(atoi(argv[4])<0||atoi(argv[4])>32)
  {
   range_error(4);
  }
  mask=maskval;   
 }
 
 else if(cidrprefix==0)
 {
  if(atoi(ip[1])<0||atoi(ip[1])>32)
  {
   range_error(1);
  }
  mask=maskval;
 }

/************ unrecognized input **************/
 else
  invalid(3);

/*******************************************/


 printf("\nip address..........:  %s\n",inet_ntoa(in));
 in.s_addr&=htonl(mask); 
 ma.s_addr=htonl(mask);
 printf("netmask.............:  %s\n\n",inet_ntoa(ma)); 
 printf("network address.....:  %s\n",inet_ntoa(in));

 lowhost=in;

 in.s_addr|=~htonl(mask);

 highhost=in;

 printf("broadcast address...:  %s\n\n",inet_ntoa(in));

if(argc==5||argc==6)
 hosts(i,temp,mask,hostlist,lowhost,highhost);
else
 hosts(i,temp,mask,cidrhostlist,lowhost,highhost);

 
 for(i=0;i<3;i++)
 {
   free(ip[i]);
 }

 return EXIT_SUCCESS;
}

void print_version(void)
{
 fprintf(stderr,"cidr version %s\n",version_val);
 exit(EXIT_SUCCESS);
}

void usage(char *arg)
{
 printf("\ncidr version %s July 17, 2001\n", version_val);
 printf("Copyright 2000 Robert L. Lineberger\n");
 printf("robert@geeksoul.com\n");
 printf("http://geeksoul.com/robert/cidr.html\n");
 printf("License: GPL\n\n");

 printf("Contributors:\n");
 printf("David A. Bandel\n");
 printf("Iain Lea\n");
 printf("Herman Robers\n\n");

 printf(
    "Usage:\n\n" 
    "%s [-v]\n\n"
    "No arguments\tprint this usage()\n"
    "\t-v\tprint version information\n\n" 
    "Short form:\n\n"
    "%s <ipaddress/prefix> [-H]\n\n"
    "Note: Short form only supports dotted-quad ip address and\n"
    "      decimal(integer) prefix.  Host list (\"-H\") is optional.\n\n" 
    "Long form:\n\n"
    "%s <-b|-q|-h|-d> <ipaddress> <-b|-q|-h|-d|-p> <mask|prefix>"
    " [-H]\n",arg,arg,arg);

 printf("\nlong form ipaddress options:\n");
 printf("\t-b\tip address as a binary number\n");
 printf("\t-q\tip address as a dotted quad address\n");
 printf("\t-h\tip address as a hexadecimal number\n");
 printf("\t-d\tip address as a decimal number\n");
 printf("\nlong form mask/prefix options:\n");
 printf("\t-b\taddress mask as a binary number\n");
 printf("\t-q\taddress mask as a dotted quad\n");
 printf("\t-h\taddress mask as a hexadecimal number\n");
 printf("\t-d\taddress mask as a decimal number\n");
 printf("\t-p\taddress mask as a prefix\n");
 printf("\t\t(e.g. a prefix of 27 represents a\n");
 printf("\t\tmask of 255.255.255.224)\n");
 printf("\nhostlist option:\n");
 printf("\t-H\tprint list of valid host ip's\n\n");
 printf("Short form example:\n");
 printf("\tcidr 192.168.100.25/27\n");
 printf("\nLong form example:\n");
 printf("\tcidr -q 192.168.30.1 -p 30 -H\n");
 printf("\tcidr -q 10.12.95.125 -q 255.255.255.224\n\n"); 

 exit(EXIT_FAILURE);
}

void range_error(int argnum)
{
 printf("range error: argv[%d]\n",argnum);
 exit(EXIT_FAILURE);
}

void invalid(int argnum)
{
 printf("invalid argument: argv[%d]\n",argnum);
 exit(EXIT_FAILURE);
}

void hosts(unsigned long int i_val,
           unsigned long int temp_val,
           unsigned long int mask_val,
           int hostlist_val,
           struct in_addr lowhost_val,
           struct in_addr highhost_val )
{
 unsigned long int j=0;

 if(!hostlist_val)
  printf("host addresses:\n\n");
 if(hostlist_val)
  printf("please wait while host addresses are validated...\n\n"); 

 temp_val=ntohl(lowhost_val.s_addr);
 for(i_val=temp_val+1;i_val<ntohl(highhost_val.s_addr);++i_val)
 {
  lowhost_val.s_addr=htonl(i_val);
  if(!(htonl(temp_val)^(lowhost_val.s_addr&htonl(mask_val))))
  {
   ++j;
   if(!hostlist_val)
    printf("%s\n",inet_ntoa(lowhost_val) );
  }
 }
 if(!hostlist_val)
  printf("\n");
 printf("total host addresses:  %lu\n\n",j);
}

