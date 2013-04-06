/*
 * tdm.h
 *
 * Copyright (C) 2013 Bo Zhi <bobo900128@gmail.com>
 */

#ifndef _LINUX_TDM_H
#define _LINUX_TDM_H

#include <linux/xattr.h>

#ifdef CONFIG_TDM
struct tdm_xattr_data;
enum tdm_trust_status {
    TDM_TRUSTED = 0x01,
    TDM_UNKNOWN,
    TDM_UNTRUSTED,
};
extern int tdm_inode_init_security(struct inode *inode, 
        struct xattr *tdm_xattr);
extern int tdm_inode_update_xattr_dummy(struct dentry *dentry);

#else
static inline int tdm_inode_init_security(struct inode *inode, 
        struct xattr *tdm_xattr)
{
    printk(KERN_INFO "TDM: using blank init function.\n");
    return 0;
}

static inline int tdm_inode_update_xattr_dummy(struct dentry *dentry)
{
    printk(KERN_INFO "TDM: using blank update function.\n");
    return 0;
}
#endif /* CONFIG_TDM */
#endif /* LINUX_TDM_H */
