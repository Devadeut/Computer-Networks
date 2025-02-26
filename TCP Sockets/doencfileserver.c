/*=====================================
Assignment 3 Submission
Name: Devashri Deulkar
Roll number: 21CS10021
Link of the pcap file: https://drive.google.com/file/d/1lp4yx-BBkubJOPMbZiB6yMnMJmzinZmO/view?usp=sharing
=====================================*/

#include<stdio.h>
#include<stdlib.h>
#include <ctype.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int CHUNK_SIZE = 20;

// Encryption function
void encrypt(char* infilename, char * key, char* outfilename){
        FILE *fp = fopen(infilename,"r");

        FILE* outfp = fopen(outfilename,"w");

        //encryption scheme
        //ASCII values (0-127)
        char mapupper[128] = {0};
        char maplower[128] = {0};

        for (int i = 0; i < 26; i++) {
            mapupper['A' + i] = key[i];       // Mapping for uppercase letters
            maplower['a' + i] = tolower(key[i]); // Mapping for lowercase letters
        }

        int ch;
        while ((ch = fgetc(fp)) != EOF) {
            if (isupper(ch)) {
                fputc(mapupper[ch], outfp);
            } else if (islower(ch)) {
                fputc(maplower[ch], outfp);
            } else {
                fputc(ch, outfp);  // Preserve spaces and newlines
            }
        }

        fclose(fp);
        fclose(outfp);

}




int main(){

    int	sockfd, newsockfd ; /* Socket descriptors */
	socklen_t	clilen;
	struct sockaddr_in	cli_addr, serv_addr;
    char key[27];
	char buf[100];		/* We will use this buffer for communication */
    pid_t pid;
    fd_set readfds;
    struct timeval timeout;

    if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0){
        perror("Unable to create socket\n");
        exit(0);
    }

    serv_addr.sin_family = AF_INET;         //AF_INET for the internet family    
    serv_addr.sin_addr.s_addr = INADDR_ANY; //This field is set to INADDR_ANY for machines having a single IP address
    serv_addr.sin_port = htons(20000);      //port number 

    if((bind(sockfd,(struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0){
        perror("Unable to bind local address\n");
        exit(0);
    }

    listen(sockfd,5);
    printf("Server is running. Waiting for a connection with a (15 sec) timeout...\n");

    

    while(1){

        // Set up the file descriptor set.
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        //set timeout to 15 seconds
        timeout.tv_sec = 15;
        timeout.tv_usec = 0;

        // Wait for a connection or timeout.
        int activity = select(sockfd + 1, &readfds, NULL, NULL, &timeout);
        if(activity < 0){
            perror("Select error\n");
            close(sockfd);
            exit(0);
        }
        else if(activity == 0){
            printf("\nNo connection within the (15 sec) timeout period. Server shutting down.\n\n");
            close(sockfd);
            exit(0);
        }
        
        clilen = sizeof(cli_addr);
        
        newsockfd = accept(sockfd, (struct sockaddr*) &cli_addr,&clilen);
        
        if (newsockfd < 0) {

			printf("Accept error\n");

			exit(0);

		}
        pid = fork();
        if(pid==0){
            close(sockfd); /* Close the old socket since all communications will be through the new socket.*/
            
            
			

            for(int i=0; i < 100; i++) buf[i] = '\0'; // initialie the buffer
            recv(newsockfd, buf, 100, 0);
            printf("%s\n", buf);

            strcpy(buf,"Message from server");
            send(newsockfd, buf,strlen(buf)+1,0);

            //key
            for(int i=0; i < 100; i++) buf[i] = '\0'; // initialie the buffer
            recv(newsockfd, buf, 100, 0);
            printf("key: %s\n", buf);
            strcpy(key,buf);

            // Get client's IP address and port
            char client_ip[INET_ADDRSTRLEN];  // Buffer for IP
            inet_ntop(AF_INET, &cli_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            int client_port = ntohs(cli_addr.sin_port);

            char filename[50];
            sprintf(filename,"%s.%d.txt",client_ip,client_port);

            FILE *fp = fopen(filename,"wb");
            size_t byteswrite;

            //file content
            for(int i=0; i < 100; i++) buf[i] = '\0'; // initialie the buffer
            while((byteswrite = recv(newsockfd,buf,100,0))>0){
                fwrite(buf,1,byteswrite,fp);
                for(int i=0; i < 100; i++) buf[i] = '\0'; // initialie the buffer
            }
            printf("\nFile received successfully and saved as: %s\n", filename);
            fclose(fp);

            //naming encrypted output file
            char outfilename[100];
            char str[] = ".enc";
            sprintf(outfilename,"%s%s",filename,str);

            //encrypt the file
            encrypt(filename,key,outfilename);
            printf("\nEncryption Done.");
            //send file content
            fp = fopen(outfilename,"r");

            //send file content
            while((byteswrite = fread(buf,1,CHUNK_SIZE,fp))>0){
                buf[byteswrite] = '\0'; // not SURE about this
                
                send(newsockfd, buf,strlen(buf)+1,0);
                usleep(100000); //100ms 
                //printf("%s",buf);
            }
            printf("\n---Encrypted File sent---\n\n");
            fclose(fp); 
            shutdown(sockfd, SHUT_WR);
            close(newsockfd); 
            exit(0);
        }
        wait(NULL);
        close(newsockfd);
        

        
    }
    return 0;
}
