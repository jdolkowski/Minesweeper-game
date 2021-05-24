#include<stdio.h>
#include<stdbool.h>
#include<time.h>
#include<conio.h>
#include<stdlib.h>
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 10
#define BOMBS 10

int X_dir = 0, Y_dir = 0;
int bomb_count = 0;
bool game_over = false;
bool win = false;
bool the_end = false;

struct square {
	int value;
	bool isVisible;
	bool isFlagged;
	bool has_bomb;
} board[10][10] = {0};

void bomb_generator();
void graphics();
void value_generator();
//char getch();
void movement();
void visibility(int, int);
void results();
float best_time(float);

int main()
{
    bomb_generator();
    value_generator();
    graphics();
    clock_t start = clock();
    while(!the_end){
        movement();
    }
    system("cls");
    if(win){
        float win_time = (1000*(clock()-start))/CLOCKS_PER_SEC;

        printf("gratulacje wygrales\nzajelo ci to: %.2f s\nnajlepszyczas to: %.2f s", win_time*0.001, best_time(win_time*0.001));
    }else{
        printf("niestety przegrales :(\n");
    }
	return 0;
}

void bomb_generator()
{
    srand(time(NULL));
    for(int i = 0; i < BOMBS;){
        int x = rand()%BOARD_WIDTH;
        int y = rand()%BOARD_HEIGHT;
        if(!board[x][y].has_bomb){
            board[x][y].has_bomb = true;
            i++;
        }
    }
}

void value_generator()
{
    for(int y = 0; y < BOARD_HEIGHT; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){

                if(board[x][y].has_bomb == true){
                    if(x+1 < BOARD_WIDTH && y+1 < BOARD_HEIGHT && board[x+1][y+1].has_bomb == false)
                        board[x+1][y+1].value++;
                    if(x+1 < BOARD_WIDTH && board[x+1][y].has_bomb == false)
                        board[x+1][y].value++;
                    if(x+1 < BOARD_WIDTH && y-1 >= 0 && board[x+1][y-1].has_bomb == false)
                        board[x+1][y-1].value++;
                    if(x-1 >= 0 && y+1 < BOARD_HEIGHT && board[x-1][y+1].has_bomb == false)
                        board[x-1][y+1].value++;
                    if(x-1 >= 0 && board[x-1][y].has_bomb == false)
                        board[x-1][y].value++;
                    if(x-1 >= 0 && y-1 >= 0 && board[x-1][y-1].has_bomb == false)
                        board[x-1][y-1].value++;
                    if(y+1 < BOARD_HEIGHT && board[x][y+1].has_bomb == false)
                        board[x][y+1].value++;
                    if(y-1 >= 0 && board[x][y-1].has_bomb == false)
                        board[x][y-1].value++;
                }
        }
    }
}

/*char getch()
{
    system("/bin/stty raw");
    char ret = getchar();
    system("/bin/stty cooked");
    return ret;
}*/
void graphics()
{
    printf("\t\t\t\t\tliczba zaznaczonych min: %d\n", bomb_count);
    for(int y = 0; y < BOARD_HEIGHT; y++){
        for(int x = 0; x < BOARD_WIDTH; x++){
            if(x == X_dir && y == Y_dir){
                printf(" * ");
            }else if(board[x][y].isFlagged){
                printf("[!]");
            }else if(board[x][y].isVisible){
                if(board[x][y].value == 0)
                    printf("   ");
                else
                    printf(" %d ", board[x][y].value);
            }else{
                printf("[ ]");
            }

        }
        printf("\n");
    }
    printf("\nsterowanie: WSAD \t odkryj pole: E \t oznacz bombe: M\nwyjscie: X");
}

void movement()
{
    switch(getch()){
        case 'w':
            if(Y_dir-1 >= 0){
                Y_dir--;
            }break;

        case 's':
            if(Y_dir+1 < BOARD_HEIGHT){
                Y_dir++;
            }break;

        case 'a':
            if(X_dir-1 >= 0){
                X_dir--;
            }break;

        case 'd':
            if(X_dir+1 < BOARD_WIDTH){
                X_dir++;
            }break;

        case 'e':
            if(board[X_dir][Y_dir].has_bomb){
                game_over = true;
                results();
            }else{
                visibility(X_dir, Y_dir);
            }
            break;

        case 'm':
            if(board[X_dir][Y_dir].isFlagged && board[X_dir][Y_dir].isVisible == false){
                board[X_dir][Y_dir].isFlagged = false;
                if(bomb_count > 0)
                    bomb_count--;
            }else if(board[X_dir][Y_dir].isFlagged == false && board[X_dir][Y_dir].isVisible == false){
                if(bomb_count < BOMBS){
                    board[X_dir][Y_dir].isFlagged = true;
                    bomb_count++;
                    results();
                }
            }
            break;

        case 'x':
            game_over = true;
            results();
            break;
    }
    system("cls");
    graphics();
}

void visibility(int x, int y)
{
    if(x<0 || x>9) return;
    if(y<0 || y>9) return;

    if(board[x][y].isVisible == true) return;

    if(board[x][y].has_bomb == false && board[x][y].value != 9 && board[x][y].isVisible == false)
        board[x][y].isVisible = true;

    if (board[x][y].value != 0) return;


    visibility(x-1,y-1);
    visibility(x-1,y);
    visibility(x-1,y+1);
    visibility(x+1,y-1);
    visibility(x+1,y);
    visibility(x+1,y+1);
    visibility(x,y-1);
    visibility(x,y);
    visibility(x,y+1);
}

void results()
{
    if(bomb_count == BOMBS){
        for(int y = 0; y < BOARD_HEIGHT; y++){
            for(int x = 0; x < BOARD_WIDTH; x++){
                if(board[x][y].isFlagged != board[x][y].has_bomb){
                    game_over = true;
                }else{
                    win = true;
                }
            }
        }
    }
    if(win || game_over){
        the_end = true;
    }
}
float best_time(float current_time)
{
    FILE *f;
    float file_time;
    f = fopen("high_score.txt", "r+");
    fscanf(f, "%f", &file_time);
    printf("%.2f\n", file_time);
    if(current_time < file_time){
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%f", current_time);
        file_time = current_time;
    }
    fclose(f);
    return file_time;
}
