#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>

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

int oppo_score = 0, my_score = 0;
int og[SIZE][SIZE], game_grid[SIZE][SIZE], opponent_game_grid[SIZE][SIZE];
char col[25] = "  |A|B|C|D|E|F|G|H|I|J|", buf[100];
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

void disp()
{
    int i;
    for(i = 0; buf[i] != '\0'; i++)
        printf("%c", buf[i]);
    printf("\n");
}

void print_outer_grid()
{
    clearScreen();
    int i, j, k;

    printf("\tYour Grid\n");
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

    printf("\tYour Grid\t\t      Opponenet Grid\n");
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
    printf("Game ends at Score 17.\n\n");
    disp();
    printf("\n");
    printf("Your Score: %d\tOpponent Score: %d", my_score, oppo_score);
    printf("\n");

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
    int xs, xe, y_starting, y_ending;
    char ys, ye;
    //inputting
    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 A 1 B\n");
    printf("Aircraft Carrier(size 5):");
    do
    {
        scanf("%d %c %d %c", &xs, &ys, &xe, &ye);
        y_starting = ys - 'A';
        y_ending = ye - 'A';
    }while(entry_og(xs, y_starting, xe, y_ending, 5) == 0);

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 A 1 B\n");
    printf("Battle Ships(size 4):");
    do
    {
        scanf("%d %c %d %c", &xs, &ys, &xe, &ye);
        y_starting = ys - 'A';
        y_ending = ye - 'A';
    }while(entry_og(xs, y_starting, xe, y_ending, 4) == 0);

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 A 1 B\n");
    printf("Destroyer(size 3):");
    do
    {
        scanf("%d %c %d %c", &xs, &ys, &xe, &ye);
        y_starting = ys - 'A';
        y_ending = ye - 'A';
    }while(entry_og(xs, y_starting, xe, y_ending, 3) == 0);

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 A 1 B\n");
    printf("Submarine(size 3):");
    do
    {
        scanf("%d %c %d %c", &xs, &ys, &xe, &ye);
        y_starting = ys - 'A';
        y_ending = ye - 'A';
    }while(entry_og(xs, y_starting, xe, y_ending, 3) == 0);    

    print_outer_grid();
    printf("Enter ship placments by specifying the the end points of each ship. Ex. Patrol Boat (size 2): 1 A 1 B\n");
    printf("Patrol Boat(size 2):");
    do
    {
        scanf("%d %c %d %c", &xs, &ys, &xe, &ye);
        y_starting = ys - 'A';
        y_ending = ye - 'A';
    }while(entry_og(xs, y_starting, xe, y_ending, 2) == 0);    
    print_outer_grid();
}

int update(int response, ship_move opponent)
{
    if(response == -3)
        return 0;
    if(response == -2)
    {
        char ship_hit[] = {"Nice move! Ship Hit!"};

        int i = 0;
        for(i = 0; ship_hit[i] != '\0'; i++)
            buf[i] = ship_hit[i];
        buf[i] = '\0';
        my_score++;
    }
    else
    {
        char ship_hit[] = {"Attack Missed."};

        int i = 0;
        for(i = 0; ship_hit[i] != '\0'; i++)
            buf[i] = ship_hit[i];
        buf[i] = '\0';
    }
    opponent_game_grid[opponent.first][opponent.second] = response;
    return 1;
}

int check_update(ship_move challenger)
{
    if(game_grid[challenger.first][challenger.second] == 1)
    {
        game_grid[challenger.first][challenger.second] = -2;
        if(++oppo_score == NO_SHIPS)
        {
            char ship_hit[] = {"Oh No! You Lost! >.>"};

            int i = 0;
            for(i = 0; ship_hit[i] != '\0'; i++)
                buf[i] = ship_hit[i];
            buf[i] = '\0';

            return -3;
        }

        char ship_hit[] = {"Alert! Opponent Hit!"};

        int i = 0;
        for(i = 0; ship_hit[i] != '\0'; i++)
            buf[i] = ship_hit[i];
        buf[i] = '\0';

        return -2;
    }

    char ship_hit[] = {"Haha! Opponent Missed"};

    int i = 0;
    for(i = 0; ship_hit[i] != '\0'; i++)
        buf[i] = ship_hit[i];
    buf[i] = '\0';

    game_grid[challenger.first][challenger.second] = -1;
    return -1;
}

void tcp_connection_client(int * client_fd)
{
    struct sockaddr_in serv_addr;
    int port_no;

    printf("Enter the port no: ");
    scanf(" %d", &port_no);

    *client_fd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(port_no);

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
    char y;
    printf("Attack\n");
    printf("Enter the x and y coordinates: ");
    scanf("%d %c", &challenger.first, &y);        
    challenger.second = y - 'A';
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

void start_game()
{
    int client_fd;
    tcp_connection_client(&client_fd);

    while(1)
    {
        print_game_grid();
        put_response(client_fd);
        print_game_grid();
        make_move(client_fd);
        print_game_grid();
    }
    close(client_fd);
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