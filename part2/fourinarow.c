#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>

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
/*Letter list*/
char columnLetterPositions[COLUMNLENGTH] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
/*Game Pieces*/
static char *gamePieces = "YR";

/*Display the board to the user*/
void printBoard(char board[COLUMNLENGTH][ROWLENGTH])
{
    /*Variable to store board output*/
    char displayedBoard[];
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


/*Device read functionality: ex. cat /dev/insperation */
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    /*Assigned Variables*/
}

/*Device write functionality: ex. echo 'hi' > /dev/insperation */
static ssize_t device_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    /*Check if the buffer access is ok*/
	int result_from = access_ok(buf, size);
	if (result_from == 0) {
		return -EFAULT;
	}
    buf[255] = '\0';
    long res = strncpy_from_user(buf, filename, 255);
    if (res > 0){
            printk(KERN_ALERT "%s\n", buf);
    }
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
    add_uevent_var(env, 'DEVMODE=%#o', 0666);
    return 0;
}

/*Variable Declarations*/
static struct cdev char_dev;
static dev_t first = 0;
static struct class *device_class; 

MODULE_LICENSE('GPL');

static int __init fourinarow_create(void) {
    /*registers a range of char device numbers*/
    alloc_chrdev_region(&first, 0, 3, 'fourinarow');
    /*creation of virtual device class*/
    device_class = class_create(THIS_MODULE, 'fourinarow');
    /*Set the permission so that it can be read and written too*/
    device_class->dev_uevent = set_permissions;
    /*Create the actual device and creates the sysfs attribute file for the device*/
    device_create(device_class, NULL, first, NULL, 'fourinarow');
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
    printk(KERN_ALERT 'Thank you for using the quote generator\n');
}
 
module_init(fourinarow_create);
module_exit(fourinarow_exit);
 
