#include <bits/stdc++.h> // Used in Cpp compiler
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 3000 /*port*/

int main(int argc, char **argv)

{

    int sd;

    struct sockaddr_in servaddr;

    char sendline[MAXLINE], recvline[MAXLINE];

    //basic check of the arguments

    //additional checks can be inserted
    if (argc != 2)
    {

        perror("Usage: TCPClient <IP address of the server>");

        exit(1);
    }
    else 
    {
        cout << argc;
    }
    //Create a socket for the client

    //If sd<0 there was an error in the creation of the socket

    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {

        perror("Problem in creating the socket");

        exit(2);
    }

    //Creation of the socket

    memset(&servaddr,  0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;

    servaddr.sin_addr.s_addr = inet_addr(argv[1]);

    servaddr.sin_port = htons(SERV_PORT); //convert to big-endian order

    //Connection of the client to the socket

    if (connect(sd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {

        perror("Problem in connecting to the server");

        exit(3);
    }
    else
        printf("\nconnected to server now.\n");

    /*file start*/

    int n;

    char buf_recv[1000];

    char buf_send[1000];

    char *filename;

    char file_buffer[1000];

    FILE *fp;

    //code for receivind statement "Enter the file name"

    n = recv(sd, buf_recv, sizeof(buf_recv), 0);

    buf_recv[n] = '\0';

    printf("%s", buf_recv);

    printf("\n\nNow sending the file name: \n\n");

    scanf("%s", buf_send);

    send(sd, buf_send, strlen(buf_send), 0);

    printf("\n\nreceiving the string \"File found or not\"\n\n");

    n = recv(sd, buf_recv, sizeof(buf_recv), 0);

    buf_recv[n] = '\0';

    printf("%s",buf_recv);

    fflush(stdout);

    printf("\nReceiving the file content\n");

    n = recv(sd, file_buffer, sizeof(file_buffer), 0);

    file_buffer[n] = '\0';

    fflush(stdout);
    printf("Received content length:%d\n",strlen(file_buffer));
    printf("%s\n",file_buffer);

    fp = fopen("received.txt", "w");
    fputs(file_buffer, fp);
    fclose(fp);
    close(sd);
    return 0;
}
