#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>
#include <error.h>
//error handling to be done

typedef struct request_move
{
    int first, second;
}ship_move;

void make_move(int client_fd)
{
    ship_move challenger;
    printf("Attack\n")
    printf("Enter the x and y coordinates: ");
    scanf("%d %d", &challenger.first, &challenger.second);        
    send(client_fd, &challenger, sizeof(challenger), 0);

    //get response
    int response;
    recv(client_fd, &response, sizeof(response), 0);
    update(response, challenger);
}

void put_response(int client_fd)
{
    ship_move opponent;
    recv(client_fd, &opponent, sizeof(opponent), 0);
    //display(opponent);
    int response = check_update(opponent);
    print_game_grid();
    send(client_fd, &response, sizeof(response), 0);
}

void display(ship_move a)
{
    printf("the x and y coordinate are: %d %d\n", a.first, a.second);
}

void tcp_connection_server(int * server_fd, int * client_fd)
{
    struct sockaddr_in server_addr, client_addr;    
    int client_addr_len;

    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(3000);

    printf("\tBinding\n");
    if( bind(*server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 )
    {
        perror("Error in Binding.\n");
        exit(0);
    }

    printf("\tBinded and Listening\n");
    if( listen(*server_fd, 5) < 0)
    {
        perror("Erorr in Listen()\n");
        exit(0);
    }

    client_addr_len = sizeof(client_addr);

    *client_fd = accept(*server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(*client_fd < 0)
    {
        perror("Error in accept()\n");
        exit(0);
    }

    printf("Successfully connected.\n");
}

void start_game()
{
    int server_fd, client_fd;

    tcp_connection_server(&server_fd, &client_fd);

    while(1)
    {
        make_move(client_fd);
        put_response();
    }
    close(server_fd);    
}

int main(int argc, char *argv[])
{
    start_game();
	return 0;
}
