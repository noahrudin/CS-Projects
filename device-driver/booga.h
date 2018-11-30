#ifndef __BOOGA_H
#define __BOOGA_H
/*
 * booga.h -- definitions for all booga methods
 *
 */

#ifndef BOOGA_MAJOR
#define BOOGA_MAJOR 0
#endif

#ifndef BOOGA_NR_DEVS
#define BOOGA_NR_DEVS 4 //only for ex. 2 and 3
#endif

/*
 * Split minors in two parts
 */
#define TYPE(dev)   (MINOR(dev) >> 4)  /* high*/
#define NUM(dev)    (MINOR(dev) & 0xf) /* low*/


/*
 * The different params for booga
 */
struct booga_stats {
	long int num_open;
	long int num_read; 
	long int num_write; 
	long int num_close;
	long int device_num;
	long int stringCount[4]; 
	long int openCount[4];
	struct semaphore sem;
};


/*
 * Read method that prints a random string to the console using a chosen device
 */
static ssize_t booga_read (struct file *, char *, size_t , loff_t *);

/*
 * "Writes" bytes to a chosen device
 */
static ssize_t booga_write (struct file *, const char *, size_t , loff_t *);

/*
 * Opens the booga device for access
 */
static int booga_open (struct inode *, struct file *);

/*
 * Releases device from either read or write
 */
static int booga_release (struct inode *, struct file *);

/*
 * Opens booga proc, calls single open
 */
static int booga_proc_open(struct inode *inode, struct file *file);



typedef struct booga_stats booga_stats;


#endif /* __EXAMPLE_H */
