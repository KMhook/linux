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

static int __init init_cred(void)
{
    return 0;
}

late_initcall(init_cred);

MODULE_DESCRIPTION("Credibility of subject");
MODULE_AUTHOR("Bo Zhi");
MODULE_LICENSE("GPL");
