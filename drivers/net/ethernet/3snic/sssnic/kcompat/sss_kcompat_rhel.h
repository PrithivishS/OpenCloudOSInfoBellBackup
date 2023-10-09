/* SPDX-License-Identifier: GPL-2.0 */
/* Copyright(c) 2022 3SNIC Co., Ltd */

#ifndef __SSS_KCOMPAT_RHEL_H_
#define __SSS_KCOMPAT_RHEL_H_

#ifdef RHEL_RELEASE_CODE

#ifndef RHEL_RELEASE_VERSION
#define RHEL_RELEASE_VERSION(a, b) (((a) << 8) + (b))
#endif

#if (RHEL_RELEASE_VERSION(7, 6) == RHEL_RELEASE_CODE) || \
	(RHEL_RELEASE_VERSION(7, 8) == RHEL_RELEASE_CODE) || \
	(RHEL_RELEASE_VERSION(7, 9) == RHEL_RELEASE_CODE)
#include <linux/module.h>
#endif

#if (RHEL_RELEASE_VERSION(9, 1) == RHEL_RELEASE_CODE)
#include <linux/filter.h>
#endif

#if (KERNEL_VERSION(3, 10, 0) <= LINUX_VERSION_CODE)
#if ((RHEL_RELEASE_VERSION(7, 0) <= RHEL_RELEASE_CODE) && \
	(RHEL_RELEASE_VERSION(8, 0) > RHEL_RELEASE_CODE))
#define HAVE_RHEL7_PCI_DRIVER_RH
#if (RHEL_RELEASE_VERSION(7, 2) <= RHEL_RELEASE_CODE)
#define HAVE_RHEL7_PCI_RESET_NOTIFY
#endif /* RHEL >= 7.2 */
#if (RHEL_RELEASE_VERSION(7, 3) <= RHEL_RELEASE_CODE)
#define HAVE_GENEVE_RX_OFFLOAD
#if !defined(HAVE_UDP_ENC_TUNNEL) && IS_ENABLED(CONFIG_GENEVE)
#define HAVE_UDP_ENC_TUNNEL
#endif
#ifdef ETHTOOL_GLINKSETTINGS
/* pay attention pangea platform when use this micro */
#define HAVE_ETHTOOL_25G_BITS
#endif /* ETHTOOL_GLINKSETTINGS */
#endif /* RHEL >= 7.3 */

/* new hooks added to net_device_ops_extended in RHEL7.4 */
#if (RHEL_RELEASE_VERSION(7, 4) <= RHEL_RELEASE_CODE)
#define HAVE_RHEL7_NETDEV_OPS_EXT_NDO_UDP_TUNNEL
#define HAVE_UDP_ENC_RX_OFFLOAD
#endif /* RHEL >= 7.4 */

#if (RHEL_RELEASE_VERSION(7, 5) <= RHEL_RELEASE_CODE)
#define HAVE_NDO_SETUP_TC_REMOVE_TC_TO_NETDEV
#endif /* RHEL > 7.5 */

#endif /* RHEL >= 7.0 && RHEL < 8.0 */
#endif /* >= 3.10.0 */

#if (KERNEL_VERSION(3, 11, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(6, 6) <= RHEL_RELEASE_CODE)
#define HAVE_NDO_SET_VF_LINK_STATE
#endif
#if (RHEL_RELEASE_VERSION(7, 2) < RHEL_RELEASE_CODE)
#define HAVE_NDO_SELECT_QUEUE_ACCEL_FALLBACK
#endif
#if (RHEL_RELEASE_VERSION(7, 3) <= RHEL_RELEASE_CODE)
#define HAVE_RHEL7_NET_DEVICE_OPS_EXT
#endif
#if (RHEL_RELEASE_VERSION(7, 4) <= RHEL_RELEASE_CODE)
#define HAVE_RHEL7_NETDEV_OPS_EXT_NDO_SET_VF_VLAN
#endif
#if (RHEL_RELEASE_VERSION(7, 5) <= RHEL_RELEASE_CODE)
#define HAVE_RHEL7_NETDEV_OPS_EXT_NDO_SETUP_TC
#define HAVE_RHEL7_NETDEV_OPS_EXT_NDO_CHANGE_MTU
#endif
#endif /* > 3.11.0 */

