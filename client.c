#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <error.h>

typedef struct request_move
{
    int first, second;
}ship_move;

void display(ship_move a)
{
	printf("the x and y coordinate are: %d %d\n", a.first, a.second);
}

void tcp_connection_client(int * client_fd)
{
	struct sockaddr_in serv_addr;

    *client_fd = socket(AF_INET, SOCK_STREAM, 0);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(3000);

	printf("Ready to ask\n");
	if( connect(*client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
	{
		perror("Error in connecting\n");
		exit(0);
	}
}

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

void start_game()
{
	int client_fd;
	tcp_connection_client(&client_fd);

	while(1)
	{
		put_response();
		make_move();
	}
	close(client_fd);
}

int main(int argc, char *argv[])
{
	

	return 0;
}