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

/*
 * get_full_file_name - dump full path
static void get_full_file_name(char *result, struct file *file)
{
    char buf[IMA_EVENT_NAME_LEN_MAX], *result_temp = NULL;

    result_temp = d_path(&file->f_path, buf, IMA_EVENT_NAME_LEN_MAX);
    strncpy(result, result_temp, IMA_EVENT_NAME_LEN_MAX);
}
 */

static int dump_xattr_data_dummy(struct tdm_xattr_data *xattr_data)
{
    u8 deg = xattr_data->degree;
    u8 thd = xattr_data->threshold;
    enum tdm_trust_status sts = xattr_data->status;

    printk(KERN_INFO "TDM: security.tdm infomation...\n");
    printk(KERN_INFO "tdm->deg: %d, tdm->thd: %d, tdm->sts: %d\n",
            deg, thd, sts);

    return 0;
}

/*
 * tdm_decrease_degree_dummy - update degree
 * TODO: replace degree number with macros
 */
int tdm_decrease_degree_dummy(struct tdm_xattr_data *xattr_data)
{
    u8 deg = xattr_data->degree;
    u8 thd = xattr_data->threshold;
    enum tdm_trust_status sts = xattr_data->status;

    printk(KERN_INFO "TDM: decreasing trust degree...\n");

    if (0 == deg){
        if (sts != TDM_UNKNOWN)
            sts = TDM_UNKNOWN;
        goto out;
    }
    if (deg < 2){
        deg = 1;
        sts = TDM_UNTRUSTED;
        goto out;
    }
    deg -= 2;
    if (deg < thd)
        sts = TDM_UNTRUSTED;
out:
    xattr_data->degree = deg;
    xattr_data->threshold = thd;
    xattr_data->status = sts;
    
    printk(KERN_INFO "TDM: trust degree has been decreased!\n");

    return 0;
}

int tdm_init_xattr_dummy(struct tdm_xattr_data *xattr_data)
{
    printk(KERN_INFO "TDM: initializing trust degree...\n");

    xattr_data->degree = 100;
    xattr_data->threshold = 90;
    xattr_data->status = TDM_TRUSTED;

    printk(KERN_INFO "TDM: trust degree has been initialized!\n");

    return 0;
}

/*
 * TODO: tdm_trust_degree_check_dummy - check security.tdm
 * nothing, but just check whether the degree > threshold
 */

/* 
 * tdm_inode_update_xattr_dummy - update security.tdm 
 * nothing, but just decreasing degree for test
 */
int tdm_inode_update_xattr_dummy(struct dentry *dentry)
{
    struct inode *inode = dentry->d_inode;
    struct tdm_xattr_data *xattr_data;
    int size, rc = 0;

    printk(KERN_INFO "TDM: updating trust degree...\n");

    xattr_data = kzalloc(sizeof(*xattr_data), GFP_NOFS);
    if (!xattr_data)
        return -ENOMEM;
    size = sizeof(struct tdm_xattr_data);

    rc = vfs_getxattr(dentry, XATTR_NAME_TDM, xattr_data, size);
    printk(KERN_INFO "TDM: get xattr return code: %d.\n", rc);
    dump_xattr_data_dummy(xattr_data);
    if (rc < 0)
        rc = tdm_init_xattr_dummy(xattr_data);
    else 
        rc = tdm_decrease_degree_dummy(xattr_data);

    if (rc)
        return rc;

    /*TODO: sth wrong
    mutex_lock(&inode->i_mutex);
    */
    rc = __vfs_setxattr_noperm(dentry, XATTR_NAME_TDM, 
            xattr_data, sizeof(*xattr_data), 0);
    /*
    mutex_unlock(&inode->i_mutex);
    */

    printk(KERN_INFO "TDM: trust degree has been updated!\n");

    return rc;
}
EXPORT_SYMBOL_GPL(tdm_inode_update_xattr_dummy);

int tdm_inode_init_security(struct inode *inode,
            /* const struct xattr *lsm_xattr, */    
                struct xattr *tdm_xattr)
{
    struct tdm_xattr_data *xattr_data;
    struct tdm_xattr_data *xattr_data_1;
    /*  
    struct dentry *d = list_entry(inode->i_dentry.next, struct dentry, d_alias);
    char *filename = d_path(d, mntget(current->fs->rootmnt), filebuf, PATH_MAX);
    mntput(current->fs->rootmnt);
    */

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

    xattr_data_1 = kzalloc(sizeof(*xattr_data_1), GFP_NOFS);
    if (!xattr_data_1)
        return -ENOMEM;
    xattr_data_1 = tdm_xattr->value;

    printk(KERN_INFO "TDM: inode number = %lu, degree = %d, threshold = %d, status = %d\n", 
            inode->i_ino,
            xattr_data_1->degree,  
            xattr_data_1->threshold, 
            xattr_data_1->status);

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
