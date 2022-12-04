#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
 
/*Used the gameboard*/
#define COLUMNLENGTH 8
#define ROWLENGTH 8
 
/*Used for takeTurn to keep track of the functionality of the game*/
#define BOARDNOTFOUND 0
#define PLAYERTURN 1
#define COMPUTERTURN 2
#define WINNERFOUND 3
#define TIEFOUND 4
 
/*Used for displaying the correctout for a move*/
#define RESETDISPLAY 0
#define BOARDDISPLAY 1
#define DROPCDISPLAY 2
#define CTURNDISPLAY 3
 
static DECLARE_RWSEM(lockStruct);
 
/*players and Computers Choice Variable (used for indexing the Game Pieces)*/
static int playerChoice = 0;
static int computerChoice = 0;
/*Tracks who's turn is it along with if the board is not set or a winner has been established*/
static int turnTracker = BOARDNOTFOUND;
static char winnerPiece = '0';
/*Checks the correct output is displayed*/
static int displayCheck = BOARDDISPLAY;
/*helps the reader function to figure out if the dropc or cturn was successful*/
/*1 means a success for dropc*/
/*2 means a success for cturn*/
static int successfulOperation = 0;
 
/*Game board*/
static char gameBoard[ROWLENGTH][COLUMNLENGTH] = {
    {'0', '0', '0', '0', '0', '0', '0', '0'}, {'0', '0', '0', '0', '0', '0', '0', '0'}, {'0', '0', '0', '0', '0', '0', '0', '0'}, {'0', '0', '0', '0', '0', '0', '0', '0'},
    {'0', '0', '0', '0', '0', '0', '0', '0'}, {'0', '0', '0', '0', '0', '0', '0', '0'}, {'0', '0', '0', '0', '0', '0', '0', '0'}, {'0', '0', '0', '0', '0', '0', '0', '0'},
};
char columnLetterPositions[COLUMNLENGTH] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
/*Game Pieces*/
static char *gamePieces = "YR";
 
/*Display the board to the user*/
void printBoard(char board[COLUMNLENGTH][ROWLENGTH]) {
    /*Variable Created*/
    int i;
    int j;
    /*Header for the User*/
    printk(KERN_ALERT "\n *****Connect tour*****\n");
    printk(KERN_ALERT "- - A B C D E F G H \n");
    printk(KERN_ALERT "  ----------------- \n");
    /*Loop through the rows*/
    for (i = 0; i < ROWLENGTH; i++) {
        /*Display which row is which and then loop through that's row's
         * pieces/positions*/
        printk(KERN_ALERT "%d | ", i + 1);
        for (j = 0; j < COLUMNLENGTH; j++) {
            printk(KERN_ALERT "%c ", board[i][j]);
        }
        printk(KERN_ALERT "\n");
    }
}
 
int mapPosition(char columnMap[COLUMNLENGTH], char columnLetter) {
    int i;
    /*Loop through and map the letter to the specific column*/
    for (i = 0; i < COLUMNLENGTH; i++) {
        if (columnMap[i] == columnLetter) {
            /*return the mapped letter*/
            return i;
        }
    }
    return 0;
}
 
int computerTurn(char board[COLUMNLENGTH][ROWLENGTH], char computerPiece) {
    /*Declared Variables: Used for the positioning of the placed piece*/
    int rowPosition;
    unsigned colPosition;
 
    /*Set the coordinates to 0 at first*/
    rowPosition = 0;
    colPosition = 0;
    /*Get a random column position on the board to place your piece*/
    /*gets the random number*/
    get_random_bytes(&colPosition, sizeof colPosition);
    colPosition = (1u + (colPosition % 8u)) - 1u;
    /*Loop through the columns from the bottom*/
    for (rowPosition = ROWLENGTH - 1; rowPosition >= 0; rowPosition--) {
        /*Check if the row and column position is empty. If it is add that piece
         * to the position*/
        if (board[rowPosition][colPosition] == '0') {
            board[rowPosition][colPosition] = computerPiece;
            return 0;
        }
    }
    return 0;
}
 
