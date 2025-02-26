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



int main() 
{    
    char buffer[100],buffer2[100],num[20],content[100];
    FILE* fp; 
    int serverfd,i=1; 
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr; 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // Create a UDP Socket 
    serverfd = socket(AF_INET, SOCK_DGRAM, 0);         
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_family = AF_INET;  
   
    // bind server address to socket descriptor 
    bind(serverfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    
    printf("\nServer Running .........\n");

   
    //receive the datagram 
    len = sizeof(cliaddr);
    int n = recvfrom(serverfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len); //receive message from client
    buffer[n] = '\0';
    printf("\nReceived from Client: %s\n",buffer); 

    if (access(buffer, F_OK) == 0) {
        
        fp = fopen(buffer,"r");
        
        fscanf(fp,"%s",content); //"Hello"
        
        
        while (strcmp(content,"FINISH"))
        {   sendto(serverfd, content, MAXLINE, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr)); 

            len = sizeof(cliaddr);
            int n = recvfrom(serverfd, buffer2, sizeof(buffer), 0, (struct sockaddr*)&cliaddr, &len); //receive message from client
            buffer2[n] = '\0';
            printf("\nReceived from Client: %s\n",buffer2); 

            sprintf(num, "%d", i);
            strcpy(buffer,"WORD");
            //if(buffer2==strcat(buffer,num)){
                    fscanf(fp,"%s",content); 
                    
                    i++;               
            //}
        }

        //sending "FINISH"
        sendto(serverfd, content, MAXLINE, 0,(struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
        close(serverfd);

    } else {
        
        char* msg = strcat("NOTFOUND ", buffer);
        // send the response 
        sendto(serverfd, msg, MAXLINE, 0, (struct sockaddr*)&cliaddr, sizeof(cliaddr));
        printf("\nMessage sent to client"); 
        close(serverfd);
    
    } 

    return 0;
} 
