#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

static struct node {
    struct node *next, *prev;
    int val;
}

static int __init my_init(void) {

}

static void __exit my_exit(void) {

}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");