int playerTurn(char board[COLUMNLENGTH][ROWLENGTH], char playerPiece, char colLetter) {
    /*Declared Variables: Used for the positioning of the placed piece*/
    int rowPosition;
    int colPosition;
 
    /*Set the coordinates to 0 at first*/
    rowPosition = 0;
    colPosition = 0;
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
 
char checkWinner(char board[COLUMNLENGTH][ROWLENGTH]) {
    /*Iterator Variables Created*/
    int i;
    int j;
    /*Checks to see if a tie is present*/
    char tieCheck;
    tieCheck = 'T';
    // check for a horizontal win
    for (i = 0; i < ROWLENGTH; i++) {
        for (j = 0; j < COLUMNLENGTH - 3; j++) {
            /*Check all the none empty tiles going horizontally*/
            if (board[i][j] != '0' && board[i][j] == board[i][j + 1] && board[i][j] == board[i][j + 2] && board[i][j] == board[i][j + 3]) {
                /*Declare a winner*/
                turnTracker = WINNERFOUND;
                /*Return the piece of the player that won*/
                return board[i][j];
            }
        }
    }
 
    // checks for a vertical win
    for (i = 0; i < ROWLENGTH - 3; i++) {
        for (j = 0; j < COLUMNLENGTH; j++) {
            /*Check all the none empty tiles going vertically*/
            if (board[i][j] != '0' && board[i][j] == board[i + 1][j] && board[i][j] == board[i + 2][j] && board[i][j] == board[i + 3][j]) {
                /*Declare a winner*/
                turnTracker = WINNERFOUND;
                /*Return the piece of the player that won*/
                return board[i][j];
            }
        }
    }
 
    // checks for a right diagonal win
    for (i = 0; i < ROWLENGTH - 3; i++) {
        for (j = 0; j < COLUMNLENGTH - 3; j++) {
            /*Check all the none empty tiles going diagonally from the right*/
            if (board[i][j] != '0' && board[i][j] == board[i + 1][j + 1] && board[i][j] == board[i + 2][j + 2] && board[i][j] == board[i + 3][j + 3]) {
                /*Declare a winner*/
                turnTracker = WINNERFOUND;
                /*Return the piece of the player that won*/
                return board[i][j];
            }
        }
    }
 
    // checks for a left diagonal win
    for (i = 0; i < ROWLENGTH - 3; i++) {
        for (j = 0; j < COLUMNLENGTH - 3; j++) {
            /*Check all the none empty tiles going diagonally from the left*/
            if (board[i][j] != '0' && board[i][j] == board[i + 1][j - 1] && board[i][j] == board[i + 2][j - 2] && board[i][j] == board[i + 3][j - 3]) {
                /*Declare a winner*/
                turnTracker = WINNERFOUND;
                /*Return the piece of the player that won*/
                return board[i][j];
            }
            /*Check if a tie is present*/
            if (board[i][j] == '0') {
                tieCheck = '0';
            }
        }
    }
    if (tieCheck == 'T') {
        turnTracker = TIEFOUND;
    }
    return tieCheck;
}
 
/*Device read functionality: ex. cat /dev/insperation */
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset) {
    if (displayCheck == RESETDISPLAY) {
        char resetDisplay[255];
        ssize_t len;
        strcpy(resetDisplay, "OK\n");
        len = strlen(resetDisplay);
        return simple_read_from_buffer(user_buffer, size, offset, resetDisplay, len);
    }
    if (displayCheck == BOARDDISPLAY) {
        /*Assigned Variables*/
        char boardDisplay[255];
        ssize_t len;
        int i;
        int j;
        char *numBanner;
        char *boardPiece;
        /*Add the Header for the User*/
        memcpy(boardDisplay,
               "\n *****Connect tour*****\n- - A B C D E F G H \n  "
               "----------------- \n",
               68);
        for (i = 0; i < ROWLENGTH; i++) {
            /*Display which row is which and then loop through that's row's
             * pieces/positions*/
            numBanner = kmalloc(80, GFP_KERNEL);
            sprintf(numBanner, "%d | ", (i + 1));
            strcat(boardDisplay, numBanner);
            for (j = 0; j < COLUMNLENGTH; j++) {
                boardPiece = kmalloc(80, GFP_KERNEL);
                sprintf(boardPiece, "%c ", gameBoard[i][j]);
                strcat(boardDisplay, boardPiece);
            }
            strcat(boardDisplay, "\n");
        }
        len = strlen(boardDisplay);
        kfree(numBanner);
        kfree(boardPiece);
        return simple_read_from_buffer(user_buffer, size, offset, boardDisplay, len);
    }
    if (displayCheck == DROPCDISPLAY) {
        char dropDisplay[255];
        ssize_t len;
	if (turnTracker == BOARDNOTFOUND) {
            strcpy(dropDisplay, "NOGAME\n");
        } else if (turnTracker == WINNERFOUND && winnerPiece == gamePieces[playerChoice]) {
            strcpy(dropDisplay, "WIN\n");
        } else if (turnTracker == TIEFOUND && winnerPiece == 'T') {
            strcpy(dropDisplay, "TIE\n");
        } else if (successfulOperation != 1) {
            strcpy(dropDisplay, "OOT\n");
        } else {
            strcpy(dropDisplay, "OK\n");
        }
        len = strlen(dropDisplay);
        return simple_read_from_buffer(user_buffer, size, offset, dropDisplay, len);
    }
    if (displayCheck == CTURNDISPLAY) {
        char cturnDisplay[255];
        ssize_t len;
        if (turnTracker == BOARDNOTFOUND) {
            strcpy(cturnDisplay, "NOGAME\n");
        } else if (turnTracker == WINNERFOUND && winnerPiece == gamePieces[computerChoice]) {
            strcpy(cturnDisplay, "LOSE\n");
        } else if (turnTracker == TIEFOUND && winnerPiece == 'T') {
            strcpy(cturnDisplay, "TIE\n");
        } else if (successfulOperation != 2) {
            strcpy(cturnDisplay, "OOT\n");
        } else {
            strcpy(cturnDisplay, "OK\n");
        }
        len = strlen(cturnDisplay);
        return simple_read_from_buffer(user_buffer, size, offset, cturnDisplay, len);
    }
    else {
        char errorDisplay[80];
    	ssize_t len;
        memcpy(errorDisplay, "ERROR IN DISPLAY\n", 18);
        len = strlen(errorDisplay);
        return simple_read_from_buffer(user_buffer, size, offset, errorDisplay, len);
    } 
}
 
