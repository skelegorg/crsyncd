#include "server/server.c"
#include "server/config.c"
#include <stdio.h>

int main(int argc, char** argv) {
	if(!start_server(8080)) {
		printf("crsyncd quit");
		return -1;
	}
	return 0;
}
