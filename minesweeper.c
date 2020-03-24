// Assignment 1 20T1 COMP1511: Minesweeper
// minesweeper.c
//
// This program was written by Marko Wong (z5309371)
// on 10/03/2020
//
// Version 1.0.0 (2020-03-08): Assignment released.
// Version 1.0.1 (2020-03-08): Fix punctuation in comment.
// Version 1.0.2 (2020-03-08): Fix second line of header comment to say 
// minesweeper.c

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
void actualPlacing(int mine_axis_y, int mine_axis_x, int minefield[SIZE][SIZE]);
int count_mines_row(int hints_left, int mines_in_row[SIZE], int axis_to_look);
int count_mines_col(int hints_left, int mines_in_col[SIZE], int axis_to_look);  
void mine_amount_square(int minefield[SIZE][SIZE], int hints_left);
int revealSquare(
    int minefield[SIZE][SIZE], 
    int rowToLook, 
    int colToLook, 
    int gameplay_toggle,
    int command_type
);
int minesInSquare(int minefield[SIZE][SIZE], 
    int rowToLook, 
    int colToLook, 
    int squareSize
);
void actual_reveal_square(
    int row_counter,
    int row_stop, 
    int column_counter, 
    int col_stop, 
    int minefield[SIZE][SIZE], 
    int squareSize, 
    int gameplay_toggle, 
    int col_restart
);
void mines_adjacent(int minefield[SIZE][SIZE], 
    int row_counter, 
    int column_counter, 
    int squareSize, 
    int gameplay_toggle
);
int safe_left(int minefield[SIZE][SIZE]);
void gameplay(int minefield[SIZE][SIZE], int game_stopper, int minesCounter);
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
    int minesCounter = 0; 
    while (scanf("%d", &command_type) == 1 && game_stopper == 1) { 
        
        //STAGE 01 Counting Mines In A Row Or Column: when you enter the command
        //then the coordinates of the row or column to want to detect it will 
        //count how many mines in a row or column.
        if (command_type == DETECT_ROW) {
            scanf("%d", &axis_to_look);  
            hints_left = count_mines_row(
                hints_left, 
                mines_in_row, 
                axis_to_look
            );                             
        }
        else if (command_type == DETECT_COL) {     
            scanf("%d", &axis_to_look);
            hints_left = count_mines_col(
                hints_left, 
                mines_in_column, 
                axis_to_look
            );                                         
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
        else if (
            command_type == REVEAL_SQUARE || 
            command_type == REVEAL_RADIAL
        ) {
            int rowToLook = 0;
            int colToLook = 0;
            scanf("%d %d", &rowToLook, &colToLook);
            //1. & 2. & 3. are in the function below
            minesCounter = revealSquare(
                minefield, 
                rowToLook, 
                colToLook, 
                gameplay_toggle,
                command_type
            );
            if (minesCounter == - 1) {
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
            gameplay(minefield, game_stopper, minesCounter);
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
//------------------------------------------------------------------------------

//STAGE 01 functions--------------------------01--------------------------------
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
            actualPlacing(mine_axis_y, mine_axis_x, minefield);
            remaining_mines = remaining_mines - 1;
        }
        //accounts for other invaild coordinates
        else {
            remaining_mines = remaining_mines - 1;
        }
    } 
}  
//this function is part of the function above                       
void actualPlacing(
    int mine_axis_y, 
    int mine_axis_x, 
    int minefield[SIZE][SIZE]
) {                
    if (mine_axis_y >= 0 && mine_axis_x >= 0 ) {
        minefield[mine_axis_y][mine_axis_x] = HIDDEN_MINE;
    }
}
//STAGE 01 Counting Mines IN Rows: the actual printing out the statement part
int count_mines_row(int hints_left, int mines_in_row[SIZE], int axis_to_look) {         
    if (hints_left > 0) {
        printf("There are %d mine(s) ", mines_in_row[axis_to_look]);
        printf("in row %d\n", axis_to_look); 
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
        printf("There are %d mine(s) ", mines_in_col[axis_to_look]);
        printf("in column %d\n", axis_to_look);
        hints_left = hints_left - 1;
    }
    else if (hints_left <= 0) {
        printf("Help already used\n");
    }           
    return hints_left;
}    
//End of STAGE 01 functions^^^^^^^^^^^^^^^^^^^01^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        
//STAGE 02 functions--------------------------02--------------------------------                     
//STAGE 02 Detect Square: counting mines in a square
void mine_amount_square(int minefield[SIZE][SIZE], int hints_left) { 
    //scanning input
    int rowToLook = 0;
    int colToLook = 0;
    int squareSize = 0;   
    scanf("%d %d %d", &rowToLook, &colToLook, &squareSize);
    //counts the amount of mines in a square
    int minesCounter = minesInSquare(
        minefield, 
        rowToLook, 
        colToLook, 
        squareSize
    );
    if (hints_left > 0) {
        printf("There are %d mine(s) in the square centered ", minesCounter);
        printf("at row %d, column %d", rowToLook, colToLook);
        printf("of size %d\n", squareSize);
    }
    else if (hints_left <= 0) {
        printf("Help already used\n");
    }   
}
         
//STAGE 02 Reveal Square: 1. no adjacent mines, then reveal all of its adjacent 
//squares. 2. If input coordinate was mine then game over 3. reveal the input
//coordinate if there is a mine adjacent to it
int revealSquare(
    int minefield[SIZE][SIZE], 
    int rowToLook, 
    int colToLook, 
    int gameplay_toggle,
    int command_type
) {
    //counts the amount of mines in a 3x3 square
    int squareSize = 3;
    int minesCounter = minesInSquare(
        minefield, 
        rowToLook, 
        colToLook, 
        squareSize
    );
    
    //1. reveal whole square when there is no mines 
    if (minesCounter == 0) {
        //changing HIDDEN_SAFE to VISIBLE_SAFE
        int row_counter = 0;
        int column_counter = 0;
        //to specify where to start scanning
        row_counter = rowToLook - 1;
        column_counter = colToLook - 1; 
        int col_restart = column_counter;
        //to specify where to stop scanning
        int row_stop = 0;
        int col_stop = 0;
        row_stop = rowToLook + 1;
        col_stop = colToLook + 1;
        
        //This section of accounting for when inputs are the same as the 
        //function above but this is needed as for the actual revealing whole 
        //square needs the bounds again.
        //accounting for when the inputs are on the corners!
        //top left bound 
        if (rowToLook == 0 && colToLook == 0) { 
            row_counter = rowToLook;
            column_counter = colToLook;
            col_restart = 0;
        }
        //bottom right bound
        else if (rowToLook == (SIZE - 1) && colToLook == (SIZE - 1)) {
            row_stop = rowToLook;
            col_stop = colToLook;
        }
        //top right bound
        else if (rowToLook == 0 && colToLook == (SIZE - 1)) {
            row_counter = rowToLook;
            col_stop = colToLook;
        }
        //bottom left bound
        else if (rowToLook == (SIZE - 1) && colToLook == 0) {
            column_counter = colToLook;
            col_restart = 0; 
            row_stop = rowToLook;
        }   
        //accounting for when the inputs are on the edge (not corners!)
        //top bound
        else if (rowToLook == 0) { 
            row_counter = rowToLook;                
        }
        //bottom bound
        else if (rowToLook == (SIZE - 1)) {
            row_stop = rowToLook;
        }
        //left bound 
        else if (colToLook == 0) { 
            column_counter = 0;
            col_restart = 0;       
        }
        //Right bound
        else if (colToLook == (SIZE - 1)) { 
            col_stop = colToLook;
        } 
        
        //actual code that reveals the square (moved to a function as then there
        //will be no need to reset the values for REVEAL_RADIAL
        actual_reveal_square(
            row_counter, 
            row_stop, 
            column_counter, 
            col_stop, 
            minefield,
            squareSize, 
            gameplay_toggle, 
            col_restart
        ); 
        
        //STAGE 04 REVEAL RADIAL: revealing a 3×3 square around the selected 
        //square, an 8 pointed star-like shape is revealed outwards. 
        if (command_type == REVEAL_RADIAL) {            
            //spike coming out of the top left corner
            int mines_nearby = 0;
            int intial_row = row_counter + 1;
            int intial_col = column_counter + 1;            
            row_counter = intial_row;
            column_counter = intial_col;   
            if (column_counter != 0 || row_counter != 0) {         
                while (mines_nearby == 0) {
                    if (row_counter > 0 && column_counter > 0) {
                        row_counter = row_counter - 1;
                        column_counter = column_counter - 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                }
            } 
            //spike on top middle
            mines_nearby = 0;
            row_counter = intial_row;
            column_counter = intial_col;
            if (row_counter != 0) {
                while (mines_nearby == 0) {
                    if (row_counter > 0) {
                        row_counter = row_counter - 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                }
            }
            //spike on top right corner
            mines_nearby = 0;
            row_counter = intial_row;
            column_counter = intial_col;
            if (column_counter != (SIZE - 1) || row_counter != 0) {
                while (mines_nearby == 0) {
                    if (row_counter > 0 && column_counter < (SIZE - 1)) {
                        row_counter = row_counter - 1;
                        column_counter = column_counter + 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                }
            }  
            //spike on right middle
            mines_nearby = 0;
            row_counter = intial_row;
            column_counter = intial_col;
            if (column_counter != (SIZE - 1)) {
                while (mines_nearby == 0) {
                    if (column_counter < (SIZE - 1)) {
                        column_counter = column_counter + 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                }
            }
            //spike on bottom right
            mines_nearby = 0;
            row_counter = intial_row;
            column_counter = intial_col;
            if (row_counter != (SIZE - 1) || column_counter != (SIZE - 1)) {
                while (mines_nearby == 0) {
                    if (row_counter > 0 && column_counter < (SIZE - 1)) {
                        column_counter = column_counter + 1;
                        row_counter = row_counter + 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                }
            } 
            //spike on bottom middle
            mines_nearby = 0;
            row_counter = intial_row;
            column_counter = intial_col;
            if (row_counter != 7) {
                while (mines_nearby == 0) {
                    if (row_counter < (SIZE - 1)) {
                        row_counter = row_counter + 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                }
            }
            //spike on bottom left
            
            mines_nearby = 0;
            row_counter = intial_row;
            column_counter = intial_col;
            if (row_counter != (SIZE - 1) || column_counter != 0) {
                while (mines_nearby == 0) {
                    if (row_counter < (SIZE - 1) && column_counter > 0) {
                        row_counter = row_counter + 1;
                        column_counter = column_counter - 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                } 
            } 
            //spike on left middle
            
            mines_nearby = 0;
            row_counter = intial_row;
            column_counter = intial_col;
            if (column_counter != 0) {
                while (mines_nearby == 0) {
                    if (column_counter > 0) {                   
                        column_counter = column_counter - 1;
                        minefield[row_counter][column_counter] = VISIBLE_SAFE;
                        mines_nearby = minesInSquare(
                            minefield, 
                            row_counter, 
                            column_counter, 
                            squareSize
                        );
                    }
                    else {
                        mines_nearby = 1;
                    }               
                }
            }              
       }
            
    }
    //2. If input coordinate was mine then game over    
    else if (minefield[rowToLook][colToLook] == HIDDEN_MINE) {
        printf("Game over\n");
        minesCounter = -1;
    }
    //3. reveal the input coordinate if there is a mine adjacent to it
    //if there adjacent mines to the user input, then display the number of 
    //mines adjacent to that coordinate.
    else if (minesCounter != 0 && gameplay_toggle == 1) {
        minefield[rowToLook][colToLook] = minesCounter;
        if (minesCounter == 1) {
            minefield[rowToLook][colToLook] = -1;
        }
        else if (minesCounter == 2) {
            minefield[rowToLook][colToLook] = -2;
        }
    } 
    else if (minesCounter != 0 && gameplay_toggle != 1) {
        minefield[rowToLook][colToLook] = VISIBLE_SAFE;
    }
    
            
    return minesCounter;    
} 

//STAGE 02: counts the amount of mines in a square
int minesInSquare(
    int minefield[SIZE][SIZE], 
    int rowToLook, 
    int colToLook, 
    int squareSize
) {
    int row_counter = 0;
    int column_counter = 0;
    //to specify where to start scanning
    row_counter = rowToLook - (squareSize / 2);
    column_counter = colToLook - (squareSize / 2); 
    int col_restart = column_counter;
    //to specify where to stop scanning
    int row_stop = 0;
    int col_stop = 0;
    row_stop = rowToLook + (squareSize / 2);
    col_stop = colToLook + (squareSize / 2);
    
    //accounting for when the inputs are on the corners!
    //top left bound 
    if (rowToLook == 0 && colToLook == 0) { 
        row_counter = rowToLook;
        column_counter = colToLook;
        col_restart = 0;
    }
    //bottom right bound
    else if (
        rowToLook == (SIZE - (squareSize / 2)) && 
        colToLook == (SIZE - (squareSize / 2))
    ) {
        row_stop = rowToLook;
        col_stop = colToLook;
    }
    //top right bound
    else if (rowToLook == 0 && colToLook == (SIZE - (squareSize / 2))) {
        row_counter = rowToLook;
        col_stop = colToLook;
    }
    //bottom left bound
    else if (rowToLook == (SIZE - (squareSize / 2)) && colToLook == 0) {
        column_counter = colToLook;
        col_restart = 0; 
        row_stop = rowToLook;
    }   
    //accounting for when the inputs are on the edge (not corners!)
    //top bound
    else if (rowToLook == 0) { 
        row_counter = rowToLook;                
    }
    //bottom bound
    else if (rowToLook == (SIZE - squareSize / 2)) {
        row_stop = rowToLook;
    }
    //left bound 
    else if (colToLook == 0) { 
        column_counter = 0;
        col_restart = 0;       
    }
    //Right bound
    else if (colToLook == (SIZE - (squareSize / 2))) { 
        col_stop = colToLook;
    }              
    //this is the same as Detect Square but it will only count mines in size 3                 ***
    //actucal counting of mines in a square 
    int minesCounter = 0;
    while (row_counter <= row_stop) {
        while (column_counter <= col_stop) {
            if (minefield[row_counter][column_counter] == HIDDEN_MINE) {
                minesCounter = minesCounter + 1;
            }          
            column_counter = column_counter + 1;         
        }
        row_counter = row_counter + 1;
        column_counter = col_restart;            
    }
    return minesCounter;
} 

//STAGE 02: actual code that reveals the square
void actual_reveal_square(
    int row_counter,
    int row_stop, 
    int column_counter, 
    int col_stop, 
    int minefield[SIZE][SIZE],  
    int squareSize, 
    int gameplay_toggle, 
    int col_restart
) {
    while (row_counter <= row_stop) {
        while (column_counter <= col_stop) {
            minefield[row_counter][column_counter] = VISIBLE_SAFE; 
            mines_adjacent(
                minefield, 
                row_counter, 
                column_counter, 
                squareSize, 
                gameplay_toggle
            );                                
            column_counter = column_counter + 1;                      
        }
        row_counter = row_counter + 1;
        column_counter = col_restart;
    }
}            
//STAGE 03 shows the number of adjacent mines (inside stage 02 reveal square)              
void mines_adjacent(
    int minefield[SIZE][SIZE], 
    int row_counter, 
    int column_counter, 
    int squareSize, 
    int gameplay_toggle
) {
    if (gameplay_toggle == 1) {
        int mines_adjacent = minesInSquare(
            minefield, 
            row_counter, 
            column_counter, 
            squareSize
        );
        minefield[row_counter][column_counter] = mines_adjacent;
        if (mines_adjacent == 1) {
            minefield[row_counter][column_counter] = -1;
        }
        else if (mines_adjacent == 2) {
            minefield[row_counter][column_counter] = -2;
        }
    }
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
//End of stage 02 functions^^^^^^^^^^^^^^^^^^^02^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//STAGE 03 functions--------------------------03--------------------------------
//STAGE 03 Formatted Printing:
void gameplay(
    int minefield[SIZE][SIZE], 
    int game_stopper, 
    int minesCounter
) {
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
            if (
                game_stopper == -1 && 
                minefield[row_counter][column_counter] == HIDDEN_MINE
            ) {
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
                printf("%.2d ", minesCounter);
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
//End of STAGE 03 functions^^^^^^^^^^^^^^^^^^^^^^^^03^^^^^^^^^^^^^^^^^^^^^^^^^^^  

//General functions--------------------------------GN---------------------------                     
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
//End of general functions^^^^^^^^^^^^^^^^^^^^^^^^^GN^^^^^^^^^^^^^^^^^^^^^^^^^^^