/*Device write functionality: ex. echo 'hi' > /dev/insperation */
static ssize_t device_write(struct file *f, const char __user *buf, size_t len, loff_t *off) {
    /*Variable Declarations*/
    char user_input[256];
    int result_from;
    long res;
    /*Detaching User Input*/
    char *command;
    char *userInputPtr;
    /*Locking mechanism added for read and write*/
    down_write(&lockStruct);
    /*Check if the buffer access is ok*/
    result_from = access_ok(buf, len);
    if (result_from == 0) {
        up_write(&lockStruct);
        return -EFAULT;
    }
    /*Get user input*/
    user_input[255] = '\0';
    res = strncpy_from_user(user_input, buf, 257);
    if (res > 0) {
        userInputPtr = user_input;
    }
    /*Get the first command input*/
    command = strsep(&userInputPtr, " ");
    if (strcmp(command, "RESET") == 0) {
        int row;
        int col;
        command = strsep(&userInputPtr, " ");
        /*Set the players pieces*/
        if (strcmp(command, "Y\n") == 0) {
            playerChoice = 0;
            computerChoice = 1;
            turnTracker = PLAYERTURN;
        } else if (strcmp(command, "R\n") == 0) {
            playerChoice = 1;
            computerChoice = 0;
            turnTracker = PLAYERTURN;
        } else {
            printk(KERN_ALERT "Invalid Piece, Please try again (Y or R)");
            if (turnTracker != WINNERFOUND) {
                turnTracker = BOARDNOTFOUND;
            }
        }
        /*Check if the player's turn has been established*/
        if (turnTracker == PLAYERTURN) {
            /*Reset the board to be empty*/
            for (row = 0; row < ROWLENGTH; row++) {
                for (col = 0; col < COLUMNLENGTH; col++) {
                    gameBoard[row][col] = '0';
                }
            }
        }
        /*Display the current pieces*/
        printk(KERN_ALERT "Player's Piece is %c\n", gamePieces[playerChoice]);
        printk(KERN_ALERT "Computer's Piece is %c\n", gamePieces[computerChoice]);
        displayCheck = RESETDISPLAY;
    } else if (strstr(command, "BOARD") != NULL) {
        displayCheck = BOARDDISPLAY;
        printk(KERN_ALERT "BOARD INPUT CAUGHT");
    } else if (strcmp(command, "DROPC") == 0) {
        if (turnTracker == PLAYERTURN) {
            command = strsep(&userInputPtr, " ");
            playerTurn(gameBoard, gamePieces[playerChoice], command[0]);
            turnTracker = COMPUTERTURN;
            winnerPiece = checkWinner(gameBoard);
            if (winnerPiece == '0') {
            	successfulOperation = 1;
            }
            printk(KERN_ALERT "Player's Turn Has Been Preformed");
        } else if (turnTracker == BOARDNOTFOUND) {
            printk(KERN_ALERT "BOARD AND PIECES HAVE NOT BEEN SET");
        } else if (turnTracker == WINNERFOUND) {
            printk(KERN_ALERT "THIS GAME HAS ENDED, THE WINNER IS %c\n", winnerPiece);
        } else {
            successfulOperation = 0;
            printk(KERN_ALERT "IT IS NOT THE PLAYERS TURN");
        }
        displayCheck = DROPCDISPLAY; 
    } else if (strstr(command, "CTURN") != NULL) {
        if (turnTracker == COMPUTERTURN) {
            computerTurn(gameBoard, gamePieces[computerChoice]);
            turnTracker = PLAYERTURN;
            winnerPiece = checkWinner(gameBoard);
            if (winnerPiece == '0') {
            	successfulOperation = 2;
            }
            printk(KERN_ALERT "Computer's Turn Has Been Preformed");
        } else if (turnTracker == BOARDNOTFOUND) {
            printk(KERN_ALERT "BOARD AND PIECES HAVE NOT BEEN SET");
        } else if (turnTracker == WINNERFOUND) {
            printk(KERN_ALERT "THIS GAME HAS ENDED, THE WINNER IS %c\n", winnerPiece);
        } else {
            successfulOperation = 0;
            printk(KERN_ALERT "IT IS NOT THE COMPUTERS TURN");
        }
        displayCheck = CTURNDISPLAY;
    }
    up_write(&lockStruct);
    return len;
}
 
