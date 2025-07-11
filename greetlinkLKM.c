* Module*/

#include <linux/module.h>   /*Include this for any kernel Module you write*/
#include <linux/netlink.h> 
#include <net/sock.h>       /*Network namespace and socket Based APIs*/
#include <linux/string.h>   /*for memset/memcpy etc..., do not use <string.h>, that is for user space*/
#include <linux/kernel.h>   /*for scnprintf*/
#include "netLinkKernelUtils.h"


#define NETLINK_TEST_USER 31
static struct sock *nl_sk = NULL; /*netlink socket structure*/


static void netlink_recv_msg_fn(struct sk_buff *skb_in){

} 


static struct netlink_kernel_cfg cfg = {
    .input = netlink_recv_msg_fn,
};
static int __init NetlinkGreetings_init(void){
    printk(KERN_INFO "Hello kernel, I am kernel Module NetlinkGreetingsLKM.ko\n");
    // Create a netlink socket with the specified configuration.
    // &init_net:     Network namespace (usually &init_net for most modules).
    // NETLINK_USER:  Protocol number for user-defined netlink communication.
    // &cfg:          Pointer to netlink_kernel_cfg struct (contains input callback).
    nl_sk = netlink_kernel_create(&init_net, NETLINK_TEST_USER, &cfg);
    if (!nl_sk)
    {   
        printk(KERN_ALERT "Error creating netlink socket.\n");
        return -ENOMEM;
    }
    
    printk(KERN_INFO "netlink socket created successfully.\n");

    return 0;
}

static int __init NetlinkGreetings_exit(void){
    printk(KERN_INFO "Bye Bye, Exiting kernel Module NetlinkGreetingsLKM.ko\n");
    // Release the netlink socket
    netlink_kernel_release(nl_sk);
    nl_sk = NULL;
    printk(KERN_INFO "netlink socket released successfully.\n");
    return 0;
}

// register init and exit 
module_init(NetlinkGreetings_init);
module_exit(NetlinkGreetings_exit);

MODULE_LICENSE("GPL");



/*
// from include/net/netlink.h in recent kernels

struct netlink_kernel_cfg {
    unsigned int		groups;
    unsigned int		flags;
    void			(*input)(struct sk_buff *skb);
    struct module		*module;
    struct net		*netns;
    void			(*bind)(int group);
    void			(*unbind)(int group);
    bool			(*compare)(struct net *net, struct sock *sk);
};
*/

static void nlmsg_dump(struct nlmsghdr *nlh)
{
    printk(KERN_INFO "nlmsg_dump: nlmsg_len=%u, nlmsg_type=%u, nlmsg_flags=%u, nlmsg_seq=%u, nlmsg_pid=%u\n",
           nlh->nlmsg_len, nlh->nlmsg_type, nlh->nlmsg_flags, nlh->nlmsg_seq, nlh->nlmsg_pid);

}
