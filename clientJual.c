#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

int main(){
    int network_socket;
    network_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int connection_status = connect(network_socket,(struct sockaddr *)&server_address,sizeof(server_address));
    if(connection_status < 0){
        printf("error making connection to remote socket\n");
    }

    char server_response[100];
    recv(network_socket,&server_response,sizeof(server_response),0);

    printf("server sent the data : %s\n",server_response);
    memset(server_response,0,sizeof(server_response));
    
    /*char message[100] = "sdsad";
    write(network_socket,message,strlen(message));*/

    char message[100];
    scanf("%s",message);
    while(send(network_socket,message,sizeof(message),0)){
        memset(message,0,sizeof(message));
        //recv(network_socket,&server_response,sizeof(server_response),0)
        //printf("server sent the data : %s\n",server_response); 
        /*if(recv(network_socket,&server_response,sizeof(server_response),0) ){ 
            printf("server sent the data : %s\n",server_response); 
        }*/
        scanf("%s",message);
        //memset(server_response,0,sizeof(server_response));
           
    }

    close(network_socket);
    return 0;

}