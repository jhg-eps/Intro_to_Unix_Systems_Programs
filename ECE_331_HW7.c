/* Joseph Garcia */
/* ECE 331 */
/* Homework 7 */
/* Andrew Sheaff */

Based on the kernel module developed in class, write a kernel driver that allows a thread (user space program) to store a single 64 bit unsigned integer. Allow multiple simultaneous readers and writers but after a value is written, block all other writers until another process reads the value back. After the read is successful, have the next writer write its value.

#include <linux/modules.h> /* needed for all modules */
#include <linux/init.h>  /* allow for exit and init macros */
#include <linux/kernel.h> /*needed for KERN_ETC log messages */
#include <linux/errno.h> /* for standardizing of returning error values. */
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/proc_fs.h>	/* Necessary because we use proc fs */
#include <linux/sched.h>	/* For putting processes to sleep and 
				   waking them up */
#include <asm/uaccess.h>	/* for get_user and put_user */

int register_chrdev(unsigned int major, const char *name, struct file_operations *fops);
Here’s the code we use in
scull
’s source to get a major number:
if (scull_major) {
dev = MKDEV(scull_major, scull_minor);
result = register_chrdev_region(dev, scull_nr_devs, "scull");
} else {
result = alloc_chrdev_region(&dev, scull_minor, scull_nr_devs,
* The Linux Standard Base specifies that init scripts should be placed in
/etc/init.d
, but some distributions still
place them elsewhere. In addition, if your script is to be run at boot time, you need to make a link to it from
the appropriate run-level directory (i.e.,
.../rc3.d
).
† Though certain kernel developers have threatened to do exactly that in the future.
‡ The init script
scull.init
doesn’t accept driver options on the command line, but it supports a configuration
file, because it’s designed for automatic use at boot and shutdown time.
,ch03.22228 Page 48 Friday, January 21, 2005 1:32 PM
This is the Title of the Book, eMatter Edition
Copyright © 2005 O’Reilly & Associates, Inc. All rights reserved.
Some Important Data Structures
|
49
"scull");
scull_major = MAJOR(dev);
}
if (result < 0) {
printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
return result;
}
// Check this out:
If you pass a major number of 0 to register_chrdev, the return value will be the dynamically allocated major number. The downside is that you can't make a device file in advance, since you don't know what the major number will be. There are a couple of ways to do this. First, the driver itself can print the newly assigned number and we can make the device file by hand. Second, the newly registered device will have an entry in /proc/devices, and we can either make the device file by hand or write a shell script to read the file in and make the device file. The third method is we can have our driver make the the device file using the mknod system call after a successful registration and rm during the call to cleanup_module.
THe register_chrdev system call should return a value greater than 0 for success
    try_module_get(THIS_MODULE): Increment the use count.(how many using the module)

    module_put(THIS_MODULE): Decrement the use count (how many using the module)

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */

There is one more point to remember. Some times processes don't want to sleep, they want either to get what they want immediately, or to be told it cannot be done. Such processes use the O_NONBLOCK flag when opening the file. The kernel is supposed to respond by returning with the error code -EAGAIN from operations which would otherwise block, such as opening the file in this example. The program cat_noblock, available in the source directory for this chapter, can be used to open a file with O_NONBLOCK.

Write every part of the module (device driver) with concurrency in mind!!!!

<linux/sched.h> defines the task_struct structure, which gives us information about the process currently being executed. It is not a global variable in the strictest sense. The "current" process is the one that invoked a system call which is currently being executed.

An example of using the "current" object

the following statement prints the process ID and the command name of the current process by accessing certain fields in struct task_struct

printk(KERN_INFO "The process is \"%s\" (pid %i)\n",
current->comm, current->pid);
The command name stored in current->comm is the base name of the program file (trimmed to 15 characters if need be) that is being executed by the current process


N.B.
Kernel code cannot do floating point arithmetic!!!!!

The lsmod program produces a list of the modules currently loaded in the kernel. Some other information, such as any other modules making use of a specific module, is also provided. lsmod works by reading the /proc/modules virtual file. Information on currently loaded modules can also be found in the sysfs virtual filesystem under /sys/module


Clearly, if a module is to work with a given kernel, it must be built with the same understanding of the target processor as that kernel was. Once again, the vermagic.o object comes in to play. When a module is loaded, the kernel checks the processor-specific configuration options for the module and makes sure they match the run-ning kernel. If the module was compiled with different options, it is not loaded

All module parameters should be given a default value;
insmod changes the value only if explicitly told to by the user.

for module parameters, a common permission mask is S_IRUGO|S_IWUSR so that the world can read the parameters, but only ROOT can change them.





