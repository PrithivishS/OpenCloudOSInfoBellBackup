#ifndef _SSS_KCOMPAT_FUSIONOS_H_
#define _SSS_KCOMPAT_FUSIONOS_H_
#ifdef __FUSIONOS__

#if (KERNEL_VERSION(4,19,90) == LINUX_VERSION_CODE)
#ifdef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#undef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#endif
#endif

#endif
#endif

