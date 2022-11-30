#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define COLUMNLENGTH 8
#define ROWLENGTH 8
#define COLUMNSTART 1

/*players Choices*/
static int playerChoice = 0;
static int computerChoice = 0;
/*Game board*/
static char gameBoard[ROWLENGTH][COLUMNLENGTH] = {
    { '0', '0', '0', '0', '0', '0', '0', '0' }, { '0', '0', '0', '0', '0', '0', '0', '0' },
    { '0', '0', '0', '0', '0', '0', '0', '0' }, { '0', '0', '0', '0', '0', '0', '0', '0' },
    { '0', '0', '0', '0', '0', '0', '0', '0' }, { '0', '0', '0', '0', '0', '0', '0', '0' },
    { '0', '0', '0', '0', '0', '0', '0', '0' }, { '0', '0', '0', '0', '0', '0', '0', '0' },
};
/*Letter list*/
char columnLetterPositions[COLUMNLENGTH] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
/*Game Pieces*/
static char *gamePieces = "YR";

int mapPosition(char columnMap[COLUMNLENGTH], char columnLetter) {
    /*Loop through and map the letter to the specific column*/
    for (int i = 0; i < COLUMNLENGTH; i++) {
        if (columnMap[i] == columnLetter) {
            /*return the mapped letter*/
            return i;
        }
    }
    return 0;
}

int resetBoard(char board[COLUMNLENGTH][ROWLENGTH]) {
    /*Var*/
    char desiredPiece;
    /*Sets the board to be empty*/
    for (int row = 0; row < ROWLENGTH; row++) {
        for (int col = 0; col < COLUMNLENGTH; col++) {
            board[row][col] = '0';
        }
    }
    printf("Please input your piece color: ");
    scanf(" %c", &desiredPiece);
    while (desiredPiece != 'R' && desiredPiece != 'Y') {
        printf("Invalid Piece Inputted Please Provide a Valid Piece (Y or R)");
        scanf(" %c", &desiredPiece);
    }
    if (desiredPiece == 'Y') {
        computerChoice = 1;
        playerChoice = 0;
    } else if (desiredPiece == 'R') {
        computerChoice = 0;
        playerChoice = 1;
    }
    return 0;
};

int
playerTurn(char board[COLUMNLENGTH][ROWLENGTH], char playerPiece)
{
    /*Declared Variables: Used for the positioning of the placed piece*/
    int rowPosition;
    int colPosition;
    char colLetter;

    /*Set the coordinates to 0 at first*/
    rowPosition = 0;
    colPosition = 0;
    /*Ask User to input a letter*/
    printf("Player (%c): \nEnter Letter Coordinate:", playerPiece);
    /*Take in user letter*/
    scanf(" %c", &colLetter);
    /*Map that letter to the appropriate column position*/
    colPosition = mapPosition(columnLetterPositions, colLetter);
    /*Loop through the columns from the bottom*/
    for (rowPosition = ROWLENGTH - 1; rowPosition >= 0; rowPosition--) {
        /*Check if the row and column position is empty. If it is add that piece to the position*/
        if (board[rowPosition][colPosition] == '0') {
            board[rowPosition][colPosition] = playerPiece;
            return 0;
        }
    }
    return 0;
}


int
computerTurn(char board[COLUMNLENGTH][ROWLENGTH], char computerPiece)
{
    /*Declared Variables: Used for the positioning of the placed piece*/
    int rowPosition;
    int colPosition;

    /*Set the coordinates to 0 at first*/
    rowPosition = 0;
    colPosition = 0;
    /*Get a random column position on the board to place your piece*/
    srand(time(NULL));
    colPosition = (rand() % 8) - 1;
    /*Loop through the columns from the bottom*/
    for (rowPosition = ROWLENGTH - 1; rowPosition >= 0; rowPosition--) {
        /*Check if the row and column position is empty. If it is add that piece to the position*/
        if (board[rowPosition][colPosition] == '0') {
            board[rowPosition][colPosition] = computerPiece;
            return 0;
        }
    }
    return 0;
}

void
printBoard(char board[COLUMNLENGTH][ROWLENGTH])
{
    /*Header for the User*/
    printf("\n *****Connect tour*****\n");
    printf("- - A B C D E F G H \n");
    printf("  ----------------- \n");
    /*Loop through the rows*/
    for (int i = 0; i < ROWLENGTH; i++) {
        /*Display which row is which and then loop through that's row's pieces/positions*/
        printf("%d | ", i + 1);
        for (int j = 0; j < COLUMNLENGTH; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

char
checkWinner(char board[COLUMNLENGTH][ROWLENGTH])
{
    int i, j, k, count;
    // check for a horizontal win
    for (i = 0; i < ROWLENGTH; i++) {
        for (j = 0; j < COLUMNLENGTH - 3; j++) {
            /*Check all the none empty tiles going horizontally*/
            if (board[i][j] != '0' && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] &&
                board[i][j] == board[i][j + 3]) {
                /*Return the piece of the player that won*/
                return board[i][j];
            }
        }
    }

    // checks for a vertical win
    for (i = 0; i < ROWLENGTH - 3; i++) {
        for (j = 0; j < COLUMNLENGTH; j++) {
            /*Check all the none empty tiles going vertically*/
            if (board[i][j] != '0' && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] &&
                board[i][j] == board[i + 3][j]) {
                /*Return the piece of the player that won*/
                return board[i][j];
            }
        }
    }

    // checks for a right diagonal win
    for (i = 0; i < ROWLENGTH - 3; i++) {
        for (j = 0; j < COLUMNLENGTH - 3; j++) {
            /*Check all the none empty tiles going diagonally from the right*/
            if (board[i][j] != '0' && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] &&
                board[i][j] == board[i + 3][j + 3]) {
                /*Return the piece of the player that won*/
                return board[i][j];
            }
        }
    }

    // checks for a left diagonal win
    for (i = 0; i < ROWLENGTH - 3; i++) {
        for (j = 0; j < COLUMNLENGTH - 3; j++) {
            /*Check all the none empty tiles going diagonally from the left*/
            if (board[i][j] != '0' && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] &&
                board[i][j] == board[i + 3][j - 3]) {
                /*Return the piece of the player that won*/
                return board[i][j];
            }
        }
    }
    return '0';
}

int
main()
{

    printBoard(gameBoard);
    resetBoard(gameBoard);
    char status = '0';
    printf("Player's Piece is %c\n", gamePieces[playerChoice]);
    printf("Computer's Piece is %c\n", gamePieces[computerChoice]);
    while (status == '0') {
        playerTurn(gameBoard, gamePieces[playerChoice]);
        status = checkWinner(gameBoard);
        if (status != '0') {
            break;
        }
        printBoard(gameBoard);
        computerTurn(gameBoard, gamePieces[computerChoice]);
        status = checkWinner(gameBoard);
        if (status != '0') {
            break;
        }
    }
    printf("%c", status);
    return 0;
}
