#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/slab.h>

struct node {
    struct node *next, *prev;
    int val;
};

struct node *beg, *end;

static int __init my_init(void) {
    int i;
    struct node* tmp;
    beg = end = NULL;
    for (i = 0; i < 20; i++) {
        tmp = kmalloc(sizeof *tmp, GFP_KERNEL);
        if (IS_ERR(tmp)) {
            pr_alert("error allocating memory: %ld\n", PTR_ERR(tmp));
            return -ENOMEM;
        }
        tmp->val = i;

        if (i == 0) beg = tmp;
        tmp->prev = end;
        if (end != NULL) end->next = tmp;
        tmp->next = NULL;
        end = tmp;
    }
    return 0;
}

void print_list(int reverse) {
    struct node* tmp;
    if (reverse) tmp = end;
    else tmp = beg;
    while (tmp != NULL) {
        printk("list_val - %d\n", tmp->val);
        if (reverse) tmp = tmp->prev;
        else tmp = tmp->next;
    }
}

void free_list(void) {
    struct node* tmp;
    while (beg != NULL) {
        tmp = beg->next;
        kfree(beg);
        beg = tmp;
    }
}

static void __exit my_exit(void) {
    print_list(0);
    print_list(1);
    free_list();
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");