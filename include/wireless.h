#pragma once

#include <windows.h>
#include <winsock.h>

#ifndef WINVER
	#define WINVER 0x0500
#endif

struct server {
	SOCKET listener;
	SOCKET client;
	SOCKADDR_IN addressInfo;
	int addressInfoSize;
};

void enableWireless(void);
void disableWireless(void);

int server_init(struct server *self, int port);
void server_deinit(struct server *self);

int server_getRequest(struct server *self, char *buffer, size_t size);
int server_sendResponse(struct server *self, char *buffer, size_t size);
