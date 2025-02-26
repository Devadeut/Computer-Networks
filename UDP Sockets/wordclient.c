// =====================================
// Assignment 2 Submission
// Name: Devashri Prakash Deulkar
// Roll number: 21CS10021
// Link of the pcap file: https://drive.google.com/file/d/1POLdvn5Q9SP68Q_irr6ganq28UeY11a3/view?usp=sharing
// =====================================

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

  
#define PORT 5000 
#define MAXLINE 1000 
  
int main(int argc, char *argv[]) 
{    
    char buffer[100],buffer2[100],num[10]; 
    char *message = argv[1]; 
    int s;
    struct sockaddr_in servaddr; 
      
    // clear servaddr 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET; 
      
    // create datagram socket 
    s = socket(AF_INET, SOCK_DGRAM, 0); 
      
    // request to send datagram (sends filename)
    sendto(s, message, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    
    // waiting for response 
    recvfrom(s, buffer, sizeof(buffer), 0, NULL, NULL); 
    printf("\nReceived from Server:%s\n",buffer); 

    strcpy(buffer2,"NOTFOUND ");
    strcat(buffer2,message);
    if(!strcmp(buffer,buffer2)){
       
        printf("ERROR:FILE NOT FOUND\n");

        // close the descriptor 
        close(s);
    }
    else if(!(strcmp(buffer,"HELLO"))){
        int i = 1;
        
        FILE* fp = fopen("newfile.txt","w");
        fprintf(fp,"%s\n",buffer);
        while (strcmp(buffer,"FINISH"))
        {   
            sprintf(num, "%d", i);
            strcpy(buffer2,"WORD");
            strcat(buffer2,num);

            sendto(s, buffer2, MAXLINE, 0, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
            i++;

            recvfrom(s, buffer, sizeof(buffer), 0, NULL, NULL);
            printf("\nReceived from Server:%s\n",buffer); 
            fprintf(fp,"%s\n",buffer); 

        }
        
        printf("\nWords recieved from server are stored in newfile.txt!\n");
        // close the descriptor 
        close(s);
        

    }
    else{
        //error
        printf("Bruhwtf!\n");
    }
  

} 
