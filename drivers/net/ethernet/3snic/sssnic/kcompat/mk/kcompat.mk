# get OS info

ifeq ($(shell uname -n), bytedance)
OS_TYPE ?= velinux
else
OS_TYPE ?= $(shell sed -n '/^ID=/p' /etc/os-release | awk -F '=' '{print $$2}' | sed 's/\"//g')
endif

ifeq ($(OS_TYPE), $(findstring $(OS_TYPE), centos opencloudos))
OS_VERSION_ID ?= $(shell cat /etc/centos-release | grep 'release' | awk -F 'release ' '{print $$2}' | awk -F " " '{print $$1}'|awk -F "." '{print $$1"."$$2}')
else ifeq ($(OS_TYPE), uos)
	OS_VERSION ?= $(shell uname -r)
	OS_PATCH_VERSION ?= $(shell echo $(OS_VERSION) | awk -F '-' '{print $$2}' | awk -F '.' '{print $$1}')
	OS_VERSION_ID_TEMP ?= $(shell sed -n '/^VERSION_ID/p' /etc/os-release | awk -F '=' '{print $$2}' | sed 's/\"//g')
	ifeq ($(OS_PATCH_VERSION), 2109)
		OS_VERSION_ID ?= $(OS_VERSION_ID_TEMP).1021e
	else ifeq ($(OS_PATCH_VERSION), 2106)
		OS_VERSION_ID ?= $(OS_VERSION_ID_TEMP).1020e
	else ifeq ($(OS_PATCH_VERSION), 2211)
		OS_VERSION_ID ?= $(OS_VERSION_ID_TEMP).1050u2e
	else ifeq ($(OS_PATCH_VERSION), 91)
		OS_PATCH_VERSION_MINOR ?= $(shell echo $(OS_VERSION) | awk -F '-' '{print $$2}' | awk -F '.' '{print $$3}')
		ifeq ($(OS_PATCH_VERSION_MINOR), 132)
			OS_VERSION_ID ?= $(OS_VERSION_ID_TEMP).1050u2a
		else ifeq ($(OS_PATCH_VERSION_MINOR), 152)
			OS_VERSION_ID ?= $(OS_VERSION_ID_TEMP).1060a
		endif
	else ifeq ($(OS_PATCH_VERSION), 2201)
		OS_VERSION_ID ?= $(OS_VERSION_ID_TEMP).1050e
	else ifeq ($(OS_PATCH_VERSION), 2305)
		OS_VERSION_ID ?= $(OS_VERSION_ID_TEMP).1060e
	endif
else ifeq ($(OS_TYPE), ubuntu)
OS_VERSION_ID ?= $(shell sed -n '/^PRETTY_NAME/p' /etc/os-release | awk -F "\"" '{print $$2}'| awk -F ' ' '{print $$2}')
else ifeq ($(OS_TYPE), debian)
OS_VERSION_ID ?= $(shell cat /etc/debian_version)
else ifneq ($(OS_TYPE), velinux)
OS_VERSION_ID ?= $(shell sed -n '/^VERSION_ID/p' /etc/os-release | awk -F '=' '{print $$2}' | sed 's/\"//g')
endif

ifeq ($(OS_TYPE),openEuler)
OS_SP_ID ?= $(shell cat /etc/openEuler-release | awk -F '[()]' '{print $$2}' | awk -F '-' '{print $$2}')
else ifeq ($(OS_TYPE), kylin)
OS_SP_ID_NAME ?= $(shell sed -n '/^VERSION=/p' /etc/os-release | awk -F '[()]' '{print $$2}')
ifeq ($(OS_SP_ID_NAME), Tercel)
OS_SP_ID ?= SP1
else ifeq ($(OS_SP_ID_NAME), Sword)
OS_SP_ID ?= SP2
else ifeq ($(OS_SP_ID_NAME), Lance)
OS_SP_ID ?= SP3
endif
else ifeq ($(OS_TYPE), euleros)
	OS_SP_ID ?= $(shell cat /etc/euleros-release | awk -F '(' '{print $$2}' | awk -F ')' '{print $$1}' | awk -F $(CPU_ARCH) '{print $$1}')
else ifeq ($(OS_TYPE), bclinux)
	ifeq ($(OS_VERSION_ID), 21.10)
		OS_SP_ID ?= _$(shell uname -r | awk -F '-' '{print $$2}' | awk -F '.oe1' '{print $$1}' | awk -F '.' '{print $$NF}')
	else
		OS_SP_ID ?= _$(shell uname -r | awk -F '-' '{print $$1}')
	endif
else ifeq ($(OS_TYPE), KylinSecOS)
	OS_VERSION_ID ?= $(shell sed -n '/^VERSION_ID/p' /etc/os-release | awk -F '=' '{print $$2}' | sed 's/\"//g')
	ifeq ($(OS_VERSION_ID), 3.4-4)
		OS_VERSION_ID = 3.4.4
	else ifeq ($(OS_VERSION_ID), 3.4-5)
		OS_VERSION_ID = 3.4.5
	endif
endif

OS_RELEASE ?= $(OS_TYPE)$(OS_VERSION_ID)$(OS_SP_ID)

CPU_ARCH ?= $(shell uname -m)
KERNEL_RELEASE ?= $(shell uname -r | awk -F '.$(CPU_ARCH)' '{print $$1}')

ifeq ($(OS_TYPE)_$(OS_VERSION_ID)_$(KERNEL_RELEASE), bclinux_22.10_4.19.0-348.20.20.oe2203.bclinux)
	EXTRA_CFLAGS += -Wno-missing-attributes
endif

export OS_TYPE OS_VERSION_ID OS_RELEASE KERNEL_RELEASE CPU_ARCH

ccflags-y += -Wno-missing-braces -Wno-declaration-after-statement

# Check OS
ifeq ($(OS_TYPE), openEuler)
ccflags-y += -D__OPENEULER__
endif

ifeq ($(OS_TYPE), opencloudos)
ccflags-y += -D__OPENCLOUDOS__
endif

ifeq ($(OS_TYPE), tencentos)
ccflags-y += -D__TENCENTOS__
endif

ifeq ($(OS_TYPE), kylin)
ccflags-y += -D__KYLIN__
ifeq ($(OS_RELEASE), kylinV10SP3)
ccflags-y += -D__KYLIN_V10_SP3__
endif
endif

ifeq ($(OS_TYPE), uos)
ccflags-y += -D__UOS__
endif

ifeq ($(OS_TYPE), bclinux)
ifeq ($(OS_VERSION_ID), 22.10)
EXTRA_CFLAGS += -D__BCLINUX_22_10__
endif
endif

ifeq ($(OS_TYPE), KylinSecOS)
EXTRA_CFLAGS += -D__KYLINSECOS__
endif

ifeq ($(OS_TYPE), $(findstring $(OS_TYPE), sles suseeuler))
ccflags-y += -D__SLES__
endif

ifeq ($(OS_TYPE), fusionos)
EXTRA_CFLAGS += -D__FUSIONOS__
endif

ifeq ($(OS_TYPE), ol)
EXTRA_CFLAGS += -D__ORACLE__
ifeq ($(OS_RELEASE), ol7.8)
ccflags-y += -D__ORACLE_78__
else ifeq ($(OS_RELEASE), ol8.5)
ccflags-y += -D__ORACLE_85__
else ifeq ($(OS_RELEASE), ol8.6)
ccflags-y += -D__ORACLE_86__
endif
endif

ifeq ($(OS_TYPE), kos)
EXTRA_CFLAGS += -D__KOS__
endif