/*Declaration of the file_operations*/
/*User can only currently read and write*/
static struct file_operations fops = {.read = device_read, .write = device_write};
 
/*Sets the permissions so that all users can read and write*/
static int set_permissions(struct device *dev, struct kobj_uevent_env *env) {
    add_uevent_var(env, "DEVMODE=%#o", 0666);
    return 0;
}
 
/*Variable Declarations*/
static struct cdev char_dev;
static dev_t first = 0;
static struct class *device_class;
MODULE_LICENSE("GPL");
 
static int __init fourinarow_create(void) {
    /*registers a range of char device numbers*/
    alloc_chrdev_region(&first, 0, 3, "fourinarow");
    /*creation of virtual device class*/
    device_class = class_create(THIS_MODULE, "fourinarow");
    /*Set the permission so that it can be read and written too*/
    device_class->dev_uevent = set_permissions;
    /*Create the actual device and creates the sysfs attribute file for the
     * device*/
    device_create(device_class, NULL, first, NULL, "fourinarow");
    /*initialize the cdev structure, adds file operations*/
    cdev_init(&char_dev, &fops);
    /*add char device to the system*/
    cdev_add(&char_dev, first, 1);
    return 0;
}
 
static void __exit fourinarow_exit(void) {
    /*unregisters an destroys device (Basically clean up of all the resources)*/
    unregister_chrdev_region(first, 3);
    device_destroy(device_class, first);
    class_destroy(device_class);
    unregister_chrdev_region(first, 1);
    printk(KERN_ALERT "Thank you for playing four in a row!\n");
}
 
module_init(fourinarow_create);
module_exit(fourinarow_exit);
