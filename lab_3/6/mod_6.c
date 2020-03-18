#include<linux/module.h>
#include<linux/random.h>
#include<linux/slab.h>
#include<linux/radix-tree.h>
 
#define UPPER_LIMIT 65535
 
static RADIX_TREE(root,GFP_KERNEL);

// Parametry
char* string1;
char* string2;
char* string3;

module_param(string1, charp, 0);
module_param(string2, charp, 0);
module_param(string3, charp, 0);
 
static int __init radixtreemod_init(void)
{
    char* strings[3] = { string1, string2, string3 };
    int i;
    for(i=0; i < 3; i++) {
        if(radix_tree_insert(&root, i, (void *)strings[i]))
            pr_alert("Error inserting item to radix tree!\n");
        else
            pr_notice("Adding \"%s\" to tree!", strings[i]);
    }
    return 0;
}
 
static void __exit radixtreemod_exit(void)
{
    char *data=NULL;
    int i;
    for(i = 0; i < 3; i++) {
        data = (char *)radix_tree_delete(&root,i);
        if(data) {
            pr_notice("Value retrieved from radix tree: %s for index: %d\n", data,i);
        kfree(data);
        } else
            pr_alert("Error retrieving data from tree!\n");
    }
}
 
module_init(radixtreemod_init);
module_exit(radixtreemod_exit);
 
MODULE_LICENSE("GPL");
