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
void place_mines(int minefield[SIZE][SIZE], int number_of_mines);
void actual_placing(int mine_axis_y, int mine_axis_x, int minefield[SIZE][SIZE]);
int count_mines_row(int hints_left, int mines_in_row[SIZE], int axis_to_look);
int count_mines_col(int hints_left, int mines_in_col[SIZE], int axis_to_look);  
void mine_amount_square(int minefield[SIZE][SIZE], int hints_left);
int reveal_square(int minefield[SIZE][SIZE], int row_to_look, int col_to_look, int gameplay_toggle);
int count_mine_square(int minefield[SIZE][SIZE], int row_to_look, int col_to_look, int square_size);
int safe_left(int minefield[SIZE][SIZE]);
void gameplay(int minefield[SIZE][SIZE], int game_stopper, int mines_counter);
void normal_revert(int minefield[SIZE][SIZE]);


// Place your function prototyes here.

int main(void) {
    int minefield[SIZE][SIZE];
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");
    //STAGE 01 Placing mines: Scan in the coordinates of mines and place them 
    //on the grid.
    //Scan in the number of mines.
    int number_of_mines = 0;
    scanf("%d", &number_of_mines);
    //user prompt
    printf("Enter pairs:\n");
    
    //Scan in the coordinates of mines and place them on the grid.
    //accounts for too many mines
    if (number_of_mines < (SIZE * SIZE)) {       
        place_mines(minefield, number_of_mines);        
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
    int gameplay_toggle = 0;
    int toggle_off = 0;  
    int mines_counter = 0; 
    while (scanf("%d", &command_type) == 1 && game_stopper == 1) { 
        
        //STAGE 01 Counting Mines In A Row Or Column: when you enter the command
        //then the coordinates of the row or column to want to detect it will 
        //count how many mines in a row or column.
        if (command_type == DETECT_ROW) {
            scanf("%d", &axis_to_look);  
            hints_left = count_mines_row(hints_left, mines_in_row, axis_to_look);                             
        }
        else if (command_type == DETECT_COL) {     
            scanf("%d", &axis_to_look);
            hints_left = count_mines_col(hints_left, mines_in_column, axis_to_look);                                         
        }
        
        //STAGE 02 Detect Square: when you enter the command then the 
        //coordinates of the square to want to detect it will count how many  
        //mines in a size x size area.
        else if (command_type == DETECT_SQUARE) {
            mine_amount_square(minefield, hints_left);
            hints_left = hints_left - 1;          
        }       
        
        //STAGE 02 Reveal Square: when you enter the command then the 
        //coordinates of the square to want to detect it will follow one of the 
        //4 options which are listed as 1. 2. 3. 4.        
        else if (command_type == REVEAL_SQUARE) {
            int row_to_look = 0;
            int col_to_look = 0;
            scanf("%d %d", &row_to_look, &col_to_look);
            //1. & 2. & 3. are in the function below
            mines_counter = reveal_square(minefield, row_to_look, col_to_look, gameplay_toggle);
            if (mines_counter == - 1) {
                game_stopper = game_stopper - 2;  
            }                  
        }
            
        //4. If there is no more HIDDEN_SAFE then game is won
        //scans for HIDDEN_SAFE remaining
        int safe_remaining = safe_left(minefield);
        //stopping the game if no HIDDEN_SAFE left
        if (safe_remaining == 0) {
            printf("Game won!\n");
            game_stopper = game_stopper - 1;
        }
        
        //STAGE 03 Formatted Printing:    
        //just changing minefield after command GAMEPLAY_MODE
        if (command_type == DEBUG_MODE) {
            printf("Debug mode activated\n");
            toggle_off = 1;
            gameplay_toggle = gameplay_toggle - toggle_off;
            normal_revert(minefield);
            print_debug_minefield(minefield);
        }
        
        else if (command_type == GAMEPLAY_MODE || gameplay_toggle == 1) { 
            if (gameplay_toggle == 0) {
                printf("Gameplay mode activated\n");
            }           
            gameplay(minefield, game_stopper, mines_counter);
            gameplay_toggle = 1;           
        }
        
        //showing minefield after commands    
        else if (command_type != GAMEPLAY_MODE) {
            print_debug_minefield(minefield);
        }      
    }    
   
           
    return 0;
}                

//Function list-----------------------------------------------------------------

//STAGE 01 Placing Mines: user input coordinates which becomes the location of 
//the mines.
void place_mines(int minefield[SIZE][SIZE], int number_of_mines) {
    int mine_axis_y = 0;
    int mine_axis_x = 0;
    int remaining_mines = number_of_mines;
    //accounts for too much mines
    while (remaining_mines > 0) {
        scanf("%d %d", &mine_axis_y, &mine_axis_x );             
        //accounts for negative coordinates
        if (mine_axis_y < 0 || mine_axis_x < 0) {
            remaining_mines = remaining_mines - 1;
        }
        //accounts for vaild coordinates
        else if (mine_axis_y < SIZE && mine_axis_x < SIZE) {
            actual_placing(mine_axis_y, mine_axis_x, minefield);
            remaining_mines = remaining_mines - 1;
        }
        //accounts for other invaild coordinates
        else {
            remaining_mines = remaining_mines - 1;
        }
    } 
}  
//this function is part of the function above                       
void actual_placing(int mine_axis_y, int mine_axis_x, int minefield[SIZE][SIZE]) {                
    if (mine_axis_y >= 0 && mine_axis_x >= 0 ) {
        minefield[mine_axis_y][mine_axis_x] = HIDDEN_MINE;
    }
}
//STAGE 01 Counting Mines IN Rows: the actual printing out the statement part
int count_mines_row(int hints_left, int mines_in_row[SIZE], int axis_to_look) {         
    if (hints_left > 0) {
        printf("There are %d mine(s) in row %d\n", mines_in_row[axis_to_look], axis_to_look); 
        hints_left = hints_left - 1;
    }
    else if (hints_left <= 0) {
        printf("Help already used\n");             
    }
    return hints_left;
}  
//STAGE 01 Counting Mines IN Columns: the actual printing out the statement part
int count_mines_col(int hints_left, int mines_in_col[SIZE], int axis_to_look) {                 
    if (hints_left > 0) {      
        printf("There are %d mine(s) in column %d\n", mines_in_col[axis_to_look], axis_to_look);
        hints_left = hints_left - 1;
    }
    else if (hints_left <= 0) {
        printf("Help already used\n");
    }           
    return hints_left;
}            
                     
//STAGE 02 Detect Square: counting mines in a square
void mine_amount_square(int minefield[SIZE][SIZE], int hints_left) { 
    //scanning input
    int row_to_look = 0;
    int col_to_look = 0;
    int square_size = 0;   
    scanf("%d %d %d", &row_to_look, &col_to_look, &square_size);
    //counts the amount of mines in a square
    int mines_counter = count_mine_square(minefield, row_to_look, col_to_look, square_size);
    if (hints_left > 0) {
        printf("There are %d mine(s) in the square centered at row %d, column %d of size %d\n", mines_counter, row_to_look, col_to_look, square_size);
    }
    else if (hints_left <= 0) {
        printf("Help already used\n");
    }   
}
         
//STAGE 02 Reveal Square: 1. no adjacent mines, then reveal all of its adjacent 
//squares. 3. reveal the input coordinate if there is a mine adjacent to it
int reveal_square(int minefield[SIZE][SIZE], int row_to_look, int col_to_look, int gameplay_toggle) {
    //counts the amount of mines in a 3x3 square
    int square_size = 3;
    int mines_counter = count_mine_square(minefield, row_to_look, col_to_look, square_size);
    
    //1. reveal whole square when there is no mines 
    if (mines_counter == 0) {
        //changing HIDDEN_SAFE to VISIBLE_SAFE
        int row_counter = 0;
        int column_counter = 0;
        //to specify where to start scanning
        row_counter = row_to_look - 1;
        column_counter = col_to_look - 1; 
        int col_restart = column_counter;
        //to specify where to stop scanning
        int row_stop = 0;
        int col_stop = 0;
        row_stop = row_to_look + 1;
        col_stop = col_to_look + 1;
        
        //This section of accounting for when inputs are the same as the 
        //function above but this is needed as for the actual revealing whole 
        //square needs the bounds again.
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
        
        //actual code that reveals the square
        while (row_counter <= row_stop) {
            while (column_counter <= col_stop) {
                minefield[row_counter][column_counter] = VISIBLE_SAFE; 
                if (gameplay_toggle == 1) {
                    int mines_adjacent = count_mine_square(minefield, row_counter, column_counter, square_size);
                    minefield[row_counter][column_counter] = mines_adjacent;
                    if (mines_adjacent == 1) {
                        minefield[row_counter][column_counter] = -1;
                    }
                    else if (mines_adjacent == 2) {
                        minefield[row_counter][column_counter] = -2;
                    }
                }                   
                column_counter = column_counter + 1;
                       
            }
            row_counter = row_counter + 1;
            column_counter = col_restart;
        }

    }
    //2. If input coordinate was mine then game over    
    else if (minefield[row_to_look][col_to_look] == HIDDEN_MINE) {
        printf("Game over\n");
        mines_counter = -1;
    }
    //3. reveal the input coordinate if there is a mine adjacent to it
    //if there adjacent mines to the user input, then display the number of 
    //mines adjacent to that coordinate.
    else if (mines_counter != 0 && gameplay_toggle == 1) {
        minefield[row_to_look][col_to_look] = mines_counter;
        if (mines_counter == 1) {
            minefield[row_to_look][col_to_look] = -1;
        }
        else if (mines_counter == 2) {
            minefield[row_to_look][col_to_look] = -2;
        }
    } 
    else if (mines_counter != 0 && gameplay_toggle != 1) {
        minefield[row_to_look][col_to_look] = VISIBLE_SAFE;
    }
    
            
    return mines_counter;    
} 

//counts the amount of mines in a square
int count_mine_square(int minefield[SIZE][SIZE], int row_to_look, int col_to_look, int square_size) {
    int row_counter = 0;
    int column_counter = 0;
    //to specify where to start scanning
    row_counter = row_to_look - (square_size / 2);
    column_counter = col_to_look - (square_size / 2); 
    int col_restart = column_counter;
    //to specify where to stop scanning
    int row_stop = 0;
    int col_stop = 0;
    row_stop = row_to_look + (square_size / 2);
    col_stop = col_to_look + (square_size / 2);

    //accounting for when the inputs are on the corners!
    //top left bound 
    if (row_to_look == 0 && col_to_look == 0) { 
        row_counter = row_to_look;
        column_counter = col_to_look;
        col_restart = 0;
    }
    //bottom right bound
    else if (row_to_look == (SIZE - (square_size / 2)) && col_to_look == (SIZE - (square_size / 2))) {
        row_stop = row_to_look;
        col_stop = col_to_look;
    }
    //top right bound
    else if (row_to_look == 0 && col_to_look == (SIZE - (square_size / 2))) {
        row_counter = row_to_look;
        col_stop = col_to_look;
    }
    //bottom left bound
    else if (row_to_look == (SIZE - (square_size / 2)) && col_to_look == 0) {
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
    else if (row_to_look == (SIZE - (square_size / 2))) {
        row_stop = row_to_look;
    }
    //left bound 
    else if (col_to_look == 0) { 
        column_counter = 0;
        col_restart = 0;       
    }
    //Right bound
    else if (col_to_look == (SIZE - (square_size / 2))) { 
        col_stop = col_to_look;
    }              
    //this is the same as Detect Square but it will only count mines in size 3
    //actucal counting of mines in a square 
    int mines_counter = 0;
    while (row_counter <= row_stop) {
        while (column_counter <= col_stop) {
            if (minefield[row_counter][column_counter] == HIDDEN_MINE) {
                mines_counter = mines_counter + 1;
            }          
            column_counter = column_counter + 1;         
        }
        row_counter = row_counter + 1;
        column_counter = col_restart;            
    }
    return mines_counter;
}    
    
//STAGE 02 condition 4: scans for HIDDEN_SAFE remaining
int safe_left(int minefield[SIZE][SIZE]) {       
    int row_counter = 0;
    int column_counter = 0;
    int safe_remaining = 0;
    // scanning if there are safe spots left
    while (row_counter < SIZE) {
        while (column_counter < SIZE) {           
            if (minefield[row_counter][column_counter] == HIDDEN_SAFE) {
                safe_remaining++;
            }          
            column_counter = column_counter + 1;         
        }
        row_counter = row_counter + 1;
        column_counter = 0;
    }
    return safe_remaining;   
}

//STAGE 03 Formatted Printing:
void gameplay(int minefield[SIZE][SIZE], int game_stopper, int mines_counter) {
    if (game_stopper >= 0) {
        printf("..\n");
        printf("\\/\n");
    }
    else {
        printf("XX\n");
        printf("/\\\n");
    }
    
    printf("    00 01 02 03 04 05 06 07\n");
    printf("   -------------------------\n");
    int row_counter = 0;
    int column_counter = 0;
    int row_number = 0;
    while (row_counter < SIZE) {
        printf("%.2d |", row_number);
        while (column_counter < SIZE) { 
            if (game_stopper == -1 && minefield[row_counter][column_counter] == HIDDEN_MINE) {
                printf("() ");
            }                
            else if (minefield[row_counter][column_counter] == VISIBLE_SAFE) {
                printf("   ");
            }    
            else if (minefield[row_counter][column_counter] == HIDDEN_MINE) {
                printf("## ");
            }
            else if (minefield[row_counter][column_counter] == HIDDEN_SAFE) {
                printf("## ");
            }
            else if (minefield[row_counter][column_counter] == -1) {
                printf("01 ");
            }
            else if (minefield[row_counter][column_counter] == -2) {
                printf("02 ");
            }
            else if (minefield[row_counter][column_counter] > 2) {
                printf("%.2d ", mines_counter);
            }
                      
            column_counter = column_counter + 1;         
        }
        row_counter = row_counter + 1;
        column_counter = 0;
        printf("|\n");
        row_number++;
    }
    printf("   -------------------------\n");
   
}
//STAGE 03 DEBUG_MODE; changing back values to normal
void normal_revert(int minefield[SIZE][SIZE]) {
    int row_counter = 0;
    int column_counter = 0;
    while (row_counter < SIZE) {
        while (column_counter < SIZE) {           
            if (minefield[row_counter][column_counter] < 0) {
                minefield[row_counter][column_counter] = 0;
            }          
            else if (minefield[row_counter][column_counter] > 3) {
                minefield[row_counter][column_counter] = 0;
            }
            column_counter = column_counter + 1;         
        }
        row_counter = row_counter + 1;
        column_counter = 0;
    }
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
