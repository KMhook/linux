/* 
 *  Copyright (C) 2013 Bo Zhi <bobo900128@gmail.com>
 *  
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 */

#include <linux/types.h>

extern int tdm_initialized;

#define TDM_DEFAULT_DEG 100
#define TDM_DEFAULT_THRESHOLD 90

enum tdm_trust_status {
    TDM_TRUSTED = 0x01,
    TDM_UNKNOWN,
    TDM_UNTRUSTED,
};

struct tdm_xattr_data {
    u8 degree;
    u8 threshold;
    enum tdm_trust_status status;
} __attribute__((packed));

/*
 * TODO
 * enum tdm_trust_status tdm_inode_get_status(struct inode* inode);
 */
