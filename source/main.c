#include <stdio.h>
#include <stdlib.h>

#include "wireless.h"
#include "file.h"

const char *http = "HTTP/1.1 200 OK\r\n"
"Connection: close\r\n"
"Content-type: text/html\r\n"
"\r\n";

char path[256];
char buffer[1024];
int size;

void sanitise(char *string) {
	char *r;
	
	for(r = string; *r; r++) {
		if(*r != '/' && *r != '\\') {
			*string = *r;
			string++;
		}
	}
	
	*string = '\0';
}

int main(void) {
	struct server server;
	struct fileReader response;
	
	enableWireless();
	
	if(!server_init(&server, 80)) {
		printf("Could not create listening socket!\n");
		return EXIT_FAILURE;
	}
	
	while(1) {
		memset(buffer, '\0', sizeof(buffer));
		
		do {
			size = server_getRequest(&server, buffer, sizeof(buffer));
		} while(size <= 0);
		
		printf("R: %.*s\n", size, buffer);
		
		//GET /index.html HTTP/1.1
		
		if(strncmp(buffer, "GET ", strlen("GET ")) == 0) {
			char *page = buffer + strlen("GET ");
			char *end = strchr(page, ' ');
			
			if(end && strncmp(end + 1, "HTTP", strlen("HTTP")) == 0) {
				snprintf(path, sizeof(path), "%.*s", end - page, page);
			}
			else strcpy(path, "index.html");
		}
		else strcpy(path, "index.html");
		
		sanitise(path);
		
		printf("F: %s\n", path);
		
		if(!fileReader_read(&response, path)) {
			if(!fileReader_read(&response, "404.html")) {
				return EXIT_FAILURE;
			}
		}
		
		size = snprintf(buffer, sizeof(buffer), "%s%.*s", http, response.size, response.buffer);
		server_sendResponse(&server, buffer, size + 1);
		
		printf("S: %.*s\n", size, buffer);
		
		free(response.buffer);
	}
	
	server_deinit(&server);
	
	disableWireless();
	
	return EXIT_SUCCESS;
}
