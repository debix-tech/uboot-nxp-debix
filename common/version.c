// SPDX-License-Identifier: GPL-2.0+
/*
 * Copyright 2000-2009
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 */

#include <timestamp.h>
#include <version.h>
#include <version_string.h>

//John_gao add uboot version
#if 1
#define Debix_C_VERSION " Debix_Model_AB V1.0.2 "
#define U_BOOT_VERSION_STRING Debix_C_VERSION " (" U_BOOT_DATE " - " U_BOOT_TIME ")"

#else
#define U_BOOT_VERSION_STRING U_BOOT_VERSION " (" U_BOOT_DATE " - " \
	U_BOOT_TIME " " U_BOOT_TZ ")" CONFIG_IDENT_STRING
#endif

const char version_string[] = U_BOOT_VERSION_STRING;
const unsigned short version_num = U_BOOT_VERSION_NUM;
const unsigned char version_num_patch = U_BOOT_VERSION_NUM_PATCH;
