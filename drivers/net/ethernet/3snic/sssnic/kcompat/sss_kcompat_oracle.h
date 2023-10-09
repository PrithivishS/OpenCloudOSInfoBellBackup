#ifndef SSS_KCOMPAT_ORACLE_H_
#define SSS_KCOMPAT_ORACLE_H_
#ifdef __ORACLE__

#ifdef __ORACLE_78__ /* for ol7.8 */
#ifdef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#undef HAVE_DEVLINK_FLASH_UPDATE_PARAMS
#endif
#endif

#if defined(__ORACLE_85__) || defined(__ORACLE_86__)
#ifdef pci_cleanup_aer_uncorrect_error_status
#undef pci_cleanup_aer_uncorrect_error_status
#endif
#define pci_cleanup_aer_uncorrect_error_status pci_aer_clear_nonfatal_status
#endif

#endif
#endif