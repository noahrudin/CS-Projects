/*
 * @author Noah Rudin
 * Uses a semaphore to allow access to multiple devices
 */
#include <linux/module.h>
#include <linux/kernel.h> /* printk() */
#include <linux/version.h> /* printk() */
#include <linux/init.h>  /*  for module_init and module_cleanup */
#include <linux/slab.h>  /*  for kmalloc/kfree */
#include <linux/fs.h>     /* everything... */
#include <linux/errno.h>  /* error codes */
#include <linux/types.h>  /* size_t */
#include <linux/proc_fs.h>	/* for the proc filesystem */
#include <linux/seq_file.h>
#include "booga.h"        /* local definitions */
#include <linux/random.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/semaphore.h>
#include <linux/kdev_t.h>
#include <linux/string.h>


#define  DEVICE_NAME "booga"

static int booga_major =   BOOGA_MAJOR;
static int booga_nr_devs = BOOGA_NR_DEVS;    // mostly unused for this project
module_param(booga_major, int, 0);
module_param(booga_nr_devs, int, 0);
MODULE_AUTHOR("Noah Rudin");
MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("Linux device driver");
MODULE_VERSION("1.0");

static booga_stats *booga_device_stats;
static struct proc_dir_entry* booga_proc_file;
static char* randStrings[] = {"booga! booga! ", "googoo! gaagaa! ", "neka! maka! ", "wooga! wooga! "};


/*  This is where we define the standard read,write,open and release function */
/*  pointers that provide the drivers main functionality. */
static struct file_operations booga_fops = {
		    read:       booga_read,
			write:      booga_write,
			open:       booga_open,
			release:    booga_release,
};

/* The file operations struct holds pointers to functions that are defined by */
/* driver impl. to perform operations on the device. What operations are needed */
/* and what they should do is dependent on the purpose of the driver. */
static const struct file_operations booga_proc_fops = {
		.owner	= THIS_MODULE,
		.open	= booga_proc_open,
		.read	= seq_read,
		.llseek	= seq_lseek,
		.release	= single_release,
};


static int booga_open (struct inode *inode, struct file *filp){
		int num = NUM(inode->i_rdev);
		booga_device_stats->device_num = num;
		printk(KERN_INFO "Booga: booga_open called\n");

		if (num >= booga_nr_devs) return -ENODEV;

		booga_device_stats->openCount[num]++;

		filp->f_op = &booga_fops;

		/* need to protect this with a semaphore if multiple processes
		   will invoke this driver to prevent a race condition */
		if (down_interruptible (&booga_device_stats->sem))
				return (-ERESTARTSYS);
		booga_device_stats->num_open++;
		up(&booga_device_stats->sem);

		try_module_get(THIS_MODULE);
		printk(KERN_INFO "Booga: booga open success!\n");
		return 0;          /* success */
}

static int booga_release (struct inode *inode, struct file *filp){
		/* need to protect this with a semaphore if multiple processes
		   will invoke this driver to prevent a race condition */
		if (down_interruptible (&booga_device_stats->sem))
				return (-ERESTARTSYS);
		booga_device_stats->num_close++;
		up(&booga_device_stats->sem);

		module_put(THIS_MODULE);
		printk(KERN_INFO "booga: Device successfully closed\n");
		return (0);
}


static ssize_t booga_read (struct file *filp, char *buf, size_t count, loff_t *f_pos){
	char randval;
	int choice_index;
	char* choice = "";
	char* substr = "";
	int j;
	int i;

	printk("<1>booga_read invoked.\n");
		/* need to protect this with a semaphore if multiple processes
		   will invoke this driver to prevent a race condition */
	if (down_interruptible (&booga_device_stats->sem)){
		return (-ERESTARTSYS);
	}

	substr = (char*)kmalloc(sizeof(char)*count, GFP_KERNEL);

	get_random_bytes(&randval, 1); 
	choice_index = (randval & 0x7F) % 4; //random index from random string array
	choice = randStrings[choice_index];

	booga_device_stats->stringCount[choice_index]++;

	i = 0;
	j = 0;

	for(j = 0; j < count; j++){
        substr[j] = choice[i]; //concat string in substr array
        i++;

        if(choice[i] == '\0') {//signifies end of the string
            i = 0;
        }
    }

	if(copy_to_user(buf, substr, count)) /* copy the buffer to user space */
		return -ERESTARTSYS;

	booga_device_stats->num_read+=count;
	up(&booga_device_stats->sem);
	return count;
}

