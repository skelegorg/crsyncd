#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

struct inbound_file {
	long buffer_size;
	char filename[100];
};

int start_server(int port) {
	int fd, sockfd, listenfd, connfd;
	pid_t childpid;
	socklen_t client;
	struct sockaddr_in servaddr, cliaddr;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	// bzero just zeroes out a byte string (which sockaddr_in is)
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	// htonl and htons convert network and host numbers, network is big endien first
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// atoi is ascii to integer
	servaddr.sin_port = htons(port);

	// bind to a port
	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
		printf("failed to bind\n");
		return -1;
	}

	listen(listenfd, 5);

	client = sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &client);

	// get an inbound_file struct that contains the recieved file name and length
	struct inbound_file inbound_fd;
	read(connfd, (struct inbound_file*)&inbound_fd, sizeof(inbound_fd));

	FILE *fp;
	char buffer[inbound_fd.buffer_size];

	// read the accepted file request (connfd) into buffer
	read(connfd, buffer, inbound_fd.buffer_size);
	fp = fopen(inbound_fd.filename, "w");
	fprintf(fp, "%s", buffer);
	fclose(fp);
	printf("file placed in %s\n", inbound_fd.filename);

	return 0;
}
