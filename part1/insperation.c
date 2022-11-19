#include <linux/kernel.h>
#include <linux/module.h>
 
MODULE_LICENSE("GPL");
 
static int __init qoute_generator(void) {
    unsigned randomNum;
    randomNum = 1;
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
    "\"Do not go where the path may lead, go instead where there is no path and leave a trail.\" - Ralph Waldo Emerson",
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
    printk(KERN_ALERT "%d\n", quotes[randomNum]);
    return 0;
}
 
static void __exit qoute_exit(void) {
    printk(KERN_ALERT "Thank you for using the quote generator\n");
}
 
module_init(qoute_generator);
module_exit(qoute_exit);
 
//https://blog.hubspot.com/sales/famous-quotes
//https://www.invajy.com/25-inspirational-quotes-to-make-you-think/
