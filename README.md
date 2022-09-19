# Rongpin board specific driver for mainline Linux kernel

Installation
======

1. Clone this tree

```
git clone https://github.com/Cyanoxygen/king3399-kernel-patches
```

2. Fetch Linux kernel source

```
wget https://cdn.kernel.org/pub/linux/kernel/v5.x/linux-5.19.9.tar.xz
tar Jxf linux-5.19.9.tar.xz
```

3. Add files into the mainline tree

```
pushd king3399-kernel-patches
find . -type f -exec install -D {} ../linux-5.19.9/{} \;
popd

pushd linux-5.19.9
find ../king3399-kernel-patches -name '*.patch' -print0 | xargs -0 patch -p1
popd
```

4. Build kernel

```
cd linux-5.19.9
make ARCH=arm64 CROSS_COMPILE=whatever_your_distros_way_it_is king3399_defconfig
make ARCH=arm64 CROSS_COMPILE=whatever_your_distros_way_it_is -j$(nproc)
```

5. Install kernel modules

Note: depends on your target directory, you may want to add `sudo` to the start of the command.

```
make ARCH=arm64 INSTALL_MOD_PATH=/whatever/you/want/to/install modules_install
```

6. Copy kernel image file

The built kernel is `arch/arm64/boot/Image`.
You can run `make ARCH=arm64 CROSS_COMPILE=whatever_your_distros_way_it_is Image.gz` to build gzip compressed kernel.

