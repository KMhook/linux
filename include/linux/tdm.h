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
static inline int tdm_inode_init_security(struct inode *inode, 
                                        struct xattr *tdm_xattr)
{
    return 0;
}
#endif /* CONFIG_TDM_H */
#endif /* LINUX_TDM_H */
