// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by Marko Wong (z5309371)
// on 10/03/2020
//
// Version 1.0.0 (2020-03-08): Assignment released.
// Version 1.0.1 (2020-03-08): Fix punctuation in comment.
// Version 1.0.2 (2020-03-08): Fix second line of header comment to say minesweeper.c

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Add any extra #defines here.

void initialise_field(int minefield[SIZE][SIZE]);
void print_debug_minefield(int minefield[SIZE][SIZE]);
void mine_amount_square(int minefield[SIZE][SIZE]);

// Place your function prototyes here.

int main(void) {
    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");
    //Scan in the number of pairs of mines.
    int number_of_mines = 0;
    scanf("%d", &number_of_mines);

    //Scan in the pairs of mines and place them on the grid.
    printf("Enter pairs:\n");
    int mine_axis_y = 0;
    int mine_axis_x = 0;
    int remaining_mines = number_of_mines;
                                                //FROM HERE TO COUNTING MINES THERE ARE TOO MANY NESTED STATEMENTS!!!
    if (remaining_mines < (SIZE * SIZE)) {
        //accounts for too much mines
        while (remaining_mines > 0) {
            scanf("%d %d", &mine_axis_y, &mine_axis_x );             
            //accounts for negative coordinates
            if (mine_axis_y < 0 || mine_axis_x <0) {
                remaining_mines = remaining_mines - 1;
            }
            //accounts for vaild coordinates
            else if (mine_axis_y < SIZE && mine_axis_x < SIZE) {
                if (mine_axis_y >= 0 && mine_axis_x >= 0) {
                    minefield[mine_axis_y][mine_axis_x] = HIDDEN_MINE;
                }
                remaining_mines = remaining_mines - 1;
            }
            //accounts for other invaild coordinates
            else {
                remaining_mines = remaining_mines - 1;
            }
            
        }
    }

    //Counting mines for rows
    int row_counter = 0;
    int column_counter = 0;
    int mines_in_row[SIZE] = {0};
    while (row_counter < SIZE) {
        while (column_counter < SIZE) {
            if (minefield[row_counter][column_counter] == HIDDEN_MINE) {
                mines_in_row[row_counter] ++;
            }          
            column_counter = column_counter + 1;         
        }
        row_counter = row_counter + 1;
        column_counter = 0;
    }
    //Counting mines for columns
    int row_counter1 = 0;
    int column_counter1 = 0;
    int mines_in_column[SIZE] = {0};
    while (column_counter1 < SIZE) {
        while (row_counter1 < SIZE) {
            if (minefield[row_counter1][column_counter1] == HIDDEN_MINE) {
                mines_in_column[column_counter1] ++;
            }          
            row_counter1 = row_counter1 + 1;         
        }
        column_counter1 = column_counter1 + 1;
        row_counter1 = 0;
    }

    //printf("there are %d mines in row 2\n", mines_in_row[2]); 
    
    printf("Game Started\n");
    print_debug_minefield(minefield);

    // Scan in commands to play the game until the game ends.
    // A game ends when the player wins, loses, or enters EOF (Ctrl+D).
    // You should display the minefield after each command has been processed.
    
    int axis_to_look = 0;
    int command_type = 0;
    while (scanf("%d %d", &command_type, &axis_to_look) == 2) { 
    
        if (command_type == DETECT_ROW) {           
            printf("There are %d mine(s) in row %d\n", mines_in_row[axis_to_look], axis_to_look);             
        }
        else if (command_type == DETECT_COL) {           
            printf("There are %d mine(s) in column %d\n", mines_in_column[axis_to_look],axis_to_look);        
        }
        
        //STAGE 02 detect square: when you enter the command then the coordinates
        // of the square to want to detect it will count how many mines in a size x 
        //size area.
        if (command_type == DETECT_SQUARE) {
            mine_amount_square(minefield);
        }
        print_debug_minefield(minefield);
    }    

    
       
           
    return 0;
}                                     
//stage 2 counting mines in a square
void mine_amount_square(int minefield[SIZE][SIZE]) { 
    int row_to_look = 0;
    int col_to_look = 0;
    int square_size = 0;   
    scanf("%d %d %d", &row_to_look, &col_to_look, &square_size);
    //actucal counting of mines in a square 
    int row_counter = 0;
    int column_counter = 0;
    int mines_in_square[] = {0};
    //to specify where to start scanning
    row_counter = row_to_look - (square_size / 2);
    column_counter = col_to_look - (square_size / 2);
    //to specify where to stop scanning
    int row_stop = 0;
    int col_stop = 0;
    row_stop = row_to_look + (square_size / 2);
    col_stop = col_to_look + (square_size / 2);
    int mines_counter = 0;
    while (row_counter <= row_stop) {
        while (column_counter <= col_stop) {
            if (minefield[row_counter][column_counter] == HIDDEN_MINE) {
                mines_in_square[mines_counter] ++;
                mines_counter = mines_counter + 1;
            }          
            column_counter = column_counter + 1;         
        }
        row_counter = row_counter + 1;
        column_counter = 0;    
    }
    printf("There are %d mine(s) in the square centered at row %d, column %d of size %d", mines_in_square[row_counter], row_to_look, col_to_look, square_size);
        
}
         
      
                 
// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}

// Print out the actual values of the minefield.
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}
