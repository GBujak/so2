#include <linux/module.h>

static int __init my_init(void) {
    printk(KERN_ALERT "zaczynam\n");
    printk(KERN_ALERT "Plik: %s, Linijka: %u", __FILE__, __LINE__);
    return 0;
}

module_init(my_init);

static void __exit my_exit(void) {
    printk(KERN_ALERT "koncze!\n");
}

module_exit(my_exit);

MODULE_LICENSE("GPL");
