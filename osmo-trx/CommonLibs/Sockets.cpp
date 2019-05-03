/*
* Copyright 2008, 2010 Free Software Foundation, Inc.
*
*
* This software is distributed under the terms of the GNU Affero Public License.
* See the COPYING file in the main directory for details.
*
* This use of this software may be subject to additional restrictions.
* See the LEGAL file in the main directory for details.

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/



#include <config.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <sys/select.h>

#include "Threads.h"
#include "Sockets.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>






bool resolveAddress(struct sockaddr_in *address, const char *hostAndPort)
{
	assert(address);
	assert(hostAndPort);
	char *copy = strdup(hostAndPort);
	char *colon = strchr(copy,':');
	if (!colon) return false;
	*colon = '\0';
	char *host = copy;
	unsigned port = strtol(colon+1,NULL,10);
	bool retVal = resolveAddress(address,host,port);
	free(copy);
	return retVal;
}

bool resolveAddress(struct sockaddr_in *address, const char *host, unsigned short port)
{
	assert(address);
	assert(host);
	// FIXME -- Need to ignore leading/trailing spaces in hostname.
	struct hostent *hp;
	int h_errno_local;
#ifdef HAVE_GETHOSTBYNAME2_R
	struct hostent hostData;
	char tmpBuffer[2048];

	// There are different flavors of gethostbyname_r(), but
	// latest Linux use the following form:
	if (gethostbyname2_r(host, AF_INET, &hostData, tmpBuffer, sizeof(tmpBuffer), &hp, &h_errno_local)!=0) {
		CERR("WARNING -- gethostbyname2_r() failed for " << host << ", " << hstrerror(h_errno_local));
		return false;
	}
#else
	static Mutex sGethostbynameMutex;
	// gethostbyname() is NOT thread-safe, so we should use a mutex here.
	// Ideally it should be a global mutex for all non thread-safe socket
	// operations and it should protect access to variables such as
	// global h_errno.
	sGethostbynameMutex.lock();
	hp = gethostbyname(host);
	h_errno_local = h_errno;
	sGethostbynameMutex.unlock();
#endif
 	if (hp==NULL) {
		CERR("WARNING -- gethostbyname() failed for " << host << ", " << hstrerror(h_errno_local));
		return false;
	}
	if (hp->h_addrtype != AF_INET) {
		CERR("WARNING -- gethostbyname() resolved " << host << " to something other then AF_INET");
 		return false;
 	}
	address->sin_family = hp->h_addrtype;
	assert(sizeof(address->sin_addr) == hp->h_length);
	memcpy(&(address->sin_addr), hp->h_addr_list[0], hp->h_length);
	address->sin_port = htons(port);
	return true;
}



DatagramSocket::DatagramSocket()
{
	memset(mDestination, 0, sizeof(mDestination));
}





void DatagramSocket::nonblocking()
{
	fcntl(mSocketFD,F_SETFL,O_NONBLOCK);
}

void DatagramSocket::blocking()
{
	fcntl(mSocketFD,F_SETFL,0);
}

void DatagramSocket::close()
{
	::close(mSocketFD);
}


DatagramSocket::~DatagramSocket()
{
	close();
}





int DatagramSocket::write( const char * message, size_t length )
{
	assert(length<=MAX_UDP_LENGTH);
	int retVal = sendto(mSocketFD, message, length, 0,
		(struct sockaddr *)mDestination, addressSize());
	if (retVal == -1 ) perror("DatagramSocket::write() failed");
	return retVal;
}

int DatagramSocket::writeBack( const char * message, size_t length )
{
	assert(length<=MAX_UDP_LENGTH);
	int retVal = sendto(mSocketFD, message, length, 0,
		(struct sockaddr *)mSource, addressSize());
	if (retVal == -1 ) perror("DatagramSocket::write() failed");
	return retVal;
}



int DatagramSocket::write( const char * message)
{
	size_t length=strlen(message)+1;
	return write(message,length);
}

int DatagramSocket::writeBack( const char * message)
{
	size_t length=strlen(message)+1;
	return writeBack(message,length);
}



int DatagramSocket::send(const struct sockaddr* dest, const char * message, size_t length )
{
	assert(length<=MAX_UDP_LENGTH);
	int retVal = sendto(mSocketFD, message, length, 0, dest, addressSize());
	if (retVal == -1 ) perror("DatagramSocket::send() failed");
	return retVal;
}

int DatagramSocket::send(const struct sockaddr* dest, const char * message)
{
	size_t length=strlen(message)+1;
	return send(dest,message,length);
}

int DatagramSocket::read(char* buffer, size_t length)
{
	socklen_t addr_len = sizeof(mSource);
	int rd_length = recvfrom(mSocketFD, (void *) buffer, length, 0,
		(struct sockaddr*) &mSource, &addr_len);

	if ((rd_length==-1) && (errno!=EAGAIN)) {
		perror("DatagramSocket::read() failed");
		throw SocketError();
	}
	return rd_length;
}

int DatagramSocket::read(char* buffer, size_t length, unsigned timeout)
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(mSocketFD,&fds);
	struct timeval tv;
	tv.tv_sec = timeout/1000;
	tv.tv_usec = (timeout%1000)*1000;
	int sel = select(mSocketFD+1,&fds,NULL,NULL,&tv);
	if (sel<0) {
		perror("DatagramSocket::read() select() failed");
		throw SocketError();
	}
	if (sel==0) return -1;
	if (FD_ISSET(mSocketFD,&fds)) return read(buffer, length);
	return -1;
}






UDPSocket::UDPSocket(const char *wSrcIP, unsigned short wSrcPort)
	:DatagramSocket()
{
	open(wSrcPort, wSrcIP);
}


UDPSocket::UDPSocket(const char *wSrcIP, unsigned short wSrcPort,
		     const char *wDestIP, unsigned short wDestPort)
	:DatagramSocket()
{
	open(wSrcPort, wSrcIP);
	destination(wDestPort, wDestIP);
}



void UDPSocket::destination( unsigned short wDestPort, const char * wDestIP )
{
	resolveAddress((sockaddr_in*)mDestination, wDestIP, wDestPort );
}


void UDPSocket::open(unsigned short localPort, const char *wlocalIP)
{
	// create
	mSocketFD = socket(AF_INET,SOCK_DGRAM,0);
	if (mSocketFD<0) {
		perror("socket() failed");
		throw SocketError();
	}

	// pat added: This lets the socket be reused immediately, which is needed if OpenBTS crashes.
	int on = 1;
	setsockopt(mSocketFD, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));


	// bind
	struct sockaddr_in address;
	size_t length = sizeof(address);
	bzero(&address,length);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(wlocalIP);
	address.sin_port = htons(localPort);
	if (bind(mSocketFD,(struct sockaddr*)&address,length)<0) {
		perror("bind() failed");
		throw SocketError();
	}
}



unsigned short UDPSocket::port() const
{
	struct sockaddr_in name;
	socklen_t nameSize = sizeof(name);
	int retVal = getsockname(mSocketFD, (struct sockaddr*)&name, &nameSize);
	if (retVal==-1) throw SocketError();
	return ntohs(name.sin_port);
}

// vim:ts=4:sw=4