#if (KERNEL_VERSION(3, 16, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(7, 1) < RHEL_RELEASE_CODE)
#define HAVE_NDO_SET_VF_MIN_MAX_TX_RATE
#endif
#if (RHEL_RELEASE_VERSION(7, 4) <= RHEL_RELEASE_CODE)
#define HAVE_VLAN_FIND_DEV_DEEP_RCU
#endif
#endif /* < 3.16.0 */

#if (KERNEL_VERSION(3, 18, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(7, 1) < RHEL_RELEASE_CODE)
#define HAVE_MULTI_VLAN_OFFLOAD_EN
#endif

/* RHEL 7.1 backported csum_level, but SLES 12 and 12-SP1 did not */
#if RHEL_RELEASE_CODE && (RHEL_RELEASE_VERSION(7, 1) <= RHEL_RELEASE_CODE)
#define HAVE_SKBUFF_CSUM_LEVEL
#endif /* >= RH 7.1 */
#endif

#if (KERNEL_VERSION(3, 19, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(7, 1) < RHEL_RELEASE_CODE)
#define HAVE_RXFH_HASHFUNC
#endif /* RHEL > 7.1 */
#endif /* < 3.19.0 */

#if (KERNEL_VERSION(4, 4, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(7, 3) <= RHEL_RELEASE_CODE)
#define HAVE_NDO_SET_VF_TRUST
#endif /* (RHEL_RELEASE >= 7.3) */
#endif /* 4.4.0 */

#if (KERNEL_VERSION(4, 8, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(7, 4) <= RHEL_RELEASE_CODE)
#define HAVE_IO_MAP_WC_SIZE
#endif
#endif /* 4.8 */

#if (KERNEL_VERSION(4, 10, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(7, 5) <= RHEL_RELEASE_CODE)
#define HAVE_NETDEVICE_EXTENDED_MIN_MAX_MTU
#endif
#endif /* 4.10.0 */

#if (KERNEL_VERSION(4, 11, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(7, 5) <= RHEL_RELEASE_CODE)
#define HAVE_VOID_NDO_GET_STATS64
#endif
#endif /* 4.11.0 */

#if (KERNEL_VERSION(4, 18, 0) <= LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(8, 2) <= RHEL_RELEASE_CODE)
#define ETH_GET_HEADLEN_NEED_DEV
#endif
#define HAVE_NDO_SELECT_QUEUE_SB_DEV
#endif/* >= 4.18.0 */

#if (KERNEL_VERSION(4, 19, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(8, 0) <= RHEL_RELEASE_CODE)
#define HAVE_NDO_SELECT_QUEUE_SB_DEV
#endif
#endif

#if (KERNEL_VERSION(5, 0, 0) > LINUX_VERSION_CODE)
#if (RHEL_RELEASE_VERSION(8, 0) <= RHEL_RELEASE_CODE)
#define dev_open(x) dev_open(x, NULL)
#endif
#endif /* >= 5.0.0 */

#if (RHEL_RELEASE_VERSION(7, 0) <= RHEL_RELEASE_CODE)
#undef create_singlethread_workqueue
#define create_singlethread_workqueue(name) \
	alloc_ordered_workqueue("%s", WQ_MEM_RECLAIM, name)

#ifndef HAVE_VXLAN_RX_OFFLOAD
#define HAVE_VXLAN_RX_OFFLOAD
#endif /* HAVE_VXLAN_RX_OFFLOAD */

#if !defined(HAVE_UDP_ENC_TUNNEL) && IS_ENABLED(CONFIG_VXLAN)
#define HAVE_UDP_ENC_TUNNEL
#endif

#ifndef HAVE_VXLAN_CHECKS
#define HAVE_VXLAN_CHECKS
#endif

#endif

#if (RHEL_RELEASE_VERSION(8, 1) == RHEL_RELEASE_CODE)
#ifdef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#undef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#endif
#endif

