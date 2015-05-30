#include <stdio.h>
#include <stdlib.h>

#include "file.h"

int fileReader_read(struct fileReader *self, char *filename) {
	enum {
		NO_ERROR,
		ERROR_FOPEN,
		ERROR_MALLOC,
		errorCount,
	} error = NO_ERROR;
	
	char *errors[errorCount] = {
		[ERROR_FOPEN] = "Could not open file",
		[ERROR_MALLOC] = "Could not allocate memory",
	};
	
	self->handle = fopen(filename, "rb");
	if(!self->handle) error = ERROR_FOPEN;
	
	else {
		fseek(self->handle, 0, SEEK_END);
		self->size = ftell(self->handle);
		
		self->buffer = malloc(self->size);
		if(!self->buffer) error = ERROR_MALLOC;
	}
	
	if(error & (ERROR_FOPEN | ERROR_MALLOC)) {
		printf("%s!\n", errors[error], error);
		return 0;
	}
	
	rewind(self->handle);
	fread(self->buffer, self->size, 1, self->handle);
	
	fclose(self->handle);
	
	return 1;
}
