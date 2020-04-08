#include <linux/module.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/proc_fs.h>

static struct proc_dir_entry *directory_entry_pointer, *file_entry_pointer;
static char *directory_name = "procfs_test", *file_name = "procfs_file";
static char *msg_buffer[PAGE_SIZE];
static int length, temp;

static int proc_open(struct inode *inode, struct file *file)
{
	return 0; 
}

static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *position)
{
	pr_alert("count write = %ld\n", count);
        length = count;
        if (length > PAGE_SIZE) {
        	length = PAGE_SIZE-1;
        }
	if (copy_from_user(msg_buffer, buffer, length)) {
                return -EFAULT;
        }
	
        msg_buffer[length] = '\0';
	temp = length;
        return length;
}

static ssize_t proc_read(
        struct file *file,
        char __user *buffer,
        size_t count,
        loff_t *position) {
        
	if (count > temp) {
		count = temp;
        }
	temp -= count;
	if (copy_to_user(buffer, msg_buffer, count)) {
		return -EFAULT;
        }
	if(count == 0) {
		temp = length;
        }
	return count;
}

static struct file_operations proc_fops = {
        .owner = THIS_MODULE,
	.open = proc_open,
        .write = proc_write,
        .read = proc_read
};

static int __init procfsmod_init(void)
{
        directory_entry_pointer = proc_mkdir(directory_name, NULL);
        if(IS_ERR(directory_entry_pointer)) {
                pr_alert("Error creating procfs directory: %s. Error code: %ld\n", directory_name,PTR_ERR(directory_entry_pointer));
                return -1;
        }
        file_entry_pointer = proc_create_data(file_name, 0666, directory_entry_pointer, &proc_fops, (void *)msg_buffer);
        if(IS_ERR(file_entry_pointer)) {
                pr_alert("Error creating procfs file: %s. Error code: %ld\n",file_name,PTR_ERR(file_entry_pointer));
                proc_remove(directory_entry_pointer);
                return -1;
        }
        return 0;
}

static void __exit procfsmod_exit(void)
{
        if (file_entry_pointer) {
                proc_remove(file_entry_pointer);
        }
        if (directory_entry_pointer) {
                proc_remove(directory_entry_pointer);
        }
}

module_init(procfsmod_init);
module_exit(procfsmod_exit);
MODULE_LICENSE("GPL");


