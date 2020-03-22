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
int reveal_square(int minefield[SIZE][SIZE], int row_to_look, int col_to_look);

// Place your function prototyes here.

int main(void) {
    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");
    //STAGE 01 Placing mines: Scan in the coordinates of mines and place them 
    //on the grid.
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
    int game_stopper = 1;
    int hints_left = 3;
    while (scanf("%d", &command_type) == 1 && game_stopper == 1) { 
        
        //STAGE 01 Counting Mines In A Row Or Column: when you enter the command
        //then the coordinates of the row or column to want to detect it will 
        //count how many mines in a row or column.
        if (command_type == DETECT_ROW && hints_left > 0) {
            scanf("%d", &axis_to_look);           
            printf("There are %d mine(s) in row %d\n", mines_in_row[axis_to_look], axis_to_look); 
            hints_left = hints_left - 1;            
        }
        else if (command_type == DETECT_COL && hints_left > 0) {     
            scanf("%d", &axis_to_look);      
            printf("There are %d mine(s) in column %d\n", mines_in_column[axis_to_look], axis_to_look);
            hints_left = hints_left - 1;          
        }
        
        //STAGE 02 Detect Square: when you enter the command then the 
        //coordinates of the square to want to detect it will count how many  
        //mines in a size x size area.
        else if (command_type == DETECT_SQUARE && hints_left > 0) {
            mine_amount_square(minefield);
            hints_left = hints_left - 1;             
        }
        
        //STAGE 03 Restrict Hints: print "Help already used" if Detect Row, 
        //Detect Column, and Detect Square had been used 3 times  

        int extra_input1 = 0;  
        int extra_input2 = 0;
        int extra_input3 = 0;  
        if (hints_left == 0 && command_type != REVEAL_SQUARE) {
            int first_time = 0;        
            if (first_time == 0) {
                first_time++;
                print_debug_minefield(minefield);
                printf ("true\n");                            
            }             
            // extra input is to use up the extra input by the user           
            else if (command_type == DETECT_ROW || command_type == DETECT_COL) {
                scanf("%d", &extra_input1);
            }
            else if (command_type == DETECT_SQUARE) {
                scanf("%d %d %d", &extra_input1, &extra_input2, &extra_input3);
            }                      
            printf("Help already used\n");
        }
        
        //STAGE 02 Reveal Square: when you enter the command then the 
        //coordinates of the square to want to detect it will follow one of the 
        //4 options.
        if (command_type == REVEAL_SQUARE) {
            int row_to_look = 0;
            int col_to_look = 0;
            scanf("%d %d", &row_to_look, &col_to_look);
                      
            int mines_counter = reveal_square(minefield, row_to_look, col_to_look);
            //changing HIDDEN_SAFE to VISIBLE_SAFE
            int row_counter2 = 0;
            int column_counter2 = 0;
            //to specify where to start scanning
            row_counter2 = row_to_look - 1;
            column_counter2 = col_to_look - 1; 
            int col_restart = column_counter2;
            //to specify where to stop scanning
            int row_stop = 0;
            int col_stop = 0;
            row_stop = row_to_look + 1;
            col_stop = col_to_look + 1;  
            
            //1. reveal whole square when there is no mines            
            if (mines_counter == 0) {
                //accounting for when the inputs are on the corners!
                //top left bound 
                if (row_to_look == 0 && col_to_look == 0) { 
                    row_counter2 = row_to_look;
                    column_counter2 = col_to_look;
                    col_restart = col_to_look;                      
                }
                //bottom right bound 
                else if (row_to_look == (SIZE - 1) && col_to_look == (SIZE - 1)) {
                    row_stop = row_to_look;
                    col_stop = col_to_look;
                }
                //top right bound 
                else if (row_to_look == 0 && col_to_look == (SIZE - 1)) {
                    row_counter2 = row_to_look;
                    col_stop = col_to_look;
                }
                //bottom left bound 
                else if (row_to_look == (SIZE - 1) && col_to_look == 0) {
                    column_counter2 = col_to_look;
                    col_restart = col_to_look; 
                    row_stop = row_to_look;
                }               
                //accounting for when the inputs are on the edge (not corners!)
                //top bound
                else if (row_to_look == 0) { 
                    row_counter2 = row_to_look;                
                }
                //bottom bound
                else if (row_to_look == (SIZE - 1)) {
                    row_stop = row_to_look;
                }
                //left bound 
                else if (col_to_look == 0) { 
                    column_counter2 = col_to_look; 
                    col_restart = col_to_look;                    
                }
                //Right bound
                else if (col_to_look == (SIZE - 1)) { 
                    col_stop = col_to_look;
                }  
                
                //actual code that reveals the square
                while (row_counter2 <= row_stop) {
                    while (column_counter2 <= col_stop) {
                        minefield[row_counter2][column_counter2] = VISIBLE_SAFE;                  
                        column_counter2 = column_counter2 + 1;         
                    }
                    row_counter2 = row_counter2 + 1;
                    column_counter2 = col_restart;
                }                                                             
            }
            //2. If input coordinate was mine then game over
            else if (minefield[row_to_look][col_to_look] == HIDDEN_MINE) {
                printf("Game over\n");
                game_stopper = game_stopper - 1;
            }
            //3. reveal the input coordinate if there is a mine adjacent to it
            else if (mines_counter != 0) {
                minefield[row_to_look][col_to_look] = VISIBLE_SAFE;
            }                      
        }
        
        //4. If there is no more HIDDEN_SAFE then game is won
        int row_counter3 = 0;
        int column_counter3 = 0;
        int safe_remaining = 0;
        // scanning if there are safe spots left
        while (row_counter3 < SIZE) {
            while (column_counter3 < SIZE) {
                if (minefield[row_counter3][column_counter3] == HIDDEN_SAFE) {
                    safe_remaining++;
                }          
                column_counter3 = column_counter3 + 1;         
            }
            row_counter3 = row_counter3 + 1;
            column_counter3 = 0;
        }
        if (safe_remaining == 0) {
            printf("Game won!\n");
            game_stopper = game_stopper - 1;
        }
            
        print_debug_minefield(minefield);
        
    }    
   
           
    return 0;
}                

