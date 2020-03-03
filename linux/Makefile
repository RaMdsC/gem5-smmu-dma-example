ARCH ?= arm64
KDIR ?= /lib/modules/`uname -r`/build
CROSS_COMPILE ?= aarch64-linux-gnu-
INSTALL_MOD_PATH ?= .

default: build install
build:
	$(MAKE) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) \
	    -C $(KDIR) M=$$PWD
install:
	$(MAKE) INSTALL_MOD_PATH=$(INSTALL_MOD_PATH) \
	    -C $(KDIR) M=$$PWD modules_install
