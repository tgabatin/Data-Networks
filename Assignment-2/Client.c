/**
 * @file Client.c
 * @author Taylor Gabatino
 * @brief A simple TCP client program in C that will connect to server program, 
 *        print the data that was received, and terminate. Demonstrates receiving
 *        data using a POSIX socket.
 *        
 *        Please use this code in conjunction with the server.c code provided in the previous assignment.
 *        Connect using `./server 80` to connect to port 80.
 *        Once the connection has been established, utilize this code in parallel by typing:
 *        `./client.c localhost 80`
 *        The time stamp received from the server should print. 
 * @version 0.1
 * @date 2022-06-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

int main(int argc, char *argv[])
{

    /** Error checking for arguments. */
    if (argv[1] == NULL || argv[2] == NULL)
    {
        printf("No argument has been detected. Please provide an IP address and port number. \n");
        exit(0);
    }

    /** Declaration of variables. */
    int sockfd, connfd, err;
    char server_response[256]; // How to null terminate the string
    /**struct sockaddr_in server_addr;**/
    struct addrinfo serv_addr, *p;

    /** Creation of the IP & Ports. */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.ai_family = AF_UNSPEC;
    serv_addr.ai_socktype = SOCK_STREAM;
    serv_addr.ai_flags = AI_PASSIVE;

    err = getaddrinfo(argv[1], argv[2], &serv_addr, &p);

    /** Error checking for the address and port information. */
    if (err != 0) 
    {
        perror("getaddrinfo");
        printf("getaddrinfo %s\n",strerror(errno));
        printf("getaddrinfo : %s \n",gai_strerror(err));
        return 1;
    }

    /** Creation of the socket. */
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

    /** Error checking for the socket creation. */
    if (sockfd == -1) 
    {
            printf("Socket creation failed.\n");
            printf("Error code: %d\n", errno);
            exit(0);
    } else 
    {
            printf("Socket successfully created.\n");
    }

    /** Creation of the connection .*/
    connfd = connect(sockfd, p->ai_addr, p->ai_addrlen);
    
    /** Error checking for the connection. */
    if (connfd == -1)
    {
        printf("Issue with the connection.\n");
        printf("Error code: %d\n", errno);
        exit(0);
    } else 
    {
        printf("Connected to the server. \n");
    }

    /** Receiving ther file from the server. */
    int file_length = recvfrom(sockfd, &server_response, sizeof(server_response), 0, NULL, NULL);
    /** Printing what was received from the server. */
    printf("%.*s\n", file_length, server_response);
    /** Close the connection. */
    close(sockfd);
    return 0;
}