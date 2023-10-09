#ifndef _SSS_KCOMPAT_SLES_H_
#define _SSS_KCOMPAT_SLES_H_

#ifdef __SLES__

#include "linux/suse_version.h"

#if (SUSE_PRODUCT(1, 12, 5, 0) <= SUSE_PRODUCT_CODE)
#ifndef HAVE_NDO_SELECT_QUEUE_SB_DEV
#define HAVE_NDO_SELECT_QUEUE_SB_DEV
#endif
#endif

#if (SUSE_PRODUCT(1, 15, 2, 0) <= SUSE_PRODUCT_CODE)
#ifndef pci_cleanup_aer_uncorrect_error_status
#define pci_cleanup_aer_uncorrect_error_status pci_aer_clear_nonfatal_status
#endif

#ifdef skb_frag_off_add
#undef skb_frag_off_add
#endif
#endif

#if (SUSE_PRODUCT(1, 15, 3, 0) <= SUSE_PRODUCT_CODE)
#if (SUSE_PRODUCT(1, 15, 3, 0) == SUSE_PRODUCT_CODE)
#ifdef HAVE_DEVLINK_FW_FILE_NAME_PARAM
#undef HAVE_DEVLINK_FW_FILE_NAME_PARAM
#endif

#ifndef HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#define HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#endif
#endif /* SUSE_PRODUCT(1, 15, 3, 0) == SUSE_PRODUCT_CODE */

#ifndef HAVE_TX_TIMEOUT_TXQUEUE
#define HAVE_TX_TIMEOUT_TXQUEUE
#endif
#endif /* SUSE_PRODUCT(1, 15, 3, 0) <= SUSE_PRODUCT_CODE */

#if (SUSE_PRODUCT(1, 15, 4, 0) <= SUSE_PRODUCT_CODE)
#ifndef HAS_DEVLINK_ALLOC_SETS_DEV
#define HAS_DEVLINK_ALLOC_SETS_DEV
#endif

#ifndef NO_DEVLINK_REGISTER_SETS_DEV
#define NO_DEVLINK_REGISTER_SETS_DEV
#endif

#ifndef DEVLINK_REGISTER_RETURN_VOID
#define DEVLINK_REGISTER_RETURN_VOID
#endif

#define devlink_params_publish(x) do{}while(0)
#define devlink_params_unpublish(x) do{}while(0)

#ifndef HAVE_ETHTOOL_COALESCE_EXTACK
#define HAVE_ETHTOOL_COALESCE_EXTACK
#endif
#endif/* SUSE_PRODUCT(1, 15, 4, 0) <= SUSE_PRODUCT_CODE */

#if (SUSE_PRODUCT(1, 2, 0, 0) == SUSE_PRODUCT_CODE)	/*for sles euler 2.0*/
#ifdef HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#undef HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#endif

/* sles euler2.0 devlink升级firmware时需要通知用户空间开始和结束状态 */
#ifndef DEVLINK_FLASH_UPDATE_NEED_TO_NOTIFY
#define DEVLINK_FLASH_UPDATE_NEED_TO_NOTIFY
#endif

#ifndef HAVE_BFP_WARN_NETDEV_PARAM
#define HAVE_BFP_WARN_NETDEV_PARAM
#endif

#ifndef HAVE_ETHTOOL_COALESCE_EXTACK
#define HAVE_ETHTOOL_COALESCE_EXTACK
#endif

#ifndef HAVE_ETHTOOL_RINGPARAM_EXTACK
#define HAVE_ETHTOOL_RINGPARAM_EXTACK
#endif
#endif /* SUSE_PRODUCT(1, 2, 0, 0) == SUSE_PRODUCT_CODE */

#endif /* __SLES__ */

#endif

