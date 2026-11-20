#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void send_file(char* filename, char* addr);
void read_file(char* address);

int main(int argc, char* argv[]){
	setbuf(stdout, NULL);
	if(argc < 3)
	{
		fprintf(stdout, "m-m-maybe u should choose meowde..? orrr u can type 'h' for help:3");
		return 1;
	}
	switch(argv[1][0])
	{
		case 'r':
			if(argc < 3){fprintf(stdout, "p-pplease.. give m-me your milk.. address"); return 1;}
			read_file(argv[2]);
			break;
		case 's':
			if(argc < 3){fprintf(stdout, "p-pplease.. give m-me your milky.. file"); return 1;}
			send_file(argv[2], argv[3]);
			break;
		case 'h':
			fprintf(stdout, "commands : \n 1. r - receive file \n 2. s - send file");
			break;
	}

    return 0;
}

void send_file(char* filedata, char* addr){
	FILE* f = fopen(filedata, "rb");
	char* buffer = malloc(1024);
	fread(buffer, 1024, 1, f);
	fprintf(stdout, "Transferring %s\n", filedata);
	fprintf(stdout, "Waiting for client...\n");
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in settings = (struct sockaddr_in){
		.sin_family = AF_INET, 
		.sin_addr.s_addr = INADDR_ANY, 
		.sin_port = htons(25656)
	};

	int err = bind(fd, (struct sockaddr*)&settings, sizeof(settings));
 
    err = listen(fd, 3);

    int client = accept(fd, NULL, NULL);
    send(client, buffer, 1024, 1);
    close(client);
    close(fd);
	fclose(f);
    fprintf(stdout, "Client connected.\n");
	int _fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in _settings = (struct sockaddr_in){
		.sin_family = AF_INET, 
		.sin_addr.s_addr = INADDR_ANY, 
		.sin_port = htons(25655)
	};
	inet_aton(addr, &_settings.sin_addr);
	sleep(2);
	connect(_fd, (struct sockaddr*)&_settings, sizeof(_settings));

 	buffer = malloc(2);
 	read(_fd, buffer, 2);
	if(strcmp(buffer, "OK") == 0) fprintf(stdout, "File transfered!\n");
}

void read_file(char* addr){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in settings = (struct sockaddr_in){
		.sin_family = AF_INET, 
		.sin_addr.s_addr = INADDR_ANY, 
		.sin_port = htons(25656)
	};
	fprintf(stdout, "Waiting for file..\n");
	inet_aton(addr, &settings.sin_addr);
 	connect(fd, (struct sockaddr*)&settings, sizeof(settings));

 	char* buffer = malloc(1024);
 	read(fd, buffer, 1024);

	int _fd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in _settings = (struct sockaddr_in){
		.sin_family = AF_INET, 
		.sin_addr.s_addr = INADDR_ANY, 
		.sin_port = htons(25655)
	};

	int err = bind(_fd, (struct sockaddr*)&_settings, sizeof(_settings));
 
	fprintf(stdout, "Waiting for handshake\n");
    err = listen(_fd, 3);
	int client = accept(_fd, NULL, NULL);
	char* _buffer = "OK";
    send(client, _buffer, 2, 1);

	fprintf(stdout, "File transfered!\n");

	FILE* f = fopen("output", "wb");
 	fprintf(f, "%s", buffer);
	fclose(f);
	free(buffer);
 	close(fd);
}