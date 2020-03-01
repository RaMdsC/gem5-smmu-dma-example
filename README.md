# gem5 DMA device behind SMMUv3 + Linux kernel module example

Minimal gem5 device model and Linux kernel module for DMA operations behind
gem5's SMMUv3 model. Tested on Ubuntu 19.10.

### Resources setup

```shell
mkdir -p resources/binaries/
mkdir resources/disks/
```

### Setup gem5

```shell
git clone https://github.com/gem5/gem5.git -b v19.0.0.0
cd gem5/
scons -j $(nproc) build/ARM/gem5.opt EXTRAS=../gem5-smmu-dma-example/gem5/
cd ../
```

### Setup Linux kernel and root filesystem

```shell
git clone https://github.com/buildroot/buildroot.git -b 2020.02-rc2
make -C buildroot/ menuconfig
```

* Target Options
  * Target Architecture -> AArch64 (little endian)
  * Target Architecture Variant -> cortex-A57
* Kernel
  * Linux Kernel -> Yes
    * Kernel configuration -> Use the architecture default configuration
    * Kernel binary format -> vmlinux
* Toolchain
  * Kernel Headers -> Same as kernel being built
* Filesystem images
  * ext2/3/4 root filesystem -> Yes
  * tar the root filesystem -> No

```shell
make BR2_JLEVEL=$(nproc) -C buildroot/
```

### Setup Linux kernel module

```shell
```

### Run simulation

```shell
export M5_PATH=${PWD}/resources/
gem5/build/ARM/gem5.opt gem5-smmu-dma-example/gem5/configs/main.py \
    --kernel vmlinux \
    --machine-type VExpress_GEM5_V2 \
    --disk rootfs.ext2 \
    --cpu-type atomic
```

### Explore DTS

```shell
dtc -I dtb -O dts m5out/system.dtb -o system.dts
less system.dts
```
