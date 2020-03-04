#include <linux/module.h>

#ifdef __KERNEL__
    #include <linux/module.h>
#else
    #include <stdio.h>
#endif
void is_kernel(void) {
    #ifdef __KERNEL__
        printk(KERN_ALERT "jestem w kernelu\n");
    #else
        printf("Nie jestem w kernelu\n");
    #endif
}

#ifdef __KERNEL__
static int __init my_init(void) {
    printk(KERN_ALERT "zaczynam\n");
    is_kernel();
    return 0;
}

module_init(my_init);

static void __exit my_exit(void) {
    printk(KERN_ALERT "koncze!\n");
}

module_exit(my_exit);

MODULE_LICENSE("GPL");

#else
int main(void) {
    is_kernel();
    return 0;
}
#endif