//bclinux22.10 4.19.0-348.20.20.oe2203.bclinux.aarch64使用RHEL8.5，需要使用如下宏
#if (RHEL_RELEASE_VERSION(8, 2) < RHEL_RELEASE_CODE)
#ifdef pci_cleanup_aer_uncorrect_error_status
#undef pci_cleanup_aer_uncorrect_error_status
#endif
#define pci_cleanup_aer_uncorrect_error_status pci_aer_clear_nonfatal_status
#ifndef HAVE_TX_TIMEOUT_TXQUEUE
#define HAVE_TX_TIMEOUT_TXQUEUE
#endif
#endif

#if (RHEL_RELEASE_VERSION(8, 3) < RHEL_RELEASE_CODE)
#ifdef HAVE_XDP_QUERY_PROG
#undef HAVE_XDP_QUERY_PROG
#endif
#endif /*(RHEL_RELEASE_VERSION(8, 3) < RHEL_RELEASE_CODE)*/

#if (RHEL_RELEASE_VERSION(8, 5) == RHEL_RELEASE_CODE)
#ifdef HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#undef HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#endif
#ifdef HAVE_DEVLINK_FW_FILE_NAME_PARAM
#undef HAVE_DEVLINK_FW_FILE_NAME_PARAM
#endif
#endif

#if (RHEL_RELEASE_VERSION(8, 6) == RHEL_RELEASE_CODE)

#ifndef HAVE_ETHTOOL_COALESCE_EXTACK
#define HAVE_ETHTOOL_COALESCE_EXTACK
#endif

#ifdef HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#undef HAVE_DEVLINK_FW_FILE_NAME_MEMBER
#endif
#ifdef HAVE_DEVLINK_FW_FILE_NAME_PARAM
#undef HAVE_DEVLINK_FW_FILE_NAME_PARAM
#endif

#endif /*(RHEL_RELEASE_VERSION(8, 6) == RHEL_RELEASE_CODE)*/

#if (RHEL_RELEASE_VERSION(9, 0) <= RHEL_RELEASE_CODE)
#ifndef HAS_DEVLINK_ALLOC_SETS_DEV
#define HAS_DEVLINK_ALLOC_SETS_DEV
#endif
#ifndef NO_DEVLINK_REGISTER_SETS_DEV
#define NO_DEVLINK_REGISTER_SETS_DEV
#endif
#ifndef NO_DEVLINK_PARAMS_PUBLISH
#define NO_DEVLINK_PARAMS_PUBLISH
#endif
#define devlink_params_publish(x) do{}while(0)
#define devlink_params_unpublish(x) do{}while(0)

#ifndef REGISTER_DEVLINK_PARAMETER_PREFERRED
#define REGISTER_DEVLINK_PARAMETER_PREFERRED
#endif

//backport对所有rhel系统放开，编译发现只有9.0以上版本不定义报错
#ifndef DEVLINK_REGISTER_RETURN_VOID
#define DEVLINK_REGISTER_RETURN_VOID
#endif

#endif /*(RHEL_RELEASE_VERSION(9, 0) <= RHEL_RELEASE_CODE)*/

#if (RHEL_RELEASE_VERSION(9, 1) == RHEL_RELEASE_CODE)
#ifndef pci_pool
#define pci_pool dma_pool
#endif

#ifndef pci_pool_alloc
#define pci_pool_alloc(pool, flags, handle) dma_pool_alloc(pool, flags, handle)
#endif

#ifndef pci_pool_free
#define pci_pool_free(pool, vaddr, addr) dma_pool_free(pool, vaddr, addr)
#endif

#ifndef HAVE_ETHTOOL_COALESCE_EXTACK
#define HAVE_ETHTOOL_COALESCE_EXTACK
#endif

#ifndef HAVE_ETHTOOL_RINGPARAM_EXTACK
#define HAVE_ETHTOOL_RINGPARAM_EXTACK
#endif

#ifndef HAVE_BFP_WARN_NETDEV_PARAM
#define HAVE_BFP_WARN_NETDEV_PARAM
#endif
#endif/*(RHEL_RELEASE_VERSION(9, 1) == RHEL_RELEASE_CODE)*/

#else /* not define RHEL_RELEASE_CODE */
#include "sss_kcompat_oracle.h"
#endif /* #ifdef RHEL_RELEASE_CODE */

#endif

