#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <math.h>

#define PORT 8080


int x=7, y=11, n, t, i, flag;
long int e[50], d[50], temp[50], j;
char en[50], m[50];
char http_header[500]="HTTP 1.1/ 200 OK\r\n\r\n";    //HTTP RESPONSE /implement
int prime(long int); 
void encryption_key();
long int cd(long int);
void encrypt();
void decrypt();


int main(){
    
    //Open a file to serve to client
    FILE *html_data; //to hold the information from our file and read from //FILE*stream
    html_data = fopen("ezzuan.html","r");
    char http_header[500]="HTTP 1.1/ 200 OK\r\n\r\n";    //HTTP RESPONSE /implement
    char http_response_data[1024];
    fgets(http_response_data,1024,html_data);
    //HTTP response //HTTP protocol implementation
    strcat(http_header, http_response_data);
    
    //ENCRYPTION AND DECRYPTION PROCESS-------------------------------------------------------------  
   flag = prime(x);
   flag = prime(y);

   for(i = 0; http_header[i]; i++)
      m[i] = http_header[i];
   n = x * y;
   t = (x - 1) * (y - 1);

   encryption_key();
 
    //END OF PROCESS--------------------------------------------------------------------------------

    //Create a Socket-------------------------------------------------------------------------------
    int server_soc;
    server_soc = socket(AF_INET,SOCK_STREAM,0);
    if (server_soc == -1)
    {
        perror("[HTTPS-SERVER] : Failed to create socket");
    }
    //----------------------------------------------------------------------------------------------

    //Address Definition----------------------------------------------------------------------------
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET; //address family
    server_address.sin_port = htons(PORT); //specify port
    server_address.sin_addr.s_addr = INADDR_ANY; 
    //----------------------------------------------------------------------------------------------

    //Bind to Client--------------------------------------------------------------------------------
    if(bind(server_soc, (struct sockaddr *) &server_address, sizeof(server_address))<0)
    {
        puts("[HTTPS-SERVER] : Bind Failed");
        return 1;
    }
    puts("[HTTPS-SERVER] : Bind Successful");
    //----------------------------------------------------------------------------------------------
    
    //listen the connection on socket---------------------------------------------------------------
    listen(server_soc,5);
   //-----------------------------------------------------------------------------------------------
    
    
    //placeholder for client socket that going to be opened
    int client_soc;char client_reply[200];
    puts("[HTTPS-SERVER] : Waiting for incoming connection . . .");
    while(1)
    {        
         client_soc = accept(server_soc,NULL,NULL); //Accept connection from client
         puts("[HTTPS-SERVER] : CONNECTED");
         recv(client_soc,&client_reply,sizeof(client_reply),0);
         printf("[HTTPS-SERVER] : RECEIVE HTTP REQUEST ...\n");
         puts(client_reply);
         encrypt(); // THE ENCRYPTED RESPONSE
         decrypt(); // SHOW THE ORIGINAL RESPONSE
         printf("\n\n[HTTPS-SERVER] : SENDING HTTP RESPONSE");
         send(client_soc, http_header, strlen(http_header),0);
    }
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
   len = strlen(http_header);

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
   printf("[HTTPS-SERVER] - THE ENCRYPTED RESPONSE IS : ");
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
   printf("\n[HTTPS-SERVER] : THE DECRYPTED RESPONSE IS  : ");
   for(i = 0; m[i] != -1; i++)
      printf("%c",m[i]);

    printf("\n\n[HTTPS-SERVER] : SENDING RESPONSE TO CLIENT . . . ");
}