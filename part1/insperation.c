#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
 
static const char *quotes[] = {
	"\"Spread love everywhere you go. Let no one ever come to you without leaving happier.\" - Mother Teresa",
	"\"When you reach the end of your rope, tie a knot in it and hang on.\" - Franklin D. Roosevelt",
	"\"Always remember that you are absolutely unique. Just like everyone else.\" - Margaret Mead",
	"\"Don't judge each day by the harvest you reap but by the seeds that you plant.\" - Robert Louis Stevenson",
	"\"The future belongs to those who believe in the beauty of their dreams.\" - Eleanor Roosevelt",
	"\"Tell me and I forget. Teach me and I remember. Involve me and I learn.\" - Benjamin Franklin",
	"\"The best and most beautiful things in the world cannot be seen or even touched — they must be felt with the heart.\" - Helen Keller",
	"\"It is during our darkest moments that we must focus to see the light.\" - Aristotle",
	"\"Whoever is happy will make others happy too.\" - Anne Frank",
	"\"Do not go where the path may lead, go instead where there is no path and 	leave a trail.\" - Ralph Waldo Emerson",
	"\"The only place where your dream becomes impossible is in your own thinking.\" - Robert H Schuller",
	"\"Happiness often sneaks in through a door you didn’t know you left open.\" - John Barrymore",
	"\"He who does not understand your silence will probably not understand your words.\" - Elbert Hubbard",
	"\"To live is the rarest thing in the world. Most people exist, that is all.\" - Oscar Wilde",
	"\"To be yourself in a world that is constantly trying to make you something else is the greatest accomplishment.\" - Ralph Waldo Emerson",
	"\"If you look at what you have in life, you’ll always have more.\" - Oprah Winfrey",
	"\"A champion is defined not by their wins but by how they can recover when they fall.\" - Serena Williams",
	"\"If my life is going to mean anything, I have to live it myself.\" - Rick Riordan",
	"\"Start where you are. Use what you have. Do what you can.\" - Arthur Ashe",
	"\"It does not matter how slowly you go, so long as you do not stop.\" - Confucius"
};

/*Device read functionality: ex. cat /dev/insperation */
static ssize_t device_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    /*Assigned Variables*/
    unsigned randomNum;
    ssize_t len;
    
    /*gets the random number*/
    get_random_bytes(&randomNum, sizeof randomNum);
    randomNum = (1u + (randomNum % 20u)) - 1u;
    /*gets the length of the randomly selected quote*/
    len = strlen(quotes[randomNum]);
    
    /*Adds the quote to the buffer*/
    return simple_read_from_buffer(user_buffer, size, offset, quotes[randomNum], len);
}

/*Device write functionality: ex. echo "hi" > /dev/insperation */
static ssize_t device_write(struct file *f, const char __user *buf, size_t len, loff_t *off)
{
    /*Print that this operation is not programmed*/
    printk(KERN_ALERT "Sorry, This operation is not supported.\n");
    /*returns appropriate error*/
    return EPERM;
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
 
static int __init qoute_generator(void) {
    /*registers a range of char device numbers*/
    alloc_chrdev_region(&first, 0, 3, "insperation");
    /*creation of virtual device class*/
    device_class = class_create(THIS_MODULE, "insperation");
    /*Set the permission so that it can be read and written too*/
    device_class->dev_uevent = set_permissions;
    /*Create the actual device and creates the sysfs attribute file for the device*/
    device_create(device_class, NULL, first, NULL, "insperation");
    /*initialize the cdev structure, adds file operations*/
    cdev_init(&char_dev, &fops);
    /*add char device to the system*/
    cdev_add(&char_dev, first, 1);

    return 0;
}
 
static void __exit qoute_exit(void) {
    /*unregisters an destroys device (Basically clean up of all the resources)*/
    unregister_chrdev_region(first, 3);
    device_destroy(device_class, first);
    class_destroy(device_class);
    unregister_chrdev_region(first, 1);
    printk(KERN_ALERT "Thank you for using the quote generator\n");
}
 
module_init(qoute_generator);
module_exit(qoute_exit);
 
//https://blog.hubspot.com/sales/famous-quotes
//https://www.invajy.com/25-inspirational-quotes-to-make-you-think/
