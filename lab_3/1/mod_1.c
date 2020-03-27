#include<linux/module.h>
#include<linux/sched.h>
#include<linux/string.h>
 
static int init_tasklist(void)
{
 
        struct task_struct *p;
 
        for_each_process(p) {
                if (strcmp(p->comm, "a.out") == 0) {

                printk(KERN_INFO"Nazwa procesu: %s", p->comm);
                printk(KERN_INFO"PID procesu: %i ", p->pid);

                if (p->state == TASK_RUNNING)
                        printk("Stan - TASK_RUNNING\n");
                if (p->state == TASK_INTERRUPTIBLE)
                        printk("Stan - TASK_INTERRUPTIBLE\n");
                if (p->state == TASK_UNINTERRUPTIBLE)
                        printk("Stan - TASK_UNINTERRUPTIBLE\n");
                if (p->state == TASK_STOPPED)
                        printk("Stan - TASK_STOPPED\n");
        }}
        return 0;
}
 
static void exit_tasklist(void) {
 
        struct task_struct *p;
 
        for_each_process(p) {
                printk(KERN_INFO "Nazwa procesu: %s, PID: %i. ",p->comm, p->pid);

                if(p->state == TASK_RUNNING)
                        printk("Stan - TASK_RUNNING.\n");
                if(p->state == TASK_INTERRUPTIBLE)
                        printk("Stan - TASK_INTERRUPTIBLE.\n");
                if(p->state == TASK_UNINTERRUPTIBLE)
                        printk("Stan - TASK_UNINTERRUPTIBLE.\n");
                if(p->state == TASK_STOPPED)
                        printk("Stan - TASK_STOPPED.\n");
        }
}
 
module_init(init_tasklist);
module_exit(exit_tasklist);
 
MODULE_LICENSE("GPL");