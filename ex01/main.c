#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Little Penguin ex01");
MODULE_AUTHOR("ngbanza <dngoyb@gmail.com>");

int __init hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

void __exit hello_exit(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
