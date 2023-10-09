#ifndef SSS_KCOMPAT_KYLIN_H_
#define SSS_KCOMPAT_KYLIN_H_
#ifdef __KYLIN__


#ifdef __KYLIN_V10_SP3__

/* arm版kylin k10 V3不支持devlink升级firmware */
#ifdef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#undef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#endif

/* 这个宏可以帮助确保正确地计算出头部长度，这样就可以为每个接收到的以太网帧分配适当的内存空间 */
#ifndef ETH_GET_HEADLEN_NEED_DEV
#define ETH_GET_HEADLEN_NEED_DEV
#endif

/* kylin V10 SP3中已经有skb_frag_off_add函数，需要undef掉 */
#ifdef skb_frag_off_add
#undef skb_frag_off_add
#endif

#endif


#endif
#endif