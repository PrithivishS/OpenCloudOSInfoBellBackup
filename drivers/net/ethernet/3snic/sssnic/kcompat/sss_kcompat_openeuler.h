#ifndef _SSSNIC_KCOMPAT_OPENEULER_H_
#define _SSSNIC_KCOMPAT_OPENEULER_H_
#ifdef __OPENEULER__
#if (KERNEL_VERSION(5, 10, 0) == LINUX_VERSION_CODE) /*for openeuler 22.03 lts*/

#ifndef HAVE_ETHTOOL_COALESCE_EXTACK
#define HAVE_ETHTOOL_COALESCE_EXTACK
#endif

#ifndef HAVE_ETHTOOL_RINGPARAM_EXTACK
#define HAVE_ETHTOOL_RINGPARAM_EXTACK
#endif

#endif /*(KERNEL_VERSION(5, 10, 0)*/

#if (KERNEL_VERSION(4, 19, 90) == LINUX_VERSION_CODE)
#ifdef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#undef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#endif
#endif

#endif /*__OPENEULER__*/
#endif

