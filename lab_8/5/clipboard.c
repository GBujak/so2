#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<asm/uaccess.h>

#define NAME "clipdev" // <--

static uint64_t first, second;

static char g_clip[1024]; // <--
static size_t read = 0; // <--
static size_t length = 0; // <--

// <--
static ssize_t fib_read(struct file *f, char __user *u, size_t size, loff_t* pos)
{
    size_t to_copy = 0;

    if (read == length) return 0;

    to_copy = length - read;
    if (size < to_copy) to_copy = size;

    if (copy_to_user(u, (void*) g_clip, to_copy))
        return -EIO;

    read += to_copy;

    return to_copy;
}

// <--
static ssize_t fib_write(struct file *f, const char __user *u, size_t size, loff_t* pos)
{
    // nadpisz bufor
    read = 0;
    length = 1024;
    if (size < length) length = size;
    
    if (copy_from_user((void*)g_clip, u, length))
        return -EIO;

    return length;
}

static int fib_open(struct inode *ind, struct file *f) 
{
    first = 0;
    second = 1;
    return 0;
}

static int fib_release(struct inode *ind, struct file *f)
{
    return 0;
}

static struct file_operations fibop = 
{
    .owner = THIS_MODULE,
    .open = fib_open,
    .release = fib_release,
    .read = fib_read,
    .write = fib_write,
};

static dev_t number = 0;
static struct cdev fib_cdev;
static struct class *fib_class;
static struct device *fib_device;

static int __init fibchar_init(void)
{
    if(alloc_chrdev_region(&number,0,1,NAME)<0) {
        printk(KERN_ALERT "[fibdev]: Region allocation error!\n");
        return -1;
    }

    fib_class = class_create(THIS_MODULE,NAME);
    if(IS_ERR(fib_class)) {
        printk(KERN_ALERT "[fibdev]: Error creating class: %ld!\n",PTR_ERR(fib_class));
        unregister_chrdev_region(number,1);
        return -1;
    }

    cdev_init(&fib_cdev,&fibop);
    fib_cdev.owner = THIS_MODULE;

    if(cdev_add(&fib_cdev,number,1)) {
        printk(KERN_ALERT "[fibdev]: Error adding cdev!\n");
        class_destroy(fib_class);
        unregister_chrdev_region(number,1);
        return -1;
    }

    fib_device = device_create(fib_class, NULL, number, NULL, NAME);
    if(IS_ERR(fib_device)) {
        printk(KERN_ALERT "[fibdev]: Error creating device: %ld!\n",PTR_ERR(fib_device));
        cdev_del(&fib_cdev);
        class_destroy(fib_class);
        unregister_chrdev_region(number,1);
        return -1;
    }

    memset(g_clip, '\0', 1024); // <--

    return 0;
}

static void __exit fibchar_exit(void)
{
    if(fib_device)
        device_destroy(fib_class,number);
    cdev_del(&fib_cdev);
    if(fib_class)
        class_destroy(fib_class);
    if(number>=0)
        unregister_chrdev_region(number,1);
}

module_init(fibchar_init);
module_exit(fibchar_exit);
MODULE_LICENSE("GPL");
MODULE_VERSION("1.0");
