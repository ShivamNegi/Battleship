#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SIZE 10
#define NO_SHIPS 17

/*
    In the gird.
    0 - Sea
    1 - Ship
    -1 - Sea Hit
    -2 - Boat Hit
    -3 - Opponent Won
*/

int counter = 0;
int og[SIZE][SIZE], game_grid[SIZE][SIZE], opponent_game_grid[SIZE][SIZE];
char col[25] = "  |0|1|2|3|4|5|6|7|8|9|";
char row[10][4] = {
                " 0|",
                " 1|",
                " 2|",
                " 3|",
                " 4|",
                " 5|",
                " 6|",
                " 7|",
                " 8|",
                " 9|" };

typedef struct request_move
{
    int first, second;
}ship_move;

void clearScreen()
{
    const char * CLEAR_SCREE_ANSI = "\e[1;1H\e[2J";
    write(STDOUT_FILENO, CLEAR_SCREE_ANSI, 12);
    printf("\n");
}

void print_outer_grid()
{
    clearScreen();
    int i, j, k;
    for(i = 0; i < sizeof(col) / sizeof(char); i++)
        printf("%c", col[i]);
    printf("\n");
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 4; j++)
            printf("%c", row[i][j]);
        for(k = 0; k < 10; k++)
            {
                if(og[i][k] == 0)
                    printf(" ");
                if(og[i][k] == 1)
                    printf("O");
                printf("|");
            }
        printf("\n");
    }
    printf("\n");
    printf("--Legend--\n");
    printf("O Ship\n");
    printf("@ Ship with hit\n");
    printf("X Missed Attack\n\n");
}

void print_game_grid()
{
    clearScreen();
    int i, j, k;

    for(i = 0; i < sizeof(col) / sizeof(char); i++)
        printf("%c", col[i]);
    printf("\t\t");
    for(i = 0; i < sizeof(col) / sizeof(char); i++)
        printf("%c", col[i]);

    printf("\n");
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < 4; j++)
            printf("%c", row[i][j]);
        for(k = 0; k < 10; k++)
            {
                if(game_grid[i][k] == 0)
                    printf(" ");
                else if(game_grid[i][k] == 1)
                    printf("0");
                else if(game_grid[i][k] == -1)
                    printf("X");
                else if(game_grid[i][k] == -2)
                    printf("@");
                printf("|");
            }

        printf("\t\t");
        for(j = 0; j < 4; j++)
            printf("%c", row[i][j]);
        for(k = 0; k < 10; k++)
            {
                if(opponent_game_grid[i][k] == 0)
                    printf(" ");
                else if(opponent_game_grid[i][k] == -1)
                    printf("X");
                else if(opponent_game_grid[i][k] == -2)
                    printf("@");
                printf("|");
            }
        printf("\n");
    }
    printf("\n");
    printf("--Legend--\n");
    printf("O Ship\n");
    printf("@ Ship with hit\n");
    printf("X Missed Attack\n\n");
}

void init_grids()
{
    int i, j;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
            {
                og[i][j] = 0;
                game_grid[i][j] = 0;
                opponent_game_grid[i][j] = 0;
            }            
    }
}

int entry_og(int xs, int ys, int xe, int ye, int size)
{
    // checking if in range if not return 0
    if( (xe - xs + ye - ys) != size - 1 || xs < 0 || xs > 9 || ys < 0 || ys > 9 || xe < 0 || xe > 9 || ye < 0 || ye > 9 || og[xe][ye] == 1)
        return 0;
    // traversing variables
    int i, j;
    // marking
    if(xs != xe)
    {
        for(i = xs; i <= xe; i++)
            {
                og[i][ys] = 1;
                game_grid[i][ys] = 1;
            }
    }
    else
    {
        for(i = ys; i <= ye; i++)
            {
                og[xs][i] = 1;
                game_grid[xs][i] = 1;
            }
    }
    return 1;
}

void enter_grid()
{
    clearScreen();
    int xs, xe, ys, ye;
    //inputting
    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 1 1 2\n");
    printf("Aircraft Carrier(size 5):");
    do
    {
        scanf("%d %d %d %d", &xs, &ys, &xe, &ye);
    }while(entry_og(xs, ys, xe, ye, 5) == 0);

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 1 1 2\n");
    printf("Battle Ships(size 4):");
    do
    {
        scanf("%d %d %d %d", &xs, &ys, &xe, &ye);
    }while(entry_og(xs, ys, xe, ye, 4) == 0);

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 1 1 2\n");
    printf("Destroyer(size 3):");
    do
    {
        scanf("%d %d %d %d", &xs, &ys, &xe, &ye);
    }while(entry_og(xs, ys, xe, ye, 3) == 0);

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 1 1 2\n");
    printf("Submarine(size 3):");
    do
    {
        scanf("%d %d %d %d", &xs, &ys, &xe, &ye);
    }while(entry_og(xs, ys, xe, ye, 3) == 0);    

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 1 1 2\n");
    printf("Patrol Boat(size 2):");
    do
    {
        scanf("%d %d %d %d", &xs, &ys, &xe, &ye);
    }while(entry_og(xs, ys, xe, ye, 2) == 0);    
    print_outer_grid();
}

int update(int response, ship_move opponent)
{
    if(response == -3)
        return 0;
    opponent_game_grid[opponent.first][opponent.second] = response;
    return 1;
}

int check_update(ship_move challenger)
{
    if(game_grid[challenger.first][challenger.second] == 1)
    {
        game_grid[challenger.first][challenger.second] = -2;
        if(++counter == NO_SHIPS)
            return -3;
        return -2;
    }
    game_grid[challenger.first][challenger.second] = -1;
    return -1;
}

void make_move(int client_fd)
{
    ship_move challenger;
    printf("Attack\n");
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
    send(client_fd, &response, sizeof(response), 0);
}

void tcp_connection_server(int * server_fd, int * client_fd)
{
    struct sockaddr_in server_addr, client_addr;    
    int client_addr_len, port_no;

    printf("Enter the port no: ");
    scanf(" %d", &port_no);

    *server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port_no);

    printf("\tBinding\n");

    while( bind(*server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0 )
    {
        clearScreen();
        perror("Error in Binding.\n");

        printf("\tBinding\n");
        printf("Enter the port no: ");
        scanf(" %d", &port_no);
        server_addr.sin_port = htons(port_no);
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
        print_game_grid();
        make_move(client_fd);
        print_game_grid();
        put_response(client_fd);
        print_game_grid();
    }
    close(server_fd);    
}

int main()
{
    int i, j, k, l;
    char name[20];

    printf("Player name: ");
    scanf("%s", name);

    init_grids();
    enter_grid();
    start_game();
 
    return 0;
}