#pragma once

#include <stdio.h>

struct fileReader {
	FILE *handle;
	size_t size;
	char *buffer;
};

int fileReader_read(struct fileReader *self, char *filename);
