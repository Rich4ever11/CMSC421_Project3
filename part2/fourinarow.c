#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/slab.h>

#define COLUMNLENGTH 8
#define ROWLENGTH 8

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
/*Letter list
char columnLetterPositions[COLUMNLENGTH] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
/*Game Pieces*/
static char *gamePieces = "YR";


/*Display the board to the user*/
void printBoard(char board[COLUMNLENGTH][ROWLENGTH])
{
    /*Variable Created*/
    int i;
    int j;
    /*Header for the User*/
    printk(KERN_ALERT "\n *****Connect tour*****\n");
    printk(KERN_ALERT "- - A B C D E F G H \n");
    printk(KERN_ALERT "  ----------------- \n");
    /*Loop through the rows*/
    for (i = 0; i < ROWLENGTH; i++) {
        /*Display which row is which and then loop through that's row's pieces/positions*/
        printk(KERN_ALERT "%d | ", i + 1);
        for (j = 0; j < COLUMNLENGTH; j++) {
            printk(KERN_ALERT "%c ", board[i][j]);
        }
        printk(KERN_ALERT "\n");
    }
}

int
computerTurn(char board[COLUMNLENGTH][ROWLENGTH], char computerPiece)
{
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
        /*Check if the row and column position is empty. If it is add that piece to the position*/
        if (board[rowPosition][colPosition] == '0') {
            board[rowPosition][colPosition] = computerPiece;
            return 0;
        }
    }
    return 0;
}


/*Device read functionality: ex. cat /dev/insperation */
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    /*Assigned Variables*/
    char boardDisplay[255];
    ssize_t len;
    int i;
    int j;
    char *numBanner;
    char *boardPiece;
    /*Add the Header for the User*/
    memcpy(boardDisplay, "\n *****Connect tour*****\n- - A B C D E F G H \n  ----------------- \n", 68);
    for (i = 0; i < ROWLENGTH; i++) {
        /*Display which row is which and then loop through that's row's pieces/positions*/
        numBanner = kmalloc(80, GFP_KERNEL);
        sprintf(numBanner, "%d | ", (i + 1));
        strcat(boardDisplay, numBanner);
        for (j = 0; j < COLUMNLENGTH; j++) {
            boardPiece = kmalloc(80, GFP_KERNEL);
            sprintf(boardPiece,"%c ", gameBoard[i][j]);
            strcat(boardDisplay, boardPiece);
        }
        strcat(boardDisplay, "\n");
    }
    len = strlen(boardDisplay);
    kfree(numBanner);
    kfree(boardPiece);
    return simple_read_from_buffer(user_buffer, size, offset, boardDisplay, len);
}

/*Device write functionality: ex. echo 'hi' > /dev/insperation */
static ssize_t device_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    /*Variable Declarations*/
    char user_input[256];
    int result_from;
    long res;
    /*Detaching User Input*/
    char *command;
    char *userInputPtr;
    /*Check if the buffer access is ok*/
	result_from = access_ok(buf, len);
	if (result_from == 0) {
		return -EFAULT;
	}
    user_input[255] = '\0';
    res = strncpy_from_user(user_input, buf, 257);
    if (res > 0){
    	    userInputPtr = user_input;
    }
    command = strsep(&userInputPtr, " ");
    if (strcmp(command, "RESET") == 0) {
        int row;
    	int col;
    	command = strsep(&userInputPtr, " ");
    	
    	for (row = 0; row < ROWLENGTH; row++) {
    		for (col = 0; col < COLUMNLENGTH; col++) {
    			gameBoard[row][col] = '0';
    		}
    	}  	
    	if (strcmp(command, "Y\n") == 0) {
    	    playerChoice = 0;
    	    computerChoice = 1;
    	}
    	else if (strcmp(command, "R\n") == 0) {
    	    playerChoice = 1;
    	    computerChoice = 0;
    	}
    	else {
    	    printk(KERN_ALERT "Invalid Piece, Please try again (Y or R)");
    	}
    	printk(KERN_ALERT "Player's Piece is %c\n", gamePieces[playerChoice]);
    	printk(KERN_ALERT "Computer's Piece is %c\n", gamePieces[computerChoice]);
    }
    else if (strstr(command, "BOARD") != NULL) {
    	printk(KERN_ALERT "BOARD INPUT CAUGHT");
    }
    else if (strcmp(command, "DROPC") == 0) {
    	printk(KERN_ALERT "DROPC INPUT CAUGHT");
    }
    else if (strstr(command, "CTURN") != NULL) {
    	computerTurn(gameBoard, gamePieces[computerChoice]);
    	printk(KERN_ALERT "Computer's Turn Preformed");
    }
    return len;
}

/*Declaration of the file_operations*/
/*User can only currently read and write*/
static struct file_operations fops = {
	.read = device_read,
	.write = device_write
};

/*Sets the permissions so that all users can read and write*/
static int set_permissions(struct device *dev, struct kobj_uevent_env *env)
{
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
    /*Create the actual device and creates the sysfs attribute file for the device*/
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
 
