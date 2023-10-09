#ifndef _SSS_KCOMPAT_KOS_H_
#define _SSS_KCOMPAT_KOS_H_
#ifdef __KOS__

/* for KOS5.8-x86,KOS5.8-x86支持devlink，但不支持flash_update */
#if (KERNEL_VERSION(4, 19, 91) == LINUX_VERSION_CODE)
#ifdef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#undef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#endif
#endif

#endif

#endif

