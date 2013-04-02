/* 
 *  Copyright (C) 2013 Bo Zhi <bobo900128@gmail.com>
 *  
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 */

#include <linux/module.h>
#include <linux/xattr.h>
#include "tdm.h"

/*  
int tdm_initialized;
*/
char *tdm_config_policies[] = {
#ifdef CONFIG_SECURITY_SELINUX
    XATTR_NAME_SELINUX,
#endif
#ifdef CONFIG_SECURITY_SMACK
    XATTR_NAME_SMACK，
#endif
#ifdef CONFIG_IMA_APPRAISE
    XATTR_NAME_IMA,
#endif
    XATTR_NAME_CAPS,
    NULL
};

/* 
static int tdm_used_xattr(const char *req_xattr_name)
{
    char **xattrname;
    int namelen;
    int found = 0;

    namelen = strlen(req_xattr_name);
    for (xattrname = tdm_config_policies; *xattrname != NULL; xattrname++){
        if ((strlen(*xattrname) == namelen))
            && (strncmp(req_xattr_name, *xattrname, namelen) == 0){
            found = 1;
            break;
        }
        if (strncmp(req_xattr_name,
                *xattrname + XATTR_SECURITY_PREFIX_LEN,
                strlen(req_xattr_name)) == 0){
            found = 1;
            break;
        }
    }
    return found;
}
*/

int tdm_inode_init_security(struct inode *inode,
            /* const struct xattr *lsm_xattr, */    
                struct xattr *tdm_xattr)
{
    struct tdm_xattr_data *xattr_data;

    /* TODO
     * if (!tdm_initialized || !tdm_used_xattr(lsm_xattr->name))   
     *      return 0;
     */

    xattr_data = kzalloc(sizeof(*xattr_data), GFP_NOFS);
    if (!xattr_data)
        return -ENOMEM;

    xattr_data->degree = TDM_DEFAULT_DEG;
    xattr_data->threshold = TDM_DEFAULT_THRESHOLD;
    xattr_data->status = TDM_TRUSTED;

    tdm_xattr->value = xattr_data;
    tdm_xattr->value_len = sizeof(*xattr_data);
    tdm_xattr->name = kstrdup(XATTR_TDM_SUFFIX, GFP_NOFS);

    struct tdm_xattr_data *xattr_data_1;
    xattr_data_1 = kzalloc(sizeof(*xattr_data_1), GFP_NOFS);
    if (!xattr_data_1)
        return -ENOMEM;
    xattr_data_1 = tdm_xattr->value;

    printk(KERN_INFO "TDM: xattr-value: degree = %d, threshold = %d, status = %d\n", 
            xattr_data_1->degree,  
            xattr_data_1->threshold, 
            xattr_data_1->status);

    printk(KERN_INFO "TDM: tdm_xattr: value = ^^^,value_len = %d,\
            tdm_xattr->name = security.tdm\n", 
            tdm_xattr->value_len);
    return 0;
}
EXPORT_SYMBOL_GPL(tdm_inode_init_security);

static void __exit cleanup_tdm(void)
{
    printk(KERN_INFO "TDM: trust degree module exit.\n");
}

static int __init init_tdm(void)
{
    printk(KERN_INFO "TDM: trust degree module initialized.\n");
    return 0;
}

static int __init tdm_display_config(void)
{
    char **xattrname;

    printk(KERN_INFO "TDM: hello, world!\n");

    for (xattrname = tdm_config_policies; *xattrname != NULL; xattrname++)
        printk(KERN_INFO "TDM: use %s\n", *xattrname);
    return 0;
}

pure_initcall(tdm_display_config);
late_initcall(init_tdm);

MODULE_DESCRIPTION("Trust Degree Module");
MODULE_AUTHOR("Bo Zhi");
MODULE_LICENSE("GPL");
