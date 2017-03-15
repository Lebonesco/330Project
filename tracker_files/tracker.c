//
//  tracker.c
//
//  Peer Tracker file
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "tracker.h"

#define PORT "8500"  // the port users will be connecting to

#define BACKLOG 100     // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    
    while(waitpid(-1, NULL, WNOHANG) > 0);
    
    errno = saved_errno;
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// Add client to total peer list
void updateTotalList(char **array, int count , const char *s){
    // Reallocate array for new addresses that connected
    //array = realloc(array, count * sizeof(*array));

    // Add address that just connected to the list
    //array[count-1] = malloc(INET6_ADDRSTRLEN * sizeof(char));
    strcpy(array[count-1],s);
}

// Add port number to port list
void updatePortList(char **array, int count , const char *s){
    // Reallocate array for new port num
    //array = realloc(array, count * sizeof(*array));
    
    // Add port num to list
    //array[count-1] = malloc(4 * sizeof(char));
    strcpy(array[count-1],s);
}

// Add file name to file name list
void updateNameList(char **array, int count , const char *s){
    // Reallocate array for new file name
    //array = realloc(array, count * sizeof(*array));
    
    // Add file name to list
    //array[count-1] = malloc(20 * sizeof(char));
    strcpy(array[count-1],s);
}


void initialize_array(char *array, int x, int y){
    for (int i = 0; i < x;i++){
        strcpy(&array[i]," ");
    }
}

//Free memory used in 2 dimensional array
void freeArray(char*** array, int index){
    for (int i = 0; i < index; i++){
        free((*array)[i]);
    }
    free(*array);
    *array = NULL;
}

// Deallocate memory for one dimensional array
void deallocateArray(char* array){
    free(array);
}

// Encode integer
char* encode_int(char* x){
    char* val = malloc(6*sizeof(*val));
    val[0] = 'i';
    for (int i = 1; i <= 4;i++){
        val[i] = x[i-1];
    }
    val[5] = 'e';
    
    return val;
}

// Encode string
char* encode_str(char* x){
    char* val = malloc (100 * sizeof(*val));
    int length = strlen(x);
    int first = length /10;
    int second = length % 10;
    
    val[0] = first + '0';
    val[1] = second + '0';
    val[2] = ':';
    for (int i = 0; i < length; i++){
        val[i+3] = x[i];
    }
    
    return val;
}



int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv,n;
    char buffer[16];
    char file_name[20];
    char port_number[4];
    
    char **data_array;
    char **port_array;
    char **name_array;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP
    
    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }
    
    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }
        
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }
        
        break;
    }
    
    freeaddrinfo(servinfo); // all done with this structure
    
    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }
    
    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
    
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    
    printf("server: waiting for connections...\n");
    
    // Data array for clients that connect to server
    data_array = (char **)malloc(1 * sizeof(char *));
    for (int i = 0; i < INET6_ADDRSTRLEN; i++){
        data_array[i] = (char *)malloc(INET6_ADDRSTRLEN * sizeof(char));
    }
    
    // Port number array
    port_array = (char **)malloc(5 * sizeof(char *));
    for (int i = 0; i < 4; i++){
        port_array[i] = (char *)malloc(4 * sizeof(char));
    }
    
    // File name array
    name_array = (char **)malloc(1 * sizeof(char *));
    for (int i = 0; i < 20; i++){
        name_array[i] = (char *)malloc(20 * sizeof(char));
    }
    
    puts("Initializing arrays");
    initialize_array(*data_array,1,INET6_ADDRSTRLEN);
    initialize_array(*port_array,5,4);
    initialize_array(*name_array,1,20);
    
    // Number of clients who want to upload something
    int uploader_count = 0;
    
    // Number of filenames that have been sent
    int filename_count = 0;
    
    // Number of clients who have connected
    int count = 0;
    
    
    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }
        
        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
        count++;
        
        printf("Number of peers: %d\n", count);
        
        // Update peer list with new client that just connected
        updateTotalList(data_array,count,s);
        
        // TESTING: Print out contents in list
        puts("Contents in total list:");
        for (int i = 0; i < count; i++){
            printf("%d) %s\n", i+1, data_array[i]);
        }
        
        // TESTING: Print out contents in port list
        puts("Contents in port list:");
        for (int i = 0; i < uploader_count; i++){
            printf("%d) %s\n", i+1, port_array[i]);
        }
        
        // TESTING: Print out contents in file name list
        puts("Contents in file name list:");
        for (int i = 0; i < filename_count; i++){
            printf("%d) %s\n", i+1, name_array[i]);
        }
        
        
        if (!fork()) { // this is the child process
            close(sockfd); // child doesn't need the listener
            
            
            bzero(buffer,16);
            // Receive message from client to see which repsone to send
            n = recv(new_fd,buffer,15,0);
            if (n < 0) perror("ERROR reading from socket");
            
            printf("Message from client: %s\n", buffer);
            
            if (strncmp(buffer,"download",8)==0){
                // Send fake message to client
                puts("requested download");
                send(new_fd, "Bencoded message", 16, 0);
            }
            
            else if (strncmp(buffer,"upload",6)==0){
                puts("requested upload");
                filename_count++;
                uploader_count++;
            
                if (recv(new_fd, port_number,100,0) < 0){
                    printf("Error receiving from client\n");
                }
                printf("Port number received: %s\n", port_number);
                send(new_fd, "Got Port Number",15,0);
                updatePortList(port_array,uploader_count,port_number);
                
                if (recv(new_fd, file_name,100,0) < 0){
                    printf("Error receiving from client\n");
                }
                
                // Add contents received to their repsective lists
                
                updateNameList(name_array,filename_count,file_name);

                printf("File name received: %s\n", file_name);
                
            }
            
            else if (strncmp(buffer,"update",6)==0){
                puts("requested update");
                uploader_count++;
                if (recv(new_fd, port_number,4,0) < 0){
                    printf("Error receiving from client\n");
                }
                updatePortList(port_array,uploader_count,port_number);
                //TODO: Send back updated list
            }
            else {
                // Client sent an invalid message
                printf("Invalid entry\n");
                send(new_fd, "Invalid entry",13,0);
            }

            
            close(new_fd);
            exit(0);
        }
        
        close(new_fd);  // parent doesn't need this
    }
    
    freeArray(&data_array,count+1);
    freeArray(&port_array,uploader_count+1);
    freeArray(&name_array, filename_count+1);
    
    return 0;
}
