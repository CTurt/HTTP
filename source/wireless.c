#include "wireless.h"

void enableWireless(void) {
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
}

void disableWireless(void) {
	WSACleanup();
}

int server_init(struct server *self, int port) {
	self->listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if(self->listener == INVALID_SOCKET) {
		return 0;
	}
	
	memset(&self->addressInfo, 0, sizeof(self->addressInfo));
	self->addressInfo.sin_family = AF_INET;
	self->addressInfo.sin_addr.s_addr = INADDR_ANY;
	self->addressInfo.sin_port = htons(port);
	
	// Non-blocking
	//u_long one = 1;
	//ioctlsocket(self->sock, FIONBIO, &one);
	
	if(bind(self->listener, (LPSOCKADDR)&self->addressInfo, sizeof(struct sockaddr)) == SOCKET_ERROR) {
		return 0;
	}
	
	if(listen(self->listener, SOMAXCONN) == SOCKET_ERROR) {
		return 0;
	}
	
	return 1;
}

int server_getRequest(struct server *self, char *buffer, size_t size) {
	self->client = accept(self->listener, (struct sockaddr *)&self->addressInfo, &self->addressInfoSize);
	return recv(self->client, buffer, size, 0);
}

int server_sendResponse(struct server *self, char *buffer, size_t size) {
	int result;
	
	if(send(self->client, buffer, size, 0) != size) result = 0;
	else result = 1;
	
	closesocket(self->client);
	
	return result;
}
