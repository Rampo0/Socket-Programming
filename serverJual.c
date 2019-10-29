#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

//char client_response[100];
int *stock;

void *jual(void *arg){

    int *client_socket;
    client_socket = (int *)arg;
    char client_response[100];
    memset(client_response,0,sizeof(client_response));
    while(/*read(*client_socket,client_response,strlen(client_response))*/recv(*client_socket,&client_response,sizeof(client_response),0)){ 
        
        if(strcmp(client_response,"tambah") == 0){
            
            *stock=*stock+1;
            

        }/*else{
            char message[100] = "bukan command yang tepat";
            if(send(*client_socket,message,sizeof(message),0)<0){
                printf("gagal\n");
            }
        }*/
        
        printf("client sent the data : %s\n",client_response);
        memset(client_response,0,sizeof(client_response));
    }

}

void *stockMonitor(void *arg){

    while(1){
        sleep(5);
        printf("Stock : %d\n",*stock);
    }

}

int main(){

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);

    *stock = 2;

    pthread_t thread,thread2;
    int server_socket;
    server_socket = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    int err=pthread_create(&thread2,NULL,stockMonitor,NULL);
    if(err){
        printf("failed\n");
    }

    bind(server_socket,(struct sockaddr*)&server_address,sizeof(server_address));

    int connect = listen(server_socket,5);

    int client_socket;
    client_socket = accept(server_socket,NULL,NULL);

    //int *arg = malloc(sizeof(*arg));
    //*arg = client_socket;

    char message[100] = "You have reached the server";
    send(client_socket,message,sizeof(message),0);
    //send(client_socket,message,sizeof(message),0);

    err=pthread_create(&thread,NULL,jual,(void *)&client_socket);
    if(err){
        printf("failed\n");
    }

    pthread_join(thread,NULL);

    /*char client_response[100];
    while(1){
        memset(client_response,0,sizeof(client_response));
        recv(client_socket,&client_response,sizeof(client_response),0);
        printf("client sent the data : %s\n",client_response);
    }*/

    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);

    exit(1);
    return 0;

}