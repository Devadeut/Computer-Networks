/*=====================================
Assignment 3 Submission
Name: Devashri Deulkar
Roll number: 21CS10021
Link of the pcap file:https://drive.google.com/file/d/1lp4yx-BBkubJOPMbZiB6yMnMJmzinZmO/view?usp=sharing
=====================================*/

#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int CHUNK_SIZE = 50;

int main(){
    int sockfd;
    struct sockaddr_in serv_addr;
    char buf[100],str[50],key[27],outfile[100],infile[100];
    size_t bytesread;


    
    while(1){
        //create socket
        if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
            perror("Unable to create socket\n");
            exit(0);
        }

        //connect to server
        serv_addr.sin_family = AF_INET;
        inet_aton("127.0.0.1",&serv_addr.sin_addr);
        serv_addr.sin_port = htons(20000);

        if((connect(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0){
            perror("Unable to connect socket\n");
            exit(0);
        }

        //send message
        strcpy(buf,"Message from client");
        send(sockfd, buf,strlen(buf)+1,0);
        //recieve message
        for(int i=0; i<100; i++) buf[i]= '\0';  // initialie the buffer
        recv(sockfd, buf,100,0);
        printf("%s\n",buf);  

        //get filename      
        printf("Enter filename:");
        scanf("%s",str); //enters
        strcpy(infile,str);
        //check if file exists
        if (access(str, F_OK) == 0) {
            printf("Enter key:");
            scanf("%s",key);
            //check if key is 26 characters long
            while(strlen(key)<26){
                printf("Enter key again:");
                scanf("%s",key);
            }
            
            //initialize buffer
            for(int i=0; i < 100; i++) buf[i] = '\0';           
            printf("Task in progress...\n");

            //send key
            strcpy(buf,key);
            send(sockfd,buf,strlen(buf)+1,0); 

            FILE *fp = fopen(str,"r");

            //send file content
            while((bytesread = fread(buf,1,CHUNK_SIZE,fp))>0){
                buf[bytesread] = '\0';
                send(sockfd, buf,bytesread,0);
                usleep(100000); //100ms delay
                
            }
            
            fclose(fp); 
            // Signal end-of-file without closing the socket completely.
            shutdown(sockfd, SHUT_WR);
            
            //recieve file content
            char* s=".enc";
            sprintf(outfile,"%s%s",str,s);
            fp = fopen(outfile,"w");

           
            for(int i=0; i < 100; i++) buf[i] = '\0'; // initialie the buffer
            while((bytesread = recv(sockfd,buf,100,0))>0){
                fwrite(buf,1,bytesread,fp);
                for(int i=0; i < 100; i++) buf[i] = '\0'; // initialie the buffer
            }
            fclose(fp);

            //last promt
            printf("\nFile received successfully and saved as: %s\n", outfile);
            printf("\nFile is Encrypted:\n\tOriginal file: %s\n\tEncrypted file: %s\n\n",infile,outfile);
            printf("\nAny more files[Yes/No]:");
            scanf("%s",str); 
            printf("\n");
            s = "No";
            char s2[] = "NO";
            char s3[] = "no";
            if(!strcmp(str,s))break;
            else if(!strcmp(str,s2))break;
            else if(!strcmp(str,s3))break;
            else { close(sockfd); continue;}
            


        } 
        else {
            printf("FILE %s NOTFOUND \n",str);
            close(sockfd);
            continue; // go back to loop again
        }

    }
    

    
    printf("Client Exiting\n");
    close(sockfd);

    return 0;



}
