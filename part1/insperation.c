#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/random.h>
 
MODULE_LICENSE("GPL");
 
static int __init qoute_generator(void) {
    unsigned randomNum;
    get_random_bytes(&randomNum, sizeof randomNum);
    randomNum = (1u + (randomNum % 20u)) - 1u;
    static const char *quotes[20];
    quotes[0] = "\"Spread love everywhere you go. Let no one ever come to you without leaving happier.\" - Mother Teresa";
    quotes[1] = "\"When you reach the end of your rope, tie a knot in it and hang on.\" - Franklin D. Roosevelt";
    quotes[2] = "\"Always remember that you are absolutely unique. Just like everyone else.\" - Margaret Mead";
    quotes[3] = "\"Don't judge each day by the harvest you reap but by the seeds that you plant.\" - Robert Louis Stevenson";
    quotes[4] = "\"The future belongs to those who believe in the beauty of their dreams.\" - Eleanor Roosevelt";
    quotes[5] = "\"Tell me and I forget. Teach me and I remember. Involve me and I learn.\" - Benjamin Franklin";
    quotes[6] = "\"The best and most beautiful things in the world cannot be seen or even touched — they must be felt with the heart.\" - Helen Keller";
    quotes[7] = "\"It is during our darkest moments that we must focus to see the light.\" - Aristotle";
    quotes[8] = "\"Whoever is happy will make others happy too.\" - Anne Frank";
    quotes[9] = "\"Do not go where the path may lead, go instead where there is no path and leave a trail.\" - Ralph Waldo Emerson";
    quotes[10] = "\"The only place where your dream becomes impossible is in your own thinking.\" - Robert H Schuller";
    quotes[11] = "\"Happiness often sneaks in through a door you didn’t know you left open.\" - John Barrymore";
    quotes[12] = "\"He who does not understand your silence will probably not understand your words.\" - Elbert Hubbard";
    quotes[13] = "\"To live is the rarest thing in the world. Most people exist, that is all.\" - Oscar Wilde";
    quotes[14] = "\"To be yourself in a world that is constantly trying to make you something else is the greatest accomplishment.\" - Ralph Waldo Emerson";
    quotes[15] = "\"If you look at what you have in life, you’ll always have more.\" - Oprah Winfrey";
    quotes[16] = "\"A champion is defined not by their wins but by how they can recover when they fall.\" - Serena Williams";
    quotes[17] = "\"If my life is going to mean anything, I have to live it myself.\" - Rick Riordan";
    quotes[18] = "\"Start where you are. Use what you have. Do what you can.\" - Arthur Ashe";
    quotes[19] = "\"It does not matter how slowly you go, so long as you do not stop.\" - Confucius";
    
    printk(KERN_ALERT "%s\n", quotes[randomNum]);
    return 0;
}
 
static void __exit qoute_exit(void) {
    printk(KERN_ALERT "Thank you for using the quote generator\n");
}
 
module_init(qoute_generator);
module_exit(qoute_exit);
 
//https://blog.hubspot.com/sales/famous-quotes
//https://www.invajy.com/25-inspirational-quotes-to-make-you-think/