//Function list-----------------------------------------------------------------
                     
//STAGE 02 Detect Square: counting mines in a square
void mine_amount_square(int minefield[SIZE][SIZE]) { 
    //scanning input
    int row_to_look = 0;
    int col_to_look = 0;
    int square_size = 0;   
    scanf("%d %d %d", &row_to_look, &col_to_look, &square_size);
    int row_counter = 0;
    int column_counter = 0;
    int mines_in_square[SIZE * SIZE] = {0};
    //to specify where to start scanning
    row_counter = row_to_look - (square_size / 2);
    column_counter = col_to_look - (square_size / 2);
    
    //printf("row_counter is %d\n", row_counter);
    //printf("col_counter is %d\n", column_counter);
    
    int col_restart = column_counter;
    //to specify where to stop scanning
    int row_stop = 0;
    int col_stop = 0;
    row_stop = row_to_look + (square_size / 2);
    col_stop = col_to_look + (square_size / 2);  
    //printf("row_stop is %d\n", row_stop);
    //printf("col_stop is %d\n", col_stop);  
    //actucal counting of mines in a square 
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
        column_counter = col_restart;            
        //printf("col_counter within the loop is %d\n", column_counter);       
    }
    printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mines_counter, row_to_look, col_to_look, square_size);
        
}
         
//STAGE 02 Reveal Square: 
int reveal_square(int minefield[SIZE][SIZE], int row_to_look, int col_to_look) {
    
    //no adjacent mines, then reveal all of its adjacent squares.
    //this is the same as Detect Square but it will only count mines in size 3
    int row_counter = 0;
    int column_counter = 0;
    int mines_in_square[SIZE * SIZE] = {0};
    //to specify where to start scanning
    row_counter = row_to_look - 1;
    column_counter = col_to_look - 1; 
    int col_restart = column_counter;
    //to specify where to stop scanning
    int row_stop = 0;
    int col_stop = 0;
    row_stop = row_to_look + 1;
    col_stop = col_to_look + 1;
    //accounting for when the inputs are on the corners!
    //top left bound 
    if (row_to_look == 0 && col_to_look == 0) { 
        row_counter = row_to_look;
        column_counter = col_to_look;
        col_restart = 0;
    }
    //bottom right bound
    else if (row_to_look == (SIZE - 1) && col_to_look == (SIZE - 1)) {
        row_stop = row_to_look;
        col_stop = col_to_look;
    }
    //top right bound
    else if (row_to_look == 0 && col_to_look == (SIZE - 1)) {
        row_counter = row_to_look;
        col_stop = col_to_look;
    }
    //bottom left bound
    else if (row_to_look == (SIZE - 1) && col_to_look == 0) {
        column_counter = col_to_look;
        col_restart = 0; 
        row_stop = row_to_look;
    }   
    //accounting for when the inputs are on the edge (not corners!)
    //top bound
    else if (row_to_look == 0) { 
        row_counter = row_to_look;                
    }
    //bottom bound
    else if (row_to_look == (SIZE - 1)) {
        row_stop = row_to_look;
    }
    //left bound 
    else if (col_to_look == 0) { 
        column_counter = 0;
        col_restart = 0;       
    }
    //Right bound
    else if (col_to_look == (SIZE - 1)) { 
        col_stop = col_to_look;
    }              
    
    //actucal counting of mines in a square 
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
        column_counter = col_restart;            
    }
    return mines_counter;

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
