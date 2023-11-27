#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/proc_fs.h>
#include <linux/stat.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/jiffies.h>  // Add this line

#define DRIVER_AUTHOR "Dalbin Dorian   Gael Gourdin"
#define DRIVER_DESC "Hello world Module"
#define DRIVER_LICENSE "GPL"

#ifdef intervalle
#define INTERVAL intervalle
#else
#define INTERVAL 1000
#endif

#define BUFSIZE  100

static int param = 3;
static struct timer_list timer;
struct file_operations fops;
char message[BUFSIZE];
ssize_t copy;

module_param(param, int, 0);
MODULE_PARM_DESC(param, "Un parametre de ce module");

ssize_t read_function(struct file *file, char __user *buf, size_t count, long long int *ppos)
{
	if (count > BUFSIZE)
		count = BUFSIZE;
	if ((copy = copy_to_user(buf, message, strlen(message)+1)))
		return EFAULT;
	
	printk(KERN_INFO "message read, %d, %p\n", copy, buf);
	return count-copy;
}

ssize_t write_function(struct file *file, const char __user *buf, size_t count, long long int *ppos)
{
	int len = count;
	if (len > BUFSIZE)
		len = BUFSIZE;
	printk(KERN_INFO "Recieving new message\n");
	if (copy_from_user(message, buf, count)) {
		return EFAULT;
	}
	message[count] = '\0';
	//size_of_message = strlen(message);
	printk(KERN_INFO "New message : %s\n", message);
	return count;
}

static void montimer(struct timer_list *t) {
	printk(KERN_INFO "Timer expiré!\n");

	/* Il faut réarmer le timer si l'on veut un appel périodique */
	mod_timer(&timer, jiffies + INTERVAL);
}

int h_init(void)
{
	struct proc_dir_entry *entry;

	printk(KERN_INFO "Hello world!\n");
    printk(KERN_DEBUG "le parametre est=%d\n", param);

	// Timer 
	setup_timer(&timer, montimer, 0);
	mod_timer(&timer, jiffies + INTERVAL);
	printk(KERN_INFO "Module du timer initialise.\n");
	
	// Proc entry
	fops.read = read_function;
	fops.write = write_function;
	entry = proc_create("hello", S_IRWXUGO, NULL, &fops); // Pour avoir les droits
	
	return 0;
}

void h_exit(void)
{
	printk(KERN_ALERT "Bye bye...\n");

	// Timer
    del_timer_sync(&timer);
    printk(KERN_INFO "Module du timer désactivé.\n");

	// Delete proc entry
	remove_proc_entry("hello", NULL);
}

module_init(h_init);
module_exit(h_exit);

MODULE_LICENSE(DRIVER_LICENSE);
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);