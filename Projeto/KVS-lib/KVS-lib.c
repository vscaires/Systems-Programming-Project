#include "KVS-lib.h"
#include <errno.h>



int establish_connection (char * group_id, char * secret){    

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if(sock == -1){
        perror("socket creation");
        exit(-1);
    }
   
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    /* Clear structure */
    server_addr.sun_family = AF_UNIX;
    /* UNIX domain address */
    strcpy(server_addr.sun_path, SOCKNAME);

    if(connect(sock, (const struct sockaddr *)&server_addr, sizeof(server_addr))){
        perror("connect");
        exit(-1);
    }
    printf("Socket connected!\n\n");
    printf("Group Name : %s\n", group_id);
    printf("Secret : %s\n\n", secret);
    int i = strlen(group_id);
    write(sock, &i, sizeof(i));
    write(sock, group_id, strlen(group_id));

    return 0;
}


int put_value(char * key, char * value){
    int flag = 1, sz = 0;

    printf("------Put Value-------\n");
    printf("Key : %s\n", key);
    printf("Value : %s\n", value);
    if ((write(sock, &flag, sizeof(flag))) < 0){
        perror("write");
        return(-1);
    }
        
    sz = strlen(key);
    if ((write(sock, &sz, sizeof(sz))) < 0){
        perror("write key size");
        return(-2);
    }
    if (write(sock, key, strlen(key)) < 0) {
        perror("write key");
        return(-3); 
    }

    sz = strlen(value);
    if ((write(sock, &sz, sizeof(sz))) < 0){
        perror("write value size");
        return(-2);
    }
    if (write(sock, value, strlen(value)) < 0) {
        perror("write value");
        return(-3); 
    }

    printf("----------------------\n\n");
    return 1;
}

int get_value(char * key, char ** value){
    int flag = 2, sz = 0;
    printf("------Get value-------\n");
    printf("Key: %s\n", key);
    if ((write(sock, &flag, sizeof(flag))) < 0){
        perror("write flag");
        return(-1);
    }

    sz = strlen(key);
    if ((write(sock, &sz, sizeof(sz))) < 0){
        perror("write key size");
        return(-2);
    }

    if (write(sock, key, strlen(key)) < 0) {
        perror("write key");
        return(-3); 
    }

    read(sock, &sz, sizeof(sz));
    if(sz == -1){
        printf("Key-Value not found...\n");
        printf("----------------------\n\n");
        return(-4);
    }

    value = malloc(sizeof(char*));
    value[0] = malloc(sz*sizeof(char));
    read(sock, value[0], sz);
    
    printf("Value : %s\n", value[0]);
    printf("----------------------\n\n");

    return 1;
}

int delete_value(char * key){
    int flag = 3, sz = 0;
    printf("-----Delete Value-----\n");
    printf("Key: %s\n", key);

    if ((write(sock, &flag, sizeof(flag))) < 0){
        perror("write flag");
        return(-1);
    }

    sz = strlen(key);
    if ((write(sock, &sz, sizeof(sz))) < 0){
        perror("write key size");
        return(-2);
    }
    if (write(sock, key, strlen(key)) < 0) {
        perror("send key");
        return(-3); 
    }
    printf("----------------------\n\n");

    return 1;
}

int register_callback(char * key, void (*callback_function)(char *)){

}

int close_connection(){
    printf("Exiting sock ...%d\n", sock);
    close(sock);
}