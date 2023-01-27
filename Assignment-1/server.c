/**
 * @file server.c
 * @author Taylor Gabatino
 * @brief A simple TCP server program that will listen for a connection, reply with an ASCII
 * string of the current date and time, close the connection, and terminate the server program. 
 * @version 0.1
 * @date 2022-06-08
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

/**
 * @brief Main function of server.c 
 * Steps: socket() -> bind() -> listen() -> accept() -> close()
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    /** Error checking for port number detection. */
    if (argv[1] == NULL) {
        printf("No argument detected. Please provide a port number. \n");
        exit(0);
    }

    /** Variable Declaration */
    int sockfd, connfd, err, len, time_sent;
    time_t current_time;
    struct tm *local_time;
    char buffer[80];
    struct addrinfo serv_addr, *res;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    /** Assign IP & Port Number*/
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.ai_family = AF_UNSPEC; 
    serv_addr.ai_socktype = SOCK_STREAM;
    serv_addr.ai_flags = AI_PASSIVE;

    err = getaddrinfo(NULL, argv[1], &serv_addr, &res);

    /** Error checking for address information. If 0, success, if != 0, check address information again. */
    if (err != 0) {
        perror("getaddrinfo");
        printf("getaddrinfo %s\n",strerror(errno));
        printf("getaddrinfo : %s \n",gai_strerror(err));
        return 1;
    }
 
    /** Creation of the socket. */
    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    /** Error checking of socket creation. */
    if (sockfd == -1) {
	    printf("Socket creation failed.\n");
	    exit(0);
    } else {
	    printf("Socket successfully created.\n");
    }
    /** Binding of the socket. */
    bind(sockfd, res->ai_addr, res->ai_addrlen);
    /** Listening on the socket. */
    listen(sockfd, 20);
    addr_size = sizeof their_addr;
    /** Accepting the connection on the socket. */
    connfd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

    /** Formatting of timestamp: 2022/06/0315:06:23 */
    time(&current_time);
    local_time = localtime(&current_time);
    strftime(buffer, sizeof(buffer), "%Y/%m/%d %X \n", local_time);
    len = strlen(buffer);
    /** Sends the formatted timestamp to the client. */
    send(connfd, buffer, len,0);
    /** Close the socket and sends message to the server notifying the close. */
    close(sockfd);
    printf("Socket has been closed.\n");
}