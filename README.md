To Start Game:
1. Compile and run Minesweeper.c in Terminal
2. Enter the number of mines
3. Enter the coordinates of the mines Row then Column
4. Enter GamePlay_Mode by entering '5'
5. Play Minesweeper using the commands below


Command codes

DETECT_ROW              1 (Reveal how many mines is on row X. E.g: "1 0" = Reveal no. of mines on row 0)

DETECT_COL              2 (Reveal how many mines is on column X. E.g: "2 0" = Reveal no. of mines on column 0)

DETECT_SQUARE           3 (Reveal how many mines is in the square centered on X,Y with size S. E.g: "3 4 5 2" = Reveal no. of mines around and including on row 4 and column 5 with a size 2 raidus around that point)

REVEAL_SQUARE           4 (Reveal square E.g: '4')

GAMEPLAY_MODE           5 (Gameplay mode hides mines location E.g: '5')

DEBUG_MODE              6 (Debug mode shows mines location E.g: '6')

REVEAL_RADIAL           7 (Reveal how many mines is on X,Y and all on all diagonials of X,Y E.g "7 2 3" = Reveal no of mines on row 2 and column 3 and along it's diagonals)

// Possible square states.

VISIBLE_SAFE    0

HIDDEN_SAFE     1

HIDDEN_MINE     2

Example: 2 mines at (1,1) and (4,4,)

![image](https://user-images.githubusercontent.com/79550698/208594021-8314f5ae-14f6-4266-a788-6fca88aa74cb.png)
