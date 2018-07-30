#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/usb.h>
#include <linux/hid.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Little Penguin ex04 / UBS module");
MODULE_AUTHOR("ngbanza");

static int my_usb_probe(struct usb_interface *interface,
			const struct usb_device_id *id)
{
	printk(KERN_INFO "[*] ngbanza usb drive plugged\n");
	return 0;
}

static void my_usb_disconnect(struct usb_interface *interface)
{
	printk(KERN_INFO "[*] ngbanza usb drive removed\n");
}

static struct usb_device_id my_usb_table[] ={
	{USB_INTERFACE_INFO(USB_INTERFACE_CLASS_HID,
			    USB_INTERFACE_SUBCLASS_BOOT,
			    USB_INTERFACE_PROTOCOL_KEYBOARD)},
	{ },
};

MODULE_DEVICE_TABLE(usb, my_usb_table);

static struct usb_driver my_usb =
{
	.name = "ngbanza",
	.id_table = my_usb_table,
	.probe = my_usb_probe,
	.disconnect = my_usb_disconnect,
};

int __init hello_init(void)
{
	int ret = -1;
	printk(KERN_INFO "Hello world!\n");
	ret = usb_register(&my_usb);
	return ret;
}

void __exit hello_exit(void)
{
	printk(KERN_INFO "Cleaning up module.\n");
	usb_deregister(&my_usb);
}

module_init(hello_init);
module_exit(hello_exit);
