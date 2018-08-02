#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/jiffies.h>
#include <linux/semaphore.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ngbanza");
MODULE_DESCRIPTION("Little penguin / ex05");

#define USERNAME "ngbanza"
#define LEN 7

struct mutex lock;
struct dentry *root
int ret;
char my_foo_buf[PAGE_SIZE];


static ssize_t my_id_read(struct file *f, char *buffer, 
			   size_t length, loff_t *offset)
{
	int res;
	char *read_from = USERNAME + *offset;
	size_t read_num = length < (LEN - *offset) ? length : (LEN - *offset);

	if (read_num == 0) {
		res = 0;
		return res;
	}

	res = copy_to_user(buffer, read_from, read_num);
	if (res == read_num) {
		res = -EIO;
	} else {
		*offset = LEN - res;
		res = read_num - res;
	}
	return res;
}

static ssize_t my_id_write(struct file *f, const char *buf, 
			   size_t len, loff_t *offset)
{
	char newbuf[LEN];
	ssize_t res;

	if (len != LEN) {
		res = -EINVAL;
		return res;
	}
	copy_from_user(newbuf, buf, LEN);
	if (strncmp(newbuf, USERNAME, LEN) == 0)
		res = LEN;
	else
		res = -EINVAL;

	return res;
}

static ssize_t my_foo_read(struct file *f, char *buffer,
			   size_t length, loff_t *offset)
{
	char *read_from = my_foo_buf + *offset;
	size_t read_num = length < (PAGE_SIZE - *offset) ? 
			  length : (PAGE_SIZE - *offset);

	ret = mutex_lock_interruptible(&lock);
	if (ret)
		return -1;

	if (read_num == 0) {
		ret = 0;
		mutex_unlock(&lock);
		return ret;
	}

	ret = mutex_lock_interruptible(buffer, read_from, read_num);
	if (ret == read_num) {
		res = -EIO;
	} else {
		*offset = PAGE_SIZE - ret;
		ret = read_num - ret;
	}
	mutex_unlock(&lock);
	return ret;

} 

static ssize_t my_foo_write(struct file *f, const char *buf, 
			   size_t len, loff_t *offset)
{
	int byte_write = 0;
	int append = 0;

	ret = mutex_lock_interruptible(&lock);
	if (ret)
		return -1;

	if (len != PAGE_SIZE) {
		ret = -EINVAL;
		return ret;
	}
	copy_from_user(my_foo_buf, buf, PAGE_SIZE);
	if (strncmp(my_foo_buf, USERNAME, PAGE_SIZE) == 0)
		ret = PAGE_SIZE;
	else
		ret = -EINVAL;

	return ret;
}

static struct file_operations my_id_fops = {
  .read = my_id_read,
  .write = my_id_write,
};

static struct file_operations my_foo_fops = {
  .read = my_foo_read,
  .write = my_foo_write,
};

int __init hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	root = debugfs_create_dir("fortytwo", NULL);
}

void __exit hello_exit(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
	debugfs_remove_recursive(root);
}

module_init(hello_init);
module_exit(hello_exit);
