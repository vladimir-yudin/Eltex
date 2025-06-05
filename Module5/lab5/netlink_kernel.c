#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
#include <net/sock.h>
#include "constants.h"

MODULE_DESCRIPTION("Example module illustrating the use of netlink.");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yudin V.");

struct sock *nl_sk = NULL;

static void nl_recv_msg(struct sk_buff *skb)
{
    struct nlmsghdr *nlh;
    int pid;
    struct sk_buff *skb_out;
    char *msg = "Hello from kernel";
    int msg_size = strlen(msg);
    int res;

    printk(KERN_INFO "netlink_kernel: message received\n");

    nlh = (struct nlmsghdr *)skb->data;
    printk(KERN_INFO "Received payload: %s\n", (char *)nlmsg_data(nlh));

    pid = nlh->nlmsg_pid;

    skb_out = nlmsg_new(msg_size, 0);
    if (!skb_out) {
        printk(KERN_ERR "Failed to make new skb\n");
        return;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    strncpy(nlmsg_data(nlh), msg, msg_size);

    res = nlmsg_unicast(nl_sk, skb_out, pid);
    if (res < 0)
        printk(KERN_INFO "Error while sending back to user\n");
}

int init_module(void)
{
    struct netlink_kernel_cfg cfg = {
        .input = nl_recv_msg,
        .flags = NL_CFG_F_NONROOT_RECV | NL_CFG_F_NONROOT_SEND,
    };

    nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
    if (!nl_sk) {
        printk(KERN_ALERT "Error creating netlink socket.\n");
        return -1;
    }

    printk(KERN_INFO "netlink_kernel: module loaded\n");
    return 0;
}

void cleanup_module(void)
{
    netlink_kernel_release(nl_sk);
    printk(KERN_INFO "netlink_kernel: module unloaded\n");
}
