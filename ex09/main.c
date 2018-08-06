#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/seq_file.h>
#include <linux/mount.h>
#include <linux/proc_fs.h>
#include <linux/fs_struct.h>
#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/kallsyms.h>
#include <linux/slab.h>

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Little Penguin ex09 <dngoyb@gmail.com>");
MODULE_AUTHOR("ngbanza");

static struct proc_dir_entry *proc_entry;
typedef int (*idef)(int(*)(struct vfsmount*, void*), void*, struct vfsmount*);
typedef struct vfsmount *(*cdef)(const struct path*);

char *buffer;

static int seq_create(struct vfsmount *root, void *data)
{
	struct super_block *root_sb;
	struct seq_file *s;
	struct path path;
	root_sb = root->mnt_sb;
	path.mnt = root;
	path.dentry = root->mnt_root;
	s = (struct seq_file *)data;
	seq_printf(s, "%-10s\t%s\n", root_sb->s_id, d_path(&path, buffer, PAGE_SIZE));
	return 0;
}

static void my_mounts(char *dir, struct seq_file *s)
{
	struct path path;
	struct vfsmount *root;
	cdef collectm;
	idef iteratem;
	iteratem = (void *)kallsyms_lookup_name("iterate_mount");
	collectm = (void *)kallsyms_lookup_name("collect_mounts");
	kern_path(dir, 0, &path);
	root = collectm(&path);
	iteratem(seq_create, (void *)s, root);
}

static int seq_mounts(struct seq_file *s, void *v)
{
	my_mounts("/", s);
	return 0;
}

static struct file_operations seqfops = {
	.owner = THIS_MODULE,
	.open = my_open,
	.read = my_read,
};

static int my_open(struct inode *i, struct file *f)
{
	return single_open(f, &seq_mounts, NULL);
}

int __init hello_init(void)
{
	printk(KERN_INFO "Hello world!\n");
	proc_entry = proc_create("mymounts", 0644, NULL, &seqfops);
	buffer = kmalloc(sizeof(char) * PAGE_SIZE, GFP_KERNEL);
	return 0;
}

void __exit hello_exit(void)
{
	kfree(buffer);
	remove_proc_entry("mymounts", NULL);
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);
