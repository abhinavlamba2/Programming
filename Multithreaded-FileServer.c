#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //for sockaddr_in
#include <string.h>
#include <unistd.h>

#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 4000 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/
#define AVAI_PORT 8000 /*use this port when previous port is unavailable*/
pthread_mutex_t m_acc;

char buf[MAXLINE];
int n;

void do_service(int sd2)

{

  /*while ( (n = recv(p, buf, MAXLINE,0)) > 0)  { 

   printf("%s","String received from and resent to the client:"); 

   puts(buf); 

   send(p, buf, n, 0); 

  } 

  if (n < 0){ 

   printf("%s\n", "Read error"); 

  exit(0); 

}*/

  int alen; /* length of address */

  char buf_recv[1000], buf_send[1000]; /* buffer for string the server send 1000 */

  char file_buffer[10000], f_buffer[1000];

  int n;
  FILE *fp;

  printf("\nRequesting client to send file name \n");

  sprintf(buf_send, "Please enter the file name: ");

  send(sd2, buf_send, strlen(buf_send), 0);

  printf("\nReceiving the file name:\n");

  n = recv(sd2, buf_recv, 1000, 0);

  buf_recv[n] = '\0';

  printf("%s\n", buf_recv);

  fflush(stdout);

  printf("Checking FILE: %s exists or not...\n", buf_recv);

  if ((fp = fopen(buf_recv, "r")) == NULL)

  {
    printf("Not found\n");
    sprintf(buf_send, "File could not be found!!!");
    send(sd2,buf_send,strlen(buf_send),0);
    return;
  }

  else

  {

    printf("\nFile found!!!\n");

    sprintf(buf_send, "File found!!!\n");

    send(sd2, buf_send, strlen(buf_send), 0);
  }

  printf("Sending the file content to client....\n");
  file_buffer[0] = 0;
  printf("Bufer berfore: %s\n",file_buffer);
  while (!feof(fp)) //loops till eof

  {

    fgets(f_buffer, 1000, fp); //extracts 1000 chars from file
    strcat(file_buffer, f_buffer);
    if (feof(fp))
      break;

  }

  fclose(fp);
  printf("Sending filelength: %d\n",strlen(file_buffer));
  send(sd2, file_buffer, strlen(file_buffer), 0); //sends 1000 extracted byte

  close(sd2);

  printf("[Server] Connection with Client closed. Server will wait now...\n");

} //end of do_service

//do service function executed by child

int curr_sd, n;

void *body(int *arg)

{

  struct sockaddr_in c_add;

  int addrlen;

  int i, l;

  int base_sd = (int)arg;

  int sd;

  while (true)
  {

    pthread_mutex_lock(&m_acc);

    sd = accept(base_sd, &c_add, &addrlen);

    pthread_mutex_unlock(&m_acc);

    printf("\nServer connected now sd=%d\n", sd);

    do_service(sd);

    close(sd);
  }
}

//creating socket and listening

int connectbody()

{

  int base_sd2;

  struct sockaddr_in c_add, servaddr;
  int p = 0;
  if ((base_sd2 = socket(AF_INET, SOCK_STREAM, 0)) < p)
  {

    perror("Problem in creating the socket");

    exit(2);
  }

  //preparation of the socket address

  servaddr.sin_family = AF_INET;

  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  servaddr.sin_port = htons(SERV_PORT);

  //bind the socket

  bind(base_sd2, (struct sockaddr *)&servaddr, sizeof(servaddr));

  //listen to the socket by creating a connection queue, then wait for clients

  listen(base_sd2, LISTENQ);

  printf("%s\n", "Server running...waiting for connections.");

  return base_sd2;
}

int main(int argc, char **argv)

{

  //Create a socket and listen

  int base_sd;

  base_sd = connectbody();

  if (pthread_mutex_init(&m_acc, NULL) != 0) { 
      printf("\n mutex init has failed\n"); 
      return 1; 
  } 
  //accepting the maximum queud connections

  pthread_t t[4];

  struct sockaddr_in c_add;

  socklen_t addrlen;

  addrlen = sizeof(c_add);

  int i;

  for (i = 0; i < 4; i++)
  {
    printf("thread created with id =t[%d]\n", i);
    // printf("%s\n", "thread created for dealing with client requests");
    pthread_create(&t[i], 0, body, (void *)base_sd);
  } //end of infinite for loop
  pause();
}
