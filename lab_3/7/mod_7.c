#include <linux/module.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <linux/random.h>

struct example_struct 
{
	int data;
    char random_letter;
	struct rb_node node;
};

static struct rb_root root = RB_ROOT;

static struct example_struct *find_node(struct rb_root *root, int number)
{
	struct rb_node *node = root->rb_node;

	while(node) {
		struct example_struct *current_data = rb_entry(node, struct example_struct, node);

		if(number<current_data->data)
			node = node->rb_left;
		else if(number>current_data->data)
			node = node->rb_right;
		else
			return current_data;
	}
	return NULL;
}

static bool insert_node(struct rb_root *root, struct example_struct *node)
{
	struct rb_node **new_node = &(root->rb_node), *parent = NULL;

	while(*new_node) {
		struct example_struct *this = rb_entry(*new_node, struct example_struct, node);

		parent = *new_node;
		if(node->data<this->data) 
			new_node = &((*new_node)->rb_left);
		else if(node->data>this->data) 
			new_node = &((*new_node)->rb_right);
		else return false;
	}

	rb_link_node(&node->node,parent,new_node);
	rb_insert_color(&node->node,root);
	return true;
}

static int __init rbtreemod_init(void) 
{
	int i;
	struct example_struct *node = NULL;

	for(i=0;i<5;i++) {
		node = (struct example_struct *)kmalloc(sizeof(struct example_struct), GFP_KERNEL);
		if(!IS_ERR(node)) {

            // Generuj mala litere
            node->random_letter = 'a' + ((char) get_random_int() % 26);

			node->data = i;
			if(!insert_node(&root,node))
				pr_alert("Error inserting new node!\n");

		} else
			pr_alert("Error allocating memory for new node: %ld\n",PTR_ERR(node));

	}

	for(i=0;i<5;i++) {
		node = find_node(&root,i);
		if(node) {
			pr_notice("Value of the node: %d\n", node->data);
            pr_notice("Letter of the node: %c\n", node->random_letter);
        } else
			pr_alert("Error retrieving node from red-black tree!\n");
	}

	return 0; 
}

static void __exit rbtreemod_exit(void) 
{ 
	int i;
	struct example_struct *node = NULL;

	for(i=0;i<5;i++) {
		node = find_node(&root,i);
		if(node) {
			rb_erase(&node->node, &root);
			kfree(node);
		} else
			pr_alert("Error retrieving node from red-black tree!\n");
	}
}

module_init(rbtreemod_init);
module_exit(rbtreemod_exit);
MODULE_LICENSE("GPL");
