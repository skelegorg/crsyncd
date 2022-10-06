// c stuff - file transfer client

#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <stdio.h>

struct outbound_file {
	long buffer_size;
	char filename[100];
};

int main(int argc, char **argv) {
	int sockfd;
	char fname[100];
	int len;

	strcpy(fname, argv[2]);

	struct sockaddr_in servaddr, cliaddr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	// converts argv[1] to a network format and stores it in servaddr.sin_addr
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		printf("failed to connect to port %s\n", argv[1]);
		return -1;
	}

	// open 3rd arg as file
	FILE *fp = fopen(argv[2], "r");
	fseek(fp, 0, SEEK_END);
	// get file buffer length
	long buffer_len = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	char *buffer = malloc(buffer_len);

	struct outbound_file outbound_fd;
	strcpy(outbound_fd.filename, fname);

	outbound_fd.buffer_size = buffer_len;

	fread(buffer, buffer_len, 1, fp);
	// write outbound_file struct to the open connection
	write(sockfd, (struct outbound_file*)&outbound_fd, sizeof(outbound_fd));
	// write to the open buffer to the open connection;
	write(sockfd, buffer, buffer_len);

	free(buffer);
}