static ssize_t booga_write (struct file *filp, const char *buf, size_t count , loff_t *f_pos){
	struct siginfo sig;
	printk("<1>booga_write invoked.\n");
	/* need to protect this with a semaphore if multiple processes
	   will invoke this driver to prevent a race condition */
	if (down_interruptible (&booga_device_stats->sem))
			return (-ERESTARTSYS);

	if(booga_device_stats->device_num == 3){ /*send SIGTERM on attempt to write booga3 */
		memset(&sig, 0, sizeof(struct siginfo));
		sig.si_signo = SIGTERM; //terminate signal, cant use SIGKILL
		sig.si_code = SI_QUEUE;
		sig.si_int = 1; //some int
		send_sig_info(SIGTERM, &sig, current); 
	}else{
		booga_device_stats->num_write+=count; // increment bytes written
	}
	
	up(&booga_device_stats->sem);
	return count;
}

static void init_booga_device_stats(void){
		int i = 0;
		booga_device_stats->num_read=0;
		booga_device_stats->num_write=0;
		booga_device_stats->num_open=0;
		booga_device_stats->num_close=0;
		for(i = 0; i < 4; i++){
			booga_device_stats->openCount[i] = 0;
		}
		for(i = 0; i < 4; i++){
			booga_device_stats->stringCount[i] = 0;
		}
		sema_init(&booga_device_stats->sem, 1);
}

static int booga_proc_show(struct seq_file *m, void *v){
	int i = 0;
	seq_printf(m, "bytes read = %ld\n", booga_device_stats->num_read);
	seq_printf(m, "bytes written = %ld\n", booga_device_stats->num_write);
	seq_printf(m, "number of opens:\n");
	for(i = 0; i < 4; i++){
		seq_printf(m, " /dev/booga%d = %ld\n", i, booga_device_stats->openCount[i]);
	}
	seq_printf(m, "strings output:\n");
	for(i = 0; i < 4; i++){
		seq_printf(m, " %s = %ld\n", randStrings[i], booga_device_stats->stringCount[i]);
	}
		return 0;
}


static int booga_proc_open(struct inode *inode, struct file *file){
		return single_open(file, booga_proc_show, NULL);
}


static __init int booga_init(void){
		int result;

		/*
		 * Register your major, and accept a dynamic number
		 */
		result = register_chrdev(booga_major, "booga", &booga_fops);
		if (result < 0) {
				printk(KERN_WARNING "booga: can't get major %d\n",booga_major);
				return result;
		}
		if (booga_major == 0) booga_major = result; /* dynamic */
		printk("<1> booga device driver version 4: loaded at major number %d\n", booga_major);

		booga_device_stats = (booga_stats *) kmalloc(sizeof(booga_stats),GFP_KERNEL);
		if (!booga_device_stats) {
				result = -ENOMEM;
				goto fail_malloc;
		}
		init_booga_device_stats();

		/* We assume that the /proc/driver exists. Otherwise we need to use proc_mkdir to
		 * create it as follows: proc_mkdir("driver", NULL);
		 */
		booga_proc_file = proc_create("driver/booga", 0, NULL, &booga_proc_fops);
		if (!booga_proc_file)  {
				result = -ENOMEM;
				goto fail_malloc;
		}

		return 0;

fail_malloc:
		unregister_chrdev(booga_major, "booga");
		return  result;
}



static __exit  void booga_cleanup(void){
		remove_proc_entry("driver/booga", NULL /* parent dir */);
		kfree(booga_device_stats);
		unregister_chrdev(booga_major, "booga");
		printk("<1> booga device driver version 4: unloaded\n");
}


module_init(booga_init);
module_exit(booga_cleanup);

/* vim: set ts=4: */
