# SPDX-License-Identifier: GPL-2.0
# Copyright (c) 2023 3SNIC
#

EXPORT_SYMBOL := true

ccflags-y += -I$(srctree)/drivers/net/ethernet/3snic/sssnic/include
ccflags-y += -I$(srctree)/drivers/net/ethernet/3snic/sssnic/include/hw
ccflags-y += -I$(srctree)/drivers/net/ethernet/3snic/sssnic/include/kernel
ccflags-y += -I$(srctree)/drivers/net/ethernet/3snic/sssnic/nic
ccflags-y += -I$(srctree)/drivers/net/ethernet/3snic/sssnic/nic/include
ccflags-y += -I$(srctree)/drivers/net/ethernet/3snic/sssnic/kcompat
ccflags-y += -I$(srctree)/drivers/net/ethernet/3snic/sssnic/nic/tool

#ccflags-y += -Werror
SYS_TIME=$(shell date +%Y-%m-%d_%H:%M:%S)
ccflags-y += -D __TIME_STR__=\"$(SYS_TIME)\"

include $(srctree)/drivers/net/ethernet/3snic/sssnic/kcompat/mk/kcompat.mk

obj-$(CONFIG_SSSNIC) += sssnic.o
sssnic-y := sss_nic_main.o \
			./tool/sss_tool_nic_func.o \
			./tool/sss_tool_nic_dcb.o \
			./tool/sss_tool_nic_phy_attr.o \
			./tool/sss_tool_nic_qp_info.o \
			./tool/sss_tool_nic_stats.o \
			sss_nic_tx.o \
			sss_nic_tx_init.o \
			sss_nic_rx.o \
			sss_nic_rx_init.o \
			sss_nic_rx_reset.o \
			sss_nic_rss.o \
			sss_nic_ntuple.o \
			sss_nic_dcb.o \
			sss_nic_ethtool.o \
			sss_nic_ethtool_api.o \
			sss_nic_ethtool_stats.o \
			sss_nic_ethtool_stats_api.o \
			sss_nic_irq.o \
			sss_nic_filter.o \
			sss_nic_netdev_ops.o \
			sss_nic_cfg.o \
			sss_nic_mag_cfg.o \
			sss_nic_vf_cfg.o \
			sss_nic_rss_cfg.o \
			sss_nic_event.o \
			sss_nic_io.o \
			sss_nic_netdev_ops_api.o
