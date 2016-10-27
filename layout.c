#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define SIZE 10

int og[SIZE][SIZE], game_grid[SIZE][SIZE];
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

void init_grids()
{
    int i, j;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
            {
                og[i][j] = 0;
                game_grid[i][j] = 0;
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

int main()
{
    struct sockaddr_in server;
    int sockfd;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(14022);

    bind(sockfd, (struct sockaddr *) &server, sizeof(server));

    int x, y;
    int len = sizeof(server);
    char c = 'a';
    sendto(sockfd, &c, 1, 0, (struct sockaddr *) &server, sizeof(server));

    recvfrom(sockfd, &x, 4, 0, (struct sockaddr *) &server, &len);
    recvfrom(sockfd, &y, 4, 0, (struct sockaddr *) &server, &len);

    printf("%d %d", x, y);

    // game part
    int i, j, k, l;
    char name[20];

    printf("Player name: ");
    scanf("%s", name);

    init_grids();
    enter_grid();
 
    return 0;
}