#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <math.h>


#define PORT 8080

int x=7, y=11, n, t, i, flag;
long int e[50], d[50], temp[50], j;
char en[50], m[50];
char http_response[500];   
int prime(long int); 
void encryption_key();
long int cd(long int);
void encrypt();
void decrypt();


int main(int argc, char *argv[]){

    char *address;
    address = argv[1];


    //create socket
    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM,0);

    //connect to an address
    struct sockaddr_in server;
    server.sin_family = AF_INET; //address family
    server.sin_port = htons(PORT); //specify port
    inet_aton(address, &(server.sin_addr));
    //connect to server
    if( connect(client_socket, (struct sockaddr *) &server,sizeof(server))<0)
    {
        puts("[CLIENT] : Connect Error.");
        return 1;
    }
    puts("[CLIENT] : Connected");



    char request[] = "\nGET /ezzuan.html HTTP/1.1\r\nContent-Type:text/html\r\nHost : 192.168.253.13\r\nAccept-Encoding: gzip, compress\r\nAccept-Language: en\n\n"; //HTTP Request
    char http_response[4096];

    puts("[CLIENT] : Sending HTTPS Request ...");


    send(client_socket,request, sizeof(request),0);//SEND HTTPS REQUEST
    recv(client_socket,&http_response, sizeof(http_response),0); //RECEIVE HTTPS REPONSE
     //ENCRYPTION AND DECRYPTION PROCESS-------------------------------------------------------------  
    flag = prime(x);
    flag = prime(y);

    for(i = 0; http_response[i]; i++)
      m[i] = http_response[i];
    n = x * y;
    t = (x - 1) * (y - 1);

     encryption_key();
 
    //END OF PROCESS--------------------------------------------------------------------------------
    printf("[CLIENT] : RECEIVED HTTP RESPONSE ...\n");
    encrypt();
    decrypt(); 
    printf("[CLIENT] : Response from the server: %s\n", http_response);
    close(client_socket);
    return 0;
}


    //ENCRYPT AND DECRYPT ALGORITHM FUNCTIONS-------------------------------------------------------
 int prime(long int pr)
{
   int i;
   j = sqrt(pr);
   for(i = 2; i <= j; i++)
   {
      if(pr % i == 0)
         return 0;
   }
   return 1;
 }

//function to generate encryption key
void encryption_key()
{
   int k;
   k = 0;
   for(i = 2; i < t; i++)
   {
      if(t % i == 0)
         continue;
      flag = prime(i);
      if(flag == 1 && i != x && i != y)
      {
         e[k] = i;
         flag = cd(e[k]);
         if(flag > 0)
         {
            d[k] = flag;
            k++;
         }
         if(k == 99)
         break;
      }
   }
}

long int cd(long int a)
{
   long int k = 1;
   while(1)
   {
      k = k + t;
      if(k % a == 0)
         return(k/a);
   }
}

//function to encrypt the message
void encrypt()
{
   long int pt, ct, key = e[0], k, len;
   i = 0;
   len = strlen(http_response);

   while(i != len)
   {
      pt = m[i];
      pt = pt - 96;
      k = 1;
      for(j = 0; j < key; j++)
      {
         k = k * pt;
         k = k % n;
      }
      temp[i] = k;
      ct= k + 96;
      en[i] = ct;
      i++;
   }
   en[i] = -1;
   for(i=0; en[i] != -1; i++)
      printf("%c",en[i]);

   printf("\n");
}

//function to decrypt the message
void decrypt()
{
   long int pt, ct, key = d[0], k;
   i = 0;
   while(en[i] != -1)
   {
      ct = temp[i];
      k = 1;
      for(j = 0; j < key; j++)
      {
         k = k * ct;
         k = k % n;
      }
      pt = k + 96;
      m[i] = pt;
      i++;
   }
   m[i] = -1;
   printf("\n[CLIENT] : DECRYPTING . . .");
   for(i = 0; m[i] != -1; i++)
      printf("%c",m[i]);

    printf("\n");
}