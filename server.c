// Server side C program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define SIZE 1024
#define PORT 8080

void send_file(FILE *fp, int sockfd)
{
    int n;
    char data[SIZE] = {0};

    while (fgets(data, SIZE, fp) != NULL)
    {
        if (send(sockfd, data, sizeof(data), 0) == -1)
        {
            perror("[-]Error in sending file.");
            exit(1);
        }
        bzero(data, SIZE);
    }
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket;
    long valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    FILE *fp;
    char *hello = "Hello from server";
    char *filename = "small_bunny_1080p_60fps.mp4";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }

    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("[-]Error in reading file.");
        exit(1);
    }

    while (1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read(new_socket, buffer, 30000);
        printf("%s\n", buffer);
        write(new_socket, hello, strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }
    return 0;
}